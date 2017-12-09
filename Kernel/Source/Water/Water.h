#ifndef WATER_H
#define WATER_H

#define WATER_IDENTIFIERS     0xf0000000
#define WATER_KEYWORD         0x0f000000
#define WATER_OPERATOR        0x000f0000
#define WATER_FUNCTION        0x00100000
#define WATER_VARIABLE        0x00200000
#define WATER_NUMERICAL       0x00001000
#define WATER_BASICTYPE       0x00000100
#define WATER_IO              0x000000f0
#define WATER_INT             1
#define WATER_STR             2
//#define WATER_USE_SHUNTING_YARD

#include "stdio.h"
#include "stdlib.h"
#include "common.h"
#include "string.h"
#include "Shell.h"
#include "task.h"

task_t* Water_Task;
uintptr_t* Working_Table;

char water_operators[] = {'-', '+', '(', ')', '=', '[', '%', '*', '/', ']'};

typedef struct Water_Token
{
  uint32_t type;
  uint32_t hash;
  uintptr_t typeClass;
  struct Water_Token* NextToken;
  struct Water_Token* BackToken;
}Water_Token_t;

typedef struct Water_TmpToken
{
  uint32_t type;
  uint32_t hash;
  uintptr_t typeClass;
  uintptr_t* NextToken;
  uintptr_t* BackToken;
  char name[32];
}Water_TmpToken_t;

typedef struct Water_KeywordToken
{
  uint32_t type;
  uint32_t hash;
  uintptr_t reserved;
  uintptr_t NextToken;
  uintptr_t BackToken;

  char name[32];
  uintptr_t (*func)(Water_Token_t*);
  uint32_t parameters;
}Water_KeywordToken_t;

typedef struct Water_OperatorObject
{
  uint32_t type;
  uint32_t hash;
  uint32_t priority;
  uint32_t associativity;
  uint32_t inherit;
  Water_Token_t* (*Function)(uintptr_t);
  char name[4];
}Water_OperatorObject_t;

typedef struct Water_OperatorToken
{
  uint32_t type;
  uint32_t hash;
  uint32_t reserved;
  uintptr_t NextToken;
  uintptr_t BackToken;

  Water_Token_t* LHS;
  Water_Token_t* RHS;
  Water_Token_t* parent;
  Water_Token_t* output;
  Water_OperatorObject_t* op;
  char name[4];
}Water_OperatorToken_t;

typedef struct Water_Func
{

}Water_Func_t;

typedef struct Water_OperatorTable
{
  Water_OperatorObject_t* equal;
  Water_OperatorObject_t* add;
  Water_OperatorObject_t* sub;
  Water_OperatorObject_t* mul;
  Water_OperatorObject_t* div;
  Water_OperatorObject_t* exp;
  Water_OperatorObject_t* and;
  Water_OperatorObject_t* or;
  Water_OperatorObject_t* xor;
  Water_OperatorObject_t* not;
  Water_OperatorObject_t* dot;
  Water_OperatorObject_t* arrow;
  Water_OperatorObject_t* rBrac_open;
  Water_OperatorObject_t* rBrac_close;
  Water_OperatorObject_t* sBrac_open;
  Water_OperatorObject_t* sBrac_close;
  Water_OperatorObject_t* cBrac_open;
  Water_OperatorObject_t* cBrac_close;
  uintptr_t OtherOperators;
}Water_OperatorTable_t;

typedef struct Water_BasicTypeClass
{
  char name[16];
  uint32_t TemplateHash;
  uint32_t TypeSize;
  uintptr_t OperatorTable;

  struct Water_BasicTypeClass* Parent;
  uint32_t nMemberV;
  uint32_t nMemberF;
  Water_Func_t** ChildFs;
}Water_BasicTypeClass_t;

Water_BasicTypeClass_t* Water_INT_TypeClass, *Water_STR_TypeClass;

typedef struct Water_Var
{
  uint32_t custom;
  char name[32];
  uint32_t buffer_sz;
  uintptr_t ext_buffer;
  uint32_t int_buffer[1];

  uint32_t type;
  uint32_t hash;
  Water_BasicTypeClass_t* TypeClass;

  struct Water_Var* Parent;
  uint32_t nChilds;
  struct Water_Var** Childs;
}Water_Var_t;

typedef struct Water_BasicTypeToken
{
  uint32_t type;
  uint32_t hash;
  Water_BasicTypeClass_t* TypeClass;
  uintptr_t NextToken;
  uintptr_t BackToken;

  Water_Var_t* var;
  uint32_t custom;
  char name[32];
}Water_BasicTypeToken_t;

uintptr_t* HashTable_OpObj;
uintptr_t* HashTable_keywords;
uintptr_t* HashTable_Vars;

func_ptr_t* BasicType_Generators = NULL;
Water_BasicTypeToken_t** TemporaryTokensL = NULL;
Water_BasicTypeToken_t** TemporaryTokensR = NULL;

Water_OperatorToken_t** OpStackTop = NULL, **OpStackBottom = NULL;
Water_BasicTypeToken_t** ValStackTop = NULL, **ValStackBottom = NULL;
Water_Token_t** OutputQueueStart = NULL, **OutputQueuePtr = NULL,**OutputQueueEnd = NULL;

