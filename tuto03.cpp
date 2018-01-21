/* Drawing multiple shapes
     1. Define a new struct MyObject with members:
          body(type dBodyID)
     2. New function for create scene:
          1. initialize world
	  2. create sphere, box, capsure, cylinder and a line
 */

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#define dsDrawLine dsDrawLineD
#endif

#define DENSITY (5.0)

struct MyObject {
  dBodyID body;
};

dReal radius = 0.25;
dReal length = 1.0;
dReal sides[3] = {0.5,0.5,1.0};

static dWorldID world;
static MyObject sphere, box, capsule, cylinder;

dsFunctions fn;

static void simLoop (int pause) {
  const dReal *pos1, *R1, *pos2, *R2, *pos3, *R3;
  // sphere
  dsSetColor(1.0,0.0,0.0);
  dsSetSphereQuality(3);
  pos1 = dBodyGetPosition(sphere.body);
  R1 = dBodyGetRotation(sphere.body);
  dsDrawSphere(pos1,R1,radius);
  // cylinder
  dsSetColorAlpha(0,1,0,1);
  pos2 = dBodyGetPosition(cylinder.body);
  R2 = dBodyGetRotation(cylinder.body);
  dsDrawCylinder(pos2,R2,length,radius);
  // capsule
  dsSetColorAlpha(1,1,1,1);
  pos2 = dBodyGetPosition(capsule.body);
  R2 = dBodyGetRotation(capsule.body);
  dsDrawCapsule(pos2,R2,length,radius);
  // box
  dsSetColorAlpha(0,0,1,1);
  pos3 = dBodyGetPosition(box.body);
  R3 = dBodyGetRotation(box.body);
  dsDrawBox(pos3,R3,sides);
  // line
  dReal posA[3] = {0,5,0};
  dReal posB[3] = {0,5,1.9};
  dsDrawLine(posA,posB);
}

static void start () {
  static float xyz[3] = {5,3,0.5};
  static float hpr[3] = {-180,0,0};
  dsSetViewpoint( xyz, hpr);
}

void prepDrawStuff() {
  fn.version = DS_VERSION;
  fn.start = &start;
  fn.step = &simLoop;
  fn.command = NULL;
  fn.stop = NULL;
  fn.path_to_textures = "textures";
}

void createScene() {
  world = dWorldCreate();

  dMass m;
  dMassSetZero(&m);

  // sphere
  sphere.body = dBodyCreate(world);
  //  dReal radius = 0.5;
  dMassSetSphere(&m,DENSITY,radius);
  dBodySetMass(sphere.body,&m);
  dBodySetPosition(sphere.body,0.0,1.0,1.0);
  // box
  box.body = dBodyCreate(world);
  dMassSetBox(&m,DENSITY,sides[0],sides[1],sides[2]);
  dBodySetMass(box.body,&m);
  dBodySetPosition(box.body,0,2,1);
  // capsule
  capsule.body = dBodyCreate(world);
  dMassSetCapsule(&m,DENSITY,3,radius,length);
  dBodySetMass(capsule.body,&m);
  dBodySetPosition(capsule.body,0,3,1);
  // cylinder
  cylinder.body = dBodyCreate(world);
  dMassSetCylinder(&m,DENSITY,3,radius,length);
  dBodySetMass(cylinder.body,&m);
  dBodySetPosition(cylinder.body,0,4,1);
}

int main(int argc, char *argv[]) {

  // asign loop functions
  prepDrawStuff();

  // intialize ODE engine
  dInitODE();
  // create world and objects
  createScene();  
  // simululation loop
  dsSimulationLoop (argc, argv, 352, 288, &fn);
  // close ODE engine
  dWorldDestroy(world);
  dCloseODE();

  return 0;
}
