/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)ffs.c	1.5	92/07/14 SMI"	/* SVr4.0 1.3	*/

#ifdef __STDC__
	#pragma weak ffs = _ffs
#endif

#include "synonyms.h"

int
ffs(field)
int field;
{
	int	idx=1;

	if (field == 0)
		return(0);
	for (;;) {
		if (field & 1)
			return(idx);
		field >>= 1;
		++idx;
	}
}