#include <stdio.h>

#include "summary.h"
#include "pccassert.h"
#include "pccerrors.h"

int
main(void) {
    pcc_error err;
    double buckets[] = {0, 0.1, 0.3};
    pcc_summary *s = pcc_new_summary("summary space", "help", buckets,
            sizeof(buckets)/sizeof(buckets[0]), &err);
    assert(err == INVALID_NAME);
    s = pcc_new_summary("summary_test", "help", buckets,
            sizeof(buckets)/sizeof(buckets[0]), &err);
    pcc_summary_print(s);

    printf("\n-------test:%g\n", 0.01);
    pcc_summary_observe(s, 0.01);
    pcc_summary_print(s);

    printf("\n-------test:%g\n", 0.1);
    pcc_summary_observe(s, 0.1);
    pcc_summary_print(s);

    printf("\n-------test:%g\n", 0.12);
    pcc_summary_observe(s, 0.12);
    pcc_summary_print(s);

    printf("\n-------test:%g\n", 100.0);
    pcc_summary_observe(s, 100);
    pcc_summary_print(s);
}
