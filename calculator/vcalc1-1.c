// Mixed vector and scalar calculus
// CyrIng v1.1
//
#include <stdio.h>
#include <stdlib.h>

//  Documentation
// ---------------
// Floating-Point Status and Control Register + Vector length and stride combinations
// 1: goto http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0344b/Chdfafia.html
// 2: goto http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0204j/Bcfiddig.html
//

typedef	float	fvector[4];

int	vcalc(int dimension, char *op, fvector *a, fvector *b, fvector *c)
{
	asm volatile	// ASM for ARMv6 + VFP
	(
		"FMRX		r4, FPSCR		\n\t"
		"BIC		r4, #0x00370000		\n\t"
		"ORR		r4, r4, #(3 << 16)	\n\t"
		"FMXR		FPSCR, r4		\n\t"
		"@					\n\t"
		"VLDMIA		%[a]!, {s8-s11}		\n\t"
		"CMP		%[op], #'/'		\n\t"
		"BEQ		Division		\n\t"
		"CMP		%[op], #'x'		\n\t"
		"BEQ		Multiplication		\n\t"
		"CMP		%[op], #'-'		\n\t"
		"BEQ		Subtraction		\n\t"
		"CMP		%[op], #'+'		\n\t"
		"BNE		Exit			\n\t"
		"Addition:				\n\t"
		"CMP		%[dim], #1		\n\t"
		"BGT		AddVector		\n\t"
		"@ --------------------------- scalar -	\n\t"
		"@ [c1 ... cn] = [a1 + b1 ... an + b1]	\n\t"
		"@ ------------------------------------	\n\t"
		"VLDM		%[b], {s0}		\n\t"
 		"VADD.F32	s12, s8, s0		\n\t"
		"B		Exit			\n\t"
		"AddVector:				\n\t"
		"@ --------------------------- vector -	\n\t"
		"@ [c1 ... cn] = [a1 + b1 ... an + bn]	\n\t"
		"@ ------------------------------------	\n\t"
		"VLDMIA		%[b], {s20-s23}		\n\t"
		"VADD.F32	s12, s8, s20		\n\t"
		"B		Exit			\n\t"
		"Subtraction:				\n\t"
		"CMP		%[dim], #1		\n\t"
		"BGT		SubVector		\n\t"
		"@ --------------------------- scalar -	\n\t"
		"@ [c1 ... cn] = [a1 - b1 ... an - b1]	\n\t"
		"@ ------------------------------------	\n\t"
		"VLDM		%[b], {s0}		\n\t"
		"VSUB.F32	s12, s8, s0		\n\t"
		"B		Exit			\n\t"
		"SubVector:				\n\t"
		"@ --------------------------- vector -	\n\t"
		"@ [c1 ... cn] = [a1 - b1 ... an - bn]	\n\t"
		"@ ------------------------------------	\n\t"
		"VLDMIA		%[b], {s20-s23}		\n\t"
		"VSUB.F32	s12, s8, s20		\n\t"
		"B		Exit			\n\t"
		"Multiplication:			\n\t"
		"CMP		%[dim], #1		\n\t"
		"BGT		MulVector		\n\t"
		"@ --------------------------- scalar -	\n\t"
		"@ [c1 ... cn] = [a1 x b1 ... an x b1]	\n\t"
		"@ ------------------------------------	\n\t"
		"VLDM		%[b], {s0}		\n\t"
		"VMUL.F32	s12, s8, s0		\n\t"
		"B		Exit			\n\t"
		"MulVector:				\n\t"
		"@ --------------------------- vector -	\n\t"
		"@ [c1 ... cn] = [a1 x b1 ... an x bn]	\n\t"
		"@ ------------------------------------	\n\t"
		"VLDMIA		%[b], {s20-s23}		\n\t"
		"VMUL.F32	s12, s8, s20		\n\t"
		"B		Exit			\n\t"
		"Division:				\n\t"
		"CMP		%[dim], #1		\n\t"
		"BGT		DivVector		\n\t"
		"@ --------------------------- scalar -	\n\t"
		"@ [c1 ... cn] = [a1 / b1 ... an / b1]	\n\t"
		"@ ------------------------------------	\n\t"
		"VLDM		%[b], {s0}		\n\t"
		"VDIV.F32	s12, s8, s0		\n\t"
		"B		Exit			\n\t"
		"DivVector:				\n\t"
		"@ --------------------------- vector -	\n\t"
		"@ [c1 ... cn] = [a1 / b1 ... an / bn]	\n\t"
		"@ ------------------------------------	\n\t"
		"VLDMIA		%[b], {s20-s23}		\n\t"
		"VDIV.F32	s12, s8, s20		\n\t"
		"@					\n\t"
		"Exit:					\n\t"
		"VSTMIA		%[c], {s12-s15}		\n\t"
		: [c]	"+r"	(c),
		  [op]	"+r"	(*op)
		: [dim]	"r"	(dimension),
		  [a]	"r"	(a),
		  [b]	"r"	(b)
		: "r4", "s0", "s1", "s2", "s3", \
			"s8", "s9", "s10", "s11", \
			"s12", "s13", "s14", "s15", \
			"s20", "s21", "s22", "s23"
	);
	return(0);
}

int	main(int argc, char *argv[])
{
	fvector	a = {0}, b = {0}, c = {0};
	int	dim = 0;
	char	op = '0';

	if(argc == 4)
	{
		op = argv[1][0];
		sscanf(argv[2], "%f %f %f %f", &a[0], &a[1], &a[2], &a[3]);
		dim = sscanf(argv[3], "%f %f %f %f", &b[0], &b[1], &b[2], &b[3]);

		vcalc( dim, &op, &a, &b, &c );

		if(op != '@')
			printf("%s: %f %f %f %f\n", \
				(dim == 1) ? "Scalar" : "Vector", \
				c[0], c[1], c[2], c[3]);
		else
			printf("DIV/0\n");
		return(op == '@' ? 1 : 0);
	}
	else
	{
		printf(	"Usage: %s Op v1 v2\n"        \
			"where\n\tOp is + - x /\n"        \
			"\tv1 is the first vector\n" \
			"\tv2 is the second vector Or scalar\n", argv[0]);
		return(1);
	}
}
