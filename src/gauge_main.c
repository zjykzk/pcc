#include <stdlib.h>

#include "gauge.h"

int
main(void)
{
    struct gauge *c = pcc_new_gauge("test", "test desc");
    pcc_inc_gauge(c);
    pcc_inc_gauge(c);
    pcc_print_gauge(c);
    pcc_inc_gauge_delta(c, 100);
    pcc_print_gauge(c);
    pcc_set_gauge(c, 300);
    pcc_print_gauge(c);

    const char *l[] = {"l1", "l2", NULL};
    struct gauge_vec *cvec = pcc_new_gauge_vec("test_vec", "test vec desc", l);
    const char *v1[] = {"1", "2", NULL};
    pcc_inc_gauge_vec(cvec, v1);
    pcc_inc_gauge_vec(cvec, v1);
    pcc_print_gauge_vec(cvec);
    pcc_inc_gauge_vec_delta(cvec, v1, 20);
    pcc_print_gauge_vec(cvec);
    const char *v2[] = {"11", "22", NULL};
    pcc_inc_gauge_vec(cvec, v2);
    pcc_print_gauge_vec(cvec);
    pcc_inc_gauge_vec_delta(cvec, v2, 100);
    pcc_print_gauge_vec(cvec);
    pcc_set_gauge_vec(cvec, v2, 300);
    pcc_print_gauge_vec(cvec);
}
