#ifndef PCC_COUNTER_PRIVATE
#define PCC_COUNTER_PRIVATE
#include "value.h"
#include "vec.h"
#include "spinlock.h"

typedef struct counter {
    const char *name, *help;
    pcc_value value;
} pcc_counter;

// `labels` memory layout
//
// +----------------+
// | label1 address |
// +----------------+
// | label2 address |
// +----------------+
// |      ...       |
// +----------------+
// | labeln address |
// +----------------+
// | label1 content |
// +----------------+
// | label2 content |
// +----------------+
// |      ...       |
// +----------------+
//
typedef struct counter_vec {
    struct vec_header h;

    // the count of counter
    struct counter_elem {
        pcc_value v;
        struct label_value lv;
    } *elem;
} pcc_counter_vec;


#endif /* ifndef PCC_COUNTER_PRIVATE */
