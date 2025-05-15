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



typedef struct {
    float x, y;
    float vx;
    float vy;
} Joueur;

typedef struct {
    float x, y;
    bool visible;
} Platforme;





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


void updatePlayer(Joueur *player)
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
    if(player->y > 159-50)
    {
        //player->y = 80;
        sauter(player);
    }
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
    NF_LoadSpriteGfx("sprite/nahel", 0, 64, 64);  // Changed to index 1 to match later usage
    NF_LoadSpritePal("sprite/nahel", 0);           // Changed to index 1 to match later usage


    NF_VramSpriteGfx(0, 0, 0, true); // Use sprite index 1 on screen 0
    NF_VramSpritePal(0, 0, 0);
}

int main(int argc, char **argv)
{
    init();

    // Set random seed based on the current time
    srand(time(NULL));

    // Initialize player structure
    Joueur player;
    player.x = -60;
    player.y = 20;
    player.vx = 0;
    player.vy = SAUT_VELOCITE;


    NF_CreateSprite(0, 0, 0, 0, player.x, player.y); // Create on screen 0, ID 0, using graphics 1, palette 1

   // NF_EnableSpriteRotScale(0,0,0,true);

  //  NF_SpriteRotScale(0,0,0,256*SCALE_PERSO,256*SCALE_PERSO);
    
    
    while (1)
    {
        

        manageInput(&player);

        updatePlayer(&player);
        NF_MoveSprite(0, 0, (int)player.x, (int)player.y);

        NF_SpriteOamSet(0);
        NF_SpriteOamSet(1);

        // Wait for the screen refresh
        swiWaitForVBlank();

        consoleClear();
        printf("x: %f, y: %f, vx: %f, vy: %f\n", player.x, player.y, player.vx, player.vy);

        // Update OAM
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }

    // If this is reached, the program will return to the loader if the loader
    printf("\n Hello World!");
    // supports it.
    return 0;
}
