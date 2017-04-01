#include "summary.h"

#include <string.h>
#include <stdio.h>

#include "value.h"
#include "pccerrors.h"
#include "spinlock.h"
#include "name.h"

typedef struct summary {
    const char *name, *help;
    size_t bucket_count;
    struct spinlock lock;
    pcc_value sum;
    pcc_value *counts;
    double buckets[0];
} pcc_summary;

pcc_summary*
pcc_new_summary(const char *name, const char *help, double buckets[], size_t count, pcc_error *err) {
    if (!validate_name(name)) {
        *err = INVALID_NAME;
        return NULL;
    }

    const size_t value_sz = (count + 2) * sizeof(pcc_value);
    const size_t bucket_sz = count * sizeof(double);
    pcc_summary *s = malloc(sizeof(*s) + value_sz + bucket_sz);
    if (NULL == s) {
        *err = OUT_OF_MEMORY;
        return NULL;
    }

    s->bucket_count = count;
    s->name = name;
    s->help = help;
    //spinlock_init(s->lock);
    s->lock.lock=0;
    init_value(&s->sum, 0);
    memcpy(s->buckets, buckets, bucket_sz);
    s->counts = (pcc_value*)((unsigned char*)s + bucket_sz + sizeof(*s));
    memset(s->counts, 0, value_sz);
    return s;
}

void
pcc_summary_observe(pcc_summary *s, double v) {
    for (size_t i = 0; i < s->bucket_count; ++i) {
        if (s->buckets[i] > v) {
            spinlock_lock(s->lock);
            add(s->counts + i, 1);
            add(&s->sum, v);
            spinlock_unlock(s->lock);
            return;
        }
    }

    spinlock_lock(s->lock);
    add(s->counts + s->bucket_count, 1);
    add(&s->sum, v);
    spinlock_unlock(s->lock);
}

void
pcc_summary_print(pcc_summary *s) {
    printf("name:%s,help:%s\n", s->name, s->help);
    printf("sum:%g\n", s->sum.v);
    printf("buckets:");
    for (size_t i = 0, cnt = s->bucket_count; i < cnt; ++i) {
        printf("%g", s->buckets[i]);
        putchar(i == cnt - 1 ? '\n' : ',');
    }
    printf("counts:");
    for (size_t i = 0, cnt = s->bucket_count + 1; i < cnt; ++i) {
        printf("%g", s->counts[i].v);
        putchar(i == cnt - 1 ? '\n' : ',');
    }
}
