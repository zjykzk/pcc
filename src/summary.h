#ifndef PCC_SUMMARY_H
#define PCC_SUMMARY_H
#include "pccerrors.h"
typedef struct summary pcc_summary; 

pcc_summary* pcc_new_summary(const char *name, const char *help, double buckets[], int count);
void pcc_summary_observe(pcc_summary *s, double v);

#endif /* ifndef PCC_SUMMARY_H */
