/*      inject_failure.c - inject_failure */
#include <xinu.h>

/* Lab 3: Simulates when disk crashes causing unacknowledged writes to be lost */


int32 inject_failure() {
	intmask mask;                   /* Saved interrupt mask         */
	mask = disable();

	struct  procent *prptr;
	
	// kill all ongoing processes 
	// Process pid 0 -- prnull
	// Process pid 1 -- rdsproc
	// Process pid 2 -- ipout
	// Process pid 3 -- netin
	for (int i = 4; i < NPROC; i++) {
                prptr = &proctab[i];
		if (prptr->prstate != PR_FREE && i != currpid) {
			kprintf("Process pid %d -- %s terminated.\n", i, prptr->prname);
			kill(i);
		}
        }

	// Clear the disk writes that have not been committed
	struct  dsecentry       *dsecptr;
	for (int i = 0; i < NSECTORS; i++) 
	{
		dsecptr = &dsectortab[i];
		strcpy(dsecptr->buffer, "");
	}

	// clear the FS structures that they write 
	fs_admin_format();

	restore(mask);
	return OK;	
}
