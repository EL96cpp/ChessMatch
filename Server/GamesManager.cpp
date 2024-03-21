#include "GamesManager.h"
#include "Game.h"
#include "GameMessage.h"
#include "ClientConnection.h"


GamesManager::GamesManager() {}

void GamesManager::Start() {


    games_creation_thread = std::thread(&GamesManager::MatchOpponents, this);
    messages_update_thread = std::thread(&GamesManager::UpdateGameMessages, this);    

    games_creation_thread.join();
    messages_update_thread.join();

}


void GamesManager::AddWaitingPlayer(std::shared_ptr<ClientConnection>& player) {

    waiting_players.push_back(player);

    std::cout << "Start waiting for opponent, player: " << player->GetNickname() << "\n";

    player->SetIsWaiting(true);

}


ThreadSafeGameMessagesQueue& GamesManager::GetGameMessagesReference() {

    return game_messages;

}


void GamesManager::MatchOpponents() {


    while (true) {

        waiting_players.wait_for_players();        

        std::vector<std::shared_ptr<Game>> new_games = waiting_players.create_games();


    }

}


void GamesManager::UpdateGameMessages() {



}


