#include "Server.h"

Server::Server(const size_t& port_id) : acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_id)) {


}

void Server::start_accept() {



}
