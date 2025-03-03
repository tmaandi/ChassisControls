// fault.h
#ifndef FAULT_H
#define FAULT_H

#include <stdbool.h>

bool detectFault(float* samples, int sampleCount, float minRange, float maxRange);

#endif