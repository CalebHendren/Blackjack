#include "Game.h"

int main() {
    Game game;
    game.displayWelcome();

    do {
        game.playRound();
    } while (game.playAgain());

    return 0;
}