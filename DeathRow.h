#ifndef DeathRow_H
#define DeathRow_H

typedef struct Deathrow
{
	void*
	datap;
	struct Deathrow*next;
	unsigned int space;
	
} deathrow;

/*@-exportlocal@*/

void DRDestroy(/*@only@*/deathrow*const);

/*@null@*/
/*@only@*/
deathrow*DRelement(void*const,unsigned int const,void(*)(/*@only@*/void*));

/*@null@*/
/*@only@*/
deathrow*DRadd(/*@only@*/deathrow*const,/*@keep@*/void*const);

void DRfor(/*@temp@*/deathrow*const,void(*)(deathrow*const));
//void DRfor(deathrow*const,void(*)(deathrow*const),void*);

int DRcount(/*@temp@*/deathrow const*const);

/*@+exportlocal@*/
#endif
