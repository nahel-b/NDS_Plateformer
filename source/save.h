#ifndef SAVE_H
#define SAVE_H

#include <stdio.h>
#include "structures.h"

extern struct VariablesToSave SaveFile;
extern FILE *savefile;

int loadHighScore();
void saveHighScore(int score);

#endif // SAVE_H
