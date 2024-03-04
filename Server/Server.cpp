#include "Server.h"

Server::Server(const size_t& port_id) : acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_id)) {}

bool Server::Start() {

    try {

        WaitForClients();

        context_thread = std::thread([this](){ io_context.run(); });
        context_thread.join();

    } catch (std::exception& e) {

        std::cerr << "Server start exception " << e.what() << "\n";
        return false;

    }

    std::cout << "Server started!\n";
    return true;

}


void Server::WaitForClients() {

    acceptor.async_accept([this](std::error_code ec, boost::asio::ip::tcp::socket socket) {

            if (!ec) {
                
                std::cout << "New connection " << socket.remote_endpoint() << "\n";
                std::shared_ptr<ClientConnection> new_connection = std::make_shared<ClientConnection>(std::move(socket), io_context);

            } else {

                std::cout << "New connection error " << ec.message();
                

            }

            WaitForClients();

        });

}
