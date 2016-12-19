#ifndef _RADAR_H_
#define _RADAR_H_

#include <stdint.h>
#include <stdbool.h>

#define TradarsPRI 90               // Priority
#define TradarsPER 10000000            // Task period, in ns 10µs
#define TradarsNAME "Radars"

typedef struct
{
	double x, y;
	double h, w;				//XXX Maybe just one size?
	double speed;
	double aperture;
	double extent;

	double angle;

	double dx[3];
	double dy[3];
	uint8_t ndet;
} Radar;

void radars_task();

#endif
