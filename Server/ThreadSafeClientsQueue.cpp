#include "ThreadSafeClientsQueue.h"
#include "ClientConnection.h"
#include "Game.h"
#include "Game.cpp"
#include "ChessFigure.h"
#include "ChessFigure.cpp"
#include "GameResult.h"

ThreadSafeClientsQueue::ThreadSafeClientsQueue(ThreadSafeQueue<GameResult>& game_results_queue) : game_results_queue(game_results_queue) {}


bool ThreadSafeClientsQueue::contains_nickname(const std::string& nickname) {

    std::scoped_lock lock(blocking_mutex);

    return (std::find_if(deque.begin(), deque.end(), [this, nickname](const std::shared_ptr<ClientConnection>& element) {

            return element->GetNickname() == nickname;

    }) != deque.end());

}



void ThreadSafeClientsQueue::wait_for_players() {

    while (deque.size() < 2) {

        std::unique_lock<std::mutex> ul(cv_mutex);
        condition_variable.wait(ul);

        std::cout << "Wait for players " << deque.size() << "\n";

    }

}


std::vector<std::shared_ptr<Game>> ThreadSafeClientsQueue::create_games() {

    std::scoped_lock lock(blocking_mutex);

    std::vector<std::shared_ptr<Game>> games;

    for (int i = 0; i < deque.size()-1; ++i) {

        for (int j = i+1; j < deque.size(); ++j) {


            std::cout << "Will create game " << deque[i]->GetNickname() << " " << deque[j]->GetNickname();

            //Some problems, game constructor is undefined error!!

            std::shared_ptr<Game> game = std::make_shared<Game>(deque[i], deque[j], game_results_queue);
            deque[i]->SetGame(game);
            deque[j]->SetGame(game);
            games.push_back(game);

            deque[i]->SetIsWaiting(false);
            deque[j]->SetIsWaiting(false);


        }


    }

    auto it = std::remove_if(deque.begin(), deque.end(), [](std::shared_ptr<ClientConnection>& client) { return !client->IsWaiting(); });
    deque.erase(it, deque.end());


    return games;

}


