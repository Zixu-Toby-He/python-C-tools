#include <stdio.h>
#include <stdlib.h>

struct
{
	double t;
	int i0,j0;
	int i1,j1;
	int i2,j2;
}
mass=
{
	80e0,
	16,17,
	44,54,
	44,54
};
int run(void)
{
	char p[1024];
	int a;
	a=snprintf(p,1024,"mass.t  = %.2f\n",mass.t);
	printf(p);
	putchar('\n');
	printf("a = %i\n",a);
	printf("mass.i0 = %i, mass.j0 =%i\n",mass.i0,mass.j0);
	printf("mass.i1 = %i, mass.j1 =%i\n",mass.i1,mass.j1);
	printf("mass.i2 = %i, mass.j2 =%i\n",mass.i2,mass.j2);
	putchar('\n');
	a=0;
	while (a<10)
	{
		printf("\t%i",a);
		a++;
	}
	putchar('\n');
	system("pause");
	return 0;
}