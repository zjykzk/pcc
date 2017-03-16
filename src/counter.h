#ifndef PCC_COUNTER_H
#define PCC_COUNTER_H 
struct counter;

struct counter* pcc_new_counter(const char *name, const char *desc);
void pcc_inc_counter(struct counter*);
void pcc_print_counter(struct counter*);

#endif /* ifndef PCC_COUNTER_H */
