#ifndef LISTING_H
#define LISTING_H

void list_files(const char *dirname, int include_hidden, int long_format, int exclude_dots, int recursive, int list_dir_only, int reverse, int time, int dereference);

void droit(const char *nom, char rwx[10], int dereference);

#endif /* LISTING_H */