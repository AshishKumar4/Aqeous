#include "neuron.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "common.h"
#include "RandomLib/random.h"
#include "math.h"
#include "common.h"

#include "NeuralProcessing.h"
#include "NeuralManager.h"

float CheckNeuralSCE(uint32_t value, uint32_t threshold)
{
  float diff = (float)value - (float)threshold;
  diff *= 2;

  //float tt = tanh(diff) + 1;  // tanh
  //float tt = log((float)1 + powf(CONST_E, diff)); //Custom
  //float tt = 1/(1+powf(CONST_E, -diff));  //Sigmoid
  //-log(-x/(1+abs(-x))+1)

  float tt = (diff/(1+(float)abs(diff)) + 1)*(diff)/3;
  return tt;
}

void NeuralForwardUpdater() // Updates the Neural Values and Connection Weights. Core Process
{
  Neuron_t* base = NeuralBase, *tmp1, **tmp2;
  int ttt = 2;
  float tt = 0;
  int *tmpw;
  while(--ttt)
  {
    base = tmp1 = tmp2 = NeuralBase;
    //TODO:
    for(int i = 0; i < Neurons; i++)
    {
      //TODO: Compare the Value of Neuron to its Threshold and determine if the Excitation needs to be passed on.
      tt = CheckNeuralSCE(tmp1->Value, tmp1->threshold); // The Summation function
      if(tt > 0)
      {
        tmp2 = tmp1->connections;
        tmpw = tmp1->Aweights;
        for(int j = 0; j < tmp1->totc; j++)
        {
          //TODO: Use the Weights of every Neuron to figure out how to transmit the signal to other Neurons
          tmp2[j]->Value = tmp2[j]->Value + (int)(((tt/2) + 1)*tmpw[j]);//pow(2,((((int)tt/2))*abs(tmpw[j])))*(sgn(tmpw[j])); // TODO: Setup a Function based on the Value and the Weight for the Neuron.
          if(CheckNeuralSCE(tmp2[j]->Value, tmp2[j]->threshold))
          {
            tmpw[j] = (tmpw[j]*5)/2;
          }
          else
          {

          }
        }
      }
      ++tmp1;
    }
  }
}

void NeuralBackwardUpdater()
{
  Neuron_t* base = NeuralBase;
  while(1)
  {
    base = NeuralBase;
    //TODO:
    
  }
}
