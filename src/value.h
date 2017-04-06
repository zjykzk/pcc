#ifndef PCC_VALUE_H
#define PCC_VALUE_H
#include <stdint.h>

#include "pcc.h"

typedef union {
    double v;
    uint64_t i;
} pcc_value;

static inline PCC_FORCEINLINE void init_value(pcc_value *pv, double v) {
    pv->v = v;
}

static inline PCC_FORCEINLINE void add(pcc_value *v, double d) {
    v->v += d;
}

static inline PCC_FORCEINLINE void atomic_add(pcc_value *v, double d) {
    for (;;) {
        uint64_t old = v->i;
        double n = v->v + d;
        if (__atomic_compare_exchange(&v->i,
                &old,
                (uint64_t *)&n,
                1,
                __ATOMIC_RELAXED,
                __ATOMIC_RELAXED)) {
            return;
        }
    }
}

static inline PCC_FORCEINLINE void set(pcc_value *v, double d) {
    v->v = d;
}

static inline PCC_FORCEINLINE void atomic_set(pcc_value *v, double d) {
    for (;;) {
        uint64_t old = v->i;
        if (__atomic_compare_exchange(&v->i,
                &old,
                (uint64_t *)&d,
                1,
                __ATOMIC_RELAXED,
                __ATOMIC_RELAXED)) {
            return;
        }
    }
}

static inline PCC_FORCEINLINE double value(pcc_value *v) {
    return v->v;
}

#endif /* ifndef PCC_VALUE_H */
