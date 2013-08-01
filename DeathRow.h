#ifndef DeathRow_H
#define DeathRow_H

#include <stdlib.h>
#include <string.h>

typedef struct Deathrow
{
	void*
	datap;
	struct Deathrow*next;
	unsigned int space;
	
} deathrow;

/*@-exportlocal@*/

void DRDestroy(deathrow*const);

/*@null@*/
deathrow*DRelement(void*const,unsigned int const,void(*)(void*));
/*@null@*/
deathrow*DRadd(deathrow*const,void*const);

void DRfor(deathrow*const,void(*)(deathrow*const));
//void DRfor(deathrow*const,void(*)(deathrow*const),void*);

int DRcount(deathrow const*const);

/*@+exportlocal@*/
#endif
