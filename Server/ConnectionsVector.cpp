#include "ClientsVector.h"

ClientsVector::ClientsVector() {}
    
void ClientsVector::PushBack(std::shared_ptr<ClientConnection>& connection) {

    std::lock_guard<std::mutex> lg(&mutex);
    connections.push_back(connection);

}

bool ClientsVector::CheckIfClientAlreadyLogged(const std::string& login) {

    std::lock_gurad<std::mutex> lg(&mutex);
    for (auto& connection : connections) {
    
        if (login == connection->GetLogin()) {
        
            return true;

        }

    }
    return false;

}


