#include "scenary.h"

#include <native/task.h>
#include <native/timer.h>

#include <SDL2/SDL.h>
extern Scenary scenary;

void update_screen_task()
{
  SDL_Window* window = NULL;
  window = SDL_CreateWindow
  (
      "STR Final Project", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      scenary.width,
      scenary.height,
      SDL_WINDOW_SHOWN
  );

  // Setup renderer
  SDL_Renderer* renderer = NULL;
  renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear( renderer );

  RT_TASK *curtask;
  RT_TASK_INFO curtaskinfo;
	int err;
  bool quit = false;
  //Event handler
  SDL_Event e;
  RTIME to=0,ta=0;
  unsigned long overruns;

  curtask = rt_task_self();
  rt_task_inquire(curtask,&curtaskinfo);
	rt_printf("%s init\n", curtaskinfo.name);
  err = rt_task_set_periodic(NULL, TM_NOW, Tupdate_screenPER);
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
    while( SDL_PollEvent( &e ) != 0 )
    {
      //User requests quit
      if( e.type == SDL_QUIT )
      {
        quit = true;
      }
    }

  //Update the surface
  SDL_UpdateWindowSurface( window );
  SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );

  SDL_RenderDrawPoint( renderer, scenary.enemy_lp.missile.x, scenary.height - scenary.enemy_lp.missile.y);
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
  rt_printf("%f - %f\n", scenary.ally_lp.missile.x, scenary.ally_lp.missile.y);

  SDL_RenderDrawPoint( renderer, scenary.ally_lp.missile.x, scenary.height - scenary.ally_lp.missile.y);
  SDL_RenderPresent( renderer );
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return;
}
