#include <nds.h>
#include <stdio.h>
#include "save.h"

struct VariablesToSave SaveFile;
FILE *savefile;

int loadHighScore()
{
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
    
    return HighScore;
}

void saveHighScore(int score)
{
    SaveFile.HiScore = score;
    savefile = fopen("fat:/Plateformer.sav", "wb");
    if (savefile != NULL) {
        fwrite(&SaveFile, 1, sizeof(SaveFile), savefile);
        fclose(savefile);
        consoleClear();
        printf("New high score saved: %d\n", score);
    } else {
        consoleClear();
        printf("Failed to save high score!\n");
    }
}
