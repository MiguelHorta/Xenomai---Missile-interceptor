#ifndef _RADAR_H_
#define _RADAR_H_

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
} Radar;

void radars_task();

#endif
