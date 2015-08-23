#include <cpu.h>
#include <intr.h>
#include <excp.h>
#include <console_io.h>
#include <timer.h>
#include <shell.h>
#include <io_port.h>
#include <uptime.h>

int main(void)
{
	unsigned char mask;
	unsigned char i;
	extern unsigned char timer_handler;

	/* Setup console */
	cli();
	cursor_pos.y += 2;
	update_cursor();
	cli();

	/* Setup exception handler */
	for (i = 0; i < EXCEPTION_NUM; i++)
		intr_set_handler(i, (unsigned int)&exception_handler);

	/* Setup tasks */
	shell_init();
	uptime_init();

	/* Setup interrupt handler and mask register */
	con_init();
	intr_set_handler(INTR_NUM_TIMER, (unsigned int)&timer_handler);
	intr_set_handler(INTR_NUM_KB, (unsigned int)&keyboard_handler);
	intr_init();
	mask = intr_get_mask_master();
	mask &= ~(INTR_MASK_BIT_TIMER | INTR_MASK_BIT_KB);
	intr_set_mask_master(mask);
	sti();

	/* Setup timer */
	timer_init();
	put_str("Timer initialized.\r\n");

	/* Start main task */
	shell_start();

	return 0;
}
