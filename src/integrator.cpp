//integrator.cpp
#include "integrator.h"


void UpdateBody(Body* b, double Fx, double Fy, double dt){
    if (b->mass < 0.0) return;

    double ax = Fx / b->mass;
    double ay = Fy / b->mass;

    b->vx += ax * dt;
    b->vy += ay * dt;

    b->x += b->vx * dt;
    b->y += b->vy * dt;

    if (b->x < 0 || b->x > 4 || b->y < 0 || b->y > 4) {
        b->mass = -1; //lost
        //may need to update some other functions if this is how im marking it.
    }


}
