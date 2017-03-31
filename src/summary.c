#include "summary.h"
#include "value.h"

typedef struct summary {
    const char *name, *help;
    int bucket_count;
    pcc_value buckets[0];
} pcc_summary;

pcc_summary*
pcc_new_summary(const char *name, const char *help, double buckets[], int count);

void pcc_summary_observe(pcc_summary *s, double v);
