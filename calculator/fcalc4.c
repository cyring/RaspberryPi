#include <stdio.h>
#include <stdlib.h>

int	fcalc4(char op, int a, int b, int *r)
{
	int c;
	asm volatile	// ASM for ARMv6 + VFP
	(
		"MOV	r1, %[a]		\n\t"
		"MOV	r2, %[b]		\n\t"
		"MOV	r4, %[op]		\n\t"
		"MOV	r3, #0	@ --> %[c]	\n\t"
		"MOV	r5, #0	@ --> %[r]	\n\t"
		"CMP	r4, #'+'		\n\t"
		"BEQ	Addition		\n\t"
		"CMP	r4, #'-'		\n\t"
		"BEQ	Substration		\n\t"
		"CMP	r4, #'x'		\n\t"
		"BEQ	Multiplication		\n\t"
		"CMP	r4, #'/'		\n\t"
		"BEQ	Division		\n\t"
		"B	Exit			\n\t"
		"@ ----------------------------	\n\t"
		"@	c = a + b		\n\t"
		"@ ----------------------------	\n\t"
		"Addition:			\n\t"
		"VMOV	s0, s1, r1, r2		\n\t"
 		"VADD.F32	s3, s0, s1	\n\t"
		"VMOV	r3, s3			\n\t"
		"B	Exit			\n\t"
		"@ ----------------------------	\n\t"
		"@	c = a - b		\n\t"
		"@ ----------------------------	\n\t"
		"Substration:			\n\t"
		"VMOV	s0, s1, r1, r2		\n\t"
		"VSUB.F32	s3, s0, s1	\n\t"
		"VMOV	r3, s3			\n\t"
		"B	Exit			\n\t"
		"@ ----------------------------	\n\t"
		"@	c = a x b		\n\t"
		"@ ----------------------------	\n\t"
		"Multiplication:		\n\t"
		"VMOV	s0, s1, r1, r2		\n\t"
		"VMUL.F32	s3, s0, s1	\n\t"
		"VMOV	r3, s3			\n\t"
		"B	Exit			\n\t"
		"@ ---------------------------- \n\t"
		"@	c = a / b		\n\t"
		"@ ----------------------------	\n\t"
		"Division:			\n\t"
		"CMP	r2, #0			\n\t"
		"MOVEQ	r5, #-1			\n\t"
		"BEQ	Exit			\n\t"
		"VMOV	s0, s1, r1, r2		\n\t"
		"VDIV.F32	s3, s0, s1	\n\t"
		"VMOV	r3, s3			\n\t"
		"@				\n\t"
		"Exit:				\n\t"
		"MOV	%[c], r3		\n\t"
		"MOV	%[r], r5		\n\t"
		"@				\n\t"
		: [c]	"=r"	(c),
		  [r]	"+r"	(*r)
		: [op]	"r"	(op),
		  [a]	"r"	(a),
		  [b]	"r"	(b)
		: "r1", "r2", "r3", "r4", "r5"
	);
	return(c);
}

int	main(int argc, char *argv[])
{
	int c = 0, r = 0;

	if(argc == 4)
	{
		c = fcalc4( argv[1][0], atoi(argv[2]), atoi(argv[3]), &r );
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
