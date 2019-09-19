	.thumb

	.global asm_div
asm_div:
	swi 0x09
	bx  lr
