#pragma once
#include <vector>

struct Body {
    int index;
    double x, y;
    double mass;
    double vx, vy;
};

struct Node {
    double x_min, x_max;
    double y_min, y_max;

    double mass;
    double cx, cy;

    Node* children[4];
    Body* body;
    bool is_leaf;

    Node(double xmin, double xmax, double ymin, double ymax)
        : x_min(xmin), x_max(xmax), y_min(ymin), y_max(ymax),
          mass(0), cx(0), cy(0), body(nullptr), is_leaf(true)
    {
        for (int i = 0; i < 4; i++)
            children[i] = nullptr;
    }
};

Node* BuildTree(std::vector<Body*>& bodies); 
void FreeTree(Node* n);
void ComputeMassDistribution(Node* n);
void ComputeForceOnBody(const Body* b, Node* n, double theta, double& Fx, double& Fy);
