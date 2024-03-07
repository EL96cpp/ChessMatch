#include "Server.h"

Server::Server(const size_t& port_id) : acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_id)) {}

bool Server::Start() {

    try {

        WaitForClients();

        context_thread = std::thread([this](){ io_context.run(); });
        //context_thread.join();

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
                std::shared_ptr<ClientConnection> new_connection = std::make_shared<ClientConnection>(std::move(socket), io_context, incoming_messages);
                client_connections.push_back(new_connection);
                client_connections.back()->StartReadingMessage();

            } else {

                std::cout << "New connection error " << ec.message();                

            }

            WaitForClients();

            //client_connections.back()->StartReadingMessage();

        });

}


void Server::Update() {

    incoming_messages.wait();
    
    std::cout << "after waiting call in server Update\n";

    while (!incoming_messages.empty()) {
   
        std::cout << "message processing\n";

        auto message = incoming_messages.pop_front();

        OnMessage(message);

    }

}
  

void Server::OnMessage(std::shared_ptr<Message>& message) {

    std::string message_str(message->body.begin(), message->body.end());
    
    std::stringstream message_stream(message_str);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(message_stream, root);

    if (root.empty()) {

        std::cout << "empty\n";

    } else {

        std::cout << root.get<std::string>("Method") << "\n";

    }



}


