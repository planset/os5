#ifndef _KERNEL_H_
#define _KERNEL_H_

#define SYSCALL_TIMER_GET_GLOBAL_COUNTER	1
#define SYSCALL_SCHED_WAKEUP_MSEC		2

void kern_lock(unsigned char *if_bit);
void kern_unlock(unsigned char *if_bit);

#endif /* _KERNEL_H_ */
