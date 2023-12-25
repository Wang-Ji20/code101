#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct lock {
    union {
        struct {
            uint8_t locked;
            uint8_t pending;
        };
        struct {
            uint16_t locked_pending;
            uint16_t tail;
        };
    };
} lock_t;

// x86 specific
static inline bool
atomic_cmpxchg(int *ptr, int *old_val, int new_val)
{
    bool success = false;
    asm volatile("lock;"
                 "cmpxchgl %[new_val], %[ptr]"
                 : "=@ccz"(success), [ptr] "+m"(*ptr), [old] "+a"(*old_val)
                 : [new_val] "q"(new_val)
                 : "memory");
    return success;
}

static void
spinlock_init(lock_t *lock);

static inline void
spinlock_lock(lock_t *lock);

static inline void
spinlock_release(lock_t *lock);
