/* fs.h: filesystem */

#ifndef NBLOCKS
#define NBLOCKS		50	/* Total number of blocks, equals NSECTORS */
#endif

#ifndef BLOCK_SIZE
#define	BLOCK_SIZE	512
#endif

#define F_NAMLEN 	128	/* Maximum length of file name  */
#define F_REGULAR	5	/* File type: Regular file	*/
#define	F_DIRECTORY	6	/* File type: Directory		*/
#define	B_RESERVED	-1	/* Reseved blocks: e.g. superblock, bitmap */
#define	NDIRECT		10	/* Number of direct blocks of the file */


#define MAX_DIR_ENTRIES_PER_BLOCK (BLOCK_SIZE / sizeof(struct file))
#define FS_SUPER_MAGICNUMBER 0xDEADBEFF
#define MAX_JOURNAL_OPS 10


#define FS_IOCTL_BLOCK_CACHE 7	/* Enable block cache 				*/
#define	BLOCK_CACHE_SIZE 5 	/* Cache size, unit: number of blocks 		*/
#define FS_IOCTL_CRASH_RECOVERY	8 /* Enable journaling for crash recovery 	*/
#define FS_IOCTL_LOCALITY	9 /* Enable minimization of fragmentation, increase access locality */

extern struct	super	superblock;

struct indirectblk {
	blkid32	f_direct[BLOCK_SIZE/4]; /* Holds up to 128 additional block numbers */
};

struct file	{
	int	ftype;			/* File type: regular file or directory */
	char	fname[F_NAMLEN];	/* File name	*/
	int	fsize;			/* File size	*/

	blkid32	f_direct[NDIRECT];	/* Block numbers of file's direct data blocks */
	blkid32	f_indirect;		/* Indirect data block (if applicable given the file size) */
};

// Each data block of a directory file conains multiple file entries
struct directory_datablk {
	struct file dir_entries[MAX_DIR_ENTRIES_PER_BLOCK];
};

#define JENTRY_TYPE_WRITE 0
#define JENTRY_TYPE_START 1
#define JENTRY_TYPE_END 2

// A jentry consists of an entry of the type "WRITE(block, data), "START TX" or "END TX"
// For a write entry:
//   "block" is stored in the "struct jentry"
//   "data" is stored in the respective journal data block, associated with
//          this jentry (Nth jentry corresponds to the Nth journal data block)
struct jentry {
	int valid;
	int type; //
	blkid32 block;
};

struct super 	{
	int beginning;
	int end;
	int magicnumber;
	int total_free_blocks;

	int journal_enabled;
	// Each jentry is associated with one of the 10 data blocks reserved for the journal.
	struct jentry journal_entries[MAX_JOURNAL_OPS];

	struct file root_file;	/* Root file directory */
};

struct blkcache	{
	int 	enabled;
	blkid32 cache[BLOCK_CACHE_SIZE];
};
