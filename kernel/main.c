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
	init_shell();
	init_uptime();

	/* Setup interrupt handler and mask register */
	con_init();
	intr_set_handler(32, (unsigned int)&timer_handler);
	intr_set_handler(INTR_NUM_KB, (unsigned int)&keyboard_handler);
	intr_init();
	mask = intr_get_mask_master();
	mask &= ~(0x01 | INTR_MASK_BIT_KB);
	intr_set_mask_master(mask);
	sti();

	/* Setup PIT */
	outb_p(0x34, 0x0043);
	/* 割り込み周期11932(0x2e9c)サイクル(=100Hz、10ms毎)に設定 */
	outb_p(0x9c, 0x0040);
	outb_p(0x2e, 0x0040);
	put_str("Timer initialized.\r\n");

	/* Start main task */
	start_shell();

	return 0;
}
