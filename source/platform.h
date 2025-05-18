#ifndef PLATFORM_H
#define PLATFORM_H

#include "structures.h"
#include "constants.h"

extern Platforme platformes[NB_PLATEFORMES];
extern int nb_p;

void initPlatformes();
void newPosPlateforme(int i);
void updatePlatformes(Camera *camera, Joueur *player);
void restartPlatformes(float playerX);

#endif // PLATFORM_H
