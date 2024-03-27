# my_ls Utility

This command-line utility lists the files and directories in a specified directory, with various options for customization.

## Options

The utility accepts the following command-line options:

- -l: Display long-format listing.
- -R: Recursively list subdirectories.
- -r: Reverse the order of listing.
- -d: List directories only, not their contents.
- -t: Sort files by modification time.
- -a: Include hidden files in the listing.
- -A: Exclude dots (.) and dots with preceding slashes (/) from the listing.
- -L: Dereference symbolic links when listing files.

## Usage

./my_ls [options] [directory]

### Compilation

To compile the program, use the following command:

```bash
gcc main.c sort.c listing.c -o my_ls
or:
make all


