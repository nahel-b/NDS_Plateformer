#include <nds.h>
#include <nf_lib.h>
#include "player.h"
#include "constants.h"
#include "platform.h"

void sauter(Joueur *player, float velocite) {
    player->vy = velocite;
}

void manageInput(Joueur *player)
{
    scanKeys();
    uint16_t keys = keysHeld();

    // if (keys & KEY_UP) {
    //     sauter(player, SAUT_VELOCITE);
    // }

    if (keys & KEY_LEFT) {
        player->vx = player->vx-COTE_VELOCITE < - MAX_COTE_VELOCITE ? - MAX_COTE_VELOCITE : player->vx-COTE_VELOCITE;
        NF_HflipSprite(0, 0, true);
    } else if (keys & KEY_RIGHT) {
        player->vx = player->vx + COTE_VELOCITE >  MAX_COTE_VELOCITE ? MAX_COTE_VELOCITE : player->vx +COTE_VELOCITE;
        NF_HflipSprite(0, 0, false);
    } 
}

void updatePlayer(Joueur *player)
{
    player->x += player->vx;
    player->y += player->vy;
    player->vy += GRAVITE;
    player->vy = player->vy > VITESSE_MAX ? VITESSE_MAX : player->vy;

    if(player->vx > 0)
        player->vx = player->vx - FROTTEMENT_COTE <= 0 ?  0 : player->vx - FROTTEMENT_COTE;
    else if(player->vx < 0)
       player->vx = player->vx + FROTTEMENT_COTE >= 0 ?  0 : player->vx + FROTTEMENT_COTE;
    
    if(player->x < 0)
    {
        player->x = 0;
        player->vx = 0;
    }
    else if(player->x > 255 - PLAYER_SPRITE_WIDTH)
    {
        player->x = 255 - PLAYER_SPRITE_WIDTH;
        player->vx = 0;
    }

    extern Platforme platformes[NB_PLATEFORMES];
    for(int i = 0; i < NB_PLATEFORMES; i++)
    {
        if(player->vy >= 0 && platformes[i].visible &&  
            player->x > platformes[i].x - 64/2 - (PLAYER_SPRITE_WIDTH/4)  && player->x < platformes[i].x + 64/2 + (PLAYER_SPRITE_WIDTH/4) &&
            player->y > platformes[i].y - PLAYER_SPRITE_HEIGHT && player->y < platformes[i].y - PLAYER_SPRITE_HEIGHT + 10
        ) {
            if(platformes[i].trampoline)
            {
                sauter(player, TRAMPOLINE_VELOCITE);   
            }
            else
            {
                sauter(player, SAUT_VELOCITE);
            }
        }
    }
}
