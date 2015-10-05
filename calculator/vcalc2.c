#include <stdio.h>
#include <stdlib.h>

//  Documentation
// ---------------
// Floating-Point Status and Control Register + Vector length and stride combinations
// 1: goto http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0344b/Chdfafia.html
// 2: goto http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0204j/Bcfiddig.html
//

typedef	float	fvector[4];

int	vcalc(char *op, fvector *a, fvector *b, fvector *c)
{
	asm volatile	// ASM for ARMv6 + VFP
	(
		"FMRX		r4, FPSCR		\n\t"
		"BIC		r4, #0x00370000		\n\t"
		"ORR		r4, r4, #(3 << 16)	\n\t"
		"FMXR		FPSCR, r4		\n\t"
		"CMP		%[op], #'+'		\n\t"
		"BEQ		Addition		\n\t"
		"CMP		%[op], #'-'		\n\t"
		"BEQ		Substration		\n\t"
		"CMP		%[op], #'x'		\n\t"
		"BEQ		Multiplication		\n\t"
		"CMP		%[op], #'/'		\n\t"
		"BEQ		Division		\n\t"
		"B		Exit			\n\t"
		"@ ------------------------------------	\n\t"
		"@	c = a + b			\n\t"
		"@ ------------------------------------	\n\t"
		"Addition:				\n\t"
 		"VADD.F32	s16, %[a], %[b]	\n\t"
		"B		Exit			\n\t"
		"@ ------------------------------------	\n\t"
		"@	c = a - b			\n\t"
		"@ ------------------------------------	\n\t"
		"Substration:				\n\t"
		"VLDMIA		%[a]!, {s0-s3}		\n\t"
		"VLDMIA		%[b]!, {s8-s11}		\n\t"
		"VSUB.F32	s16, s0, s8		\n\t"
		"VSTMIA		%[c]!, {s16-s19}	\n\t"
		"B		Exit			\n\t"
		"@ ------------------------------------	\n\t"
		"@	c = a x b			\n\t"
		"@ ------------------------------------	\n\t"
		"Multiplication:			\n\t"
		"VLDMIA		%[a]!, {s0-s3}		\n\t"
		"VLDMIA		%[b]!, {s8-s11}		\n\t"
		"VMUL.F32	s16, s0, s8		\n\t"
		"VSTMIA		%[c]!, {s16-s19}	\n\t"
		"B		Exit			\n\t"
		"@ ------------------------------------ \n\t"
		"@	c = a / b			\n\t"
		"@ ------------------------------------	\n\t"
		"Division:				\n\t"
		"VLDMIA		%[a]!, {s0-s3}		\n\t"
		"VLDMIA		%[b]!, {s8-s11}		\n\t"
		"VDIV.F32	s16, s0, s8		\n\t"
		"VSTMIA		%[c]!, {s16-s19}	\n\t"
		"@					\n\t"
		"Exit:					\n\t"
		: [c]	"+Uv"	(c),
		  [op]	"+r"	(*op)
		: [a]	"Uv"	(a),
		  [b]	"Uv"	(b)
		: "r4"
	);
	return(0);
}

int	main(int argc, char *argv[])
{
	fvector	a = {0}, b = {0}, c = {0};
	char	op = '0';

	if(argc == 4)
	{
		op = argv[1][0];
		sscanf(argv[2], "%f %f %f %f", &a[0], &a[1], &a[2], &a[3]);
		sscanf(argv[3], "%f %f %f %f", &b[0], &b[1], &b[2], &b[3]);

		vcalc( &op, &a, &b, &c );

		if(op != '@')
			printf("%f %f %f %f\n", c[0], c[1], c[2], c[3]);
		else
			printf("DIV/0\n");
		return(op == '@' ? 1 : 0);
	}
	else
	{
		printf(	"Usage: %s Op v1 v2\n"        \
			"where\n\tOp is + - x /\n"        \
			"\tv1 is the first vector\n" \
			"\tv2 is the second vector\n", argv[0]);
		return(1);
	}
}
