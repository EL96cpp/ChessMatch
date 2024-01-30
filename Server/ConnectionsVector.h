#pragma once

#include <mutex>
#include <memory>
#include <vector>

#include "ClientConnection.h"

class ConnectionsVector {

public:
    ConnectionsVector();

    void PushBack(std::shared_ptr<ClientConnection>& connections);
    bool CheckIfClientAlreadyLogged(const std::string& login);
    std::shared_ptr<ClientConnection> GetOpponentForPlayer(std::shared_ptr<ClientConnection>& player);

private:
    std::vector<std::shared_ptr<ClientConnection>> connections;
    std::mutex mutex;

};

