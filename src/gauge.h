#ifndef PCC_GAUGE_H
#define PCC_GAUGE_H
struct gauge;

struct gauge* pcc_new_gauge(const char *name, const char *desc);
void pcc_inc_gauge(struct gauge *g);
void pcc_inc_gauge_delta(struct gauge *g, double delta);
void pcc_set_gauge(struct gauge *g, double delta);
void pcc_print_gauge(struct gauge *g);


struct gauge_vec;
struct gauge_vec* pcc_new_gauge_vec(const char *name, const char *desc, const char *labels[]);
void pcc_inc_gauge_vec(struct gauge_vec *g, const char *values[]);
void pcc_inc_gauge_vec_delta(struct gauge_vec *g, const char *values[], double delta);
void pcc_set_gauge_vec(struct gauge_vec *g, const char *values[], double delta);
void pcc_print_gauge_vec(struct gauge_vec *gv);

#endif /* ifndef PCC_GAUGE_H */
