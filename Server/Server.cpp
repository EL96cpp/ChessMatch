#include "Server.h"

Server::Server(const size_t& port_id) : acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_id)) {}

bool Server::Start() {

    try {

        WaitForClients();

        context_thread = std::thread([this](){ io_context.run(); });

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

        });

}


void Server::UpdateIncomingMessages() {

    incoming_messages.wait();
    
    while (!incoming_messages.empty()) {
   
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

        std::string method = root.get<std::string>("Method");
        std::string resource = root.get<std::string>("Resource");

        std::cout << method << " " << resource << "\n";

        if (method == "POST") {

            if (resource == "Login") {

                std::string nickname = root.get<std::string>("Nickname");
                std::string password = root.get<std::string>("Password");

                OnLogin(nickname, password);              


            } else if (resource == "Register") {


            } else if (resource == "Start_waiting") {


            } 



        }


    }



}


void Server::OnLogin(const std::string& nickname, const std::string& password) {

    LoginResult result = sql_service.Login(nickname, password);

    if (result == LoginResult::SUCCESS) {
    
        
        std::cout << "Success login!\n";

        
        int rating = sql_service.GetPlayerRating(nickname);
        int games_played = sql_service.GetPlayersGamesPlayed(nickname);
        std::map<std::string, int> top_players_rating = sql_service.GetTopHundredPlayersRating();
        
        boost::property_tree::ptree property_tree;
        property_tree.put("Method", "POST");
        property_tree.put("Resource", "Login");
        property_tree.put("Code", "200");
        property_tree.put("Rating", rating);
        property_tree.put("Games_played", games_played);
        


    } else if (result == LoginResult::INCORRECT_PASSWORD) {



    } else if (result == LoginResult::NO_NICKNAME_IN_DATABASE) {


    }


}

void Server::OnLogout(const std::string& nickname) {


}

void Server::OnDisconnect(const std::string& nickname) {


}

void Server::OnRegister(const std::string& nickname, const std::string& password) {


}
    

void Server::OnStartWaiting(const std::string& nickname) {


}
    
void Server::OnStopWaiting(const std::string& nickname) {


}

void Server::OnResign(const std::string& sender_nickname) {


}
    
void Server::OnOfferDraw(const std::string& sender_nickname) {


}
    
void Server::OnCancelDraw(const std::string& sender_nickname) {


}   



