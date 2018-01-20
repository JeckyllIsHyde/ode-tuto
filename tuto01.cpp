/* A simple visualization Window
   Just need to include ode.h and drawstuff.h
   To compile after 
     1. install ODE on /usr/local/include and /usr/local/lib
     2. install drawstuff on /usr/local/include and /usr/local/lib
     3. link with libode, libdrawstuff
     4. link with libGL, libGLU, libX11
     5. install textures-dir on the current directory
       g++ -o tuto01 tuto01 -lode -ldrawstuff -lGL -lGLU -lX11
*/

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

static void simLoop (int pause) {}
static void start () {}

int main(int argc, char *argv[]) {

  dsFunctions fn;
  fn.version = DS_VERSION;
  fn.start = &start;
  fn.step = &simLoop;
  fn.command = NULL;
  fn.stop = NULL;
  fn.path_to_textures = "textures";

  // intialize ODE engine
  dInitODE();

  // simululation loop
  dsSimulationLoop (argc, argv, 352, 288, &fn);
  // close ODE engine
  dCloseODE();

  return 0;
}
