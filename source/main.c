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



#define GRAVITE 0.2
#define SAUT_VELOCITE -5

#define COTE_VELOCITE 4.0 
#define FROTTEMENT_COTE 0.5

#define VITESSE_MAX 3

#define SCALE_PERSO 1

#define NB_PLATEFORMES 5

#define PLAYER_SPRITE_WIDTH 64
#define PLAYER_SPRITE_HEIGHT 64





typedef struct {
    float x, y;
    float vx;
    float vy;
} Joueur;

typedef struct {
    float x, y;
    bool visible;
} Platforme;

typedef struct {
    float  y;
} Camera;



Platforme platformes[NB_PLATEFORMES] = {
    {50, 100,true},
    {140, 160,true},
    {100, 160,false},
    {150, 200,false},
    {200, 240,false}
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
        
    }  
}


void updateCamera(Camera *camera, Joueur *player)
{
    float milieu = 35;

    if(player->y < milieu &&  camera->y > player->y - milieu) {
        camera->y = player->y - milieu;
    } 
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

            if(platformes[i].y > camera->y + 200)
            {
                platformes[i].visible = false;
            }

            NF_ShowSprite(0, i+1,true);
            NF_MoveSprite(0, i+1, (int)platformes[i].x, (int)platformes[i].y - camera->y);
        }
        else
        {
            NF_ShowSprite(0, i+1,false);

        }
       
    }
}




void sauter(Joueur *player) {
    player->vy = SAUT_VELOCITE;
}

void manageInput(Joueur *player)
{
    scanKeys();
    uint16_t keys = keysHeld();

    if (keys & KEY_UP) {
        sauter(player);
    }


    if (keys & KEY_LEFT) {

       player->vx = -COTE_VELOCITE;
        NF_HflipSprite(0, 0, true);

    } else if (keys & KEY_RIGHT) {


        player->vx = COTE_VELOCITE;
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

    for( int i = 0; i < NB_PLATEFORMES; i++)
    {

        if(player->vy >= 0 && platformes[i].visible &&  
            player->x > platformes[i].x - 64/2 - (PLAYER_SPRITE_WIDTH/4)  && player->x < platformes[i].x + 64/2 + (PLAYER_SPRITE_WIDTH/4) &&
            player->y > platformes[i].y - PLAYER_SPRITE_HEIGHT && player->y < platformes[i].y - PLAYER_SPRITE_HEIGHT + 10
        ) {
            // Collision detected
            
            sauter(player);
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

    // Create top screen background 
    NF_CreateTiledBg(0, 3, "layer_3");

    // Create bottom screen backgrounds
    // NF_CreateTiledBg(1, 3, "layer_3");


    // Load sprite files from NitroFS
    NF_LoadSpriteGfx("sprite/nahel", 0, PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT);  // Changed to index 1 to match later usage
    NF_LoadSpritePal("sprite/nahel", 0);           // Changed to index 1 to match later usage


    NF_VramSpriteGfx(0, 0, 0, true); // Use sprite index 1 on screen 0
    NF_VramSpritePal(0, 0, 0);


    initPlatformes();

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


    NF_CreateSprite(0, 0, 0, 0, player.x, player.y); // Create on screen 0, ID 0, using graphics 1, palette 1

   // NF_EnableSpriteRotScale(0,0,0,true);

  //  NF_SpriteRotScale(0,0,0,256*SCALE_PERSO,256*SCALE_PERSO);
    
    bool lose = false;
    while (1)
    {
        

        updateCamera(&camera, &player);


        manageInput(&player);

        updatePlayer(&player);
        NF_MoveSprite(0, 0, (int)player.x, (int)player.y - camera.y);
        updatePlatformes(&camera,&player);

        if(player.y > camera.y+120){lose = true;}


        NF_SpriteOamSet(0);
        NF_SpriteOamSet(1);

        // Wait for the screen refresh
        swiWaitForVBlank();

        consoleClear();
        printf("x: %f,\ny:%f,\nvx:%f,\nvy:%f\ncam y:%f\nloose :%d\n", player.x, player.y, player.vx, player.vy,camera.y,lose);

        // Update OAM
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }

    // If this is reached, the program will return to the loader if the loader
    printf("\n Hello World!");
    // supports it.
    return 0;
}
