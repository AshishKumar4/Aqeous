#ifndef ACPI_h
#define ACPI_h


void acpiPowerOff(void);

int acpiCheckHeader(unsigned int *ptr, char *sig);

unsigned int *acpiGetRSDPtr(void);

unsigned int *acpiCheckRSDPtr(unsigned int *ptr);

#endif // ACPI_h
