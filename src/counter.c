#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "counter.h"
#include "counter_private.h"
#include "pccassert.h"
#include "name.h"


pcc_counter *
pcc_new_counter(const char *name, const char *help, pcc_error *err) {
    if (!validate_name(name)) {
        *err = INVALID_NAME;
        return NULL;
    }

    pcc_counter *counter = malloc(sizeof(*counter));
    if (counter == NULL) {
        *err = OUT_OF_MEMORY;
        return NULL;
    }

    counter->name = name;
    counter->help = help;
    memset(&counter->value, 0, sizeof(counter->value));

    return counter;
}

inline PCC_FORCEINLINE void
pcc_inc_counter(pcc_counter *counter) {
    atomic_add(&counter->value, 1);
}

inline PCC_FORCEINLINE void
pcc_inc_counter_delta(pcc_counter *counter, double v) {
    assert(v > 0);
    atomic_add(&counter->value, v);
}

PCC_FORCEINLINE void
pcc_set_counter_delta(pcc_counter *counter, double v) {
    atomic_set(&counter->value, v);
}

void
pcc_print_counter(pcc_counter *counter) {
    printf("name:%s\nhelp:%s\nvalue:%g\n",
            counter->name,
            counter->help,
            counter->value.v);
}

pcc_counter_vec *
pcc_new_counter_vec(const char *name, const char *help, const char *labels[], pcc_error *err) {
    if (!validate_name(name)) {
        *err = INVALID_NAME;
        return NULL;
    }

    const char **l = labels;
    while(*l) {
        if (!validate_label(*l)) {
            *err = INVALID_LABEL;
            return NULL;
        }
        ++l;
    }

    pcc_counter_vec *vec = malloc(sizeof(*vec));
    if (vec == NULL) {
        *err = OUT_OF_MEMORY;
        return vec;
    }

    init_vec_header(&vec->h, name, help, labels, err);
    if (*err == OUT_OF_MEMORY) {
        free(vec);
        return NULL;
    }

    vec->elem = NULL;
    return vec;
}


static bool
new_counter(pcc_counter_vec *vec, const char *values[], double delta) {
    size_t value_count = 0, total_len = 0;
    PCC_COUNT_LENGTH(values, value_count, total_len);

    struct counter_elem *elem = malloc(sizeof(*elem) + total_len);
    if (elem == NULL) {
        return false;
    }
    init_vec_label_value(&elem->lv, values, value_count, total_len);

    set(&elem->v, delta);
    if (vec->elem) elem->lv.next = &vec->elem->lv;
    vec->elem = elem;
    return true;
}

void
pcc_update_counter_vec_delta(pcc_counter_vec *vec, const char *values[], double v, bool is_add, pcc_error *err) {
    assert(v > 0);
    size_t value_count = 0, total_len = 0;
    PCC_COUNT_LENGTH(values, value_count, total_len);

    if (vec->h.label_count < value_count) {
        *err = TOO_MANY_VALUES;
        return;
    }

    spinlock_lock(vec->h.locker);
    struct label_value *lv = NULL;
    if (vec->elem) lv = &vec->elem->lv;
    while (lv) {
        if (lv->value_len != total_len) {
            goto NEXT;
        }

        const char *label_values = lv->label_values;
        for (size_t i = 0; i < value_count; i++) {
            size_t vl = strlen(values[i]);
            if (0 != memcmp(label_values, values[i], vl)) {
                goto NEXT;
            }
            label_values = PCC_ADVANCE(label_values, vl + 1);
        }

        struct counter_elem *counter = PCC_CONTAINER_OF(lv, struct counter_elem, lv);
        if (is_add) add(&counter->v, v);
        else set(&counter->v, v);

        spinlock_unlock(vec->h.locker);
        return;
NEXT:
        lv = lv->next;
    }

    if (!new_counter(vec, values, v)) {
        *err = OUT_OF_MEMORY;
    }
    spinlock_unlock(vec->h.locker);
}

inline PCC_FORCEINLINE void
pcc_inc_counter_vec_delta(pcc_counter_vec *vec, const char *values[], double v, pcc_error *err) {
    pcc_update_counter_vec_delta(vec, values, v, true, err);
}

inline PCC_FORCEINLINE void
pcc_inc_counter_vec(pcc_counter_vec *vec, const char *values[], pcc_error *err) {
    pcc_inc_counter_vec_delta(vec, values, 1, err);
}

void
pcc_print_counter_vec(pcc_counter_vec* vec) {
    printf("name:%s\thelp:%s\n", vec->h.name, vec->h.help);
    printf("labels:[");
    for (int i = 0, len = vec->h.label_count; i < len; i++) {
        printf("%s", vec->h.labels[i]);
        putchar(i == len - 1 ? ']' : ',');
    }
    puts("\nvalues:");
    struct label_value *lv = &vec->elem->lv;
    while (lv) {
        printf("value length:%lu\n", lv->value_len);
        printf("[");
        char *v = lv->label_values;
        char *last = PCC_ADVANCE(v, lv->value_len);
        for (;;) {
            printf("%s", v);
            v = PCC_ADVANCE(v, strlen(v) + 1);
            bool is_last = v == last;
            putchar(is_last ? ']' : ',');

            if (is_last) {
                break;
            }
        }
        printf(":%g\n", value(&PCC_CONTAINER_OF(lv, struct counter_elem, lv)->v));
        lv = lv->next;
    }
}

