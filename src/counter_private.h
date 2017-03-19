#ifndef PCC_COUNTER_PRIVATE
#define PCC_COUNTER_PRIVATE
#include "value.h"
#include "spinlock.h"

struct counter {
    const char *name, *desc;
    pcc_value value;
};

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
struct counter_vec {
    const char *name, *desc, **labels;
    unsigned short label_count;
    struct spinlock locker;

    // the count of counter
    struct label_value {
        pcc_value v;
        struct label_value *next;
        size_t value_len;
        char label_values[0];
    } *counter;
};


#endif /* ifndef PCC_COUNTER_PRIVATE */
