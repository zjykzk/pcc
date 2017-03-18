#ifndef PCC_COUNTER_H
#define PCC_COUNTER_H 
struct counter;

struct counter * pcc_new_counter(const char *name, const char *desc);
void pcc_inc_counter(struct counter*);
void pcc_inc_counter_delta(struct counter *c, double delta);
void pcc_print_counter(struct counter*);

struct counter_vec;
// `labels` must end with `NULL`
struct counter_vec * pcc_new_counter_vec(const char *name, const char *desc, const char *labels[]);
// increment `counter_vec` by one
// `values` must end with `NULL`
void pcc_inc_counter_vec(struct counter_vec *vec, const char *values[]);
// increment `counter_vec` by `delta` value
// `values` must end with `NULL`
void pcc_inc_counter_vec_delta(struct counter_vec *vec, const char *values[], double delta);
void pcc_print_counter_vec(struct counter_vec *vec);

#endif /* ifndef PCC_COUNTER_H */
