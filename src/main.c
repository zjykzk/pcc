#include <stdlib.h>

#include "counter.h"
#include "pccassert.h"

int
main(void)
{
    struct counter *c = pcc_new_counter("test", "test desc");
    pcc_inc_counter(c);
    pcc_inc_counter(c);
    pcc_print_counter(c);
    pcc_inc_counter_delta(c, 100);
    pcc_print_counter(c);

    const char *l[] = {"l1", "l2", NULL};
    struct counter_vec *cvec = pcc_new_counter_vec("test_vec", "test vec desc", l);
    const char *v1[] = {"1", "2", NULL};
    pcc_inc_counter_vec(cvec, v1);
    pcc_inc_counter_vec(cvec, v1);
    pcc_print_counter_vec(cvec);
    pcc_inc_counter_vec_delta(cvec, v1, 20);
    pcc_print_counter_vec(cvec);
    const char *v2[] = {"11", "22", NULL};
    pcc_inc_counter_vec(cvec, v2);
    pcc_print_counter_vec(cvec);
    pcc_inc_counter_vec_delta(cvec, v2, 100);
    pcc_print_counter_vec(cvec);
    //pcc_inc_counter_vec_delta(cvec, v2, -1);

    assert(NULL == pcc_new_counter("&a233", "123"));
}
