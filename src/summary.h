#ifndef PCC_SUMMARY_H
#define PCC_SUMMARY_H
#include <stdlib.h>
#include "pccerrors.h"
typedef struct summary pcc_summary;

pcc_summary* pcc_new_summary(const char *name, const char *help, double buckets[], size_t count, pcc_error *err);
void pcc_summary_observe(pcc_summary *s, double v);
void pcc_summary_print(pcc_summary *s);

typedef struct summary_vec pcc_summary_vec;
pcc_summary_vec* pcc_new_summary_vec(const char *name, const char *help, double buckets[], size_t count, const char *labels[], pcc_error *err);
void pcc_summary_vec_observe(pcc_summary_vec *sv, const char *values[], double v);
void pcc_summary_vec_print(pcc_summary_vec *sv);
#endif /* ifndef PCC_SUMMARY_H */
