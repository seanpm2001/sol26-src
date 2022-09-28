/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)tcsetpgrp.c	1.6	92/07/14 SMI"	/* SVr4.0 1.1	*/

#ifdef __STDC__
	#pragma weak tcsetpgrp = _tcsetpgrp
#endif
#include "synonyms.h"
#include <sys/termios.h>
#include <sys/types.h>

tcsetpgrp(fd,pgrp)
int fd;
pid_t pgrp;
{
	if (tcgetsid(fd) < 0)
		return -1;
	return ioctl(fd,TIOCSPGRP,&pgrp);
}