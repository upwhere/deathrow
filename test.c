#include "DeathRow.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef NROFTESTS
#define NROFTESTS 5
#endif

bool testDRcount(void)
{
	int i;
	printf("### Testing DRcount ###\n\n");
	for(i=1;i<NROFTESTS;i++)
	{
		int*data=malloc(sizeof*data);
		int j=0;
		deathrow*d=DRelement(data,i);
		printf("TEST %d\t starts with  %d/%d elements\n",i,DRcount(d),i);
		for(j=2;j<i+5;j++)
		{
			int counted,*data=malloc(sizeof*data);
			d=DRadd(d,data);
			counted=DRcount(d);
			printf("\t increased to %d/%d elements\n",counted,i);
			if(j<i&&counted!=j)
			{
				printf("\n!!! count (%d) is off !!!\n\n",j);
				DRDestroy(d);
				return false;
			}
			if(j==i&&counted!=j)
			{
				printf("\n!!! cannot fill !!!\n\n");
			}
			if(j>i&&counted!=i)
			{
				printf("\n!!! overflow detected !!!\n\n");
				DRDestroy(d);
				return false;
			}
		}
		DRDestroy(d);
	}
	return true;
}

int main(void)
{
	int passes=0,fails=0;
	testDRcount()?passes++:fails++;
	printf("%d out of %d tests passed\n",passes,passes+fails);
	return !fails?EXIT_SUCCESS:EXIT_FAILURE;
}
