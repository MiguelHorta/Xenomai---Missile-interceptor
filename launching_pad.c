#include "launching_pad.h"
#include "scenary.h"

#include <native/task.h>
#include <native/timer.h>
#include <math.h>

void launching_pads_task()
{
  extern Scenary scenary;
  Missile* enemy_missile = &scenary.enemy_lp.missile;
  Missile* ally_missile = &scenary.ally_lp.missile;
  RT_TASK *curtask;
  RT_TASK_INFO curtaskinfo;
	int err;
  RTIME to=0,ta=0;
  unsigned long overruns;

  curtask = rt_task_self();
  rt_task_inquire(curtask,&curtaskinfo);
	rt_printf("%s init\n", curtaskinfo.name);
  err = rt_task_set_periodic(NULL, TM_NOW, Tlaunching_padsPER);
  if(err)
  {
    rt_printf("%s ERR!!! %d\n", curtaskinfo.name, err);
  }
  for(;;)
  {
    err = rt_task_wait_period(NULL);
    ta = rt_timer_read();
    if(err)
    {
      rt_printf("%s overrun!!!\n", curtaskinfo.name);
      break;
    }
    if(to!=0)
    {
      rt_printf("%s Measured period (ns)= %lu\n", curtaskinfo.name, ta-to);
    }
    to=ta;
    if(enemy_missile->launched > 0 && !enemy_missile->destroyed)
    {
      double t = (double)( (ta - enemy_missile->launched) / 1000000000.0 );
      enemy_missile->x = enemy_missile->vx * t + scenary.enemy_lp.x;
      enemy_missile->y = enemy_missile->vy * t - 5 * pow( t, 2 ) + scenary.enemy_lp.y;
    }

    if(ally_missile->launched > 0 && !ally_missile->destroyed)
    {
      double t = (double)( (ta - ally_missile->launched) / 1000000000.0 );
      ally_missile->x = ally_missile->vx * t + scenary.ally_lp.x;
      ally_missile->y = ally_missile->vy * t - 5 * pow( t, 2 ) + scenary.ally_lp.y;
    }
  }
  return;
}
