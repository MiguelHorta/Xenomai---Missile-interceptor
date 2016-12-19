#include "scenary.h"

#include <native/task.h>
#include <native/timer.h>

#include <SDL2/SDL.h>
extern Scenary scenary;
SDL_Window* _pwindow = NULL;
// Setup renderer
SDL_Renderer* _prenderer = NULL;

void drawBackground();
void drawMissile(int x, int y);
void drawExplosion(int x, int y);
void drawRadar(int x, int y, int);

void update_screen_task()
{
  _pwindow = SDL_CreateWindow
  (
      "STR Final Project", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      scenary.width,
      scenary.height,
      SDL_WINDOW_SHOWN
  );
  _prenderer =  SDL_CreateRenderer( _pwindow, -1, SDL_RENDERER_ACCELERATED);
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
    // if(to!=0)
    // {
    //   rt_printf("%s Measured period (ns)= %lu\n", curtaskinfo.name, ta-to);
    // }
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
  SDL_UpdateWindowSurface( _pwindow );
  drawBackground();
  drawMissile(scenary.enemy_lp.missile.x, scenary.enemy_lp.missile.y);
  drawMissile(scenary.ally_lp.missile.x, scenary.ally_lp.missile.y);
  drawRadar(scenary.radar.x, scenary.radar.y, scenary.radar.extent);
  if(scenary.ally_lp.missile.destroyed > 0)
  {
    drawExplosion(scenary.ally_lp.missile.x, scenary.ally_lp.missile.y);
  }
  SDL_RenderPresent( _prenderer );
  }

  SDL_DestroyWindow(_pwindow);
  SDL_Quit();
  return;
}

void drawBackground()
{
  SDL_SetRenderDrawColor( _prenderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear( _prenderer );
}

void drawMissile(int x, int y)
{
  SDL_Rect missile;
  missile.x = x;
  missile.y = scenary.height - y;
  missile.w = 6;
  missile.h = 6;
  SDL_SetRenderDrawColor( _prenderer, 0x00, 0x00, 0x00, 0xFF );
  SDL_RenderFillRect( _prenderer, &missile);
}
void drawExplosion(int x, int y)
{
  SDL_Rect missile;
  missile.x = x-3;
  missile.y = scenary.height - y-6;
  missile.w = 24;
  missile.h = 24;
  SDL_SetRenderDrawColor( _prenderer, 0xFF, 0x00, 0x00, 0xFF );
  SDL_RenderFillRect( _prenderer, &missile);
}
void drawRadar(int x, int y, int extent)
{
  SDL_Rect missile;
  missile.x = x;
  missile.y = scenary.height - y-4;
  missile.w = 12;
  missile.h = 4;
  SDL_SetRenderDrawColor( _prenderer, 0x00, 0xFF, 0x00, 0xFF );
  SDL_RenderFillRect( _prenderer, &missile);

  int centrex=x,centrey=y;// centre of circle in pixel coords
  int radius=extent;

  float two_pi=6.283f;

  float angle_inc=1.0f/radius;
  float angle;
  float xpos, ypos;
  for(angle=0.0f; angle<= two_pi;angle+=angle_inc){
      xpos=centrex+radius*cos(angle);
      ypos=centrey+radius*sin(angle);
      SDL_RenderDrawPoint(_prenderer, xpos, scenary.height-ypos);
  }
}
