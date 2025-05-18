#include <nds.h>
#include <stdio.h>
#include <nf_lib.h>
#include "structures.h"
#include "constants.h"
#include "player.h"
#include "platform.h"
#include "camera.h"
#include "ui.h"
#include "save.h"

void gameLoop()
{
    Joueur player;
    player.x = 152;
    player.y = 80;
    player.vx = 0;
    player.vy = SAUT_VELOCITE;

    Camera camera;
    camera.y = -20;

    int HighScore = loadHighScore();

    NF_CreateSprite(0, 0, 0, 0, player.x, player.y);
    
    while (1)
    {
        bool loose = false;
        int frameFinAnime = -1;
        int score = 0;
        
        // PARTIE DE JEU
        while(1)
        {
            updateCamera(&camera, &player, loose, &score);

            if(!loose)
            {
                updateScoreText(score, NB_PLATEFORMES, TAILLE_SCORE);
            }
            manageInput(&player);

            updatePlayer(&player);
            NF_MoveSprite(0, 0, (int)player.x, (int)player.y - camera.y);
            updatePlatformes(&camera, &player);

            if(player.y > camera.y+140) {
                loose = true;
                NF_SpriteFrame(0, 0, 1);
                placeScoreTextMilieu();

                if (score > HighScore)
                {
                    HighScore = score;
                    saveHighScore(HighScore);
                }
            }
            
            if(loose && frameFinAnime == -1) {
                frameFinAnime = 0;
            }

            if(loose) {
                frameFinAnime++;
            }

            if(frameFinAnime >= 0)
            {
                int frame = frameFinAnime > 90 ? 90 : frameFinAnime;
                NF_ScrollBg(0, 2, 0, 120 - frame);
            }
            
            if(frameFinAnime >= 90)
            {
                initHightScoreText();
                int affScore = frameFinAnime - 90;
                updateScoreText(affScore >= score ? score : affScore, NB_PLATEFORMES, TAILLE_SCORE);
                int affHighScore = frameFinAnime - 90;
                updateScoreText(affHighScore >= HighScore ? HighScore : affHighScore, NB_PLATEFORMES + TAILLE_SCORE, TAILLE_SCORE);
            }

            NF_SpriteOamSet(0);
            NF_SpriteOamSet(1);

            swiWaitForVBlank();

            consoleClear();
            printf("x: %f,\ny:%f,\nvx:%f,\nvy:%f\ncam y:%f\nloose :%d\n%d", player.x, player.y, player.vx, player.vy, camera.y, loose, HighScore);

            oamUpdate(&oamMain);
            oamUpdate(&oamSub);
        }

        bool fin_animation = false;
        while(fin_animation)
        {
            NF_SpriteOamSet(0);
            NF_SpriteOamSet(1);
            swiWaitForVBlank();
            consoleClear();
            oamUpdate(&oamMain);
            oamUpdate(&oamSub);
        }

        bool restart = false;
        while(!restart)
        {
            consoleClear();
            printf("Game Over\n\n");
            printf("Press A to restart\n");
            printf("Press B to exit\n");

            scanKeys();
            uint16_t keys = keysHeld();

            if (keys & KEY_A) {
                restart = true;
                player.x = 152;
                player.y = 80;
                player.vx = 0;
                player.vy = SAUT_VELOCITE;
                camera.y = -20;

                for (int i = 0; i < NB_PLATEFORMES; i++) {
                    platformes[i].visible = false;
                    newPosPlateforme(i);
                }
            } else if (keys & KEY_B) {
                restart = true;
            }
        }
    }
}
