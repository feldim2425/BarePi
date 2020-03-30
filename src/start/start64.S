.section ".text.boot"

.global _start

_start:
	// Load the _start address into the stack pointer. It will grow down from _start to 0
	ldr x4, =_start
	mov sp, x4

	ldr x4, =__bss_start
	ldr x5, =__bss_end
bss_init_loop:
	cmp x4,x5
	bhs bss_init_end
	stmia x4!, {x6-x9}
	b bss_init_loop

bss_init_end:
	// Calls _sysinit since r0,r1,r2 are preserved in the upper code
	// these 3 registers are setup by the raspberry firmware:
	// r0 = zero ... seems to be unused
	// r1 = machine id. Please refer to the armstubs for more info https://github.com/raspberrypi/tools/tree/master/armstubs
	// r2 = atags
	mov x0, x1
	mov x1, x2
	bl	_sysinit

halt:
	wfi
	b halt