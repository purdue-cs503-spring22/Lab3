/*  disk.h      */
  
/* Lab 3: This simulates the functionality of disk hardware */

// The disk size is guaranteed to have 
// at least 32 sectors and less than 128.
#ifndef NSECTORS
#define NSECTORS	50		/* Number of disk sectors, if not defined */
#endif

#define SECTOR_SIZE 	512		/* Each sector has fixed size 256 */

/* Disk sector entry */
struct	dsecentry	{
	int32	dsecnum;		/* Sector Number		*/
	char	dsector[SECTOR_SIZE];		/* A sector of size SECTOR_SIZE	*/
	char 	buffer[SECTOR_SIZE];			/* Buffer for the current sector*/
					/* Buffer Lost during crash	*/
	sid32   bufmutex;       	/* Mutex Lock for write buffer	*/
};

extern  struct  dsecentry dsectortab[];

#define isbadsecnum(b)     ((int32)(b) < 0 || (b) >= NSECTORS)


