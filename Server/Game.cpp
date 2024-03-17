#include "Game.h"

Game::Game(std::shared_ptr<ClientConnection>& first_player, 
           std::shared_ptr<ClientConnection>& second_player) : first_player(first_player), 
                                                               second_player(second_player) {}



