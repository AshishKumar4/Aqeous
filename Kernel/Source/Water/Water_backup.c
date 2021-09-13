#include "stdio.h"
#include "stdlib.h"
#include "common.h"
#include "string.h"
#include "Shell/shell.h"
#include "Water.h"
#include "ctype.h"
#include "OperationFuncs.c"

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

Water_Keyword_t* Water_GetKeywordToken_hash(uint32_t hash)
{
  return (Water_Keyword_t*)HashTable_keywords[hash];
}

Water_BasicTypeObject_t* Water_GetTypeObjToken_hash(uint32_t hash)
{
  return (Water_BasicTypeObject_t*)HashTable_BasicTypes[hash];
}

int Water_SetFuncParam(Water_BasicTypeObject_t* func, const char* param)
{
  return 0;
}

Water_BasicTypeObject_t* Water_NumTypeCheckAndGenerate(const char* str, int sz)
{
  /** FOR INT **/
  Water_BasicTypeObject_t* obj = (Water_BasicTypeObject_t*)Water_alloc(sizeof(Water_BasicTypeObject_t));
  obj->type = WATER_NUMERICAL | WATER_INT | WATER_BASICTYPE;
  obj->TypeClass = Water_INT_TypeClass;

  strncpy(obj->name, str, sz);
  uint32_t _int = StrToInt(obj->name);
//  printf("\n%d ", _int);

  obj->buffer_sz = 4;
  obj->ext_buffer = NULL;
  obj->int_buffer[0] = _int;
  return obj;
}

Water_BasicTypeObject_t* Water_StrTypeGenerate(const char* str, int sz)
{
  uint32_t sz4 = ROUNDUP(sz, 4);
  uint32_t n = sizeof(Water_BasicTypeObject_t) + sz4 + 4;
  Water_BasicTypeObject_t* obj = (Water_BasicTypeObject_t*)Water_alloc(n);
  obj->type = WATER_STR | WATER_BASICTYPE;
  obj->TypeClass = Water_STR_TypeClass;

  strncpy(obj->name, str, sz);
//  printf("\n%s ", obj->name);

  obj->buffer_sz = sz;
  obj->ext_buffer = NULL;
  char* tmp = (char*)&obj->int_buffer[0];
  strncpy(tmp, str, sz);

  return obj;
}

Water_OperatorObject_t* Water_OpGetToken(const char* str, int sz)
{
  char itmp[sz+1];
  strncpy(itmp, str, sz);
//  printf("\n[%s]", itmp);

  uint32_t opHash = Water_GenerateOpHash(str, sz);
  //Water_OperatorClass_t* opClass = (Water_OperatorClass_t*)HashTable_Operator[opHash];
  Water_OperatorObject_t* opObj = (Water_OperatorObject_t*)HashTable_Operator[opHash];
//  if(!opClass) return 0;
  Water_OperatorObject_t* obj = (Water_OperatorObject_t*)Water_alloc(sizeof(Water_OperatorObject_t));
  memcpy(obj, opObj, sizeof(Water_OperatorObject_t));
//  printf("[%s; %x]", obj->name, obj->type);
  return obj;
}

Water_BasicTypeObject_t* Water_BuildFuncValue(Water_BasicTypeObject_t* func)
{
  return NULL;
}

Water_OperatorObject_t* Water_NewOperatorObj(const char* name, uint32_t str_len, uint32_t priority, uint32_t associativity, Water_BasicTypeObject_t* (*Func)(Water_OperatorObject_t*))
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
    HashTable_Operator[opHash] = (uintptr_t)obj;
    return obj;
}

uintptr_t Water_INT_generate()
{
  Water_BasicTypeObject_t* obj = (Water_BasicTypeObject_t*)Water_alloc(sizeof(Water_BasicTypeObject_t));
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
  uint32_t n = sizeof(Water_BasicTypeObject_t);// + sz4 + 4;
  Water_BasicTypeObject_t* obj = (Water_BasicTypeObject_t*)Water_alloc(n);
  obj->type = WATER_STR | WATER_BASICTYPE;
  obj->TypeClass = Water_STR_TypeClass;

  obj->buffer_sz = 4;
  obj->ext_buffer = NULL;
  return (uintptr_t)obj;
}

Water_BasicTypeObject_t* Water_GenerateBasicTypeobj(const char* str, uint32_t type)
{
  func_ptr_t func = BasicType_Generators[type];
  Water_BasicTypeObject_t* obj = (Water_BasicTypeObject_t*)func();
  strcpy(obj->name, str);
  return obj;
}

