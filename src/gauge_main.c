#include <stdlib.h>

#include "gauge.h"
#include "pccassert.h"
#include "pccerrors.h"

int
main(void)
{
    int err;
    struct gauge *c = pcc_new_gauge("test a", "test desc", &err);
    assert(err == INVALID_NAME);
    c = pcc_new_gauge("test", "test desc", &err);
    pcc_inc_gauge(c);
    pcc_inc_gauge(c);
    pcc_print_gauge(c);
    pcc_inc_gauge_delta(c, 100);
    pcc_print_gauge(c);
    pcc_set_gauge(c, 300);
    pcc_print_gauge(c);

    const char *l[] = {"l1", "l2", NULL};
    struct gauge_vec *cvec = pcc_new_gauge_vec("test_vec", "test vec desc", l, &err);
    const char *v1[] = {"1", "2", NULL};
    pcc_inc_gauge_vec(cvec, v1, &err);
    pcc_inc_gauge_vec(cvec, v1, &err);
    pcc_print_gauge_vec(cvec);
    pcc_inc_gauge_vec_delta(cvec, v1, 20, &err);
    pcc_print_gauge_vec(cvec);
    const char *v2[] = {"11", "22", NULL};
    pcc_inc_gauge_vec(cvec, v2, &err);
    pcc_print_gauge_vec(cvec);
    pcc_inc_gauge_vec_delta(cvec, v2, 100, &err);
    pcc_print_gauge_vec(cvec);
    pcc_set_gauge_vec(cvec, v2, 300, &err);
    pcc_print_gauge_vec(cvec);
}
