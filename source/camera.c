#include "camera.h"
#include "constants.h"

void updateCamera(Camera *camera, Joueur *player, bool loose, int *score)
{
    float milieu = 35;

    if(player->y < milieu && camera->y > player->y - milieu) {
        camera->y = player->y - milieu;
        *score = -camera->y/10;
    } 

    if(loose)
    {
        float marge = 90;
        float vx = VITESSE_MAX + 1;
        camera->y = camera->y + vx >= player->y - marge ? player->y - marge : camera->y + vx;
    }
}