void Water_init()
{
  Working_Table = (uintptr_t*)pmem_4k(2);
  Water_INT_TypeClass = (Water_BasicTypeClass_t*)Water_alloc(sizeof(Water_BasicTypeClass_t));
  Water_STR_TypeClass = (Water_BasicTypeClass_t*)Water_alloc(sizeof(Water_BasicTypeClass_t));

  HashTable_Operator = (uintptr_t*)pmem_4k(1);
  HashTable_keywords = (uintptr_t*)pmem_4k(2);
  HashTable_BasicTypes = (uintptr_t*)pmem_4k(2);

  Water_NewOperatorObj("=", 1, 0, 0, Water_opEqual);
  Water_NewOperatorObj("-", 1, 4, 0, Water_opMinus);
  Water_NewOperatorObj("+", 1, 4, 0, Water_opPlus);
  Water_NewOperatorObj("*", 1, 8, 0, Water_opMul);
  Water_NewOperatorObj("/", 1, 8, 0, Water_opDiv);
  Water_NewOperatorObj("^", 1, 12, 1, Water_opExp);
  Water_NewOperatorObj("(", 1, 16, 0, Water_RoundBracketO);
  Water_NewOperatorObj(")", 1, 16, 0, Water_RoundBracketC);

  BasicType_Generators = (func_ptr_t*)pmem_4k(1);
  BasicType_Generators[WATER_INT] = Water_INT_generate;
  BasicType_Generators[WATER_STR] = Water_STR_generate;

  TemporaryTokensL = (Water_BasicTypeObject_t**)kmalloc(512);
  TemporaryTokensL[WATER_INT] = Water_GenerateBasicTypeobj("_temp_intL", WATER_INT);
  TemporaryTokensR = (Water_BasicTypeObject_t**)kmalloc(512);
  TemporaryTokensR[WATER_INT] = Water_GenerateBasicTypeobj("_temp_intR", WATER_INT);

  OpStackBottom = (Water_OperatorObject_t**)pmem_4k(1);
  OpStackTop = OpStackBottom;
  *OpStackTop = NULL;

  ValStackBottom = (Water_BasicTypeObject_t**)pmem_4k(1);
  ValStackTop = ValStackBottom;
  *ValStackTop = NULL;

  OutputQueueStart = (Water_Token_t**)pmem_4k(1);
  OutputQueueEnd = OutputQueueStart;
}

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
    int error = Water_Execute(shell_buf);
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

int Water_Execute(const char* sentence)
{
  uintptr_t opArr[32];
  asm volatile("cli");
  //Water_MakeSymbolTable(sentence, opArr);
  printf("\n{%u}", *HPET_main_counter);
  Water_Token_t* tree = Water_ExecuteRemoteExp(sentence, strlen(sentence));
//  printf("\n\t==>{%d}", ((Water_BasicTypeObject_t*)tree)->int_buffer[0]);
  printf("\n{%u}", *HPET_main_counter);
  Water_ExecuteRemoteExpSY(sentence, strlen(sentence));
  printf("\n{%u}", *HPET_main_counter);
  asm volatile("sti");
  //printf("\n%s ", InfixToPostFix(sentence));
/*  printf("\n");

  for(Water_Token_t** i = OutputQueueStart; i != OutputQueueEnd; i++)
  {
    if((*i)->type & WATER_OPERATOR)
      printf("%s ", ((Water_OperatorObject_t*)*i)->name);
    else printf("%s ", ((Water_BasicTypeObject_t*)*i)->name);
  }*/
  OutputQueueEnd = OutputQueueStart;
  return 0;
}

int Water_KeywordHandler(Water_Token_t* TokenStart)   // If any keywords, Execute them
{

}

Water_Token_t* Water_ExecuteRemoteExp(const char* sentence, int size)   // Similar to Water_Execute, used to execute remote expressions recursively
{
  Water_Token_t* TokenStart = (Water_Token_t*)Water_alloc(sizeof(Water_Token_t));
  Water_Token_t* TokenEnd = (Water_Token_t*)Water_alloc(sizeof(Water_Token_t));
  TokenStart->NextToken = TokenEnd;   // Create a Doubly linked Linked List
  TokenEnd->BackToken = TokenStart;
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
  Water_KeywordHandler(TokenStart);   // If any keywords, Execute them
  Water_Token_t* tok;

  tok = Water_ShuntingYardSolver(TokenStart);

  //TODO: Solve the TokenList using Arithmatic Solver and RETURN THE RESULTING TOKEN
  return tok;
}

