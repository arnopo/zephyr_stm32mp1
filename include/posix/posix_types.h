/*
 * Copyright (c) 2018 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_POSIX_SYS_TYPES_H_
#define ZEPHYR_INCLUDE_POSIX_SYS_TYPES_H_

#ifndef CONFIG_ARCH_POSIX
#include <sys/types.h>
#endif

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long useconds_t;

/* time related attributes */
#ifndef CONFIG_NEWLIB_LIBC
typedef u32_t clockid_t;
#endif /*CONFIG_NEWLIB_LIBC */
typedef unsigned long timer_t;

#ifdef CONFIG_PTHREAD_IPC
/* Thread attributes */
typedef struct pthread_attr_t {
	int priority;
	void *stack;
	size_t stacksize;
	u32_t flags;
	u32_t delayedstart;
	u32_t schedpolicy;
	s32_t detachstate;
	u32_t initialized;
} pthread_attr_t;

typedef void *pthread_t;

/* Semaphore */
typedef struct k_sem sem_t;

/* Mutex */
typedef struct pthread_mutex {
	pthread_t owner;
	u16_t lock_count;
	int type;
	_wait_q_t wait_q;
} pthread_mutex_t;

typedef struct pthread_mutexattr {
	int type;
} pthread_mutexattr_t;

/* Condition variables */
typedef struct pthread_cond {
	_wait_q_t wait_q;
} pthread_cond_t;

typedef struct pthread_condattr {
} pthread_condattr_t;

/* Barrier */
typedef struct pthread_barrier {
	_wait_q_t wait_q;
	int max;
	int count;
} pthread_barrier_t;

typedef struct pthread_barrierattr {
} pthread_barrierattr_t;

typedef u32_t pthread_rwlockattr_t;

typedef struct pthread_rwlock_obj {
	struct k_sem rd_sem;
	struct k_sem wr_sem;
	struct k_sem reader_active;/* blocks WR till reader has acquired lock */
	s32_t status;
	k_tid_t wr_owner;
} pthread_rwlock_t;

#endif /* CONFIG_PTHREAD_IPC */

#ifdef __cplusplus
}
#endif

#endif	/* ZEPHYR_INCLUDE_POSIX_SYS_TYPES_H_ */
