	.arch msp430g2553
	.p2align 1,0
	.text

	.global red_on
	.extern P1OUT

red_on:
	bis #64, &P1OUT
	and #~1, &P1OUT
	mov #0, r12
	pop r0		