char* Water_ConvertToPostfix(const char* sentence)
{

}

int Water_MakeTokenList(const char* sentence, Water_Token_t* TokenStart)
{
  int SentenceSize = strlen(sentence);
  char* tmpStr = (char*)kmalloc(SentenceSize+2);
  strcpy(tmpStr, sentence);
  tmpStr[SentenceSize] = '\0';

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
        tmpToken = token;
      }
      else  // Must be a Variable or a Function
      {
        token = (Water_Token_t*)Water_GetTypeObjToken_hash(hash);   // Get the token for variable/function
        if(!token)
        {
          printf("\n%s Variable/Function not defined!", tmpStr);
          return 1;
        }
        if(token->type & WATER_FUNCTION)  // If function
        {
          if(tmpStr[i] == '(')    // Get the Parameters
          {
            char* param = &tmpStr[i];
            for(; tmpStr[i] != ')'; i++)
            {
            }
            ++i;
            Water_SetFuncParam((Water_BasicTypeObject_t*)token, param);    //  Store the Function Parameters in its structure
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
      }
      tmpStr += i;
    }
    else if(isdigit(tmpStr[0])) // Is a Numerical
    {
      int i = 0;
      for(; isnumeric(tmpStr[i]); i++)      // Continue until u encounter space/operator
      {
      }
      // Create the Type Struct
      Water_Token_t* token = (Water_Token_t*)Water_NumTypeCheckAndGenerate(tmpStr, i);   // Check the Type => Int/Float and create the struc
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
      for(; tmpStr[i] != '\"'; i++)   // Wait for the String to End
      {
      }

      Water_Token_t* token = (Water_Token_t*)Water_StrTypeGenerate(tmpStr, i);    // Generate the String type token
      if(!token)
      {
        printf("\n%s String type Error", tmpStr);
        return 3;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;

      tmpStr += i;
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
      Water_Token_t* token = (Water_Token_t*)Water_OpGetToken(tmpStr, i);   //  Execute the Internal Expression RECURSIVELY

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

int Water_MakeTokenListSY(const char* sentence, Water_Token_t* TokenStart)
{
  int SentenceSize = strlen(sentence);
  char* tmpStr = (char*)kmalloc(SentenceSize+2);
  strcpy(tmpStr, sentence);
  tmpStr[SentenceSize] = '\0';

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
        tmpToken = token;
      }
      else  // Must be a Variable or a Function
      {
        token = (Water_Token_t*)Water_GetTypeObjToken_hash(hash);   // Get the token for variable/function
        if(!token)
        {
          printf("\n%s Variable/Function not defined!", tmpStr);
          return 1;
        }
        if(token->type & WATER_FUNCTION)  // If function
        {
          if(tmpStr[i] == '(')    // Get the Parameters
          {
            char* param = &tmpStr[i];
            for(; tmpStr[i] != ')'; i++)
            {
            }
            ++i;
            Water_SetFuncParam((Water_BasicTypeObject_t*)token, param);    //  Store the Function Parameters in its structure
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
      }
      tmpStr += i;
    }
    else if(isdigit(tmpStr[0])) // Is a Numerical
    {
      int i = 0;
      for(; isnumeric(tmpStr[i]); i++)      // Continue until u encounter space/operator
      {
      }
      // Create the Type Struct
      Water_Token_t* token = (Water_Token_t*)Water_NumTypeCheckAndGenerate(tmpStr, i);   // Check the Type => Int/Float and create the struc
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
      for(; tmpStr[i] != '\"'; i++)   // Wait for the String to End
      {
      }

      Water_Token_t* token = (Water_Token_t*)Water_StrTypeGenerate(tmpStr, i);    // Generate the String type token
      if(!token)
      {
        printf("\n%s String type Error", tmpStr);
        return 3;
      }
      tmpToken->NextToken = token;    // Put the Token into the Token List.
      token->BackToken = tmpToken;
      token->NextToken = EndToken;
      tmpToken = token;

      tmpStr += i;
    }
    else if(tmpStr[0] == '(' || tmpStr[0] == ')')   // A Round Bracket means another arithmatic/logical expression
    {
      Water_Token_t* token;
      token = (Water_Token_t*)Water_OpGetToken(tmpStr, 1);   //  Execute the Internal Expression RECURSIVELY
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
      // Get the Token
      Water_Token_t* token = (Water_Token_t*)Water_OpGetToken(tmpStr, i);   //  Execute the Internal Expression RECURSIVELY

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

  Water_OperatorObject_t* tree = NULL;
#ifndef WATER_USE_SHUNTING_YARD
  tree = (Water_OperatorObject_t*)Water_OperatorTreeGenerate(TokenStart);
  if(!tree) return TokenStart;
  tree = Water_ReduceOpTree(tree);
  //printf("\n\t==>{%d}", ((Water_BasicTypeObject_t*)tree)->int_buffer[0]);
#else
  Water_ShuntingYardSolver(TokenStart);
#endif

  return (Water_Token_t*)tree;
}

Water_OperatorObject_t* Water_ShowTree(Water_OperatorObject_t* tree)
{
  if(tree->RHS->type & WATER_OPERATOR)   // Need to solve recursively
  {
    Water_ShowTree((Water_OperatorObject_t*)tree->RHS);
  }
  if(tree->LHS->type & WATER_OPERATOR)   // Need to solve recursively
  {
    Water_ShowTree((Water_OperatorObject_t*)tree->LHS);
  }
  printf("[%s]->",tree->name);
  return tree;
}

Water_OperatorObject_t* Water_OperatorTreeGenerate(Water_Token_t* TokenStart)
{
  Water_Token_t* tmpToken = TokenStart;

  Water_OperatorObject_t* tree = NULL, *tmpOp, *tmpOp2, *lastOp = NULL;

  Water_BasicTypeObject_t* lastBasicType = NULL;

  tmpToken = tmpToken->NextToken; // Skip the Start Token

  for(;tmpToken->NextToken;)
  {
    if(tmpToken->type & WATER_OPERATOR)
    {
      lastOp = (Water_OperatorObject_t*)tmpToken;
    //  printf("\n%s", lastOp->name);
      lastOp->LHS = (Water_Token_t*)lastBasicType; // Put last variable/func/numerical as LHS
      if(!lastBasicType)
      {
        lastOp->LHS = (Water_Token_t*)TemporaryTokensL[WATER_INT];
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
          if((tmpOp2->priority < tmpOp->priority) || (tmpOp2->priority == tmpOp->priority && tmpOp->associativity))
          {
              //Move on to the RHS node of the tree
              if(!(tmpOp2->RHS->type & WATER_OPERATOR) || !tmpOp2->RHS) // If end of tree is reached
              {
                //Put it here in the end
                tmpOp2->RHS = (Water_Token_t*)tmpOp;
                tmpOp->parent = (Water_Token_t*)tmpOp2;
                tmpOp->inherit = 1;
                break;
              }
              else
              {
                tmpOp2 = (Water_OperatorObject_t*)tmpOp2->RHS;
              }
          }
          else
          {
            // Put the tmpOp2 node to tmpOp's LHS and put tmpOp in its place
            if(tmpOp2->parent)
              ((Water_OperatorObject_t*)tmpOp2->parent)->RHS = (Water_Token_t*)tmpOp;
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
            tmpOp2->inherit = 0;
            break;
          }
        }
    //    Water_ShowTree(tree);
      }
    }
    else if(tmpToken->type & WATER_BASICTYPE)
    {
    //  printf("\n->%d %x, %s", ((Water_BasicTypeObject_t*)tmpToken)->int_buffer[0], tmpToken, ((Water_BasicTypeObject_t*)tmpToken)->name);
      lastBasicType = (Water_BasicTypeObject_t*)tmpToken;
      lastOp->RHS = (Water_Token_t*)tmpToken;
    }
    else if(tmpToken->type & WATER_FUNCTION)
    {
      lastBasicType  = Water_BuildFuncValue((Water_BasicTypeObject_t*)tmpToken); // Derive the value of the type (Get value for Variable/Numerical, Solve for Function)
    }
    tmpToken = tmpToken->NextToken;
  }
  //printf("\n{%s}", tree->name);

  return tree;
}

Water_BasicTypeObject_t* Water_ReduceOpTree(Water_OperatorObject_t* treeStart)
{
  if(treeStart->RHS->type & WATER_OPERATOR)   // Need to solve recursively
  {
    Water_Token_t* RHS = (Water_Token_t*)Water_ReduceOpTree((Water_OperatorObject_t*)treeStart->RHS);
    treeStart->RHS = RHS;
  }
  if(treeStart->LHS->type & WATER_OPERATOR)   // Need to solve recursively
  {
    Water_Token_t* LHS = (Water_Token_t*)Water_ReduceOpTree((Water_OperatorObject_t*)treeStart->LHS);
    treeStart->LHS = LHS;
  }
  //printf("\n[%s]", treeStart->name);
  return treeStart->Function(treeStart);
}

void Water_PushOpStack(Water_OperatorObject_t* op)
{
  ++OpStackTop;
  *OpStackTop = op;
//  printf("{%s}", (*OpStackTop)->name);
}

Water_OperatorObject_t* Water_PopOpStack()
{
  Water_OperatorObject_t* tmp = *OpStackTop;
  if(OpStackTop != OpStackBottom)
    --OpStackTop;
  else return NULL;
//  printf("[-->%s<--]", (*OpStackTop)->name);
  return tmp;
}

void Water_PushValStack(Water_BasicTypeObject_t* Val)
{
  ++ValStackTop;
  *ValStackTop = Val;
  //printf("{%s}", (*ValStackTop)->name);
}

Water_BasicTypeObject_t* Water_PopValStack()
{
  Water_BasicTypeObject_t* tmp = *ValStackTop;
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
  /*if(OutputQueueEnd != OutputQueueStart)
    --OutputQueueEnd;
  else return NULL;
  return *OutputQueueEnd;*/
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
    //  printf("\n=>%s",((Water_OperatorObject_t*)tmpToken)->name);
      if(!strcmp(((Water_OperatorObject_t*)tmpToken)->name, "("))  //Token is left Bracket
      {
      //  printf("-->Left Bracket");
        Water_PushOpStack(tmpToken);
      }
      else if(!strcmp(((Water_OperatorObject_t*)tmpToken)->name, ")"))
      {
        //printf("-->Right Bracket");
        Water_Token_t* tmp;
        while(strcmp((*OpStackTop)->name, "("))
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
        while((*OpStackTop)->priority != 16 && ((*OpStackTop)->priority > ((Water_OperatorObject_t*)tmpToken)->priority || ((*OpStackTop)->priority == ((Water_OperatorObject_t*)tmpToken)->priority && !((Water_OperatorObject_t*)tmpToken)->associativity)) )
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
      //printf("\n%s",((Water_BasicTypeObject_t*)tmpToken)->name);
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
  printf("\n\t===>{%d}", ((Water_BasicTypeObject_t*)tmp)->int_buffer[0]);
  return;
}

Water_Token_t* Water_EvaluateShuntingYard()
{
  Water_Token_t* tmp = Water_PopQueue();
  Water_OperatorObject_t* opTmp;
  while(tmp)
  {
    if(tmp->type & WATER_OPERATOR)
    {
      opTmp = tmp;
      //printf("\n====>%s", opTmp->name);
      opTmp->RHS = (Water_Token_t*)Water_PopValStack();
      opTmp->LHS = (Water_Token_t*)Water_PopValStack();
      Water_PushValStack(opTmp->Function(opTmp));
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

#define W_STACK_SIZE 50            /* Size of Stack */

char W_s[W_STACK_SIZE];
int w_top=-1;       /* Global declarations */

void w_push(char elem)
{                       /* Function for PUSH operation */
    W_s[++w_top]=elem;
}

char w_pop()
{                      /* Function for POP operation */
    return(W_s[w_top--]);
}

int pr(char elem)
{                 /* Function for precedence */
    switch(elem)
    {
      case '#': return 0;
      case '(': return 1;
      case '+':
      case '-': return 2;
      case '*':
      case '/': return 3;
    }
}

char* InfixToPostFix(char* infx)
{
  char ch,elem;
  char *pofx = (char*)kmalloc(50);
  int i = 0, k = 0;
  w_push('#');
  while( (ch=infx[i++]) != '\0')
  {
      if( ch == '(') w_push(ch);
      else
          if(isalnum(ch)) pofx[k++]=ch;
          else
              if( ch == ')')
              {
                  while( W_s[w_top] != '(')
                      pofx[k++]=w_pop();
                  elem=w_pop(); /* Remove ( */
              }
              else
              {       /* Operator */
                  while( pr(W_s[w_top]) >= pr(ch) )
                      pofx[k++]=w_pop();
                  w_push(ch);
              }
  }
  while( W_s[w_top] != '#')     /* Pop from stack till empty */
      pofx[k++]=w_pop();
  pofx[k]='\0';          /* Make pofx as valid string */
  return pofx;
}
