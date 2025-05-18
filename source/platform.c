#include <nds.h>
#include <stdlib.h>
#include <nf_lib.h>
#include "platform.h"
#include "constants.h"

int nb_p = 0;

void restartPlatformes(float playerX)
{
    for (int i = 0; i < NB_PLATEFORMES; i++) {
        platformes[i].visible = false;
        platformes[i].mouvante = false;
        platformes[i].trampoline = false;
        platformes[i].vx = 0;
        if(i== 0)
        {
            platformes[i].x = playerX ;
            platformes[i].y = 160;
            platformes[i].visible = true;
        }
        else
        {
            platformes[i].x = 100;
            platformes[i].y = 200;
        }
    }

    for(int i = 0; i < NB_PLATEFORMES; i++) {
        newPosPlateforme(i+1);
    }
    
}

Platforme platformes[NB_PLATEFORMES] = {
    {50, 100, true, false, 0, false},
    {140, 160, true, false, 0, false},
    {100, 200, true, false, 0, false},
    {150, 200, true, false, 0, false},
    {200, 200, true, false, 0, false}
};

void initPlatformes()
{
    NF_LoadSpriteGfx("sprite/plateforme", 1, 64, 32);
    NF_LoadSpritePal("sprite/plateforme", 1);  
    NF_VramSpriteGfx(0, 1, 1, false);
    NF_VramSpritePal(0, 1, 1);
    for (int i = 0; i < NB_PLATEFORMES; i++) {
       NF_CreateSprite(0, i+1, 1, 1, platformes[i].x, platformes[i].y);
       NF_SpriteLayer(0, i+1, 3);
    }  
}

void newPosPlateforme(int i)
{
    float min_y = 300;
    for (int i = 0; i < NB_PLATEFORMES; i++) {
        if(platformes[i].y < min_y){min_y = platformes[i].y;}
    }
    platformes[i].y = min_y - (rand()%10) - 50;
    platformes[i].x = (((rand()%170)*7)%(MAX_X_PLATEFORME-MIN_X_PLATEFORME)) + MIN_X_PLATEFORME;
    platformes[i].visible = false;

    if(rand()%5 == 0)
    {
        platformes[i].x = rand()%2==0 ? MIN_X_PLATEFORME : MAX_X_PLATEFORME;
        nb_p++;
    }

    if(rand()%3 == 0)
    {
        platformes[i].mouvante = true;
        platformes[i].vx = rand()%(MAX_VX_PLATEFORME-MIN_VX_PLATEFORME) + MIN_VX_PLATEFORME;
    }
    else
    {
       platformes[i].mouvante = false; 
    }

    if(rand()%8 == 0)
    {
        platformes[i].trampoline = true;
    }
    else
    {
        platformes[i].trampoline = false;
    }

    NF_ShowSprite(0, i+1, false);
}

void updatePlatformes(Camera *camera, Joueur *player)
{
    for (int i = 0; i < NB_PLATEFORMES; i++) {
        if(platformes[i].visible) {
            if(platformes[i].mouvante)
            {
                platformes[i].x += platformes[i].vx;
                if(platformes[i].x<MIN_X_PLATEFORME || platformes[i].x>MAX_X_PLATEFORME )
                {
                    platformes[i].vx *=-1;
                }
            }

            if(platformes[i].y > camera->y + 200)
            {
                NF_ShowSprite(0, i+1, false);
                platformes[i].visible = false;
                newPosPlateforme(i);
            }
            else if(platformes[i].y < camera->y - 20)
            {
               NF_ShowSprite(0, i+1, false);
               platformes[i].visible = false; 
            }
            else{
                NF_MoveSprite(0, i+1, (int)platformes[i].x, (int)platformes[i].y - camera->y);
                NF_ShowSprite(0, i+1, true);
            }
        }
        else if(platformes[i].y > camera->y - 20 && platformes[i].y < camera->y + 200)
        {
            NF_MoveSprite(0, i+1, (int)platformes[i].x, (int)platformes[i].y - camera->y);
            platformes[i].visible = true;
            NF_ShowSprite(0, i+1, true);
        }
        else
        {
            NF_ShowSprite(0, i+1, false);
        }
    }
}
