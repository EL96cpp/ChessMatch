#include <iostream>
#include <boost/asio.hpp>

#include "Server.h"

int main() {

    boost::asio::io_context io_context;
    Server server(io_context);

    return 0;

}
