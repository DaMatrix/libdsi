	.global div
div:
	swi 0x09
	bx  lr
