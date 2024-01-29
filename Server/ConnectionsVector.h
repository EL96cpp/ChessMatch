#pragma once

#include <mutex>
#include <memory>
#include <vector>

ConnectionsVector {

public:
    ConnectionsVector();

    void PushBack(std::shared_ptr<ClientConnection>& connections);
    bool CheckIfClientAlreadyLogged(const std::string& login);


private:
    std::vector<std::shared_ptr<ClientConnection>> connections;
    std::mutex mutex;

};

