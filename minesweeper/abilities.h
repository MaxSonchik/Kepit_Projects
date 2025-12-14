#ifndef ABILITIES_H
#define ABILITIES_H

#include "common.h"

// Ability functions
bool activateDetector(GameState* game, int level);
bool activateShield(GameState* game, int level);
bool activateScanner(GameState* game, int level);
void updateShield(GameState* game);
int getAbilityCooldown(GameState* game, int abilityType); // Returns remaining cooldown in seconds, or 0 if ready

#endif // ABILITIES_H
