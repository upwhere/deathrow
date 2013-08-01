#include "DeathRow.h"

static void _DRDestroyElement(deathrow*const element)
{
	free(element->datap);
	free(element);
}

static void _DRDestroyRow(deathrow*const element)
{
	if(!element)return;
	_DRDestroyRow(element->next);
	_DRDestroyElement(element);
}

void DRfor(deathrow*const row,void(*callback)(deathrow*const))
{
	DRfor(row->next,callback);
	callback(row->datap);
}

void DRDestroy(deathrow*const row)
{
	_DRDestroyRow(row);
}

/*@null@*/
static deathrow*_DRelement(void*const e,/*@null@*/deathrow*const n,unsigned int const space)
{
	deathrow a={
		.datap=e,
		.next=n,
		.space=space?space-1:0,
	},*p=malloc(sizeof*p);
	if(!p)
		return n;
	memcpy(p,&a,sizeof*p);
	return p;
}

deathrow*DRelement(void*const datapointer,unsigned int const space)
{
	return _DRelement(datapointer,NULL,space);
}

/*@null@*/
static deathrow*_ale(/*@null@*/deathrow*const r)
{
	// return the next-to-last element or r itself.
	return !r?NULL:(!r->next||!r->next->next)?r:_ale(r->next);
}

deathrow*DRadd(deathrow*q,void*const e)
{
	// cannot add to nonthing and cannot add nothing.
	if(!q||!e) return NULL;

	// no space left?
	if(q->space==0)
	{
		deathrow*const le=_ale(q);

		/*
			if the list was  0 elements in length, q was null (hopefully)
			if the list was  1 element  in length, ale returend q, and q/le should be freed.
			if the list was  2 elements in length, ale returned q, and q->next/le->next should be freed.
			if the list was  3 elements in length, ale returned q->next, and le->next. should be freed.
			if the list was >3 elements in length, ale returned q->...->next and le->next should be freed.
		
		*/

		// q :. le, no need to check for null dereferences here.
		// is last element?
		if(!(le)->next)
		{
			// remove the last element
			DRDestroy(le);
			// has no reference other than q;
			q=NULL;
		}
		// q :. le + ! le->next :. le->next->next + ! le->next->next->next
		else
		{
			// remove the last element
			DRDestroy((le)->next);
			// remove its reference
			(le)->next=NULL;
		}
	}

	q=_DRelement(e,q,(!!q&&(q->space>0))?q->space:0);
	// Disallow inserting
	if(q&&q->next)q->next->space=0;
	return q;
}

int DRcount(deathrow const*const row)
{
	return row->next?
		DRcount(row->next)+1:
		1;
}
