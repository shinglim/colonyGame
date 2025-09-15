//
// Created by Shing Lim on 8/8/25.
//

#include "MainMenu.h"
bool MainMenu::init() {
    if (!window.create("Mini RimWorld", 0, 0, true)) {
        return false;
    }
    Button ContinueGameButton(window.getWidth()/2,window.getHeight()/2,window.getWidth()/10,window.getHeight()/10);


    isRunning = true;
    return true;
}

void MainMenu::run() {
    handleEvents();
    update();
    render();
    SDL_Delay(100);
}

void MainMenu::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) isRunning = false;

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_g) {
                play = true;
                isRunning = false;
                std::cout << "pressed";

            }
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                clean();
            }
        }
        }
}

void MainMenu::update() {}

void MainMenu::render() {
    ContinueGameButton.render();
}

bool MainMenu::clean() {
    window.destroy();
    return play;
}