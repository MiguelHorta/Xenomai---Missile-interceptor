#include "radar.h"

#include <native/task.h>
#include <native/timer.h>
#include "scenary.h"
#include <math.h>

extern Scenary scenary;

void radars_task()
{
  Missile* missile = &scenary.enemy_lp.missile;
  Missile* ally_missile = &scenary.ally_lp.missile;
  Radar* radar = &scenary.radar;
  RT_TASK *curtask;
  RT_TASK_INFO curtaskinfo;
	int err;
  RTIME to=0,ta=0;
  unsigned long overruns;

  curtask = rt_task_self();
  rt_task_inquire(curtask,&curtaskinfo);
	rt_printf("%s init\n", curtaskinfo.name);
  err = rt_task_set_periodic(NULL, TM_NOW, TradarsPER);
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

    if((pow(missile->x - radar->x, 2) + pow(missile->y - radar->y, 2)) <= pow(radar->extent, 2) && !missile->targed )
    {
      radar->dx[radar->ndet] = missile->x;
      radar->dy[radar->ndet] = missile->y;
      if(radar->ndet == 2)
      {
        missile->targed = true;
        ally_missile->vx = -30;
        ally_missile->vy = 80;
        ally_missile->launched = rt_timer_read();
      }else
        radar->ndet++;
    }
  }
  return;
}
