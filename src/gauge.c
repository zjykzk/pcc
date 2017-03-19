#include <stdbool.h>

#include <sys/time.h>

#include "gauge.h"
#include "counter_private.h"

struct gauge {
    struct counter c;
};

struct counter* pcc_new_counter(const char *name, const char *desc);
inline PCC_FORCEINLINE struct gauge*
pcc_new_gauge(const char *name, const char *desc) {
    return (struct gauge *) pcc_new_counter(name, desc);
}

void pcc_inc_counter(struct counter *c);
inline PCC_FORCEINLINE void
pcc_inc_gauge(struct gauge *g) {
    pcc_inc_counter(&g->c);
}

extern void pcc_inc_counter_delta(struct counter *c, double delta);
inline PCC_FORCEINLINE void
pcc_inc_gauge_delta(struct gauge *g, double delta) {
    pcc_inc_counter_delta(&g->c, delta);
}

void pcc_set_gauge(struct gauge *g, double delta) {
    atomic_set(&g->c.value, delta);
}

void pcc_print_counter(struct counter *c);
inline PCC_FORCEINLINE void
pcc_print_gauge(struct gauge *g) {
    pcc_print_counter(&g->c);
}

struct gauge_vec {
    struct counter_vec cv;
};

struct counter_vec* pcc_new_counter_vec(const char *name, const char *desc, const char *labels[]);

inline PCC_FORCEINLINE struct gauge_vec*
pcc_new_gauge_vec(const char *name, const char *desc, const char *labels[]) {
    return (struct gauge_vec *) pcc_new_counter_vec(name, desc, labels);
}

void pcc_inc_counter_vec(struct counter_vec *g, const char *values[]);
inline PCC_FORCEINLINE void
pcc_inc_gauge_vec(struct gauge_vec *gv, const char *values[]) {
    pcc_inc_counter_vec(&gv->cv, values);
}

void pcc_inc_counter_vec_delta(struct counter_vec *cv, const char *values[], double delta);
inline PCC_FORCEINLINE void
pcc_inc_gauge_vec_delta(struct gauge_vec *gv, const char *values[], double delta) {
    pcc_inc_counter_vec_delta(&gv->cv, values, delta);
}

void pcc_update_counter_vec_delta(struct counter_vec *vec, const char *values[], double v, bool is_add);
void
pcc_set_gauge_vec(struct gauge_vec *gv, const char *values[], double delta) {
    pcc_update_counter_vec_delta(&gv->cv, values, delta, false);
}

void pcc_print_counter_vec(struct counter_vec *cv);
inline PCC_FORCEINLINE void
pcc_print_gauge_vec(struct gauge_vec *gv) {
    pcc_print_counter_vec(&gv->cv);
}

void
set_to_current_time(struct gauge *g) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    atomic_set(&g->c.value, (double)tv.tv_sec);
}
