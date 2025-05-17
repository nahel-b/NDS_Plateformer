// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: NightFox & Co., 2009-2011
//
// NightFox's Lib Template
// http://www.nightfoxandco.com
#include <nds.h>
#include <time.h>

#include <filesystem.h>

#include <nf_lib.h>
#include <stdio.h>
#include <fat.h>


#define TAILLE_SCORE 5

#define GRAVITE 0.2
#define SAUT_VELOCITE -5

#define COTE_VELOCITE 0.3 
#define MAX_COTE_VELOCITE 4.0
#define FROTTEMENT_COTE 0.1

#define VITESSE_MAX 3

#define SCALE_PERSO 1

#define NB_PLATEFORMES 5

#define PLAYER_SPRITE_WIDTH 64
#define PLAYER_SPRITE_HEIGHT 64


#define MIN_X_PLATEFORME 10
#define MAX_X_PLATEFORME 180

#define MIN_VX_PLATEFORME 2
#define MAX_VX_PLATEFORME 3

#define TRAMPOLINE_VELOCITE -15


// 0 : charactere
// 1 - 5 : plateforme
// 6 - 10 : score Texte
// 16-20 : score best

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
    float  y;
} Camera;


struct VariablesToSave
{
	int HiScore;
} SaveFile;

//File system
FILE *savefile;

int nb_p = 0;

Platforme platformes[NB_PLATEFORMES] = {
    {50, 100,true,false,0,false},
    {140, 160,true,false,0,false},
    {100, 200,true,false,0,false},
    {150, 200,true,false,0,false},
    {200, 200,true,false,0,false}
};

void initPlatformes()
{
    NF_LoadSpriteGfx("sprite/plateforme", 1, 64, 32);  // Changed to index 1 to match later usage
    NF_LoadSpritePal("sprite/plateforme", 1);  
    NF_VramSpriteGfx(0, 1, 1, false); // Use sprite index 1 on screen 0
    NF_VramSpritePal(0, 1, 1);
    for (int i = 0; i < NB_PLATEFORMES; i++) {
                 // Changed to index 1 to match later usage

 
         

       NF_CreateSprite(0, i+1, 1, 1, platformes[i].x, platformes[i].y);
        NF_SpriteLayer(0,i+1,3);
    }  
}



void initScoreText()
{

    int espacement =  13;
    //load font
    NF_LoadSpriteGfx("sprite/font", 2, 16, 16);
    NF_LoadSpritePal("sprite/font", 2);

    NF_VramSpriteGfx(0, 2, 2, false); // Use sprite index 2 on screen 0
    NF_VramSpritePal(0, 2, 2);

    for(int i = NB_PLATEFORMES; i < NB_PLATEFORMES+TAILLE_SCORE; i++)
    {
        NF_CreateSprite(0, i+1, 2, 2, (i-NB_PLATEFORMES )*espacement, 0);
        NF_SpriteFrame(0, i+1, 0);
        NF_ShowSprite(0, i+1,i == NB_PLATEFORMES ? true : false);
        NF_SpriteLayer(0,i+1,1);
       // NF_EnableSpriteRotScale(0,i+1,0,true);
    }

   // NF_SpriteRotScale(0,0,0,350,400);
}

void hideScoreText()
{
    
   for(int i = NB_PLATEFORMES; i < NB_PLATEFORMES+TAILLE_SCORE; i++)
    {
        NF_ShowSprite(0,i+1,false);
    } 
}

void placeScoreTextMilieu()
{
    int espacement =  13;
    for(int i = NB_PLATEFORMES; i < NB_PLATEFORMES+TAILLE_SCORE; i++)
    {

        NF_MoveSprite(0, i+1, 130 + (i-NB_PLATEFORMES )*espacement, 34);
        //NF_ShowSprite(0,i+1,true);
    } 
}


