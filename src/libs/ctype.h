#ifndef CTYPE_H
#define CTYPE_H

int isdigit(int c)
{
    return c >= '0' && c <= '9';
}
int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
}
int isupper(int c)
{
    return c >= 'A' && c <= 'Z';
}
int islower(int c)
{
    return c >= 'a' && c <= 'z';
}
int isalpha(int c)
{
    return isupper(c) || islower(c);
}
int isalnum(int c)
{
    return isalpha(c) || isdigit(c);
}
int isprint(int c)
{
    return c >= ' ' && c <= '~';
}
int isxdigit(int c)
{
    return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}
int iscntrl(int c)
{
    return (unsigned)c < ' ' || c == 127;
}
int isgraph(int c)
{
    return c > ' ' && c <= '~';
}
int ispunct(int c)
{
    return isgraph(c) && !isalnum(c);
}
int tolower(int c)
{
    return isupper(c) ? c + 'a' - 'A' : c;
}
int toupper(int c)
{
    return islower(c) ? c + 'A' - 'a' : c;
}

#endif