uintptr_t Water_alloc(uint32_t sz);
void Water_free(uintptr_t ptr);
void Water_MemManager();

uint32_t Water_GenerateHash(const char* str, int size);
uint32_t Water_GenerateOpHash(const char* str, int size);

inline Water_KeywordToken_t* Water_GetKeywordToken_hash(uint32_t hash);
inline Water_Var_t* Water_GetVar_hash(uint32_t hash);
inline Water_OperatorObject_t* Water_GetOpObj_hash(uint32_t hash, uint32_t* HashTable);

inline void Water_SetKeywordToken_hash(uint32_t hash, uintptr_t val);
inline void Water_SetVar_hash(uint32_t hash, uintptr_t val);
inline void Water_SetOpObj_hash(uint32_t hash, uintptr_t val, uint32_t* HashTable);

Water_BasicTypeToken_t* Water_NumTypeGenerate(const char* str, int sz);
Water_BasicTypeToken_t* Water_StrTypeGenerate(const char* str, int sz);

Water_BasicTypeToken_t* Water_BuildFuncValue(Water_BasicTypeToken_t* func);

uintptr_t Water_INT_generate();
uintptr_t Water_STR_generate();

Water_Var_t* Water_NewVarObj(const char* str, uint32_t type);
Water_Var_t* Water_NewVarObj_h(const char* str, uint32_t type, uint32_t hash);
Water_OperatorObject_t* Water_NewOperatorObj(const char* name, uint32_t str_len, uint32_t priority, uint32_t associativity, Water_Token_t* (*Func)(uintptr_t), uint32_t* HashTable);
Water_KeywordToken_t* Water_NewKeyword(const char* str, uint32_t strln, uint32_t type, uintptr_t (*func)(Water_Token_t*), uint32_t parameters);

Water_OperatorToken_t* Water_GetOpToken(const char* str, int sz, uint32_t* HashTable);
Water_BasicTypeToken_t* Water_GetBasicTypeToken_vh(uint32_t hash);
Water_OperatorToken_t* Water_GetOpToken_vh(uint32_t hash, uint32_t* HashTable);

Water_BasicTypeToken_t* Water_GenerateBasicTypeobj(const char* str, uint32_t type);
Water_BasicTypeToken_t* Water_TempVarGen(uint32_t type);

uint32_t Water_KeywordHandlerDirect(Water_Token_t* TokenStart, const char* str);
uint32_t Water_GenTokenList(const char* str, uint32_t nToks, Water_Token_t* TokenStart);
int Water_KeywordHandler(Water_Token_t* TokenStart);

/************************* Water Initialization *****************************/

void Water_init();

/***************************** Shell Process *****************************/
void Water_shell();

/*************************** BACKEND Abstraction Layer ************************/

int Water_Execute(const char* sentence);

Water_Token_t* Water_ExecuteRemoteExp(const char* sentence, int size);

Water_Token_t* Water_ExecuteRemoteExpSY(const char* sentence, int size);

int Water_MakeTokenList(const char* sentence, Water_Token_t* TokenStart);

int Water_MakeTokenListSY(const char* sentence, Water_Token_t* TokenStart);

Water_Token_t* Water_ArithmaticSolver(Water_Token_t* TokenStart);

Water_OperatorToken_t* Water_ShowTree(Water_OperatorToken_t* tree);

Water_Token_t* Water_OperatorTreeGenerate(Water_Token_t* TokenStart);

Water_BasicTypeToken_t* Water_ReduceOpTree(Water_OperatorToken_t* treeStart);

void Water_PushOpStack(Water_OperatorToken_t* op);

Water_OperatorToken_t* Water_PopOpStack();

void Water_PushValStack(Water_BasicTypeToken_t* Val);

Water_BasicTypeToken_t* Water_PopValStack();

void Water_PushQueue(Water_Token_t* tok);

Water_Token_t* Water_PopQueue();

void Water_QueueRewind();

Water_Token_t* Water_ShuntingYardSolver(Water_Token_t* TokenStart);

Water_Token_t* Water_EvaluateShuntingYard();

uintptr_t Water_OpGetClass(uint32_t hash);

char* Water_GetErrorName(int error);

/********************* Operator Declarations ********************/
Water_BasicTypeToken_t* Water_INT_opEqual(Water_OperatorToken_t* op);
Water_BasicTypeToken_t* Water_INT_opMinus(Water_OperatorToken_t* op);
Water_BasicTypeToken_t* Water_INT_opPlus(Water_OperatorToken_t* op);
Water_BasicTypeToken_t* Water_INT_opMul(Water_OperatorToken_t* op);
Water_BasicTypeToken_t* Water_INT_opDiv(Water_OperatorToken_t* op);
Water_BasicTypeToken_t* Water_INT_opExp(Water_OperatorToken_t* op);
Water_BasicTypeToken_t* Water_INT_RoundBracketO(Water_OperatorToken_t* op);
Water_BasicTypeToken_t* Water_INT_RoundBracketC(Water_OperatorToken_t* op);

#endif
