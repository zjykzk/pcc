#ifndef PCC_SPIN_LOCK_H
#define PCC_SPIN_LOCK_H

#include "pccassert.h"

struct spinlock {
    short lock;
};

#define spinlock_init(l) ((l) = {0})
#define spinlock_lock(l)                        \
    do {                                        \
    short is_free = 0;                          \
    if (__atomic_compare_exchange_n(&l.lock,    \
                &is_free,                       \
                1,                              \
                1,                              \
                __ATOMIC_RELAXED,               \
                __ATOMIC_RELAXED)) break;       \
} while(1)

#define spinlock_unlock(l)                      \
    assert(l.lock == 1);                        \
    do {                                        \
    short is_lock = 1;                          \
    if (__atomic_compare_exchange_n(&l.lock,    \
                &is_lock,                       \
                0,                              \
                1,                              \
                __ATOMIC_RELAXED,               \
                __ATOMIC_RELAXED)) break;       \
} while(1)


#endif /* ifndef PCC_SPIN_LOCK_H */
