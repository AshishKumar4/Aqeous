#include "stdio.h"
#include "stdlib.h"
#include "common.h"
#include "string.h"
#include "Shell/shell.h"
#include "Water/Water.h"
#include "ctype.h"
#include "math.h"

Water_BasicTypeToken_t* Water_INT_opEqual(Water_OperatorToken_t* op)
{
  Water_BasicTypeToken_t* tmp = (Water_BasicTypeToken_t*)op->RHS;
  printf("\n[%d] %s", tmp->var->int_buffer[0], op->op->name);

  int t = (int)tmp->var->int_buffer[0];

  tmp = (Water_BasicTypeToken_t*)op->LHS;
  printf("\nEqual operator called");
  tmp->var->int_buffer[0] = t;

  return tmp;
}

Water_BasicTypeToken_t* Water_INT_opMinus(Water_OperatorToken_t* op)
{
  Water_BasicTypeToken_t* tmp = (Water_BasicTypeToken_t*)op->LHS;
  printf("\n[%d] %s", tmp->var->int_buffer[0], op->op->name);

  int t = (int)tmp->var->int_buffer[0];
  tmp = (Water_BasicTypeToken_t*)op->RHS;
  printf(" %d", tmp->var->int_buffer[0]);

  t -= (int)tmp->var->int_buffer[0];


  tmp = Water_TempVarGen(WATER_INT);
  tmp->var->int_buffer[0] = t;

  return tmp;
}

Water_BasicTypeToken_t* Water_INT_opPlus(Water_OperatorToken_t* op)
{
  Water_BasicTypeToken_t* tmp = (Water_BasicTypeToken_t*)op->LHS;
  printf("\n[%d] %s", tmp->var->int_buffer[0], op->op->name);

  int t = (int)tmp->var->int_buffer[0];
  tmp = (Water_BasicTypeToken_t*)op->RHS;
  printf(" %d", tmp->var->int_buffer[0]);

  t += (int)tmp->var->int_buffer[0];


  tmp = Water_TempVarGen(WATER_INT);
  tmp->var->int_buffer[0] = t;

  return tmp;
}

Water_BasicTypeToken_t* Water_INT_opMul(Water_OperatorToken_t* op)
{
  Water_BasicTypeToken_t* tmp = (Water_BasicTypeToken_t*)op->LHS;
  printf("\n[%d] %s", tmp->var->int_buffer[0], op->op->name);

  int t = (int)tmp->var->int_buffer[0];
  tmp = (Water_BasicTypeToken_t*)op->RHS;
  printf(" %d", tmp->var->int_buffer[0]);

  t *= (int)tmp->var->int_buffer[0];


  tmp = Water_TempVarGen(WATER_INT);
  tmp->var->int_buffer[0] = t;

  return tmp;
}

Water_BasicTypeToken_t* Water_INT_opDiv(Water_OperatorToken_t* op)
{
  Water_BasicTypeToken_t* tmp = (Water_BasicTypeToken_t*)op->LHS;
  printf("\n[%d] %s", tmp->var->int_buffer[0], op->op->name);

  int t = (int)tmp->var->int_buffer[0];
  tmp = (Water_BasicTypeToken_t*)op->RHS;
  printf(" %d", tmp->var->int_buffer[0]);

  t /= (int)tmp->var->int_buffer[0];
  //printf("\n[%d]", t);

  tmp = Water_TempVarGen(WATER_INT);
  tmp->var->int_buffer[0] = t;

  return tmp;
}

Water_BasicTypeToken_t* Water_INT_opExp(Water_OperatorToken_t* op)
{
  Water_BasicTypeToken_t* tmp = (Water_BasicTypeToken_t*)op->LHS;
  printf("\n[%d] %s", tmp->var->int_buffer[0], op->op->name);

  int t = (int)tmp->var->int_buffer[0];
  tmp = (Water_BasicTypeToken_t*)op->RHS;
  printf(" %d", tmp->var->int_buffer[0]);

  t = pow(t, (int)tmp->var->int_buffer[0]);
  //printf("\n[%d]", t);

  tmp = Water_TempVarGen(WATER_INT);
  tmp->var->int_buffer[0] = t;

  return tmp;
}

Water_BasicTypeToken_t* Water_INT_RoundBracketO(Water_OperatorToken_t* op)
{
  printf("[%d]", op);
  return NULL;
}

Water_BasicTypeToken_t* Water_INT_RoundBracketC(Water_OperatorToken_t* op)
{
  printf("[%d]", op);
  return NULL;
}
