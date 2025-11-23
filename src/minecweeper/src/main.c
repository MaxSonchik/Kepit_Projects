#include <stdio.h>
#include <stdlib.h>
#include "core/game.h"
#include "ui/display.h"
#include "ui/menu.h"
#include "data/config.h"

void play_game() {
    initialize_game();

    while (1) {
        print_field();

        if (is_game_over()) {
            print_game_over();
            break;
        }
        
        if (check_win()) {
            print_field();
            print_victory();
            break;
        }

        show_game_menu();
        char cmd = ' ';
        int x, y;
        
        int result = scanf(" %c", &cmd);
        if (result != 1) {
            while (getchar() != '\n');
            printf("Ошибка ввода! Попробуйте снова\n");
            continue;
        }
        
        if (cmd == 'q') break;

        if (cmd == 'o' || cmd == 'f') {
            result = scanf("%d %d", &x, &y);
            if (result != 2) {
                while (getchar() != '\n');
                printf("Ошибка координат: Используйте: o x y\n");
                continue;
            }
            
            x = x - 1;
            y = y - 1;

            if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
                printf("Неверные координаты! Диапозон: 0-%d\n", SIZE-1);
                continue;
            }

            if (cmd == 'o') {
                reveal(x,y);
            } else if (cmd == 'f') {
                place_flag(x, y);
            }
        } else {
            printf("Неизвестная команда! Используйте o, f или q\n");
        }
    }
}

int main() {
    while (1) {
        show_main_menu();
        int choice;

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Неверный выбор! ВВедите 1 или 2\n");
            continue;
        }

        switch (choice) {
            case 1:
                play_game();
                break;
            case 2:
                printf("До свидания\n");
                return 0;
            default:
                printf("Неверный выбор\n");
        }
    }
    return 0;
}