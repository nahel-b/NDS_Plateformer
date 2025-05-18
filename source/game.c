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


void restartGame(Joueur *player, Camera *camera)
{
    player->x = player->x == -1 ? 152 : player->x;
    player->y = 80;
    player->vx = 0;
    player->vy = SAUT_VELOCITE;
    camera->y = -20;
    NF_SpriteFrame(0, 0, 0);
    restartPlatformes(player->x );
    NF_ScrollBg(0, 2, 0, 120);
    NF_ScrollBg(1, 2, 0, 130);
    restartText();
}


void gameLoop()
{
    Joueur player;
    player.x = -1;
    Camera camera;

    restartGame(&player, &camera);
 

    int HighScore = loadHighScore();

    NF_CreateSprite(0, 0, 0, 0, player.x, player.y);
    
    while (1)
    {
        bool loose = false;
        int frameFinAnime = -1;
        int score = 0;
        
        bool skipAnimation = false;
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

                NF_ScrollBg(1, 2, 0,frameFinAnime*1.5 > 130 ? 0 : frameFinAnime*1.5 - 130);
            }
            
            if(frameFinAnime >= 90)
            {
                
                int affScore = frameFinAnime - 90;
                updateScoreText(affScore >= score ? score : affScore, NB_PLATEFORMES, TAILLE_SCORE);
                int affHighScore = frameFinAnime - 90;
                updateScoreText(affHighScore >= HighScore ? HighScore : affHighScore, NB_PLATEFORMES + TAILLE_SCORE, TAILLE_SCORE);

                if(affScore>=score && affHighScore>=HighScore)
                {
                    break;
                }
                scanKeys();
                uint16_t keys_held = keysHeld();
                if (keys_held & KEY_TOUCH)
                {
                    skipAnimation = true;
                            break;
                }

                
            }

            NF_SpriteOamSet(0);
            NF_SpriteOamSet(1);

            swiWaitForVBlank();

            //consoleClear();
            //printf("x: %f,\ny:%f,\nvx:%f,\nvy:%f\ncam y:%f\nloose :%d\n%d", player.x, player.y, player.vx, player.vy, camera.y, loose, HighScore);

            oamUpdate(&oamMain);
            oamUpdate(&oamSub);
        }

        // WAIT TOUCH RESTART
        int frameRestart = 0;
        while(!skipAnimation)
        {
            frameRestart+=4;

           // NF_ScrollBg(1, 2, 0,frameRestart > 130 ? 0 : frameRestart - 130);

            scanKeys();
            uint16_t keys_held = keysHeld();
            if (keys_held & KEY_TOUCH)
                        break;
            //printf("Raw coords: [%d, %d]\n", touch_pos.rawx, touch_pos.rawy);

            NF_SpriteOamSet(0);
            NF_SpriteOamSet(1);
            swiWaitForVBlank();
            consoleClear();
            oamUpdate(&oamMain);
            oamUpdate(&oamSub);
        }
        restartGame(&player, &camera);
       
    }
}
