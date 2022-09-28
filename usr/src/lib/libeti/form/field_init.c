/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)field_init.c	1.2	92/07/14 SMI"	/* SVr4.0 1.1	*/

#include "utility.h"

	/*******************
	*  set_field_init  *
	*******************/

int set_field_init (f, func)
FORM * f;
PTF_void func;
{
	Form (f) -> fieldinit = func;
	return E_OK;
}

PTF_void field_init (f)
FORM * f;
{
	return Form (f) -> fieldinit;
}
