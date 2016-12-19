#ifndef _LAUNCHING_PADS_H_
#define _LAUNCHING_PADS_H_

#include <stdint.h>
#include <stdbool.h>
#include <native/task.h>
#include <native/timer.h>

#define Tlaunching_padsPRI 90       // Priority
#define Tlaunching_padsPER 10000000    // Task period, in ns 10µs
#define Tlaunching_padsNAME "Launching Pads"

typedef struct Missile
{
	double x, y;
	double vx, vy;
	double h, w;
	double init_speed, init_angle;

	RTIME launched;
	RTIME destroyed;
	RTIME targed;

} Missile;

typedef struct
{
	double x, y;
	double h, w;				//XXX Maybe just one size?

	Missile missile;
	bool is_enemy; 				//XXX Not sure if better/more realistic approach.

} LaunchingPad;

void launching_pads_task();
void updatePos(Missile*);

#endif
