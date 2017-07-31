#include "stdio.h"
#include "stdlib.h"
#include "common.h"
#include "string.h"
#include "Shell.h"
#include "Water.h"
#include "ctype.h"
#include "OperationFuncs.c"
#include "Water_Keywords.c"

uintptr_t Water_alloc(uint32_t sz)
{
  return kmalloc(sz);
}

void Water_free(uintptr_t ptr)
{
  kfree(ptr);
}

void Water_MemManager()
{
  while(1)
  {
    //TODO: Keep a finite memory cached for Water_alloc to use and replenish it
    //      whenever it gets full. Also manage clearing up memory using water_free.
  }
}

uint32_t Water_GenerateHash(const char* str, int size)
{
  uint32_t tmp = 0;
  for(int i = 0 ; i < size; i++)				// Hash Function
  {
     if((uint32_t)str[i] >= 97)
        tmp += (((uint32_t)str[i]) - 97)*(i+1);
     else
        tmp += (((uint32_t)str[i]) - 65)*(i+1);
  }
  return tmp;
}


uint32_t Water_GenerateOpHash(const char* str, int size)
{
  uint32_t tmp = 0;
  for(int i = 0; i < size; i++)
  {
    tmp += ((uint32_t)str[i])*(i+1);
  }
  return tmp;
}

inline Water_KeywordToken_t* Water_GetKeywordToken_hash(uint32_t hash)
{
  return (Water_KeywordToken_t*)HashTable_keywords[hash];
}

inline Water_Var_t* Water_GetVar_hash(uint32_t hash)
{
  return (Water_Var_t*)HashTable_Vars[hash];
}

inline Water_OperatorObject_t* Water_GetOpObj_hash(uint32_t hash, uint_fast32_t* HashTable)
{
  return (Water_OperatorObject_t*)HashTable[hash];
}

inline void Water_SetKeywordToken_hash(uint32_t hash, uintptr_t val)
{
  HashTable_keywords[hash] = val;
}

inline void Water_SetVar_hash(uint32_t hash, uintptr_t val)
{
  HashTable_Vars[hash] = val;
}

inline void Water_SetOpObj_hash(uint32_t hash, uintptr_t val, uint_fast32_t* HashTable)
{
  HashTable[hash] = val;
}

Water_BasicTypeToken_t* Water_NumTypeGenerate(const char* str, int sz)
{
  /** FOR INT **/
  Water_BasicTypeToken_t* obj = (Water_BasicTypeToken_t*)Water_alloc(sizeof(Water_BasicTypeToken_t) + sizeof(Water_Var_t) + 4);
  obj->type = WATER_NUMERICAL | WATER_INT | WATER_BASICTYPE;
  obj->TypeClass = Water_INT_TypeClass;

  strncpy(obj->name, str, sz);
  uint32_t _int = StrToInt(obj->name);
//  printf("\n%d ", _int);

  obj->var = (Water_Var_t*)&obj->custom;
  Water_Var_t* var = obj->var;

  var->buffer_sz = 4;
  var->ext_buffer = NULL;
  var->int_buffer[0] = _int;
  return obj;
}

Water_BasicTypeToken_t* Water_StrTypeGenerate(const char* str, int sz)
{
  uint32_t sz4 = ROUNDUP(sz, 4);
  uint32_t n = sizeof(Water_BasicTypeToken_t) + sz4 + 4 + sizeof(Water_Var_t);
  Water_BasicTypeToken_t* obj = (Water_BasicTypeToken_t*)Water_alloc(n);
  obj->type = WATER_STR | WATER_BASICTYPE;
  obj->TypeClass = Water_STR_TypeClass;

  strncpy(obj->name, str, sz);
//  printf("\n%s ", obj->name);
  obj->var = (Water_Var_t*)&obj->custom;
  Water_Var_t* var = obj->var;

  var->buffer_sz = sz4;
  var->ext_buffer = NULL;
  char* tmp = (char*)&var->int_buffer[0];
  strncpy(tmp, str, sz);
  return obj;
}

Water_BasicTypeToken_t* Water_BuildFuncValue(Water_BasicTypeToken_t* func)
{
  return NULL;
}

int Water_SetFuncParam(Water_BasicTypeToken_t* func, const char* param)
{
  return 0;
}

uintptr_t Water_INT_generate()
{
  Water_Var_t* obj = (Water_Var_t*)Water_alloc(sizeof(Water_Var_t));
  obj->type = WATER_NUMERICAL | WATER_INT | WATER_BASICTYPE;
  obj->TypeClass = Water_INT_TypeClass;

  obj->buffer_sz = 4;
  obj->ext_buffer = NULL;
  obj->int_buffer[0] = 0;
  return (uintptr_t)obj;
}

uintptr_t Water_STR_generate()
{
  //uint32_t sz4 = ROUNDUP(sz, 4);
  uint32_t n = sizeof(Water_Var_t);// + sz4 + 4;
  Water_Var_t* obj = (Water_Var_t*)Water_alloc(n);
  obj->type = WATER_STR | WATER_BASICTYPE;
  obj->TypeClass = Water_STR_TypeClass;

  obj->buffer_sz = 4;
  obj->ext_buffer = NULL;
  return (uintptr_t)obj;
}


Water_Var_t* Water_NewVarObj(const char* str, uint32_t type)
{
  func_ptr_t func = BasicType_Generators[type];
  Water_Var_t* var = (Water_Var_t*)func();
  strcpy(var->name, str);
  uint32_t hash = Water_GenerateHash(str, strlen(str));

  HashTable_Vars[hash] = (uintptr_t)var;
  return var;
}

