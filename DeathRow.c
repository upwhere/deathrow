#include "DeathRow.h"

struct _deathrow
{
	deathrow base;
	void(*deepfree)(void*);
};

static void _DRDestroyElement(deathrow*const element)
{
	((struct _deathrow*)element)->deepfree(element->datap);
	free(element);
}

void DRfor(deathrow*const row,void(*callback)(deathrow*const))
{
	DRfor(row->next,callback);
	callback(row->datap);
}

void DRDestroy(deathrow*const head)
{
	if(!head)return;
	DRDestroy(head->next);
	_DRDestroyElement(head);
}

/*@null@*/
static deathrow*_DRelement(void*const e,/*@null@*/deathrow*const n,unsigned int const space,void(*deepfree)(void*))
{
	struct _deathrow a={
		.base={
			.datap=e,
			.next=n,
			.space=space?space-1:0,
		},
		.deepfree=deepfree,
	},*p=malloc(sizeof*p);
	if(!p)
		return n;
	memcpy(p,&a,sizeof*p);
	return (deathrow*)p;
}

deathrow*DRelement(void*const datapointer,unsigned int const space,void(*deepfree)(void*))
{
	return _DRelement(datapointer,NULL,space,deepfree);
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
	{

		void(*df)(void*)=((struct _deathrow*)q)->deepfree;
	
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
			if(!le->next)
			{
				// remove the last element
				DRDestroy(q);
				// has no reference other than q;
				q=NULL;
	
			}
			// q :. le + ! le->next :. le->next->next + ! le->next->next->next
			else
			{
				// remove the last element
				DRDestroy(le->next);
				// remove its reference
				le->next=NULL;
			}
		}

		q=_DRelement(e,q,!!q?q->space:0,df);
	}
	// Disallow inserting
	if(!!q&&!!q->next)q->next->space=0;
	return q;
}

int DRcount(deathrow const*const row)
{
	return row->next?
		DRcount(row->next)+1:
		1;
}
