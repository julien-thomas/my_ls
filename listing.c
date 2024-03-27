#include "listing.h"
#include "sort.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <sys/types.h>

// écriture des droits d'accès sous forme de chaînes de caractères dans un tableau 'rwx'
void droit(const char *nom, char rwx[10], int dereference)
{
    // Permet de stocker les infos du fichier obtenues avec lstat
    struct stat file_stat;
    if (dereference == 0)
    {
        lstat(nom, &file_stat);
    }
    else
    {
        // show information for the file the link references rather than for the link itself
        stat(nom, &file_stat);
    }

    // initialisation du tableau avec des '-'
    memset(rwx, '-', 10);

    // détermination du type de fichier
    if (S_ISREG(file_stat.st_mode))
        rwx[0] = '-'; // fichier classique
    else if (S_ISDIR(file_stat.st_mode))
        rwx[0] = 'd'; // répertoire
    else if (S_ISLNK(file_stat.st_mode))
        rwx[0] = 'l'; // lien symbolique
    else if (S_ISCHR(file_stat.st_mode))
        rwx[0] = 'c'; // périphérique de type caractère
    else if (S_ISBLK(file_stat.st_mode))
        rwx[0] = 'b'; // périphérique de type bloc
    else if (S_ISFIFO(file_stat.st_mode))
        rwx[0] = 'p'; // fifo
    else if (S_ISSOCK(file_stat.st_mode))
        rwx[0] = 's'; // socket

    // refactorisation en utilisant masques binaires correspondant à chaque droit d'accès possible
    const mode_t modes[] = {
        S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
    // tableau de caractères représentant les droits dans le même ordre que les masques
    const char *permissions = "rwx";
    for (int i = 0; i < 9; ++i)
    {
        if (file_stat.st_mode & modes[i])
        {                                    // si défini => droit d'accès autorisé
            rwx[i + 1] = permissions[i % 3]; // écriture dans le tableau rwx du caractère correspondant dans le tableau permissions
        }
    }
}

// Function to list files in a directory
void list_files(const char *dirname, int include_hidden, int long_format, int exclude_dots, int recursive, int list_dir_only, int reverse, int time, int dereference)
{
    // open specified directory
    DIR *directory;
    directory = opendir(dirname);
    if (directory == NULL)
    {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    // init variables to go through the directory, to get file information and store file's paths
    struct dirent *entry;
    struct stat file_stat;
    char full_path[1024];

    // init a pointer array to store files's paths (for sorting)
    char **file_paths = NULL;
    int num_files = 0;
    int total_blocks = 0; // Variable pour stocker le total des blocs utilisés

    // Loop through directory entries
    while ((entry = readdir(directory)) != NULL)
    {
        // Check if entry is a hidden file and whether to include it
        if (entry->d_name[0] == '.')
        {
            if (!include_hidden)
            {
                continue; // Skip hidden files if include_hidden flag is not set
            }
            else if (exclude_dots && (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0))
            {
                continue; //  skip . and .. if almost-all is set
            }
        }

        // construct full path
        snprintf(full_path, 1024, "%s/%s", dirname, entry->d_name);

        // stat the file to get information
        if (lstat(full_path, &file_stat) == -1)
        {
            perror("Error getting file information");
            continue;
        }

        // allocation of memory to store new file path in the array
        file_paths = realloc(file_paths, (num_files + 1) * sizeof(char *));
        if (!file_paths)
        {
            perror("Error allocating memory");
            closedir(directory);
            exit(EXIT_FAILURE);
        }

        // duplication of the full path to store in the array
        file_paths[num_files] = strdup(full_path);

        // count number of files
        num_files++;

        // Add number of blocks of 512 bytes allocated for this file
        total_blocks += (int)(file_stat.st_blocks / 2);
    }

    // Close the directory after treating all entries
    closedir(directory);

    // Sort file paths array
    if (time == 0)
    {
        if (reverse == 1)
        {
            qsort(file_paths, num_files, sizeof(char *), compare_name_desc);
        }
        else
        {
            qsort(file_paths, num_files, sizeof(char *), compare_name_asc);
        }
    }
    else
    {
        if (reverse == 0)
        {
            qsort(file_paths, num_files, sizeof(char *), compare_time_desc);
        }
        else
        {
            qsort(file_paths, num_files, sizeof(char *), compare_time_asc);
        }
    }

    // print total of blocks
    if (long_format && num_files > 0)
    {
        printf("total %d\n", total_blocks);
    }

    // Loop through files paths array
    // and print file info
    for (int i = 0; i < num_files; i++)
    {
        const char *path = file_paths[i];

        if ((list_dir_only == 1) && !S_ISDIR(file_stat.st_mode))
        {
            continue; // Skip if only listing directories and it's not a directory
        }
        /* if (list_dir_only == 1) {
            if (!S_ISDIR(file_stat.st_mode)) {
                continue;
            } else {
                printf("%s\n", path);
            }
        } */
        if (long_format) // write a function for this
        // print details
        {
            char date[64];
            lstat(path, &file_stat);
            char rwx[10];
            droit(path, rwx, dereference);
            strftime(date, sizeof(date), "%b %d %H:%M", localtime(&file_stat.st_mtime));
            printf("%s %d %s %s %d %s %s \n", rwx, (int)file_stat.st_nlink, getpwuid(file_stat.st_uid)->pw_name, getpwuid(file_stat.st_gid)->pw_name, (int)file_stat.st_size, date, strrchr(path, '/') + 1);
        }
        else
        {
            // Print only the file/directory name
            printf("%s\n", strrchr(path, '/') + 1);
        }
    }

    // Recursively list subdirectories
    if (recursive == 1)
    {
        for (int i = 0; i < num_files; i++)
        {
            if (lstat(file_paths[i], &file_stat) == -1)
            {
                perror("lstat");
                continue;
            }
            if (S_ISDIR(file_stat.st_mode))
            {
                if (strcmp(file_paths[i], ".") != 0 && strcmp(file_paths[i], "..") != 0)
                {
                    printf("\n%s:\n", file_paths[i]);
                    list_files(file_paths[i], include_hidden, long_format, exclude_dots, recursive, list_dir_only, reverse, time, dereference);
                }
            }
        }
    }
    // Free allocated memory
    for (int i = 0; i < num_files; i++)
    {
        free(file_paths[i]);
    }
    free(file_paths);
}