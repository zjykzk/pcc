#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "value.h"
#include "counter.h"

struct counter {
    const char *name, *desc;
    pcc_value value;
};

struct counter *
pcc_new_counter(const char *name, const char *desc) {
    struct counter *counter = malloc(sizeof(*counter));
    if (counter == NULL) {
        return NULL;
    }

    counter->name = name;
    counter->desc = desc;
    bzero(&counter->value, sizeof(counter->value));

    return counter;
}

void
pcc_inc_counter(struct counter *counter) {
    add(&counter->value, 1);
}

void
pcc_print_counter(struct counter *counter) {
    printf("name:%s\ndesc:%s\nvalue:%g\n",
            counter->name,
            counter->desc,
            counter->value.v);
}

struct counter_vec {
    const char *name, *desc;
};

struct counter_vec *
pcc_new_counter_vec(const char *name, const char *desc, char *labels[]) {
    return NULL;
}

struct counter *
pcc_counter_vec_labels(char *values[]) {
    return NULL;
}
