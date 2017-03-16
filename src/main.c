#include "counter.h"

int
main(void)
{
    struct counter *c = pcc_new_counter("test", "test desc");
    pcc_inc_counter(c);
    pcc_print_counter(c);
}
