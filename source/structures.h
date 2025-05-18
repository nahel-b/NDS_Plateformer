#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>

typedef struct {
    float x, y;
    float vx;
    float vy;
} Joueur;

typedef struct {
    float x, y;
    bool visible;

    bool mouvante;
    float vx;

    bool trampoline;
} Platforme;

typedef struct {
    float y;
} Camera;

struct VariablesToSave
{
    int HiScore;
};

#endif // STRUCTURES_H
