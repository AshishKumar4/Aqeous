#ifndef NEURON_h
#define NEURON_h

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

#endif
