#ifndef PCC_ASSERT_H
#include <unistd.h> /* for _exit() */
#include <stdio.h>

#define assert(_e) ((_e)?(void)0 : (fprintf(stderr,"%s:%d: %s",__FILE__,__LINE__,#_e),_exit(1)))

#define PCC_ASSERT_H
#endif /* ifndef PCC_ASSERT_H */
