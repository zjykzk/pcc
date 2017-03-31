#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "counter.h"
#include "counter_private.h"
#include "pccassert.h"
#include "name.h"

#define ADVANCE(ptr, offset) (((char *)ptr)+(offset))

pcc_counter *
pcc_new_counter(const char *name, const char *desc, pcc_error *err) {
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
    counter->desc = desc;
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
    printf("name:%s\ndesc:%s\nvalue:%g\n",
            counter->name,
            counter->desc,
            counter->value.v);
}

#define COUNT_LENGTH(values, c, l)               \
    do {                                         \
    for (const char **_v = values; *_v; ++_v) {  \
        c++;                                     \
        l += strlen(*_v);                        \
    }                                            \
    l += c;                                      \
    } while(0)

pcc_counter_vec *
pcc_new_counter_vec(const char *name, const char *desc, const char *labels[], pcc_error *err) {
    if (!validate_name(name)) {
        *err = INVALID_NAME;
        return NULL;
    }

    const char **l = labels;
    while(*l) {
        if (!validate_label(*l)) {
            *err = INVALID_NAME;
            return NULL;
        }
        ++l;
    }

    pcc_counter_vec *vec = malloc(sizeof(*vec));
    if (vec == NULL) {
        *err = OUT_OF_MEMORY;
        return vec;
    }

    vec->name = name;
    vec->desc = desc;
    vec->counter = NULL;

    size_t label_count = 0, total_len = 0;
    COUNT_LENGTH(labels, label_count, total_len);

    vec->label_count = (unsigned short)label_count;
    const char **label_vec = malloc(sizeof(*label_vec) * label_count + total_len);
    if (NULL == label_vec) {
        *err = OUT_OF_MEMORY;
        free(vec);
        return NULL;
    }

    vec->labels = label_vec;
    char *label = ADVANCE(label_vec, (sizeof(*label_vec) * label_count));
    for (size_t i = 0; i < label_count; ++i) {
        label_vec[i] = label;

        size_t len = strlen(labels[i]) + 1;
        memcpy(label, labels[i], len);
        label = ADVANCE(label, len);
    }

    return vec;
}


static bool
new_counter(pcc_counter_vec *vec, const char *values[], double delta) {
    size_t value_count = 0, total_len = 0;
    COUNT_LENGTH(values, value_count, total_len);

    struct label_value *lv = malloc(sizeof(*lv) + total_len);
    if (lv == NULL) {
        return false;
    }

    lv->v.v = delta;
    lv->value_len = total_len;

    char *label_values = lv->label_values;
    for (size_t i = 0; i < value_count; i++) {
        const char *v = values[i];
        size_t len = strlen(v) + 1;
        memcpy(label_values, v, len);
        label_values = ADVANCE(label_values, len);
    }

    lv->next = vec->counter;
    vec->counter = lv;
    return true;
}

void
pcc_update_counter_vec_delta(pcc_counter_vec *vec, const char *values[], double v, bool is_add, pcc_error *err) {
    assert(v > 0);
    size_t value_count = 0, total_len = 0;
    COUNT_LENGTH(values, value_count, total_len);

    if (vec->label_count < value_count) {
        *err = TOO_MANY_VALUES;
        return;
    }

    spinlock_lock(vec->locker);
    struct label_value *lv = vec->counter;
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
            label_values = ADVANCE(label_values, vl + 1);
        }

        if (is_add) add(&lv->v, v);
        else set(&lv->v, v);

        spinlock_unlock(vec->locker);
        return;
NEXT:
        lv = lv->next;
    }

    if (!new_counter(vec, values, v)) {
        *err = OUT_OF_MEMORY;
    }
    spinlock_unlock(vec->locker);
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
    printf("name:%s\tdesc:%s\n", vec->name, vec->desc);
    printf("labels:[");
    for (int i = 0, len = vec->label_count; i < len; i++) {
        printf("%s", vec->labels[i]);
        putchar(i == len - 1 ? ']' : ',');
    }
    puts("\nvalues:");
    struct label_value *c = vec->counter;
    while (c) {
        printf("[");
        char *v = c->label_values;
        char *last = ADVANCE(v, c->value_len);
        for (;;) {
            printf("%s", v);
            v = ADVANCE(v, strlen(v) + 1);
            bool is_last = v == last;
            putchar(is_last ? ']' : ',');

            if (is_last) {
                break;
            }
        }
        printf(":%g\n", c->v.v);
        c = c->next;
    }
}

#undef ADVANCE
#undef COUNT_LENGTH
