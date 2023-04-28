#ifndef CTYPE_H
#define CTYPE_H
#include <types.h>

int isdigit(int c);
int isspace(int c);
int isupper(int c);
int islower(int c);
int isalpha(int c);
int isalnum(int c);
int isprint(int c);
int isxdigit(int c);
int iscntrl(int c);
int isgraph(int c);
int ispunct(int c);
int tolower(int c);
int toupper(int c);

#endif