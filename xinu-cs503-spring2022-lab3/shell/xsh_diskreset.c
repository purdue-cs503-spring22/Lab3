/* xsh_diskreset.c - xsh_diskreset */
  
#include <xinu.h>

shellcmd xsh_diskreset(int nargs, char *args[])
{

        /* For argument '--help', emit help about the 'memstat' command */

        if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
                printf("use: %s \n\n", args[0]);
                printf("Description:\n");
                printf("\tReset the simulated disk\n");
                printf("Options:\n");
                printf("\t--help\t\tdisplay this help and exit\n");
                return 0;
        }

        /* Check for valid number of arguments */

        if (nargs > 1) {
                fprintf(stderr, "%s: too many arguments\n", args[0]);
                fprintf(stderr, "Try '%s --help' for more information\n",
                                args[0]);
                return 1;
        }

	// Clear the disk sectors
        struct  dsecentry       *dsecptr;
        for (int i = 0; i < NSECTORS; i++)
        {
                dsecptr = &dsectortab[i];
                strcpy(dsecptr->dsector, "");
        }


        return 0;
}
