#include <stdio.h>
#include <stdlib.h>

int	fcalc2(char op, int a, int b, int *r)
{
	int c;
	asm volatile	// ASM for ARMv6
	(
		"MOV	%[c], #0		\n\t"
		"MOV	%[r], #0		\n\t"
		"@ ---- c = a + b ------------- \n\t"
		"CMP	%[op], #'+'		\n\t"
		"ADDEQ	%[c], %[a], %[b]	\n\t"
		"BEQ	Exit			\n\t"
		"@ ---- c = a - b ------------- \n\t"
		"CMP	%[op], #'-'		\n\t"
		"SUBEQ	%[c], %[a], %[b]	\n\t"
		"BEQ	Exit			\n\t"
		"@ ---- c = a x b ------------- \n\t"
		"CMP	%[op], #'x'		\n\t"
		"MULEQ	%[c], %[a], %[b]	\n\t"
		"BEQ	Exit			\n\t"
		"@ ---- c = a / b ------------- \n\t"
		"CMP	%[op], #'/'		\n\t"
		"BNE	Exit			\n\t"
		"CMP	%[b], #0		\n\t"
		"MOVEQ	%[r], #-1		\n\t"
		"BEQ	Exit			\n\t"
		"@ ---- a = (b x c) + r ------- \n\t"
		"MOV	%[r], %[b]		\n\t"
		"CMP	%[r], %[a], LSR #1	\n\t"
		"L90:				\n\t"
		"MOVLS	%[r], %[r], LSL #1	\n\t"
		"CMP	%[r], %[a], LSR #1	\n\t"
		"BLS	L90			\n\t"
		"L91:				\n\t"
		"CMP	%[a], %[r]		\n\t"
		"SUBCS	%[a], %[a], %[r]	\n\t"
		"ADC	%[c], %[c], %[c]	\n\t"
		"MOV	%[r], %[r], LSR #1	\n\t"
		"CMP	%[r], %[b]		\n\t"
		"BHS	L91			\n\t"
		"MOV	%[r], %[a]		\n\t"
		"Exit:				\n\t"
		: [c]	"=r"	(c),
		  [r]	"+r"	(*r)
		: [op]	"r"	(op),
		  [a]	"r"	(a),
		  [b]	"r"	(b)
	);
	return(c);
}

int	main(int argc, char *argv[])
{
	int c = 0, r = 0;

	if(argc == 4)
	{
		c = fcalc2( argv[1][0], atoi(argv[2]), atoi(argv[3]), &r );
		if(r != -1)
			printf("%d (%d)\n", c, r);
		else
			printf("DIV/0\n");
		return(r == -1 ? 1 : 0);
	}
	else
	{
		printf(	"Usage: %s Op Num1 Num2\n"        \
			"where\n\tOp is + - x /\n"        \
			"\tNum#1 is the first argument\n" \
			"\tNum#2 is the second argument\n", argv[0]);
		return(1);
	}
}