Water_Var_t* Water_NewVarObj_h(const char* str, uint32_t type, uint32_t hash)
{
  func_ptr_t func = BasicType_Generators[type];
  Water_Var_t* var = (Water_Var_t*)func();
  strcpy(var->name, str);
  HashTable_Vars[hash] = (uintptr_t)var;
  return var;
}

Water_OperatorObject_t* Water_NewOperatorObj(const char* name, uint32_t str_len, uint32_t priority, uint32_t associativity, Water_Token_t* (*Func)(uintptr_t), uint_fast32_t* HashTable)
{
    uint32_t opHash = Water_GenerateOpHash(name, str_len);

    Water_OperatorObject_t* obj = (Water_OperatorObject_t*)Water_alloc(sizeof(Water_OperatorObject_t));
    memset((void*)obj, 0, sizeof(Water_OperatorObject_t));
    obj->hash = opHash;
    obj->type = WATER_OPERATOR;
    obj->Function = Func;
    obj->priority = priority;
    obj->associativity = associativity;
    strncpy(obj->name, name, str_len);
  //  printf("=>[%s; %x]", obj->name, obj->type);
    HashTable[opHash] = (uintptr_t)obj;
    return obj;
}


Water_KeywordToken_t* Water_NewKeyword(const char* str, uint32_t strln, uint32_t type, uintptr_t (*func)(Water_Token_t*), uint32_t parameters)
{
  Water_KeywordToken_t* tmp = Water_alloc(sizeof(Water_KeywordToken_t));
  strncpy(tmp->name, str, strln);
  tmp->type = type | WATER_KEYWORD;
  tmp->hash = Water_GenerateHash(tmp->name, strln);
  tmp->func = func;
  tmp->parameters = parameters;
  HashTable_keywords[tmp->hash] = (uintptr_t)tmp;
  return tmp;
}

Water_OperatorToken_t* Water_GetOpToken(const char* str, int sz, uint_fast32_t* HashTable)
{
  uint32_t opHash = Water_GenerateOpHash(str, sz);
  Water_OperatorObject_t* opObj = (Water_OperatorObject_t*)HashTable[opHash];
//  if(!opClass) return 0;
  Water_OperatorToken_t* obj = (Water_OperatorToken_t*)Water_alloc(sizeof(Water_OperatorToken_t));
  obj->op = opObj;
  obj->hash = opHash;
  obj->type = opObj->type;
  printf("[%s, %d, %s]", obj->op->name, opHash, ((Water_OperatorObject_t*)HashTable[Water_GenerateOpHash("=", 1)])->name);
  return obj;
}

Water_BasicTypeToken_t* Water_GetBasicTypeToken_vh(uint32_t hash)
{
  Water_BasicTypeToken_t* obj = (Water_BasicTypeToken_t*)Water_alloc(sizeof(Water_BasicTypeToken_t));
  //obj->hash = Water_GenerateHash(str, strlen(str));
  obj->hash = hash;
  obj->var = (Water_Var_t*)HashTable_Vars[obj->hash];
  if(!obj->var) return obj;
  obj->type = obj->var->type;
  obj->TypeClass = obj->var->TypeClass;
  return obj;
}

Water_OperatorToken_t* Water_GetOpToken_vh(uint32_t hash, uint_fast32_t* HashTable)
{
  Water_OperatorObject_t* opObj = (Water_OperatorObject_t*)HashTable[hash];
//  if(!opClass) return 0;
  Water_OperatorToken_t* obj = (Water_OperatorToken_t*)Water_alloc(sizeof(Water_OperatorToken_t));
  obj->op = opObj;
  obj->hash = hash;
  obj->type = opObj->type;
//  printf("[%s; %x]", obj->name, obj->type);
  return obj;
}

Water_BasicTypeToken_t* Water_GenerateBasicTypeobj(const char* str, uint32_t type)
{
  Water_Var_t* var = Water_NewVarObj(str, type);
  return Water_GetBasicTypeToken_vh(var->hash);
}

int Water_KeywordHandler(Water_Token_t* TokenStart)   // If any keywords, Execute them
{
  Water_Token_t* tmpToken = TokenStart;
  tmpToken = tmpToken->NextToken;
  for(;tmpToken;)
  {
    if(tmpToken->type & WATER_KEYWORD)
    {
      Water_KeywordToken_t* tmp = (Water_KeywordToken_t*)tmpToken;
      tmp->func(tmpToken);
    }
    tmpToken = tmpToken->NextToken;
  }
}

uint32_t Water_KeywordHandlerDirect(Water_Token_t* TokenStart, const char* str)
{
  Water_Token_t* tmpToken = TokenStart;
  Water_KeywordToken_t* tmp = (Water_KeywordToken_t*)tmpToken;
  uint32_t t = Water_GenTokenList(str, tmp->parameters, TokenStart);
  tmp->func(tmpToken);
  printf("\n{%d}", t);
  return t;
}

Water_BasicTypeToken_t* Water_TempVarGen(uint32_t type)
{
  /** FOR INT **/
  Water_BasicTypeToken_t* obj = (Water_BasicTypeToken_t*)Water_alloc(sizeof(Water_BasicTypeToken_t) + sizeof(Water_Var_t) + 4);
  obj->type = WATER_NUMERICAL | WATER_INT | WATER_BASICTYPE;
  obj->TypeClass = Water_INT_TypeClass;

  strncpy(obj->name, "_tmp_", 5);
  uint32_t _int = 0;
//  printf("\n%d ", _int);

  obj->var = (Water_Var_t*)&obj->custom;
  Water_Var_t* var = obj->var;

  var->buffer_sz = 4;
  var->ext_buffer = NULL;
  var->int_buffer[0] = _int;
  return obj;
}

