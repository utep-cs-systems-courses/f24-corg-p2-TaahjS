	.arch msp430g2553
	.p2align 1,0
	.text

	.global green_on
	.extern P1OUT
green_on:
	bis #1, &P1OUT
	and #~64, &P1OUT
	mov #1, r12		
	pop r0			
