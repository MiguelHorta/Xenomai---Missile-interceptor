#ifndef _LAUNCHING_PADS_H_
#define _LAUNCHING_PADS_H_

#include <stdint.h>
#include <stdbool.h>

#define Tlaunching_padsPRI 90       // Priority
#define Tlaunching_padsPER 10000000    // Task period, in ns 10µs
#define Tlaunching_padsNAME "Launching Pads"

typedef struct Missile
{
	double x, y;
	double vx, vy;
	double h, w;
	double init_speed, init_angle;
	bool launched;
	bool destroyed;
	bool targed;

	void (*updatePos)(struct Missile);
} Missile;

typedef struct
{
	double x, y;
	double h, w;				//XXX Maybe just one size?

	Missile* missiles;
	uint8_t n_missiles;
	bool is_enemy; 				//XXX Not sure if better/more realistic approach.

} LaunchingPad;

void launching_pads_task();

#endif
