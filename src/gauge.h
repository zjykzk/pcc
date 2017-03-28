#ifndef PCC_GAUGE_H
#define PCC_GAUGE_H
#include "pccerrors.h"
struct gauge;

struct gauge* pcc_new_gauge(const char *name, const char *desc, pcc_error *err);
void pcc_inc_gauge(struct gauge *g);
void pcc_inc_gauge_delta(struct gauge *g, double delta);
void pcc_set_gauge(struct gauge *g, double delta);
void pcc_print_gauge(struct gauge *g);


struct gauge_vec;
struct gauge_vec* pcc_new_gauge_vec(const char *name, const char *desc, const char *labels[], pcc_error *err);
void pcc_inc_gauge_vec(struct gauge_vec *g, const char *values[], pcc_error *err);
void pcc_inc_gauge_vec_delta(struct gauge_vec *g, const char *values[], double delta, pcc_error *err);
void pcc_set_gauge_vec(struct gauge_vec *g, const char *values[], double delta, pcc_error *err);
void pcc_print_gauge_vec(struct gauge_vec *gv);
void set_to_current_time(struct gauge *g);

#endif /* ifndef PCC_GAUGE_H */
