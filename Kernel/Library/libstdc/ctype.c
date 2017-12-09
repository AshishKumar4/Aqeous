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

int isalnum(char c)
{
  return ((c <= 'z' && c >= 'a')||
          (c <= 'Z' && c >= 'Z') ||
          (c <= '9' && c >= '0'));
}

int isalpha(char c)
{
  return ((c <= 'z' && c >= 'a')||
          (c <= 'Z' && c >= 'Z'));
}

int isdigit(char c)
{
  return ((c <= '9' && c >= '0'));
}

int isnumeric(char c)
{
  return ((c <= '9' && c >= '0'));
}
