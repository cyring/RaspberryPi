#include <stdio.h>
#include <stdlib.h>

int	fcalc(int *f, char op, int a, int b)
{
	int c = 0;
	*f = 0;
	switch(op)
	{
		case '+': c = a + b;
		break;
		case '-': c = a - b;
		break;
		case 'x': c = a * b;
		break;
		case '/': if(b != 0)
				{
				 c = (int) (a / b);
				*f = (int) ((10 * (a - (c * b))) / b);	// 10 x (a - (c x b))
				}					// ------------------
			  else						//        b
				*f = -1;
		break;
	}
	return(c);
}

int	main(int argc, char *argv[])
{
	int f = 0;
	int r = 0;

	if(argc == 4)
	{
		r = fcalc( &f, argv[1][0], atoi(argv[2]), atoi(argv[3]) );
		if(f != -1)
			printf("%d.%d\n", r, f);
		else
			printf("DIV/0\n");
	}
	else
		printf(	"Usage: %s Op Num1 Num2\n"        \
			"where\n\tOp is + - x /\n"        \
			"\tNum#1 is the first argument\n" \
			"\tNum#2 is the second argument\n", argv[0]);

	return(f!=-1?0:f);
}