void updateScoreText(int score)
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
    
    // for (int i = 0; i < TAILLE_SCORE; i++) {
    //     NF_ShowSprite(0, NB_PLATEFORMES + i + 1, false);
    // }
    
    tmp = score;
    for (int i = 0 ; i < num_digits ; i++) {
        int digit = tmp % 10;
        tmp /= 10;
        
        int sprite_id = NB_PLATEFORMES + (num_digits - 1 - i) + 1;
        NF_SpriteFrame(0, sprite_id, digit);
        NF_ShowSprite(0, sprite_id, true);
    }
    
}


void updateCamera(Camera *camera, Joueur *player,bool loose,int * score)
{
    float milieu = 35;

    if(player->y < milieu &&  camera->y > player->y - milieu) {
        camera->y = player->y - milieu;
        *score = -camera->y/10;
    } 

    if(loose)
    {
        float marge = 90;
        float vx = VITESSE_MAX + 1;
        camera->y = camera->y + vx  >= player->y - marge ? player->y - marge : camera->y + vx;
    }
    
}


void newPosPlateforme(int i)
{
    float min_y = 300; // plus grand impossible
    for (int i = 0; i < NB_PLATEFORMES; i++) {
        if(platformes[i].y < min_y){min_y =platformes[i].y;}

    }
    // new pos
    platformes[i].y = min_y - (rand()%10) - 50; // 50 - 60
    platformes[i].x = (((rand()%170)*7)%(MAX_X_PLATEFORME-MIN_X_PLATEFORME)) + MIN_X_PLATEFORME;
    platformes[i].visible = false;

    if(rand()%5 == 0)
    {
        platformes[i].x = rand()%2==0 ? MIN_X_PLATEFORME : MAX_X_PLATEFORME;
        nb_p++;
    }

    // mouvante ?
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

    NF_ShowSprite(0, i+1,false);
}


void updatePlatformes(Camera *camera, Joueur *player)
{
    for (int i = 0; i < NB_PLATEFORMES; i++) {
        if(platformes[i].visible) {
            // if (player.x > platformes[i].x && player.x < platformes[i].x + 64 &&
            //     player.y > platformes[i].y && player.y < platformes[i].y + 64) {
            //     // Collision detected
            //     player.vy = 0;
            //     player.y = platformes[i].y - 64; // Adjust player's position to be on top of the platform
            // }

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
                NF_ShowSprite(0, i+1,false);
                platformes[i].visible = false;
                newPosPlateforme(i);

            }
            else if(platformes[i].y < camera->y - 20)
            {
               NF_ShowSprite(0, i+1,false);
                platformes[i].visible = false; 
            }
            else{

                NF_MoveSprite(0, i+1, (int)platformes[i].x, (int)platformes[i].y - camera->y);
                NF_ShowSprite(0, i+1,true);
            }
        }
        else if(platformes[i].y > camera->y - 20 && platformes[i].y < camera->y + 200)
        {
            NF_MoveSprite(0, i+1, (int)platformes[i].x, (int)platformes[i].y - camera->y);
            platformes[i].visible = true;
            NF_ShowSprite(0, i+1,true);
        }
        else
        {
            NF_ShowSprite(0, i+1,false);

        }
       
    }
}




void sauter(Joueur *player,float velocite) {
    player->vy = velocite ;
}

void manageInput(Joueur *player)
{
    scanKeys();
    uint16_t keys = keysHeld();

    if (keys & KEY_UP) {
        sauter(player,SAUT_VELOCITE);
    }


    if (keys & KEY_LEFT) {

       player->vx = player->vx-COTE_VELOCITE < - MAX_COTE_VELOCITE ? - MAX_COTE_VELOCITE : player->vx-COTE_VELOCITE ;
        NF_HflipSprite(0, 0, true);

    } else if (keys & KEY_RIGHT) {


        player->vx = player->vx + COTE_VELOCITE >  MAX_COTE_VELOCITE ? MAX_COTE_VELOCITE : player->vx +COTE_VELOCITE ;
        NF_HflipSprite(0, 0, false);

    } 
}


