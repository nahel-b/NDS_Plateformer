#ifndef PLAYER_H
#define PLAYER_H

#include "structures.h"

void sauter(Joueur *player, float velocite);
void manageInput(Joueur *player);
void updatePlayer(Joueur *player);

#endif // PLAYER_H
