/* A simple bouncing ball example
   Collision, contact and impact depentd on dSpaceID class
   dSpaceID has several dGeomIDs to define collision scene 
     1. Define a new struct MyObject with members:
          body(type dBodyID)
          geom(type dGeomID)
 */

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#ifdef dDOUBLE
#define dsDrawSphere dsDrawSphereD
#endif

#define DENSITY (5.0)

const dReal radius = 0.2;
const dReal mass = 1.0;

static int flag = 0;

struct MyObject {
  dBodyID body;
  dGeomID geom;
};

static dWorldID world;
static dSpaceID space;
static dGeomID ground;
static dJointGroupID contactgroup;
static MyObject ball;

dsFunctions fn;
static void nearCallback(void *data, dGeomID o1, dGeomID o2) {
  const int N = 10;
  dContact contact[N];

  int isGround = ((ground==o1) || (ground==o2));

  int n = dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));

  if (isGround) {
    if (n>=1) flag=1;
    else flag=0;
    for (int i = 0; i<n; i++) {
      contact[i].surface.mode = dContactBounce;
      contact[i].surface.mu = dInfinity;
      contact[i].surface.bounce = 0.0;
      contact[i].surface.bounce_vel = 0.0;
      dJointID c = dJointCreateContact(world,
				       contactgroup,
				       &contact[i]);
      dJointAttach(c,
		   dGeomGetBody(contact[i].geom.g1),
		   dGeomGetBody(contact[i].geom.g2));
    }			       
  }
}

static void simLoop (int pause) {
  const dReal *pos, *R;

  flag = 0;
  dSpaceCollide(space,0,&nearCallback);

  dWorldStep(world,0.01);
  dJointGroupEmpty(contactgroup);
  
  // ball
  if (flag==0)
    dsSetColor(1.0,0.0,0.0);
  else
    dsSetColor(0.0,0.0,1.0);
  pos = dBodyGetPosition(ball.body);
  R = dBodyGetRotation(ball.body);
  dsDrawSphere(pos,R,radius);
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
  // create world
  world = dWorldCreate();
  // create collision space
  space = dHashSpaceCreate(0);
  contactgroup = dJointGroupCreate(0);

  dWorldSetGravity(world,0,0,-0.5);

  //create a ground
  ground = dCreatePlane(space,0,0,1,0);

  // create a ball
  dReal x0 = 0.0, y0 = 0.0,z0 = 2.0;
  dMass m1;
  ball.body = dBodyCreate(world);
  dMassSetZero(&m1);
  dMassSetSphere(&m1,mass,radius);
  dBodySetMass(ball.body,&m1);
  dBodySetPosition(ball.body,x0,y0,z0);

  ball.geom = dCreateSphere(space,radius);
  dGeomSetBody(ball.geom,ball.body);
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
