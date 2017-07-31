#ifndef NEURON_h
#define NEURON_h
/*
typedef struct Neuron
{
  uint32_t threshold;
  uint32_t reaction;
  uint32_t reserved;
  uint32_t* Axons_table;
}Neuron_t;

typedef struct NConnection
{
  uint32_t weight;
  uint32_t reserved;
  uint32_t NeuronA; //Neuron Ahead
  uint32_t NeuronB; //Neuron Behind
}NConnection_t;

typedef struct AxonTable
{
  Neuron_t* ParentNeuron;
  uint32_t Connections[1022];
  struct AxonTable* next;
}AxonTable_t;
*/

volatile uint32_t NeuralBase;
volatile uint32_t NeuralSize;
volatile uint32_t Neurons;
#define NEURONS_PER_NEURON 30

char annCounter[40] = "NeuralNets/annCounter.dat";
char annDirName[20] = "NeuralNets";
char annFileNames[20] = "NeuralNets/ann";

volatile uint32_t Neuron_New = 0;

typedef struct __attribute__((packed)) Neuron
{
  int Value;
  int threshold;
  uint16_t totc;
  uint16_t totp;
  uint32_t id;
  uintptr_t connections[NEURONS_PER_NEURON];
  int Aweights[NEURONS_PER_NEURON];
  uintptr_t parents[NEURONS_PER_NEURON];
  int Bweights[NEURONS_PER_NEURON];
}Neuron_t;

void NeuralNet_init();

void NeuralNet_SaveToFile(uint32_t* base, uint32_t size);

#endif
