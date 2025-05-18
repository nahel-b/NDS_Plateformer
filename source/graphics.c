#include <nds.h>
#include <nf_lib.h>
#include <filesystem.h>
#include <fat.h>
#include "constants.h"
#include "platform.h"
#include "ui.h"

void initGraphics()
{
    // Prepare a NitroFS initialization screen
    NF_Set2D(0, 0);
    NF_Set2D(1, 0); 
    consoleDemoInit();
    printf("\n NitroFS init. Please wait.\n\n");
    printf(" Iniciando NitroFS,\n por favor, espere.\n\n");
    swiWaitForVBlank();

    // Initialize NitroFS and set it as the root folder of the filesystem
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

    // Initialize 2D engine in both screens and use mode 0
    NF_Set2D(0, 0); 
   
    // Initialize FAT filesystem for save files
    fatInitDefault();
    
    // Initialize tiled backgrounds system
    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(0);

    // Initialize sprite system 
    NF_InitSpriteBuffers();
    NF_InitSpriteSys(0);

    // Load background files from NitroFS
    NF_LoadTiledBg("bg/bg3", "layer_3", 256, 256);
    NF_LoadTiledBg("bg/bgScore","bgScore", 256, 256);

    // Create top screen background 
    NF_CreateTiledBg(0, 3, "layer_3");
    NF_CreateTiledBg(0, 2, "bgScore");

    NF_ScrollBg(0, 2, 0, 120);

    // Load sprite files from NitroFS
    NF_LoadSpriteGfx("sprite/nahel", 0, PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT);
    NF_LoadSpritePal("sprite/nahel", 0);

    NF_VramSpriteGfx(0, 0, 0, true);
    NF_VramSpritePal(0, 0, 0);
 
    initPlatformes();
    initScoreText();
}
