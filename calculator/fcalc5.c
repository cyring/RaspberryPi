#include <stdio.h>
#include <stdlib.h>

//  Documentation
// ---------------
// Floating-Point Status and Control Register + Vector length and stride combinations
// 1: goto http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0344b/Chdfafia.html
// 2: goto http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0204j/Bcfiddig.html
//
float	fcalc5(char *op, float a, float b)
{
	float c;
	asm volatile	// ASM for ARMv6 + VFP
	(
		"MOV		%[c], #0		\n\t"
		"FMRX		r0, FPSCR		\n\t"
		"BIC		r0, #0x00370000		\n\t"
		"ORR		r0, r0, #(0 << 16)	\n\t"
		"FMXR		FPSCR, r0		\n\t"
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
		"VMOV		s0, %[a]		\n\t"
		"VMOV		s8, %[b]		\n\t"
 		"VADD.F32	s16, s0, s8		\n\t"
		"VMOV		%[c], s16		\n\t"
		"B		Exit			\n\t"
		"@ ------------------------------------	\n\t"
		"@	c = a - b			\n\t"
		"@ ------------------------------------	\n\t"
		"Substration:				\n\t"
		"VMOV		s0, %[a]		\n\t"
		"VMOV		s8, %[b]		\n\t"
		"VSUB.F32	s16, s0, s8		\n\t"
		"VMOV		%[c], s16		\n\t"
		"B		Exit			\n\t"
		"@ ------------------------------------	\n\t"
		"@	c = a x b			\n\t"
		"@ ------------------------------------	\n\t"
		"Multiplication:			\n\t"
		"VMOV		s0, %[a]		\n\t"
		"VMOV		s8, %[b]		\n\t"
		"VMUL.F32	s16, s0, s8		\n\t"
		"VMOV		%[c], s16		\n\t"
		"B		Exit			\n\t"
		"@ ------------------------------------ \n\t"
		"@	c = a / b			\n\t"
		"@ ------------------------------------	\n\t"
		"Division:				\n\t"
		"CMP		%[b], #0		\n\t"
		"MOVEQ		%[op], #'@'		\n\t"
		"BEQ		Exit			\n\t"
		"VMOV		s0, %[a]		\n\t"
		"VMOV		s8, %[b]		\n\t"
		"VDIV.F32	s16, s0, s8		\n\t"
		"VMOV		%[c], s16		\n\t"
		"@					\n\t"
		"Exit:					\n\t"
		: [c]	"=r"	(c),
		  [op]	"+r"	(*op)
		: [a]	"r"	(a),
		  [b]	"r"	(b)
		: "r0"
	);
	return(c);
}

int	main(int argc, char *argv[])
{
	char	op = '0';
	float	c = 0;

	if(argc == 4)
	{
		op = argv[1][0];
		c = fcalc5( &op, strtof(argv[2], NULL), strtof(argv[3], NULL) );
		if(op != '@')
			printf("%f\n", c);
		else
			printf("DIV/0\n");
		return(op == '@' ? 1 : 0);
	}
	else
	{
		printf(	"Usage: %s Op r1 r2\n"        \
			"where\n\tOp is + - x /\n"        \
			"\tr1 is the first real number\n" \
			"\tr2 is the second real number\n", argv[0]);
		return(1);
	}
}
