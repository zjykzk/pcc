#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "value.h"
#include "counter.h"


#define ADVANCE(ptr, offset) (((unsigned long)ptr)+(offset))

struct counter {
    const char *name, *desc;
    pcc_value value;
};

struct counter *
pcc_new_counter(const char *name, const char *desc) {
    struct counter *counter = malloc(sizeof(*counter));
    if (counter == NULL) {
        return NULL;
    }

    counter->name = name;
    counter->desc = desc;
    bzero(&counter->value, sizeof(counter->value));

    return counter;
}

void
pcc_inc_counter(struct counter *counter) {
    add(&counter->value, 1);
}

void
pcc_print_counter(struct counter *counter) {
    printf("name:%s\ndesc:%s\nvalue:%g\n",
            counter->name,
            counter->desc,
            counter->value.v);
}

// `labels` memory layout
//
// +----------------+
// | label1 address |
// +----------------+
// | label2 address |
// +----------------+
// |      ...       |
// +----------------+
// | labeln address |
// +----------------+
// | label1 content |
// +----------------+
// | label2 content |
// +----------------+
// |      ...       |
// +----------------+
//
struct counter_vec {
    const char *name, *desc, **labels;
    short label_count;

    // the count of counter
    struct label_value {
        struct counter counter;
        struct label_value *next;
        size_t value_len;
        char label_values[0];
    } *counter;
};

struct counter_vec *
pcc_new_counter_vec(const char *name, const char *desc, const char *labels[]) {
    struct counter_vec *vec = malloc(sizeof(*vec));
    if (vec == NULL) {
        return vec;
    }

    vec->name = name;
    vec->desc = desc;
    vec->counter = NULL;

    size_t label_count = 0, total_len = 0;
    for (const char *l = labels[0]; l; ++l) {
        label_count++;
        total_len += (strlen(l) + 1);
    }

    vec->label_count = (short)label_count;
    char **label_vec = malloc(sizeof(*label_vec) * label_count + total_len);
    if (NULL == label_vec) {
        return NULL;
    }

    char *label = (char *)ADVANCE(label_vec, (sizeof(*label_vec) * label_count));
    for (size_t i = 0; i < label_count; ++i) {
        label_vec[i] = label;

        size_t len = strlen(labels[i]) + 1;
        memcpy(label, labels[i], len);
        label = (char *)ADVANCE(label, len);
    }

    return vec;
}

#define COUNT_LENGTH(values)                  \
    for (const char *v = values[0]; v; ++v) { \
        value_count++;                        \
        total_len += strlen(v);               \
    }


static struct counter *
new_counter(struct counter_vec *vec, char *values[]) {
    size_t value_count = 0, total_len = 0;
    COUNT_LENGTH(values)
    struct label_value *lv = malloc(sizeof(*lv) + total_len);
    if (lv == NULL) {
        return NULL;
    }

    lv->counter.desc = vec->desc;
    lv->counter.name = vec->name;
    bzero(&lv->counter.value, sizeof(lv->counter.value));
    lv->value_len = total_len;

    char *label_values = lv->label_values;
    for (size_t i = 0; i < value_count; i++) {
        char *v = values[i];
        size_t len = strlen(v);
        memcpy(label_values, v, len);
        label_values = (char *)ADVANCE(label_values, len);
    }

    lv->next = vec->counter;
    vec->counter = lv;
    return &lv->counter;
}

struct counter *
pcc_counter_of(struct counter_vec *vec, char *values[]) {
    size_t value_count = 0, total_len = 0;
    COUNT_LENGTH(values)

    if (vec->label_count < (short)value_count) {
        return NULL;
    }

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
            label_values = (char *)ADVANCE(label_values, vl);
        }

        return (struct counter *)lv;
NEXT:
        lv = lv->next;
    }

    return new_counter(vec, values);
}

#undef ADVANCE
#undef COUNT_LENGTH
