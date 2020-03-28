.section ".text.boot"

.global _start

_start:
	// Get current processor id
	mrc p15, 0, r5, c0, c0, 5
	and r5, r5, #3
	// If Id is not 0 keep the processor in an infinite loop. Basically shut them down
	cmp r5, #0
	bne halt

	// Load the _start address into the stack pointer. It will grow down from _start to 0
	ldr r4, =_start
	mov sp, r4

	ldr r4, =__bss_start
	ldr r5, =__bss_end
	// Prepare a 128Bit (or 8 byte or 4 words) long 0 initialized block
	// The linker script should guarantee that the space after .bss is filled up to the next page
	// so the number of bytes initialized with 0 will not crash into the next section since the page size is divisible by 8 byte
	mov r6, #0x00
	mov r7, #0x00
	mov r8, #0x00
	mov r9, #0x00

bss_init_loop:
	cmp r4,r5
	bhs bss_init_end
	stmia r4!, {r6-r9}
	b bss_init_loop

bss_init_end:
	// Calls _sysinit since r0,r1,r2 are preserved in the upper code
	// these 3 registers are setup by the raspberry firmware:
	// r0 = zero ... seems to be unused
	// r1 = machine id. Please refer to the armstubs for more info https://github.com/raspberrypi/tools/tree/master/armstubs
	// r2 = atags
	mov r0, r1
	mov r1, r2
	bl	_sysinit

halt:
	wfi
	b halt