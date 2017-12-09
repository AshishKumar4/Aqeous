#ifndef NEURALPROCESSING_H
#define NEURALPROCESSING_H

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

float CheckNeuralSCE(uint32_t value, uint32_t threshold);

void NeuralForwardUpdater();

#endif
