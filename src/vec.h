#ifndef PCC_VEC_H
#define PCC_VEC_H
#include <stdlib.h>
#include <string.h>
#include "spinlock.h"
#include "pccerrors.h"

struct vec_header {
    const char *name, *help, **labels;
    unsigned short label_count;
    struct spinlock locker;
};

static inline void
init_vec_header(struct vec_header *h, const char *name, const char *help, const char **labels, pcc_error *err) {
#define ADV(ptr, offset) (((char *)ptr)+(offset))

#define CNT_LEN(values, c, l)                    \
    do {                                         \
    for (const char **_v = values; *_v; ++_v) {  \
        c++;                                     \
        l += strlen(*_v);                        \
    }                                            \
    l += c;                                      \
    } while(0)

    h->name = name;
    h->help = help;
    size_t label_count = 0, total_len = 0;
    CNT_LEN(labels, label_count, total_len);

    h->label_count = (unsigned short)label_count;
    const char **label_vec = malloc(sizeof(*label_vec) * label_count + total_len);
    if (NULL == label_vec) {
        *err = OUT_OF_MEMORY;
        return;
    }

    h->labels = label_vec;
    char *label = ADV(label_vec, (sizeof(*label_vec) * label_count));
    for (size_t i = 0; i < label_count; ++i) {
        label_vec[i] = label;

        size_t len = strlen(labels[i]) + 1;
        memcpy(label, labels[i], len);
        label = ADV(label, len);
    }

#undef ADV
#undef CNT_LEN
}

#endif /* ifndef PCC_VEC_H */
