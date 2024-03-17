#include "GamesManager.h"


GamesManager::GamesManager() {}

void GamesManager::AddWaitingPlayer(std::shared_ptr<ClientConnection>& player) {

    waiting_players.push_back(player);
    player->SetClientState(ClientState::WAITING_FOR_OPPONENT);

}


ThreadSafeQueue<std::shared_ptr<GameMessage>>& GamesManager::GetGameMessagesReference() {

    return game_messages;

}
