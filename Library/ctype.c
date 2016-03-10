#include <ctype.h>

char toupper(char a)
{
    if(a>=97&&a<=122)
        return a-32;
    else return a;
}
char tolower(char a)
{
    if(a>=65&&a<=91)
        return a+32;
    else return a;
}
