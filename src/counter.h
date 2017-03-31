#ifndef PCC_COUNTER_H
#define PCC_COUNTER_H 
#include "pccerrors.h"
typedef struct counter pcc_counter;

pcc_counter * pcc_new_counter(const char *name, const char *desc, pcc_error *err);
void pcc_inc_counter(pcc_counter*);
void pcc_inc_counter_delta(pcc_counter *c, double delta);
void pcc_print_counter(pcc_counter*);

typedef struct counter_vec pcc_counter_vec;
// `labels` must end with `NULL`
pcc_counter_vec * pcc_new_counter_vec(const char *name, const char *desc, const char *labels[], pcc_error *err);
// increment `counter_vec` by one
// `values` must end with `NULL`
void pcc_inc_counter_vec(pcc_counter_vec *vec, const char *values[], pcc_error *err);
// increment `counter_vec` by `delta` value
// `values` must end with `NULL`
void pcc_inc_counter_vec_delta(pcc_counter_vec *vec, const char *values[], double delta, pcc_error *err);
void pcc_print_counter_vec(pcc_counter_vec *vec);

#endif /* ifndef PCC_COUNTER_H */
