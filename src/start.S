.section ".text.boot"

.global _start

_start:
	ldr r5, =_start
	mov sp, r5

	b	_sysinit