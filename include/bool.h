#ifndef BANJO_KAZOOIE_BOOL_H
#define BANJO_KAZOOIE_BOOL_H

#include <stdbool.h>

#define NOT(boolean) ((boolean) ^ 1)
#define BOOL(boolean) ((boolean) ? true : false)

#if 0
#include <ultra64.h>

typedef int bool;
#endif
#endif
