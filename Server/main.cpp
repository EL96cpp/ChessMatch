#include <iostream>
#include <boost/asio.hpp>

#include "Server.h"

int main() {

    try {

        Server server(40000);
    
    } catch (std::exception& e) {
    
        std::cerr << e.what() << "\n";

    }

    return 0;

}
