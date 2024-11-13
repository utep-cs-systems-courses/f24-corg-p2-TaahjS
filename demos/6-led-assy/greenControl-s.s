	.arch msp430g2553
	.p2align 1,0
	.text


	.global greenControl
	.extern P1OUT

greenControl:
	cmp #0, r12 		;checks if r12 is 0 or 1
	jz off 			;if r12 is 0, we jump
	bis #1, &P1OUT		;P1OUT |= GREEN_LED
	pop r0			;return
off:	and #~1, &P1OUT 	;P1OUT &= ~GREEN_LED
	pop r0			;return
