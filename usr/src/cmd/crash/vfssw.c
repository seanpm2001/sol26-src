#ident	"@(#)vfssw.c	1.4	93/05/28 SMI"		/* SVr4.0 1.1 */

/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 *		Copyright (C) 1991  Sun Microsystems, Inc
 *			All rights reserved.
 *		Notice of copyright on this source code
 *		product does not indicate publication.
 *
 *		RESTRICTED RIGHTS LEGEND:
 *   Use, duplication, or disclosure by the Government is subject
 *   to restrictions as set forth in subparagraph (c)(1)(ii) of
 *   the Rights in Technical Data and Computer Software clause at
 *   DFARS 52.227-7013 and in similar clauses in the FAR and NASA
 *   FAR Supplement.
 */

/*
 * This file contains code for the crash function vfssw.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/var.h>
#include <sys/fstyp.h>
#include <sys/vfs.h>
#include <sys/elf.h>
#include "crash.h"

Elf32_Sym *Nfstyp;		/* namelist symbol pointers */
Elf32_Sym *Vfssw;
extern Elf32_Sym *Vfs;

static void prvfssw();

/* get arguments for vfssw function */
int
getvfssw()
{
	int slot = -1;
	int full = 0;
	int phys = 0;
	long addr = -1;
	long arg1 = -1;
	long arg2 = -1;
	int c;
	int nfstypes;

	if (!Vfssw)
		if ((Vfssw = symsrch("vfssw")) == NULL)
			error("vfssw not found in symbol table\n");
	if (!Nfstyp)
		if ((Nfstyp = symsrch("nfstype")) == NULL)
			error("nfstyp not found in symbol table\n");

	optind = 1;
	while ((c = getopt(argcnt, args, "fpw:")) != EOF) {
		switch (c) {
			case 'f' :	full = 1;
					break;
			case 'p' :	phys = 1;
					break;
			case 'w' :	redirect();
					break;
			default  :	longjmp(syn, 0);
		}
	}
	readmem(Nfstyp->st_value, 1, -1, (char *)&nfstypes, sizeof (int),
		"number of file systems types");
	fprintf(fp, "FILE SYSTEM SWITCH TABLE SIZE = %d\n", nfstypes-1);
	if (full)
		fprintf(fp, "SLOT   NAME     FLAGS    INIT    VFSOPS\n");
	else
		fprintf(fp, "SLOT   NAME     FLAGS\n");
	if (args[optind]) {
		do {
			getargs(nfstypes, &arg1, &arg2, phys);
			if (arg1 == -1)
				continue;
			if (arg1 == 0) {
				fprintf(fp, "0 is out of range\n");
				continue;
			}
			if (arg2 != -1)
				for (slot = arg1; slot <= arg2; slot++)
					prvfssw(full, slot, phys, addr,
					    nfstypes);
			else {
				if ((unsigned long)arg1 < nfstypes)
					slot = arg1;
				else addr = arg1;
				prvfssw(full, slot, phys, addr, nfstypes);
			}
			slot = addr = arg1 = arg2 = -1;
		} while (args[++optind]);
	} else for (slot = 1; slot < nfstypes; slot++)
		prvfssw(full, slot, phys, addr, nfstypes);
	return (0);
}

/* print vfs switch table */
static void
prvfssw(full, slot, phys, addr, max)
int full, slot, phys, max;
long addr;
{
	struct vfssw vfsswbuf;
	char name[FSTYPSZ+1];

	readbuf(addr, (long)(Vfssw->st_value+slot*sizeof (vfsswbuf)), phys,
		-1, (char *)&vfsswbuf, sizeof (vfsswbuf),
		"file system switch table");
	if (vfsswbuf.vsw_name == NULL)
		return;
	if (addr > -1)
		slot = getslot(addr, (long)Vfssw->st_value, sizeof (vfsswbuf),
		    phys, max);
	readmem((unsigned)vfsswbuf.vsw_name, 1, -1, name, sizeof (name),
								"fs_name");
	if (name[0] == '\0')
		return;
	if (slot == -1)
		fprintf(fp, "  - ");
	else
		fprintf(fp, "%4d", slot);
	fprintf(fp, "   %-8s %3x", name, vfsswbuf.vsw_flag);
	if (full)
		fprintf(fp, "   %8x  %8x", vfsswbuf.vsw_init,
		    vfsswbuf.vsw_vfsops);
	fprintf(fp, "\n");
}