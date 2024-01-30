#include "ConnectionsVector.h"

ConnectionsVector::ConnectionsVector() {}
    
void ConnectionsVector::PushBack(std::shared_ptr<ClientConnection>& connection) {

    std::lock_guard<std::mutex> lg(mutex);
    connections.push_back(connection);

}

bool ConnectionsVector::CheckIfClientAlreadyLogged(const std::string& login) {

    std::lock_guard<std::mutex> lg(mutex);
    
    for (int i = 0; i < connections.size(); ++i) {
    
        if (login == connections[i]->GetLogin()) {
        
            return true;

        }

    }

    return false;

}

std::shared_ptr<ClientConnection> ConnectionsVector::GetOpponentForPlayer(std::shared_ptr<ClientConnection>& player) {

    std::lock_guard<std::mutex> lg(mutex);
    
    for (int i = 0; i < connections.size(); ++i) {

        if (player->GetLogin() != connections[i]->GetLogin() && connections[i]->GetClientState() == ClientState::WAITING_FOR_AN_OPPONENT) {

            return connections[i];

        }


    }

    return nullptr;

}