void updatePlayer( Joueur *player)
{
    player->x += player->vx;
    player->y += player->vy;
    player->vy += GRAVITE;
    player->vy = player->vy > VITESSE_MAX ? VITESSE_MAX : player->vy;

   // float marge_x =64*1.4*(1-SCALE_PERSO);

    if(player->vx > 0)
        player->vx = player->vx - FROTTEMENT_COTE <= 0 ?  0 : player->vx - FROTTEMENT_COTE;
    else if(player->vx < 0)
       player->vx = player->vx + FROTTEMENT_COTE >= 0 ?  0 : player->vx + FROTTEMENT_COTE;
    
    
    // if (player->x < -marge_x) {
    //     player->x = -marge_x;
    // } else if (player->x > 255 - 32) {
    //     player->x = 255 - 32;
    // }

    // if (player->y < 0 || player->y > 159) {
    //     player->vy *= -1;
    // }

    if(player->x < 0)
    {
        player->x = 0;
        player->vx = 0;
        //player->vx = -player->vx/2;
    }
    else if(player->x > 255 - PLAYER_SPRITE_WIDTH)
    {
        player->x = 255 - PLAYER_SPRITE_WIDTH;
        player->vx = 0;
        //player->vx = -player->vx/2;
    }

    for( int i = 0; i < NB_PLATEFORMES; i++)
    {

        if(player->vy >= 0 && platformes[i].visible &&  
            player->x > platformes[i].x - 64/2 - (PLAYER_SPRITE_WIDTH/4)  && player->x < platformes[i].x + 64/2 + (PLAYER_SPRITE_WIDTH/4) &&
            player->y > platformes[i].y - PLAYER_SPRITE_HEIGHT && player->y < platformes[i].y - PLAYER_SPRITE_HEIGHT + 10
        ) {
            // Collision detected
            if(platformes[i].trampoline)
            {
                sauter(player,TRAMPOLINE_VELOCITE);   
            }
            else
            {
                sauter(player,SAUT_VELOCITE);
            }
            
        }
    }


    // if(player->y > 159-PLAYER_SPRITE_HEIGHT/2)
    // {
    //     //player->y = 80;
    //     sauter(player);
    // }
}

void init()
{
    // Prepare a NitroFS initialization screen
    NF_Set2D(0, 0);
    NF_Set2D(1, 0); 
    consoleDemoInit(); // Initialize console
    printf("\n NitroFS init. Please wait.\n\n");
    printf(" Iniciando NitroFS,\n por favor, espere.\n\n");
    swiWaitForVBlank();

     // Initialize NitroFS and set it as the root folder of the filesystem
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

    // Initialize 2D engine in both screens and use mode 0
    NF_Set2D(0, 0); 
   // NF_Set2D(1, 0);
   
    // Initialize FAT filesystem for save files
    fatInitDefault();
    
    // Initialize tiled backgrounds system
    NF_InitTiledBgBuffers();    // Initialize storage buffers
    NF_InitTiledBgSys(0);       // Top screen
   // NF_InitTiledBgSys(1);       // Bottom screen

    // Initialize sprite system
    NF_InitSpriteBuffers();     // Initialize storage buffers
    NF_InitSpriteSys(0);        // Top screen
   // NF_InitSpriteSys(1);        // Bottom screen


    // Load background files from NitroFS
    NF_LoadTiledBg("bg/bg3", "layer_3", 256, 256);

    NF_LoadTiledBg("bg/bgScore","bgScore",256, 256);

    // Create top screen background 
    NF_CreateTiledBg(0, 3, "layer_3");
    NF_CreateTiledBg(0, 2, "bgScore");

    NF_ScrollBg(0,2,0,120); // debut : 120 - fin : 30

    // Create bottom screen backgrounds
    // NF_CreateTiledBg(1, 3, "layer_3");


    // Load sprite files from NitroFS
    NF_LoadSpriteGfx("sprite/nahel", 0, PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT);  // Changed to index 1 to match later usage
    NF_LoadSpritePal("sprite/nahel", 0);           // Changed to index 1 to match later usage


    NF_VramSpriteGfx(0, 0, 0, true); // Use sprite index 1 on screen 0
    NF_VramSpritePal(0, 0, 0);

    
 
 
    initPlatformes();

    initScoreText();
} 



