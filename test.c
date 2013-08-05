#include "DeathRow.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef NROFTESTS
#define NROFTESTS 5
#endif

static void _freedeepdata(void*deepdata)
{
	struct _deepdata
	{
		int s,*sa;
	};
	printf("\tFreeing deep data @ %p\n",deepdata);
	free(((struct _deepdata*)deepdata)->sa);
	free(deepdata);
}

static bool _testdeepfree(void)
{
	int i;
	struct _deepdata
	{
		int s,*sa;
	};
	printf("### Testing deep frees ###\n\n");
	for(i=1;i<NROFTESTS;i++)
	{
		int j=0;
		deathrow*q;
		{
			struct _deepdata s={
				.s=j,
				.sa=malloc(sizeof(int)*5),
			},*p=malloc(sizeof*p);
			printf("Test %d:",i);
			if(!p){
				printf("out of memory\n");
				continue;
			}
			memcpy(p,&s,sizeof*p);
			q=DRelement(p,(unsigned)i,&_freedeepdata);
		}
		for(j=2;j<i+5;j++)
		{
			struct _deepdata s={
				.s=j,
				.sa=malloc(sizeof(int)*5),
			},*p=malloc(sizeof*p);
			if(!p)
			{
				printf("out of memory\n");
				break;
			}
			memcpy(p,&s,sizeof*p);
			q=DRadd(q,(void*)p);
		}
		DRDestroy(q);
	}
	return true;
}

static bool _testDRcount(void)
{
	int i;
	printf("### Testing DRcount ###\n\n");
	for(i=1;i<NROFTESTS;i++)
	{
		int j=0;
		deathrow*d;
		{
			int*data=malloc(sizeof*data*5);
			if(!data)
			{
				printf("out of memory!\n");
				continue;
			}
			d=DRelement(data,(unsigned)i,&free);
			if(!d)
			{
				printf("out of memory!\n");
				continue;
			}
		}
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
	_testdeepfree()?passes++:fails++;
	_testDRcount()?passes++:fails++;
	printf("%d out of %d tests passed\n",passes,passes+fails);
	return !fails?EXIT_SUCCESS:EXIT_FAILURE;
}
