#ifndef PCIE_H
#define PCIE_H

uintptr_t PCIe_base = 0;

uintptr_t find_MCFGtable();

void init_PCI();

#endif
