#ifndef PCC_SUMMARY_H
#define PCC_SUMMARY_H
#include <stdlib.h>
#include "pccerrors.h"
typedef struct summary pcc_summary; 

pcc_summary* pcc_new_summary(const char *name, const char *help, double buckets[], size_t count, pcc_error *err);
void pcc_summary_observe(pcc_summary *s, double v);
void pcc_summary_print(pcc_summary *s);

#endif /* ifndef PCC_SUMMARY_H */
