#include "GamesManager.h"
#include "Game.h"
#include "GameMessage.h"
#include "ClientConnection.h"


GamesManager::GamesManager(ThreadSafeQueue<GameResult>& game_results) : game_results(game_results) {

    board_navigation_map["1"] = 0;
    board_navigation_map["2"] = 1;
    board_navigation_map["3"] = 2;
    board_navigation_map["4"] = 3;
    board_navigation_map["5"] = 4;
    board_navigation_map["6"] = 5;
    board_navigation_map["7"] = 6;
    board_navigation_map["8"] = 7;

    board_navigation_map["a"] = 0;
    board_navigation_map["b"] = 1;
    board_navigation_map["c"] = 2;
    board_navigation_map["d"] = 3;
    board_navigation_map["e"] = 4;
    board_navigation_map["f"] = 5;
    board_navigation_map["g"] = 6;
    board_navigation_map["h"] = 7;

}

void GamesManager::Start() {


    games_creation_thread = std::thread(&GamesManager::MatchOpponents, this);
    messages_processing_thread = std::thread(&GamesManager::ProcessGameMessages, this);    

    games_creation_thread.join();
    messages_processing_thread.join();

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
        
        for (auto& new_game : new_games) {

            games.push_back(new_game);

        }

        new_games.clear();


    }

}


void GamesManager::ProcessGameMessages() {


    while (true) {

        game_messages.wait(); 

        while (!game_messages.empty()) {

            std::shared_ptr<GameMessage> game_message = game_messages.pop_back();

            std::string message_str(game_message->body.begin(), game_message->body.end());
        
            std::stringstream message_stream(message_str);
            boost::property_tree::ptree root;
            boost::property_tree::read_json(message_stream, root);

            if (root.empty()) {

                std::cout << "empty\n";

            } else {

                std::string action = root.get<std::string>("Action");

                if (action == "Make_move") {

                    if (game_message->sender->GetPlayerColor() == game_message->game->GetCurrentTurnPlayerColor()) {

                        //bool move_is_correct = game_message->game->CheckIfMoveIsCorrect()

                    }


                } else if (action == "Make_castling") {

                    

                } else if (action == "Offer_draw") {

                    

                } else if (action == "Accept_draw") {

                

                } else if (action == "Cancel_draw") {

                    

                } else if (action == "Resign") {

                    

                }

            
            }



        }


    }    


}


