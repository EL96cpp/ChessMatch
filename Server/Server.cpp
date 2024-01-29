#include "Server.h"

Server::Server(boost::asio::io_context& io_context) : io_context(io_context), acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 40000)) {

    

}
