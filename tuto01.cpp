/* A simple visualization
   Just need to include ode.h and drawstuff.h
   To compile after 
     1. instal ODE on /usr/local/include:
       g++ -o tuto01 tuto01 -I /usr/local/include -lode
*/

#include <ode/ode.h>

#ifdef dDOUBLE
#define dsDrawSphere dsDrawSphereD
#endif

int main(int argc, char *argv[]) {
  // intialize ODE engine
  dInitODE();

  // close ODE engine
  dCloseODE();
  return 0;
}
