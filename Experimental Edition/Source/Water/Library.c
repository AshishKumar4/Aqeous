#include "stdio.h"
#include "stdlib.h"
#include "common.h"
#include "string.h"
#include "Shell.h"
#include "Water.h"
#include "ctype.h"


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
