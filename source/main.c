// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: NightFox & Co., 2009-2011
//
// NightFox's Lib Template
// http://www.nightfoxandco.com
#include <nds.h>
#include <time.h>

#include "constants.h"
#include "structures.h"
#include "graphics.h"
#include "game.h"


// 0 : charactere
// 1 - 5 : plateforme
// 6 - 10 : score Texte
// 16-20 : score best


int main(int argc, char **argv)
{
    initGraphics();

    // Set random seed based on the current time
    srand(time(NULL));

    // Start the game
    gameLoop();

    // If this is reached, the program will return to the loader
    printf("\n Hello World!");
    return 0;
}
