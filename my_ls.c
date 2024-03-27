#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <sys/types.h>
#include "sort.h"
#include "listing.h"

int main(int argc, char *argv[])
{

    // Variables to track options
    int include_hidden = 0;
    int long_format = 0;
    int recursive = 0;
    int exclude_dots = 0;
    int list_dir_only = 0;
    int reverse = 0;
    int time = 0;
    int dereference =0;
    char *dirname = NULL;

    // Parse command-line options
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            // Process options
            for (size_t j = 1; j < strlen(argv[i]); j++)
            {
                switch (argv[i][j])
                {
                case 'l':
                    long_format = 1;
                    break;
                case 'R':
                    recursive = 1;
                    break;
                case 'r':
                    reverse = 1;
                    break;
                case 'd':
                    list_dir_only = 1;
                    break;
                case 't':
                    time = 1;
                    break;
                case 'a':
                    include_hidden = 1;
                    break;
                case 'A':
                    exclude_dots = 1;
                    include_hidden = 1;
                    break;
                case 'L':
                    // handling option -L
                    dereference = 1;
                    break;
                default:
                    fprintf(stderr, "Unknown option: -%c\n", argv[i][j]);
                    exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            // Set the directory name if not an option
            dirname = argv[i];
        }
    }

    // If directory name is not provided, use current directory
    if (dirname == NULL)
    {
        dirname = ".";
    }

    // List files in the specified directory
    list_files(dirname, include_hidden, long_format, exclude_dots, recursive, list_dir_only, reverse, time, dereference);

    return 0;
}