int main(int argc, char **argv)
{
    init();

    // Set random seed based on the current time
    srand(time(NULL));

    // Initialize player structure
    Joueur player;
    player.x = 152;
    player.y = 80;
    player.vx = 0;
    player.vy = SAUT_VELOCITE;

    // Initialize Camera structure
    Camera camera;
    camera.y = -20;


    int HighScore = 0;
    
    savefile = fopen("fat:/Plateformer.sav", "rb");
    if (savefile != NULL) {
        fread(&SaveFile, 1, sizeof(SaveFile), savefile);
        fclose(savefile);
        HighScore = SaveFile.HiScore;
        consoleClear();
        printf("Loaded high score: %d\n", HighScore);
        swiWaitForVBlank();
    } else {
        SaveFile.HiScore = 0;
        consoleClear();
        printf("No save file found, creating new one\n");
        swiWaitForVBlank();
    }


    NF_CreateSprite(0, 0, 0, 0, player.x, player.y); // Create on screen 0, ID 0, using graphics 1, palette 1

   // NF_EnableSpriteRotScale(0,0,0,true);

  //  NF_SpriteRotScale(0,0,0,256*SCALE_PERSO,256*SCALE_PERSO);

   
    
    
    while (1)
    {
        
        
        bool loose = false;
        int frameFinAnime = -1;
        int score = 0;
        // PARTIE DE JEU
        while(1)
        {
            updateCamera(&camera, &player,loose,&score);

            updateScoreText(score);
            manageInput(&player);

            updatePlayer(&player);
            NF_MoveSprite(0, 0, (int)player.x, (int)player.y - camera.y);
            updatePlatformes(&camera,&player);

            if(player.y > camera.y+140){loose = true;  NF_SpriteFrame(0, 0, 1);
                
                placeScoreTextMilieu();

                if (score > HighScore)
                {
                    HighScore = score;
                    SaveFile.HiScore = HighScore;
                    savefile = fopen("fat:/Plateformer.sav", "wb");
                    if (savefile != NULL) {
                        fwrite(&SaveFile, 1, sizeof(SaveFile), savefile);
                        fclose(savefile);
                        consoleClear();
                        printf("New high score saved: %d\n", HighScore);
                    } else {
                        consoleClear();
                        printf("Failed to save high score!\n");
                    }
                }
               // hideScoreText();
            
            }
            if(loose && frameFinAnime == -1){frameFinAnime = 0;}

            if(loose){frameFinAnime++;}

            if(frameFinAnime >= 0)
            {
                //120 - 30
                int frame = frameFinAnime > 90 ? 90 : frameFinAnime;
                NF_ScrollBg(0,2,0,120 - frame);
            }

            NF_SpriteOamSet(0);
            NF_SpriteOamSet(1);

            // Wait for the screen refresh
            swiWaitForVBlank();

            consoleClear();
            printf("x: %f,\ny:%f,\nvx:%f,\nvy:%f\ncam y:%f\nloose :%d\n%d", player.x, player.y, player.vx, player.vy,camera.y,loose,HighScore);

            // Update OAM
            oamUpdate(&oamMain);
            oamUpdate(&oamSub);
        }

        bool fin_animation = false;
        while(fin_animation)
        {





            
            NF_SpriteOamSet(0);
            NF_SpriteOamSet(1);

            // Wait for the screen refresh
            swiWaitForVBlank();

            consoleClear();

            // Update OAM
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

    // If this is reached, the program will return to the loader if the loader
    printf("\n Hello World!");
    // supports it.
    return 0;
}
