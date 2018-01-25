#include "stdio.h"
#include "string.h"
#include "Hashtree.h"
#include "stdlib.h"
//#include "math.h"
#include "stdint.h"

uintptr_t HashBranch_Lookup(uintptr_t Hashmap, uint_fast32_t entry)
{
  uintptr_t tmp = ((uint_fast32_t*)Hashmap)[entry+1];
  if(tmp) return tmp;
  tmp = malloc(4096);
  ((uint_fast32_t*)Hashmap)[entry+1] = tmp;
  return tmp;
}

uintptr_t HashTree_Lookup(uintptr_t HashTree, const char* str, int sz)
{
  uintptr_t tmp = HashTree;
  for(int i =0; i < sz; i++)
  {
    tmp = HashBranch_Lookup(tmp, (uint_fast32_t)(str[i] - 'a'));
  }
  return ((uint_fast32_t*)tmp);
}

void main()
{
  char str[100];
  uint_fast32_t ny = 1;
  uint_fast32_t tmp2;
  uintptr_t ht = malloc(4096);
  while(ny)
  {
    printf("\n\nEnter a string: ");
    scanf("%s", str);
    uintptr_t tmp = HashTree_Lookup(ht, str, strlen(str));

    printf("\nValue: %d", *((uint_fast32_t*)tmp));
    printf("\nEnter new value: ");
    scanf("%d", &tmp2);
    *((uint_fast32_t*)tmp) = tmp2;
    scanf("%d", &ny);
  }
}
