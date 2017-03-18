#ifndef PCC_COUNTER_H
#define PCC_COUNTER_H 
struct counter;

struct counter * pcc_new_counter(const char *name, const char *desc);
void pcc_inc_counter(struct counter*);
void pcc_print_counter(struct counter*);

struct counter_vec;
// `labels` must end with `NULL`
struct counter_vec * pcc_new_counter_vec(const char *name, const char *desc, const char *labels[]);
// `values` must end with `NULL`
void pcc_inc_counter_vec(struct counter_vec*, const char *values[]);
void pcc_print_counter_vec(struct counter_vec*);

#endif /* ifndef PCC_COUNTER_H */
