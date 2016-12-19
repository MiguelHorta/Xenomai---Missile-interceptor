#ifndef _SCENARY_H_
#define _SCENARY_H_

#include <stdint.h>
#include <stdbool.h>

#include "radar.h"
#include "launching_pad.h"

#define Tupdate_screenPRI 90        // Priority
#define Tupdate_screenPER 33333333     // Task minimum period, in ns try 30FPS
#define Tupdate_screenNAME "Update Screen"

typedef struct
{
	double x, y;
	double h, w;				//XXX Maybe just one size?

	bool is_enemy; 				//XXX Not sure if better/more realistic approach.
} City;
typedef struct
{
	City city;

	Radar radar;

	LaunchingPad ally_lp;
	LaunchingPad enemy_lp;

	double width;
	double height;

} Scenary;

void update_screen_task();

#endif
