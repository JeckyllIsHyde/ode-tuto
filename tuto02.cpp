/* Drawing and moving down a simple sphere
   Only kinematics of a simple object
     1. Moving forward from tuto01
     2. Create a World and add a sphere body, inertias and position
     3. Difine what will be drawn on simLoop
*/

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#ifdef dDOUBLE
#define dsDrawSphere dsDrawSphereD
#endif

static dWorldID world;
dBodyID ball;
const dReal radius = 0.2;

dsFunctions fn;

static void simLoop (int pause) {
  const dReal *pos, *R;
  dWorldStep(world, 0.05);
  dsSetColor(1.0,0.0,0.0);
  pos = dBodyGetPosition(ball);
  R = dBodyGetRotation(ball);
  dsDrawSphere(pos,R,radius);
}

static void start () {
  static float xyz[3] = {0.0, -3.0, 1.0};
  static float hpr[3] = {90.0, 0.0, 0.0};
  dsSetViewpoint (xyz,hpr);
}

void prepDrawStuff() {
  fn.version = DS_VERSION;
  fn.start = &start;
  fn.step = &simLoop;
  fn.command = NULL;
  fn.stop = NULL;
  fn.path_to_textures = "textures";
}

int main(int argc, char *argv[]) {

  // asign loop functions
  prepDrawStuff();

  // intialize ODE engine
  dInitODE();
  // create world
  world = dWorldCreate();
  dWorldSetGravity(world, 0.0, 0.0, -0.001);
  // create object
  const dReal mass = 1.0;
  dReal x0 = 0.0, y0 = 0.0, z0 = 1.0;
  dMass m1;
  ball = dBodyCreate(world);
  dMassSetZero(&m1);
  dMassSetSphereTotal(&m1,mass,radius);
  dBodySetMass(ball,&m1);
  dBodySetPosition(ball,x0,y0,z0);

  // simululation loop
  dsSimulationLoop (argc, argv, 352, 288, &fn);
  // close ODE engine
  dCloseODE();

  return 0;
}
