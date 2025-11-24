#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "input.h"
#include "../../core/game.h"

void handle_input(InputCode input);


void handle_menu_input(InputCode input);
void handle_difficulty_input(InputCode input);
void handle_custom_input(InputCode input);
void handle_game_input(InputCode input);
void handle_game_over_input(InputCode input);

#endif