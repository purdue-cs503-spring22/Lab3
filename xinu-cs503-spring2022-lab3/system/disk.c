/*      disk.c - disk */
#include <xinu.h>

/* Lab 3: Disk Simulator */

struct  dsecentry dsectortab[NSECTORS];

int32 disk() {
	/* Initialize disk blocks */
	int32	i;
	struct	dsecentry	*dsecptr;		/* Ptr to disk block entry	*/

	/* Initialize disk block table entries 	*/
	for (i = 0; i < NSECTORS; i++) {
		dsecptr = &dsectortab[i];
		dsecptr->dsecnum = i;
		dsecptr->bufmutex = semcreate(1); 
		//kprintf("DEBUG: creating semaphore %d\n", dsecptr->bufmutex); 
	}

	return OK;	
}


/*------------------------------------------------------------------------
 *  get_disk_size  -  Return the disk size in number of sectors.
 *------------------------------------------------------------------------
 */

int32 get_disk_size() {
        return NSECTORS;
}

/*------------------------------------------------------------------------
 *  read_sector.c   -  Returns a sector from disk
 *                     Writing its contents to sector_out
 *------------------------------------------------------------------------
 */

int32 read_sector(int32 sector_number, char * sector_out) {
        intmask mask;                   /* Saved interrupt mask         */
        mask = disable();
        if (isbadsecnum(sector_number)) {
                restore(mask);
                return SYSERR;
        }

        struct  dsecentry       *dsecptr;               /* Ptr to disk block entry      */
        dsecptr = &dsectortab[sector_number];
        // Writing the contents to sector_out
	memcpy(sector_out, dsecptr->dsector, SECTOR_SIZE);

        restore(mask);
        return OK;
}

/*------------------------------------------------------------------------
 *  write_sector  -  Schedules a write of a sector to disk.
 *------------------------------------------------------------------------
 */

int32 write_sector(int32 sector_number, char * sector_in) {
         intmask mask;                   /* Saved interrupt mask         */
         mask = disable();
         if (isbadsecnum(sector_number)) {
                restore(mask);
                return SYSERR;
         }

        struct  dsecentry       *dsecptr;               /* Ptr to disk block entry      */
        dsecptr = &dsectortab[sector_number];
        // Writing the contents to buffer
        wait(dsecptr->bufmutex);
	memcpy(dsecptr->buffer, sector_in, SECTOR_SIZE);        
        signal(dsecptr->bufmutex);

        restore(mask);
        return OK;
}

/*------------------------------------------------------------------------
 *  flush  -  Ensures that unacknowledged writes are written to the disk.
 *------------------------------------------------------------------------
 */


int32 flush() {
        intmask mask;                   /* Saved interrupt mask         */
        mask = disable();
        int32 i;

        struct  dsecentry       *dsecptr;       /* Ptr to disk block entry*/
        for (i = 0; i < NSECTORS; i++) {
                dsecptr = &dsectortab[i];
                // flush is not atomic
        	memcpy(dsecptr->dsector, dsecptr->buffer, SECTOR_SIZE);
        }

  	restore(mask);
  	return OK;
}
