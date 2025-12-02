//bh.cpp
#include "bh.h"
#include <cmath>
constexpr double G = 0.0001;
constexpr double R_LIMIT = 0.03;


void Subdivide(Node* n){
    double xm = (n->x_max + n->x_min) / 2.0;
    double ym = (n->y_max + n->y_min) / 2.0;

    n->children[0] = new Node(n->x_min, xm, ym, n->y_max); // NW
    n->children[1] = new Node(xm, n->x_max, ym, n->y_max); // NE
    n->children[2] = new Node(xm, n->x_max, n->y_min, ym); // SE
    n->children[3] = new Node(n->x_min, xm, n->y_min, ym); // SW


}

int location(Node* n, Body* b) {
    double xm = (n->x_max + n->x_min) / 2.0;
    double ym = (n->y_max + n->y_min) / 2.0;
    //ima set it up as a coordinate plane clockwise from NW. NW == children[0]
    if (xm <= b->x) { // East and if middle locate east
        if (ym <= b->y) { // North  if equal default up //children[1]
            return 1; //NE
        } else { // South //children[2]
            return 2; //SE
        }
    } else  { // West
        if (ym <= b->y) { // North  if equal default up //children[0]
            return 0; //NW
        } else { // South //children[3]
            return 3; //SW
        }
    }

    return 0;
}

void InsertBody(Node* n, Body* b){

    if (n->is_leaf && n->body == nullptr) {
        n->body = b;
        return;
    }
    // leaf with one body → subdivide
    if (n->is_leaf && n->body != nullptr) {
        Body* old = n->body;
        n->body = nullptr;
        n->is_leaf = false;
        Subdivide(n);

        int old_idx = location(n, old);
        InsertBody(n->children[old_idx], old);

        int new_idx = location(n, b);
        InsertBody(n->children[new_idx], b);
        return;
    }
    int idx = location(n, b);
    InsertBody(n->children[idx], b);

}


void ComputeMassDistribution(Node* n){
    if (n == nullptr) { return; }
    if (n->is_leaf){
        if (n->body != nullptr){
            n->mass = n->body->mass;
            n->cx = n->body->x;
            n->cy = n->body->y;

        } else {
            n->mass = 0.0;
            n->cx = 0.0;
            n->cy = 0.0;
        }
        return;
    }
    double total_mass = 0.0;
    double sum_cx = 0.0;
    double sum_cy = 0.0;

    // now we need to iterate and find COM 
    // and value of COM in non leaf nodes bc a non leaf can contain many bodies.
    for (int i = 0; i < 4; i++){
        Node* child = n->children[i];
        if (child == nullptr) continue;

        ComputeMassDistribution(child);

        total_mass += child->mass;
        sum_cx += child->mass * child->cx;
        sum_cy += child->mass * child->cy;
    } 
    n->mass = total_mass;
    if(total_mass > 0.0) {
        n->cx = sum_cx / total_mass;
        n->cy =  sum_cy / total_mass;
    } else {
        n->cx = 0.0;
        n->cy = 0.0;
    }
}

//if L / D < theta use node COM and mass
// otherwise use each individual bodies.

void ComputeForceOnBody(const Body* b, Node* n, double theta, double& Fx, double& Fy){

    if (n == nullptr) { return; }
    if (n->mass <= 0.0) { return; }

   // double d = ComputeDistance(b, n);
    double L = n->x_max - n->x_min;

    double dist = 0.0;

    double dx = n->cx - b->x;
    double dy = n->cy - b->y;
    double sq_dist = dx*dx + dy*dy;

    dist = sqrt(sq_dist);
    if (dist < R_LIMIT){
        dist = R_LIMIT;
    }

    double invDist = 1.0 / dist;
    double invDist3 = invDist * invDist * invDist;

    if (n->is_leaf) {
        if (n->body == nullptr) { return; }
        if (n->body == b) { return; }
        Fx += G * b->mass * n->mass * dx * invDist3;
        Fy += G * b->mass * n->mass * dy * invDist3;
        return;
    }

    double thresh = ( (double)L ) / dist;

    if (thresh < theta) { // too far use node mass
        Fx += G * b->mass * n->mass * dx * invDist3;
        Fy += G * b->mass * n->mass * dy * invDist3;
        return;

    }
    // use each body
    for (int i = 0; i < 4; i++){
        ComputeForceOnBody(b, n->children[i], theta, Fx, Fy);
    }
}

Node* BuildTree(std::vector<Body*>& bodies) {
    Node* root = new Node(0.0, 4.0, 0.0, 4.0);
    root->mass = 0;
    root->is_leaf = true;
    for (Body* b : bodies){
        if (b->mass < 0){
            continue;
        }
        InsertBody(root, b);
    }
    return root;
}

void FreeTree(Node* n){
    if (n == nullptr) return;
    if (!n->is_leaf){
        for (int i = 0; i < 4; i++){
            FreeTree(n->children[i]);
        }
    }
    delete n;
}

