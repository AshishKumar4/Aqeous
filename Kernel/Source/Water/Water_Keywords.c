
#include "Water/Water.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "common.h"

uintptr_t WaterKeyword_func(Water_Token_t* TokenList)
{
  return (uintptr_t)TokenList;
}

uintptr_t WaterKeyword_int(Water_Token_t* TokenList)
{
  Water_Token_t* tmpToken = TokenList;
  tmpToken = tmpToken->NextToken;
  Water_Var_t* var = Water_NewVarObj_h(((Water_BasicTypeToken_t*)tmpToken)->name, WATER_INT, tmpToken->hash);
  Water_Token_t* varTok = (Water_Token_t*)Water_GetBasicTypeToken_vh(tmpToken->hash);

  varTok->BackToken = TokenList->BackToken;
  TokenList->BackToken->NextToken = varTok;
  TokenList = (Water_Token_t*)varTok;
  return (uintptr_t)varTok;
}

uintptr_t WaterKeyword_float(Water_Token_t* TokenList)
{
  return (uintptr_t)TokenList;
}

uintptr_t WaterKeyword_str(Water_Token_t* TokenList)
{
  return (uintptr_t)TokenList;
}

uintptr_t WaterKeyword_print(Water_Token_t* TokenList)
{
  Water_Token_t* tmpToken = TokenList;
  tmpToken = tmpToken->NextToken;
  for(;tmpToken;)
  {
    if(tmpToken->type & WATER_INT)
    {
      printf("Result >>%d<<", ((Water_BasicTypeToken_t*)tmpToken)->var->int_buffer[0]);
    }
    else if(tmpToken->type & WATER_STR)
    {
      //printf("%s", (char*)&((Water_BasicTypeToken_t*)tmpToken)->var->int_buffer[0]);
    }
    else if(tmpToken->type & WATER_OPERATOR)
    {
      if(strncmp(((Water_OperatorToken_t*)tmpToken)->op->name, ".", 1))
      {
        ////printf
      }
      else
      {

      }
    }
    tmpToken = tmpToken->NextToken;
  }
  return TokenList;
}