uint32_t Water_GenTokenList(const char* str, uint32_t nToks, Water_Token_t* TokenStart)
{
  char* tmpStr = str;
  Water_Token_t* tmpToken = TokenStart;
  Water_Token_t* EndToken = tmpToken->NextToken;
  printf("\n[%s]", tmpStr);

  uint32_t nt = 0;
  int ln = 0;

  for(;nt < nToks;)
  {
    if(isalpha(tmpStr[0]))    // If First letter is alphabet
    {
      int i = 0;
      for(; isalnum(tmpStr[i]); i++)      // Continue until u encounter space/operator
      {
      }
      uint32_t hash = Water_GenerateHash(tmpStr, i);    // Generate Hash
      Water_Token_t* token = (Water_Token_t*)Water_GetKeywordToken_hash(hash);   // Check the Type => Int/Float and create the struc

      if(token)    // If the token is a Keyword
      {
      //  printf("\t Is a Keyword");
        tmpToken->NextToken = token;    // Put the Token into the Token List.
        token->BackToken = tmpToken;
        token->NextToken = EndToken;
        tmpStr += i;
        ln += i;
        ++nt;
        tmpStr += Water_KeywordHandlerDirect(token, tmpStr);
        tmpToken = token->BackToken->NextToken;
      }
      else  // Must be a Variable or a Function
      {
        token = (Water_Token_t*)Water_GetBasicTypeToken_vh(hash);   // Get the token for variable/function
        if(!((Water_BasicTypeToken_t*)token)->var)  // Must be a Variable
        {
        //  printf("\n%s Variable/Function not defined!", tmpStr);
        //  return 1;
          strncpy(((Water_BasicTypeToken_t*)token)->name, tmpStr, i);
        }
        else if(token->type & WATER_FUNCTION)  // If function
        {
          if(tmpStr[i] == '(')    // Get the Parameters
          {
            char* param = &tmpStr[i];
            for(; tmpStr[i] != ')'; i++)
            {
            }
            ++i;
            Water_SetFuncParam((Water_BasicTypeToken_t*)token, param);    //  Store the Function Parameters in its structure
          }
          else
          {
            printf("\nError, Function called as Variable");
          }
        }

        tmpToken->NextToken = token;    // Put the Token into the Token List.
        token->BackToken = tmpToken;
        token->NextToken = EndToken;
        tmpToken = token;
        tmpStr += i;
        ln += i;
        ++nt;
      }
    }
    else if(isdigit(tmpStr[0])) // Is a Numerical
    {
      int i = 0;
      for(; isnumeric(tmpStr[i]); i++)      // Continue until u encounter space/operator
      {
      }
      // Create the Type Struct
      Water_Token_t* token = (Water_Token_t*)Water_NumTypeGenerate(tmpStr, i);   // Check the Type => Int/Float and create the struc
      if(!token)
      {
        printf("\n%s Unknown Number Size", tmpStr);
        return 2;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;

      tmpStr += i;
      ln += i;
      ++nt;
    }
    else if(tmpStr[0] == '\"')    // Must be a String Datatype
    {
      int i = 1;
      ++tmpStr;
      for(; tmpStr[i] != '\"'; i++)   // Wait for the String to End
      {
      }

      Water_Token_t* token =  (Water_Token_t*)Water_StrTypeGenerate(tmpStr, i);    // Generate the String type token
      if(!token)
      {
        printf("\n%s String type Error", tmpStr);
        return 3;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;

      /*printf("%s, %s",(char*) &((Water_BasicTypeToken_t*)tmpToken)->var->int_buffer[0], ((Water_BasicTypeToken_t*)tmpToken)->var->name);
      while(1);*/
      tmpStr += i+1;
      ln += i+1;
      ++nt;
    }
    else if(tmpStr[0] == '(')   // A Round Bracket means another arithmatic/logical expression
    {
      Water_Token_t* token;
      int i = 0, c = 1;
      ++tmpStr;
      for(;c; i++)  // Get the complete expression
      {
        if(tmpStr[i] == '(')
          ++c;
        else if(tmpStr[i] == ')')
          --c;
      }
      --i;
      tmpStr[i] = '\0';
      token = Water_MakeTokenListSY(tmpStr, tmpToken);   //  Execute the Internal Expression RECURSIVELY
      ++tmpStr;
      if(!token)
      {
        printf("\n%s Internal Expression Error", tmpStr);
        return 4;
      }
      /*
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;*/
      tmpToken = token;
      ln += i;
      ++nt;
      tmpStr += i;
    }
    else if(tmpStr[0] != ' ')   // Must be an Operator
    {
      int i = 0;
      for(; !isalnum(tmpStr[i]) && tmpStr[i] != ' ' && tmpStr[i] != '('; i++)
      {
      }
      char tt[100];
      strncpy(tt, tmpStr, i);

      printf("\n{%s}", tt);
      // Get the Token
      Water_Token_t* token = (Water_Token_t*)Water_GetOpToken(tmpStr, i, ((Water_BasicTypeToken_t*)tmpToken)->TypeClass->OperatorTable);   //  Execute the Internal Expression RECURSIVELY

      if(!token)
      {
        printf("\n%s Operator Type Error", tmpStr);
        return 4;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;

      ln += i;
      tmpStr += i;
      ++nt;
    }
    else  // Must be a Space
    {
      tmpStr[0] = '\0';
      ++tmpStr;
      ++ln;
    }
  }
  return ln;
}


/************************* Water Initialization *****************************/

void Create_BasicTypeClasses()
{
  Water_INT_TypeClass = (Water_BasicTypeClass_t*)Water_alloc(sizeof(Water_BasicTypeClass_t));
  Water_STR_TypeClass = (Water_BasicTypeClass_t*)Water_alloc(sizeof(Water_BasicTypeClass_t));

  uint32_t hash = Water_GenerateHash("int", 3);
  Water_INT_TypeClass->TemplateHash = hash;
  Water_INT_TypeClass->TypeSize = 4;
  Water_INT_TypeClass->OperatorTable = (uintptr_t)kmalloc(4096);
  uint_fast32_t* tb = (uint_fast32_t*)Water_INT_TypeClass->OperatorTable;

  Water_NewOperatorObj("=", 1, 1, 0, (Water_Token_t * (*)(uintptr_t))Water_opEqual, tb);
  Water_NewOperatorObj("-", 1, 4, 0, (Water_Token_t * (*)(uintptr_t))Water_opMinus, tb);
  Water_NewOperatorObj("+", 1, 4, 0, (Water_Token_t * (*)(uintptr_t))Water_opPlus, tb);
  Water_NewOperatorObj("*", 1, 8, 0, (Water_Token_t * (*)(uintptr_t))Water_opMul, tb);
  Water_NewOperatorObj("/", 1, 8, 0, (Water_Token_t * (*)(uintptr_t))Water_opDiv, tb);
  Water_NewOperatorObj("^", 1, 12, 1, (Water_Token_t * (*)(uintptr_t))Water_opExp, tb);
  Water_NewOperatorObj("(", 1, 16, 0, (Water_Token_t * (*)(uintptr_t))Water_RoundBracketO, tb);
  Water_NewOperatorObj(")", 1, 16, 0, (Water_Token_t * (*)(uintptr_t))Water_RoundBracketC, tb);
  Water_NewOperatorObj("[", 1, 2, 0, (Water_Token_t * (*)(uintptr_t))Water_RoundBracketO, tb);
  Water_NewOperatorObj("]", 1, 2, 0, (Water_Token_t * (*)(uintptr_t))Water_RoundBracketC, tb);
  Water_NewOperatorObj("{", 1, 2, 0, (Water_Token_t * (*)(uintptr_t))Water_RoundBracketO, tb);
  Water_NewOperatorObj("}", 1, 2, 0, (Water_Token_t * (*)(uintptr_t))Water_RoundBracketC, tb);
  Water_NewOperatorObj(".", 1, 3, 0, (Water_Token_t * (*)(uintptr_t))Water_RoundBracketC, tb);
  Water_NewOperatorObj("->", 2, 3, 0, (Water_Token_t * (*)(uintptr_t))Water_RoundBracketC, tb);
}

void Water_init()
{
  Working_Table = (uintptr_t*)pmem_4k(2);

  Create_BasicTypeClasses();

  // = (uintptr_t*)pmem_4k(1);
  HashTable_keywords = (uintptr_t*)pmem_4k(2);
  HashTable_Vars = (uintptr_t*)pmem_4k(2);



  BasicType_Generators = (func_ptr_t*)pmem_4k(1);
  BasicType_Generators[WATER_INT] = Water_INT_generate;
  BasicType_Generators[WATER_STR] = Water_STR_generate;

  OpStackBottom = (Water_OperatorToken_t**)pmem_4k(1);
  OpStackTop = OpStackBottom;
  *OpStackTop = NULL;

  ValStackBottom = (Water_BasicTypeToken_t**)pmem_4k(1);
  ValStackTop = ValStackBottom;
  *ValStackTop = NULL;

  OutputQueueStart = (Water_Token_t**)pmem_4k(1);
  OutputQueueEnd = OutputQueueStart;

  Water_NewKeyword("int", 3, WATER_INT, WaterKeyword_int, 1);
  Water_NewKeyword("print", 5, WATER_IO, WaterKeyword_print, 1);
}

/***************************** Shell Process *****************************/
void Water_shell()
{
  shell_harbor = 1;
  _ctrl_C_pressed = 0;
  Water_init();
  shell_awake = 0;
  while(shell_awake);
  while(1)
  {
    printf("\n%g%s%g$>>%g",13,"Water",10,15);
    while(!shell_in)
    {
      asm volatile("int $50");
    }
    asm volatile("cli");

    if(_ctrl_C_pressed)
    {
      _ctrl_C_pressed = 0;
      shell_harbor = 0;
      backspace();
      Shell_wakeup();
      kill();
    }

    /******* MAIN PART ********/
    int error = Water_Execute(shell_buf);
    /********** END **********/

    printf("\n...");
    if(error)
    {
      printf("\nError %d %s Occured", error, Water_GetErrorName(error));
    }
    *Shell_Istream = (uint32_t)shell_buf;
    ++Shell_Istream;
    shell_in = 0;
    shell_buf = 0;
    tmpIstream = 0;
  }
}

/*************************** BACKEND Abstraction Layer ************************/

int Water_Execute(const char* sentence)
{
  uintptr_t opArr[32];

  printf("\n{%u}", *HPET_main_counter);
/*
  Water_Token_t* tree = Water_ExecuteRemoteExp(sentence, strlen(sentence));
  printf("\n\t==>{%d}", ((Water_BasicTypeToken_t*)tree)->var->int_buffer[0]);
//*/
//  printf("\n{%u}", *HPET_main_counter);

  Water_ExecuteRemoteExpSY(sentence, strlen(sentence));

  printf("\n{%u}", *HPET_main_counter);

  OutputQueueEnd = OutputQueueStart;
  return 0;
}

Water_Token_t* Water_ExecuteRemoteExp(const char* sentence, int size)   // Similar to Water_Execute, used to execute remote expressions recursively
{
  Water_Token_t* TokenStart = (Water_Token_t*)Water_alloc(sizeof(Water_Token_t));
  TokenStart->NextToken = NULL;//TokenEnd;   // Create a Doubly linked Linked List
  TokenStart->hash = 0x4284;

  Water_MakeTokenList(sentence, TokenStart);
  Water_KeywordHandler(TokenStart);   // If any keywords, Execute them
  Water_Token_t* tok;

  tok = Water_ArithmaticSolver(TokenStart);

  //TODO: Solve the TokenList using Arithmatic Solver and RETURN THE RESULTING TOKEN
  return tok;
}


Water_Token_t* Water_ExecuteRemoteExpSY(const char* sentence, int size)   // Similar to Water_Execute, used to execute remote expressions recursively
{
  Water_Token_t* TokenStart = (Water_Token_t*)Water_alloc(sizeof(Water_Token_t));
  Water_Token_t* TokenEnd = (Water_Token_t*)Water_alloc(sizeof(Water_Token_t));
  TokenStart->NextToken = TokenEnd;   // Create a Doubly linked Linked List
  TokenEnd->BackToken = TokenStart;
  Water_MakeTokenListSY(sentence, TokenStart);
//  Water_KeywordHandler(TokenStart);   // If any keywords, Execute them
  Water_Token_t* tok;

  tok = Water_ShuntingYardSolver(TokenStart);

  //TODO: Solve the TokenList using Arithmatic Solver and RETURN THE RESULTING TOKEN
  return tok;
}


int Water_MakeTokenList(const char* sentence, Water_Token_t* TokenStart)
{
  int SentenceSize = strlen(sentence);
  char* tmpStr = (char*)kmalloc(SentenceSize+2);
  strcpy(tmpStr, sentence);
  tmpStr[SentenceSize] = '\0';

  char tmp;

  Water_Token_t* tmpToken = TokenStart;   // Start Token i always defined as well as End Token
  Water_Token_t* EndToken = NULL;// = tmpToken->NextToken;

  for(;tmpStr[0] != '\0';)
  {
    if(isalpha(tmpStr[0]))    // If First letter is alphabet
    {
      int i = 0;
      for(; isalnum(tmpStr[i]); i++)      // Continue until u encounter space/operator
      {
      }

      uint32_t hash = Water_GenerateHash(tmpStr, i);    // Generate Hash
      Water_Token_t* token = (Water_Token_t*)Water_GetKeywordToken_hash(hash);   // Check if its a keyword

      if(token)    // If the token is a Keyword
      {
      //  printf("\t Is a Keyword");
        tmpToken->NextToken = token;    // Put the Token into the Token List.
        token->BackToken = tmpToken;
        token->NextToken = EndToken;
        tmpToken = token;
        tmpStr += i;
        tmpStr += Water_KeywordHandlerDirect(token, tmpStr);
      }
      else  // Must be a Variable or a Function
      {
        token = (Water_Token_t*)Water_GetBasicTypeToken_vh(hash);   // Get the token for variable/function
        if(!((Water_BasicTypeToken_t*)token)->var)  // Must be a Variable
        {
          printf("\n%s Variable/Function not defined!", tmpStr);
        //  return 1;
          strncpy(((Water_BasicTypeToken_t*)token)->name, tmpStr, i);
        }
        else if(token->type & WATER_FUNCTION)  // If function
        {
          if(tmpStr[i] == '(')    // Get the Parameters
          {
            char* param = &tmpStr[i];
            for(; tmpStr[i] != ')'; i++)
            {
            }
            ++i;
            Water_SetFuncParam((Water_BasicTypeToken_t*)token, param);    //  Store the Function Parameters in its structure
          }
          else
          {
            printf("\nError, Function called as Variable");
          }
        }

        tmpToken->NextToken = token;    // Put the Token into the Token List.
        token->BackToken = tmpToken;
        token->NextToken = EndToken;
        tmpToken = token;
        tmpStr += i;
      }
    }
    else if(isdigit(tmpStr[0])) // Is a Numerical
    {
      int i = 0;
      for(; isnumeric(tmpStr[i]); i++)      // Continue until u encounter space/operator
      {
      }
      // Create the Type Struct
      Water_Token_t* token = (Water_Token_t*)Water_NumTypeGenerate(tmpStr, i);   // Check the Type => Int/Float and create the struc
      if(!token)
      {
        printf("\n%s Unknown Number Size", tmpStr);
        return 2;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;

      tmpStr += i;
    }
    else if(tmpStr[0] == '\"')    // Must be a String Datatype
    {
      int i = 1;
      ++tmpStr;
      for(; tmpStr[i] != '\"'; i++)   // Wait for the String to End
      {
      }

      Water_Token_t* token =  (Water_Token_t*)Water_StrTypeGenerate(tmpStr, i);    // Generate the String type token
      if(!token)
      {
        printf("\n%s String type Error", tmpStr);
        return 3;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;

      /*printf("%s, %s",(char*) &((Water_BasicTypeToken_t*)tmpToken)->var->int_buffer[0], ((Water_BasicTypeToken_t*)tmpToken)->var->name);
      while(1);*/
      tmpStr += i+1;
    }
    else if(tmpStr[0] == '(')   // A Round Bracket means another arithmatic/logical expression
    {
      Water_Token_t* token;
      int i = 0, c = 1;
      ++tmpStr;
      for(;c; i++)  // Get the complete expression
      {
        if(tmpStr[i] == '(')
          ++c;
        else if(tmpStr[i] == ')')
          --c;
      }
      --i;
      tmpStr[i] = '\0';
      token = Water_ExecuteRemoteExp(tmpStr, i);   //  Execute the Internal Expression RECURSIVELY
      ++tmpStr;
      if(!token)
      {
        printf("\n%s Internal Expression Error", tmpStr);
        return 4;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;

      tmpStr += i;
    }
    else if(tmpStr[0] != ' ')   // Must be an Operator
    {
      int i = 0;
      for(; !isalnum(tmpStr[i]) && tmpStr[i] != ' ' && tmpStr[i] != '('; i++)
      {
      }

      // Get the Token
      Water_Token_t* token = NULL;
      if(((Water_BasicTypeToken_t*)tmpToken)->TypeClass)
      {
        token = (Water_Token_t*)Water_GetOpToken(tmpStr, i, ((Water_BasicTypeToken_t*)tmpToken)->TypeClass->OperatorTable);
      }
      else
      {
        printf("\nError No literal before operator");
        return;
      }

      if(!token)
      {
        printf("\n%s Operator Type Error", tmpStr);
        return 4;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;

      tmpStr += i;
    }
    else  // Must be a Space
    {
      tmpStr[0] = '\0';
      ++tmpStr;
    }
  }
  /*for(;TokenStart;)
  {
    printf("\n%x", TokenStart->hash);
    TokenStart = TokenStart->NextToken;
  }*/
  return 0;
}


int Water_MakeTokenListSY(const char* sentence, Water_Token_t* TokenStart)
{
  int SentenceSize = strlen(sentence);
  char* tmpStr = (char*)kmalloc(SentenceSize+2);
  strcpy(tmpStr, sentence);
  tmpStr[SentenceSize] = '\0';
  //printf("\n[%s]", tmpStr);

  char tmp;

  Water_Token_t* tmpToken = TokenStart;   // Start Token i always defined as well as End Token
  Water_Token_t* EndToken = tmpToken->NextToken;

  for(;tmpStr[0] != '\0';)
  {
    if(isalpha(tmpStr[0]))    // If First letter is alphabet
    {
      int i = 0;
      for(; isalnum(tmpStr[i]); i++)      // Continue until u encounter space/operator
      {
      }
      uint32_t hash = Water_GenerateHash(tmpStr, i);    // Generate Hash
      Water_Token_t* token = (Water_Token_t*)Water_GetKeywordToken_hash(hash);   // Check the Type => Int/Float and create the struc

      if(token)    // If the token is a Keyword
      {
        printf("\t Is a Keyword");
        tmpToken->NextToken = token;    // Put the Token into the Token List.
        token->BackToken = tmpToken;
        token->NextToken = EndToken;
        tmpStr += i;
        tmpStr += Water_KeywordHandlerDirect(token, tmpStr);
        tmpToken = token->BackToken->NextToken;
      }
      else  // Must be a Variable or a Function
      {
        token = (Water_Token_t*)Water_GetBasicTypeToken_vh(hash);   // Get the token for variable/function
        if(!((Water_BasicTypeToken_t*)token)->var)  // Must be a Variable
        {
          printf("\n%s Variable/Function not defined!", tmpStr);
        //  return 1;
          strncpy(((Water_BasicTypeToken_t*)token)->name, tmpStr, i);
        }
        else if(token->type & WATER_FUNCTION)  // If function
        {
          if(tmpStr[i] == '(')    // Get the Parameters
          {
            char* param = &tmpStr[i];
            for(; tmpStr[i] != ')'; i++)
            {
            }
            ++i;
            Water_SetFuncParam((Water_BasicTypeToken_t*)token, param);    //  Store the Function Parameters in its structure
          }
          else
          {
            printf("\nError, Function called as Variable");
          }
        }

        tmpToken->NextToken = token;    // Put the Token into the Token List.
        token->BackToken = tmpToken;
        token->NextToken = EndToken;
        tmpToken = token;
        tmpStr += i;
      }
    }
    else if(isdigit(tmpStr[0])) // Is a Numerical
    {
      int i = 0;
      for(; isnumeric(tmpStr[i]); i++)      // Continue until u encounter space/operator
      {
      }
      // Create the Type Struct
      Water_Token_t* token = (Water_Token_t*)Water_NumTypeGenerate(tmpStr, i);   // Check the Type => Int/Float and create the struc
      if(!token)
      {
        printf("\n%s Unknown Number Size", tmpStr);
        return 2;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;

      tmpStr += i;
    }
    else if(tmpStr[0] == '\"')    // Must be a String Datatype
    {
      int i = 1;
      ++tmpStr;
      for(; tmpStr[i] != '\"'; i++)   // Wait for the String to End
      {
      }

      Water_Token_t* token =  (Water_Token_t*)Water_StrTypeGenerate(tmpStr, i);    // Generate the String type token
      if(!token)
      {
        printf("\n%s String type Error", tmpStr);
        return 3;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;

      /*printf("%s, %s",(char*) &((Water_BasicTypeToken_t*)tmpToken)->var->int_buffer[0], ((Water_BasicTypeToken_t*)tmpToken)->var->name);
      while(1);*/
      tmpStr += i+1;
    }
    else if(tmpStr[0] == '(' || tmpStr[0] == ')')   // A Round Bracket means another arithmatic/logical expression
    {
      Water_Token_t* token;
      token = (Water_Token_t*)Water_GetOpToken(tmpStr, 1, ((Water_BasicTypeToken_t*)tmpToken)->TypeClass->OperatorTable);   //  Execute the Internal Expression RECURSIVELY
      if(!token)
      {
        printf("\n%s Internal Expression Error", tmpStr);
        return 4;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;
      ++tmpStr;
    }
    else if(tmpStr[0] != ' ')   // Must be an Operator
    {
      int i = 0;
      for(; !isalnum(tmpStr[i]) && tmpStr[i] != ' ' && tmpStr[i] != '('; i++)
      {
      }
      char tt[100];
      strncpy(tt, tmpStr, i);

      printf("\n{%s}", tt);
      // Get the Token
      Water_Token_t* token = NULL;
      if(((Water_BasicTypeToken_t*)tmpToken)->TypeClass)
      {
        token = (Water_Token_t*)Water_GetOpToken(tmpStr, i, ((Water_BasicTypeToken_t*)tmpToken)->TypeClass->OperatorTable);
      }
      else
      {
        printf("\nError No literal before operator");
        return;
      }
      if(!token)
      {
        printf("\n%s Operator Type Error", tmpStr);
        return 4;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;

      tmpStr += i;
    }
    else  // Must be a Space
    {
      tmpStr[0] = '\0';
      ++tmpStr;
    }
  }
  return 0;
}


Water_Token_t* Water_ArithmaticSolver(Water_Token_t* TokenStart)
{
  // TODO: Solve a given sentence using that array of BasicTypes.
  Water_OperatorToken_t* tree = NULL;

  tree = (Water_OperatorToken_t*)Water_OperatorTreeGenerate(TokenStart);
  if(!tree) return TokenStart->NextToken;
  tree = Water_ReduceOpTree(tree);

  return (Water_Token_t*)tree;
}


Water_OperatorToken_t* Water_ShowTree(Water_OperatorToken_t* tree)
{
  if(tree->RHS->type & WATER_OPERATOR)   // Need to solve recursively
  {
    Water_ShowTree((Water_OperatorToken_t*)tree->RHS);
  }
  if(tree->LHS->type & WATER_OPERATOR)   // Need to solve recursively
  {
    Water_ShowTree((Water_OperatorToken_t*)tree->LHS);
  }
  printf("[%s]->",tree->op->name);
  return tree;
}


Water_Token_t* Water_OperatorTreeGenerate(Water_Token_t* TokenStart)
{
  Water_Token_t* tmpToken = TokenStart;

  Water_OperatorToken_t* tree = NULL, *tmpOp, *tmpOp2, *lastOp = NULL;

  Water_BasicTypeToken_t* lastBasicType = NULL;

  tmpToken = tmpToken->NextToken; // Skip the Start Token

  for(;tmpToken;)
  {
    //printf("\n---<>");
    if(tmpToken->type & WATER_OPERATOR)
    {
      lastOp = (Water_OperatorToken_t*)tmpToken;
    //  printf("\n[%s]", lastOp->name);
      lastOp->LHS = (Water_Token_t*)lastBasicType; // Put last variable/func/numerical as LHS
      if(!lastBasicType)
      {
        lastOp->LHS = (Water_Token_t*)TemporaryTokensL[WATER_INT];
        printf("\nPANIC");
      }
      if(!tree) // No tree before existed, create one
      {
        tree = lastOp;
        tree->parent = NULL;
      }
      else
      {
        tmpOp = lastOp;
        tmpOp2 = tree;
        //printf("\n");
        for(;tmpOp2;)
        {
        //  printf("<%s>=>", tmpOp2->name);
          if((tmpOp2->op->priority < tmpOp->op->priority) || (tmpOp->op->associativity && tmpOp2->op->priority == tmpOp->op->priority))
          {
              //Move on to the RHS node of the tree
              if(!(tmpOp2->RHS->type & WATER_OPERATOR) || !tmpOp2->RHS) // If end of tree is reached
              {
                //Put it here in the end
                tmpOp2->RHS = (Water_Token_t*)tmpOp;
                tmpOp->parent = (Water_Token_t*)tmpOp2;
                tmpOp->op->inherit = 1;
                break;
              }
              else
              {
                tmpOp2 = (Water_OperatorToken_t*)tmpOp2->RHS;
              }
          }
          else
          {
            // Put the tmpOp2 node to tmpOp's LHS and put tmpOp in its place
            if(tmpOp2->parent)
              ((Water_OperatorToken_t*)tmpOp2->parent)->RHS = (Water_Token_t*)tmpOp;
            else
            {
              tree = tmpOp;
            //  printf("\n<-%s, %s->", tree->name, tmpOp2->name);
              tmpOp->parent = NULL;
            }
            //printf("\t%s, %s", tmpOp2->name, tmpOp->name);
            tmpOp->parent = tmpOp2->parent;
            tmpOp2->parent = (Water_Token_t*)tmpOp;
            tmpOp->LHS = (Water_Token_t*)tmpOp2;
            tmpOp2->op->inherit = 0;
            break;
          }
        }
      //  Water_ShowTree(tree);
      }
    }
    else if(tmpToken->type & WATER_BASICTYPE)
    {
    //  printf("\n->%d %x, %s", ((Water_BasicTypeObject_t*)tmpToken)->int_buffer[0], tmpToken, ((Water_BasicTypeObject_t*)tmpToken)->name);
      lastBasicType = (Water_BasicTypeToken_t*)tmpToken;
      lastOp->RHS = (Water_Token_t*)tmpToken;
    }
    else if(tmpToken->type & WATER_FUNCTION)
    {
      lastBasicType  = Water_BuildFuncValue((Water_BasicTypeToken_t*)tmpToken); // Derive the value of the type (Get value for Variable/Numerical, Solve for Function)
    }
    tmpToken = tmpToken->NextToken;
  }
  //printf("\n{%s}", tree->name);

  return (Water_Token_t*)tree;
}


Water_BasicTypeToken_t* Water_ReduceOpTree(Water_OperatorToken_t* treeStart)
{
  if(treeStart->RHS->type & WATER_OPERATOR)   // Need to solve recursively
  {
    Water_Token_t* RHS = (Water_Token_t*)Water_ReduceOpTree((Water_OperatorToken_t*)treeStart->RHS);
    treeStart->RHS = RHS;
  }
  if(treeStart->LHS->type & WATER_OPERATOR)   // Need to solve recursively
  {
    Water_Token_t* LHS = (Water_Token_t*)Water_ReduceOpTree((Water_OperatorToken_t*)treeStart->LHS);
    treeStart->LHS = LHS;
  }
  //printf("\n[%s]", treeStart->name);
  return treeStart->op->Function(treeStart);
}

void Water_PushOpStack(Water_OperatorToken_t* op)
{
  ++OpStackTop;
  *OpStackTop = op;
//  printf("{%s}", (*OpStackTop)->name);
}

Water_OperatorToken_t* Water_PopOpStack()
{
  Water_OperatorToken_t* tmp = *OpStackTop;
  if(OpStackTop != OpStackBottom)
    --OpStackTop;
  else return NULL;
//  printf("[-->%s<--]", (*OpStackTop)->name);
  return tmp;
}

void Water_PushValStack(Water_BasicTypeToken_t* Val)
{
  ++ValStackTop;
  *ValStackTop = Val;
  //printf("{%s}", (*ValStackTop)->name);
}

Water_BasicTypeToken_t* Water_PopValStack()
{
  Water_BasicTypeToken_t* tmp = *ValStackTop;
  if(ValStackTop != ValStackBottom)
    --ValStackTop;
  else return NULL;
  //printf("[-->%s<--]", (*ValStackTop)->name);
  return tmp;
}

void Water_PushQueue(Water_Token_t* tok)
{
  //if(OutputQueueStart == OutputQueueEnd)
  *OutputQueueEnd = tok;
  ++OutputQueueEnd;
}

Water_Token_t* Water_PopQueue()
{
  if(OutputQueuePtr != OutputQueueEnd)
  {
    Water_Token_t* tmp = *OutputQueuePtr;
    ++OutputQueuePtr;
    return tmp;
  }
  return NULL;
}

void Water_QueueRewind()
{
  OutputQueuePtr = OutputQueueEnd = OutputQueueStart;
}

Water_Token_t* Water_ShuntingYardSolver(Water_Token_t* TokenStart)
{
  Water_QueueRewind();
  Water_Token_t* tmpToken = TokenStart;
  tmpToken = tmpToken->NextToken; // Skip the start Token
  for(;tmpToken->NextToken;)
  {
    if(tmpToken->type & WATER_OPERATOR)
    {
      printf("\n=>%s",((Water_OperatorToken_t*)tmpToken)->op->name);
      if(!strcmp(((Water_OperatorToken_t*)tmpToken)->op->name, "("))  //Token is left Bracket
      {
      //  printf("-->Left Bracket");
        Water_PushOpStack(tmpToken);
      }
      else if(!strcmp(((Water_OperatorToken_t*)tmpToken)->op->name, ")"))
      {
        //printf("-->Right Bracket");
        Water_Token_t* tmp;
        while(strcmp((*OpStackTop)->op->name, "("))
        {
          tmp = (Water_Token_t*)Water_PopOpStack();
        //  if(!tmp) break;
          Water_PushQueue(tmp);
          //printf("<%s>", ((Water_OperatorObject_t*)tmp)->name);
        }
        Water_PopOpStack();
      }
      else
      {
        Water_Token_t* tmp;
        while((*OpStackTop)->op->priority != 16 && ((*OpStackTop)->op->priority > ((Water_OperatorToken_t*)tmpToken)->op->priority || ((*OpStackTop)->op->priority == ((Water_OperatorToken_t*)tmpToken)->op->priority && !((Water_OperatorToken_t*)tmpToken)->op->associativity)) )
        {
          tmp = (Water_Token_t*)Water_PopOpStack();
          if(!tmp) break;
          Water_PushQueue(tmp);
          //printf("-b-");
        }
        //Push the read operator onto the operator stack.
        Water_PushOpStack(tmpToken);
      }
    }
    else  // Function/Variable/Numerical
    {
      printf("\n%s",((Water_BasicTypeToken_t*)tmpToken)->name);
      // Push to Output Queue
      Water_PushQueue(tmpToken);
    }
    tmpToken = tmpToken->NextToken;
  }
  Water_Token_t* tmp;
  while((*OpStackTop))
  {
    tmp = (Water_Token_t*)Water_PopOpStack();
    if(!tmp) break;
    Water_PushQueue(tmp);
    //printf("a-");
  }
  tmp = Water_EvaluateShuntingYard();
  printf("\n\t===>{%d}", ((Water_BasicTypeToken_t*)tmp)->var->int_buffer[0]);
  return;
}

Water_Token_t* Water_EvaluateShuntingYard()
{
  Water_Token_t* tmp = Water_PopQueue();
  Water_OperatorToken_t* opTmp;
  while(tmp)
  {
    if(tmp->type & WATER_OPERATOR)
    {
      opTmp = tmp;
      //printf("\n====>%s", opTmp->name);
      opTmp->RHS = (Water_Token_t*)Water_PopValStack();
      opTmp->LHS = (Water_Token_t*)Water_PopValStack();
      Water_PushValStack(opTmp->op->Function(opTmp));
    }
    else
    {
    //  printf("\n<{%s}>", ((Water_BasicTypeObject_t*)tmp)->name);
      Water_PushValStack(tmp);
    }
    tmp = Water_PopQueue();
  }
  return (Water_Token_t*)Water_PopValStack();
}

uintptr_t Water_OpGetClass(uint32_t hash)
{
  return NULL;
}

char* Water_GetErrorName(int error)
{
  switch(error)
  {
    case 1:
      break;
    default: return "Unknown Error";
  }
  return NULL;
}
