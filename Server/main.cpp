#include <iostream>
#include <boost/asio.hpp>

#include "ThreadSafeQueue.h"
#include "Server.h"

int main() {

    try {

        Server server(40000);
        server.Start();

        while (true) {

            server.UpdateIncomingMessages();

        }

    } catch (std::exception& e) {
    
        std::cout << "Can't start server in main.cpp!\n";
        std::cerr << e.what() << "\n";

    }

    return 0;

}
