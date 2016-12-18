#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <rtdk.h>

#include "radar.h"
#include "launching_pad.h"
#include "scenary.h"

#define TASK_MODE 0  	              // No flags
#define TASK_STKSZ 0 	              // Default stack size

static RT_TASK Tlaunching_pads;
static RT_TASK Tradars;
static RT_TASK Tupdate_screen;

/*
* Catches CTRL + C to allow a controlled termination of the application
*/
void catch_signal(int sig) {}

void wait_for_ctrl_c(void) {
	signal(SIGTERM, catch_signal); //catch_signal is called if SIGTERM received
	signal(SIGINT, catch_signal);  //catch_signal is called if SIGINT received

	// Wait for CTRL+C or sigterm
	pause();

	// Will terminate
	rt_printf("Terminating ...\n");
}

int main(int argc, char *argv[])
{
  int err;

	/* Perform auto-init of rt_print buffers if the task doesn't do so */
	rt_print_auto_init(1);

	/* Lock memory to prevent paging */
	mlockall(MCL_CURRENT|MCL_FUTURE);

	err = rt_task_create(&Tlaunching_pads, Tlaunching_padsNAME , TASK_STKSZ, Tlaunching_padsPRI, TASK_MODE);
  if(err)
  {
    rt_printf("Error creating task "Tlaunching_padsNAME" (error code = %d)\n",err);
    return err;
  }else
    rt_printf("Task "Tlaunching_padsNAME" created successfully\n");

  err = rt_task_create(&Tradars, TradarsNAME , TASK_STKSZ, TradarsPRI, TASK_MODE);
  if(err)
  {
    rt_printf("Error creating task "TradarsNAME" (error code = %d)\n",err);
    return err;
  }else
    rt_printf("Task "TradarsNAME" created successfully\n");

  err = rt_task_create(&Tupdate_screen, Tupdate_screenNAME , TASK_STKSZ, Tupdate_screenPRI, TASK_MODE);
  if(err)
  {
    rt_printf("Error creating task "Tupdate_screenNAME" (error code = %d)\n",err);
    return err;
  }else
    rt_printf("Task "Tupdate_screenNAME" created successfully\n");

  rt_task_start(&Tlaunching_pads, &launching_pads_task, NULL);
  rt_task_start(&Tradars, &radars_task, NULL);
  rt_task_start(&Tupdate_screen, &update_screen_task, NULL);

	/* wait for termination signal */
	wait_for_ctrl_c();
	return 0;
}
