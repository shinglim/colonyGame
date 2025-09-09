#include "Game.h"
#include "MainMenu.h"
int main() {
    MainMenu menu;
    if (menu.init()) {
        while (menu.isRunning) {
            menu.run();
        }
        //clean return whether or not game window should be opened upon main menu close
        if (menu.clean()) {
            Game game;
            if (game.init()) {
                game.run();
            }
        }
    }

    return 0;
}
