#ifndef PCC_GAUGE_H
#define PCC_GAUGE_H
#include "pccerrors.h"
typedef struct gauge pcc_gauge;

pcc_gauge* pcc_new_gauge(const char *name, const char *help, pcc_error *err);
void pcc_inc_gauge(pcc_gauge *g);
void pcc_inc_gauge_delta(pcc_gauge *g, double delta);
void pcc_set_gauge(pcc_gauge *g, double delta);
void pcc_print_gauge(pcc_gauge *g);
void set_to_current_time(pcc_gauge *g);


typedef struct gauge_vec pcc_gauge_vec;
pcc_gauge_vec* pcc_new_gauge_vec(const char *name, const char *help, const char *labels[], pcc_error *err);
void pcc_inc_gauge_vec(pcc_gauge_vec *g, const char *values[], pcc_error *err);
void pcc_inc_gauge_vec_delta(pcc_gauge_vec *g, const char *values[], double delta, pcc_error *err);
void pcc_set_gauge_vec(pcc_gauge_vec *g, const char *values[], double delta, pcc_error *err);
void pcc_print_gauge_vec(pcc_gauge_vec *gv);

#endif /* ifndef PCC_GAUGE_H */
