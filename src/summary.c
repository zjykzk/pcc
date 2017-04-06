#include "summary.h"

#include <string.h>
#include <stdio.h>

#include "value.h"
#include "vec.h"
#include "pccerrors.h"
#include "spinlock.h"
#include "name.h"

// layout
// +----------+
// | bucket 0 |
// +----------+
// | ...      |
// +----------+
// | bucket n |
// +----------+
// | count 0  |
// +----------+
// | ...      |
// +----------+
// | count n  |
// +----------+
typedef struct summary {
    const char *name, *help;
    size_t bucket_count;
    struct spinlock lock;
    pcc_value sum;
    double buckets[0];
} pcc_summary;

#define COUNTS(s) (pcc_value*)((unsigned char*)s + sizeof(*s) + s->bucket_count * sizeof(double))

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
    pcc_value *counts = COUNTS(s);
    memset(counts, 0, value_sz);
    return s;
}

void
pcc_summary_observe(pcc_summary *s, double v) {
    pcc_value *c = COUNTS(s);
    for (size_t i = 0; i < s->bucket_count; ++i) {
        if (s->buckets[i] > v) {
            spinlock_lock(s->lock);
            add(c + i, 1);
            add(&s->sum, v);
            spinlock_unlock(s->lock);
            return;
        }
    }

    spinlock_lock(s->lock);
    add(c + s->bucket_count, 1);
    add(&s->sum, v);
    spinlock_unlock(s->lock);
}

void
pcc_summary_print(pcc_summary *s) {
    printf("name:%s,help:%s\n", s->name, s->help);
    printf("sum:%g\n", value(&s->sum));
    printf("buckets:");
    for (size_t i = 0, cnt = s->bucket_count; i < cnt; ++i) {
        printf("%g", s->buckets[i]);
        putchar(i == cnt - 1 ? '\n' : ',');
    }
    printf("counts:");
    pcc_value *c = COUNTS(s);
    for (size_t i = 0, cnt = s->bucket_count + 1; i < cnt; ++i) {
        printf("%g", c[i].v);
        putchar(i == cnt - 1 ? '\n' : ',');
    }
}
#undef COUNTS

typedef struct summary_vec {
    struct header {
        struct vec_header h;
        size_t bucket_count;
        double buckets[0];
    } *header;

    // layout
    // +----------+
    // | label 0  |
    // +----------+
    // | ...      |
    // +----------+
    // | label n  |
    // +----------+
    // | count 0  |
    // +----------+
    // | ...      |
    // +----------+
    // | count n  |
    // +----------+
    // the count of counter
    struct summery_elem {
        pcc_value sum;
        struct label_value lv;
    } *elem;
} pcc_summary_vec;

pcc_summary_vec*
pcc_new_summary_vec(const char *name, const char *help, double buckets[], size_t count, const char *labels[], pcc_error *err) {
    if (!validate_name(name)) {
        *err = INVALID_NAME;
        return NULL;
    }

    void *buf = malloc(sizeof(pcc_summary_vec) + sizeof(struct header) + count * sizeof(double));
    if (buf == NULL) {
        *err = OUT_OF_MEMORY;
        return NULL;
    }

    pcc_summary_vec *sv = buf;

    struct header *header = (struct header *)(((unsigned char *) buf) + sizeof(*sv));

    init_vec_header(&header->h, name, help, labels, err);
    if (*err == OUT_OF_MEMORY) {
        return NULL;
    }

    header->bucket_count = count;
    memcpy(header->buckets, buckets, count * sizeof(double));

    sv->header = header;
    sv->elem = NULL;

    return sv;
}
void
pcc_summary_vec_observe(pcc_summary_vec *sv, const char *values[], double v) {
// TODO
}
void pcc_summary_vec_print(pcc_summary_vec *sv);

