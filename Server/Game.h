#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "ClientConnection.h"
#include "ChessFigure.h"

class Game {

public:
    Game(std::shared_ptr<ClientConnection>& first_player, std::shared_ptr<ClientConnection>& second_player);

private:
    std::shared_ptr<ClientConnection> first_player;
    std::shared_ptr<ClientConnection> second_player;

    

};



