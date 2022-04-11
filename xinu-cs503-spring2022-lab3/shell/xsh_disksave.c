/* xsh_flush.c - xsh_flush */
  
#include <xinu.h>

shellcmd xsh_flush(int nargs, char *args[])
{

        /* For argument '--help', emit help about the 'memstat' command */

        if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
                printf("use: %s \n\n", args[0]);
                printf("Description:\n");
                printf("\tCommit the current write to the disk\n");
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

        flush();

        return 0;
}
