#include "launching_pad.h"

#include <native/task.h>
#include <native/timer.h>

void launching_pads_task()
{
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
  }
  return;
}
