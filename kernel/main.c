#include <cpu.h>
#include <intr.h>
#include <excp.h>
#include <memory.h>
#include <console_io.h>
#include <timer.h>
#include <kernel.h>
#include <kern_task.h>
#include <shell.h>
#include <uptime.h>

void kern_lock(unsigned char *if_bit)
{
	/* Save EFlags.IF */
	*if_bit = (x86_get_eflags() & X86_EFLAGS_IF) ? 1 : 0;

	/* if saved IF == true, then cli */
	if (*if_bit)
		cli();
}

void kern_unlock(unsigned char *if_bit)
{
	/* if saved IF == true, then sti */
	if (*if_bit)
		sti();
}

int main(void)
{
	unsigned char mask;
	unsigned char i;

	/* Setup console */
	cursor_pos.y += 2;
	update_cursor();

	/* Setup exception handler */
	for (i = 0; i < EXCEPTION_MAX; i++)
		intr_set_handler(i, (unsigned int)&exception_handler);
	intr_set_handler(EXCP_NUM_GP, (unsigned int)&general_protection_handler);
	intr_set_handler(EXCP_NUM_PF, (unsigned int)&page_fault_handler);

	/* Setup devices */
	con_init();
	timer_init();
	mem_init();

	/* Setup tasks */
	kern_task_init();
	shell_init();
	uptime_init();

	/* Start paging */
	mem_page_start();

	/* Setup interrupt handler and mask register */
	intr_set_handler(INTR_NUM_TIMER, (unsigned int)&timer_handler);
	intr_set_handler(INTR_NUM_KB, (unsigned int)&keyboard_handler);
	intr_init();
	mask = intr_get_mask_master();
	mask &= ~(INTR_MASK_BIT_TIMER | INTR_MASK_BIT_KB);
	intr_set_mask_master(mask);
	sti();

	/* End of kernel initialization process */
	while (1) {
		x86_halt();
	}

	return 0;
}
