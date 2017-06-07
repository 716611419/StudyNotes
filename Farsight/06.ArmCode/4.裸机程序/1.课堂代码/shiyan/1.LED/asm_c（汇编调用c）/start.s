/*
 * start.s 0x1007_0000
 *
 *  Created on: 2014-8-29
 *      Author: Administrator
 */
.section .text    @area code
.arm              @code32
.global  _start   @entry

_start:

	@�õ�ջ�������ʼ��
	ldr sp, =0x41000000

	@��bss��
	ldr r0, =__bss_start
	ldr r1, =__bss_end
	mov r2, #0
bss_loop:
	cmp r0, r1
	strlt r2, [r0], #4
	blt bss_loop
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
led_loop:
	@ ������C ���Σ�����ATCPS ���� ����0 ��r0��   ������1��r1 �� ����2�� r2    �� ����3��r3
	@ ��������4����ջ����
	@ ����ֵ��r0����

	mov r0, #3
	bl led_main
@	bl led_on   @call C
@	bl asm_delay
@	bl led_off  @call C
@	bl asm_delay
@	b led_loop
stop:
	b stop


asm_delay:
	mov r4, #0x100000
delay_loop:
	cmp r4, #0
	subgt r4, r4, #1
	bgt delay_loop
	mov pc, lr


	.end         @end
