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
  double tmax = 0;
  double tinc = 0;
  for(;;)
  {
    err = rt_task_wait_period(NULL);
    ta = rt_timer_read();
    if(err)
    {
      rt_printf("%s overrun!!!\n", curtaskinfo.name);
      break;
    }
    // if(to!=0)
    // {
    //   rt_printf("%s Measured period (ns)= %lu\n", curtaskinfo.name, ta-to);
    // }
    to=ta;

    if((pow(missile->x - radar->x, 2) + pow(missile->y - radar->y, 2)) <= pow(radar->extent, 2) )
    {
      if(missile->targed == 0)
      {
        missile->targed = rt_timer_read();
      }
      if(radar->ndet < 2)
      {
        radar->dx[radar->ndet] = missile->x - radar->x;
        radar->dy[radar->ndet] = missile->y;
      }
      if(radar->ndet < 4) radar->ndet++;
      double mtime = rt_timer_read();
      if(radar->ndet == 2)
      {
        radar->dx[radar->ndet] = (radar->dx[1] - radar->dx[0])/ ((mtime - missile->targed) / 1000000000.0);
        radar->dy[radar->ndet] = (radar->dy[1]-radar->dy[0]+5*pow((mtime - missile->targed)/1000000000.0,2))/((mtime - missile->targed)/1000000000.0);
        // ally_missile->vx = 0;
        // ally_missile->vy = 110;

        tmax = (radar->dy[radar->ndet] + sqrt(pow(radar->dy[radar->ndet],2)+20.0*(radar->dy[0]-48)))/10.0;

        double xint = radar->dx[0] + radar->dx[2]*tmax;

        ally_missile->vx = xint/tmax;
        ally_missile->vy = (5*pow(tmax,2)+48)/tmax;
        ally_missile->launched = mtime;
        //tinc = (missile->vy - sqrt(pow(missile->vy,2)-960))/10.0;

        // double tint = radar->dx[0]/(ally_missile->vx-radar->dx[radar->ndet])
        //rt_printf("*\n*\n*\n*\n%f - %f @ %f || %f | %f\n*\n*\n*\n*", ally_missile->vx, ally_missile->vy, tmax, missile->vy, (tmax-tinc));
      }
      //rt_printf("k\nk\nk\nk\n%f - %f @ %f || %f | %f\n*\n*\n*\n*", ally_missile->vx, ally_missile->vy, tmax, missile->vy, (tmax-tinc));
      /*if(tmax > 0)
      {
        rt_printf("*\n*\n*\n*\n%f | %f\n*\n*\n*\n*", (tmax -tinc), ((mtime - missile->targed) / 1000000000.0));
        if( tinc <=  ((mtime - missile->targed) / 1000000000.0) && ally_missile->launched == 0){
          ally_missile->vx = missile->vx;
          ally_missile->vy = missile->vy;
          ally_missile->launched = mtime;
          rt_printf("*\n*\n*\n*\n____________________\n*\n*\n*\n*");
        }
      }*/
    }
  }
  return;
}
