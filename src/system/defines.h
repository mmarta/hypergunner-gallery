#ifndef _DEFINES_H
#define _DEFINES_H

#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <uzebox.h>

typedef enum {
  SOUTH, WEST, NORTH, EAST
} direction;

#define EDGE_EAST 26
#define EDGE_WEST 0
#define EDGE_SOUTH 2
#define EDGE_NORTH 28

#endif
