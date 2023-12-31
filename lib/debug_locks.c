/*
 * lib/debug_locks.c
 *
 * Generic place for common debugging facilities for various locks:
 * spinlocks, rwlocks, mutexes and rwsems.
 *
 * Started by Ingo Molnar:
 *
 *  Copyright (C) 2006 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 */
#include <linux/rwsem.h>
#include <linux/mutex.h>
#include <linux/export.h>
#include <linux/spinlock.h>
#include <linux/debug_locks.h>

#ifdef CONFIG_LOCKDEP
#include <linux/sched/clock.h>
unsigned long long debug_locks_off_ts;
#endif

/*
 * We want to turn all lock-debugging facilities on/off at once,
 * via a global flag. The reason is that once a single bug has been
 * detected and reported, there might be cascade of followup bugs
 * that would just muddy the log. So we report the first one and
 * shut up after that.
 */
int debug_locks = 0;
EXPORT_SYMBOL_GPL(debug_locks);

/*
 * The locking-testsuite uses <debug_locks_silent> to get a
 * 'silent failure': nothing is printed to the console when
 * a locking bug is detected.
 */
int debug_locks_silent;
EXPORT_SYMBOL_GPL(debug_locks_silent);

/*
 * Generic 'turn off all lock debugging' function:
 */
int debug_locks_off(void)
{
	if (debug_locks && __debug_locks_off()) {
#ifdef CONFIG_LOCKDEP
		debug_locks_off_ts = sched_clock();
#endif
		if (!debug_locks_silent) {
			console_verbose();
			return 1;
		}
	}
	return 0;
}
EXPORT_SYMBOL_GPL(debug_locks_off);
