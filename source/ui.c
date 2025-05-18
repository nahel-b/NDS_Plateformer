#include <nds.h>
#include <nf_lib.h>
#include "ui.h"
#include "constants.h"

void initScoreText()
{
    int espacement = 13;
    NF_LoadSpriteGfx("sprite/font", 2, 16, 16);
    NF_LoadSpritePal("sprite/font", 2);

    NF_VramSpriteGfx(0, 2, 2, false);
    NF_VramSpritePal(0, 2, 2);

    for(int i = NB_PLATEFORMES; i < NB_PLATEFORMES+TAILLE_SCORE; i++)
    {
        NF_CreateSprite(0, i+1, 2, 2, (i-NB_PLATEFORMES)*espacement, 0);
        NF_SpriteFrame(0, i+1, 0);
        NF_ShowSprite(0, i+1, i == NB_PLATEFORMES ? true : false);
        NF_SpriteLayer(0, i+1, 1);
    }
}



void initHightScoreText()
{
    int espacement = 13;
    
    for(int i = NB_PLATEFORMES + TAILLE_SCORE; i < NB_PLATEFORMES+(2*TAILLE_SCORE); i++)
    {
        NF_CreateSprite(0, i+1, 2, 2, 130 + (i-NB_PLATEFORMES - TAILLE_SCORE)*espacement, 63);
        NF_SpriteFrame(0, i+1, 0);
        NF_ShowSprite(0, i+1, false);
        NF_SpriteLayer(0, i+1, 1);
    }
}

void restartText()
{
    
    int espacement = 13;
    for(int i = NB_PLATEFORMES; i < NB_PLATEFORMES+TAILLE_SCORE; i++)
    {
        NF_ShowSprite(0, i+1, false);
        NF_MoveSprite(0, i+1,(i-NB_PLATEFORMES)*espacement, 0);
    } 
    for(int i = NB_PLATEFORMES + TAILLE_SCORE; i < NB_PLATEFORMES+(2*TAILLE_SCORE); i++)
    {
        NF_ShowSprite(0, i+1, false);
    } 
}

void hideScoreText()
{
    for(int i = NB_PLATEFORMES; i < NB_PLATEFORMES+TAILLE_SCORE; i++)
    {
        NF_ShowSprite(0, i+1, false);
    } 
}

void placeScoreTextMilieu()
{
    int espacement = 13;
    for(int i = NB_PLATEFORMES; i < NB_PLATEFORMES+TAILLE_SCORE; i++)
    {
        NF_MoveSprite(0, i+1, 130 + (i-NB_PLATEFORMES)*espacement, 34);
        NF_ShowSprite(0, i+1, false);
    } 
}

void updateScoreText(int score, int firstSpriteId, int taille)
{
    int tmp = score;
    int num_digits = 0;
    
    if (tmp == 0) {
        num_digits = 1;
    } else {
        while (tmp > 0) {
            num_digits++;
            tmp /= 10;
        }
    }
    
    tmp = score;
    for (int i = 0; i < num_digits; i++) {
        int digit = tmp % 10;
        tmp /= 10;
        
        int sprite_id = firstSpriteId + (num_digits - 1 - i) + 1;
        if(sprite_id >= firstSpriteId + taille)
        {
            return;
        }
        NF_SpriteFrame(0, sprite_id, digit);
        NF_ShowSprite(0, sprite_id, true);
    }
}
