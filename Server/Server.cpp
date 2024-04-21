#include "Server.h"
#include "ClientConnection.h"
#include "ThreadSafeQueue.h"
#include "ThreadSafeQueue.cpp"


Server::Server(const size_t& port_id) : acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_id)), 
                                        client_connections(game_results), games_manager(game_results, client_connections) {}

bool Server::Start() {

    try {

        WaitForClients();

        context_thread = std::thread([this](){ io_context.run(); });


    } catch (std::exception& e) {

        std::cerr << "Server start exception " << e.what() << "\n";
        return false;

    }

    games_manager_thread = std::thread(&GamesManager::Start, &games_manager);
    games_manager_thread.detach();

    game_results_thread = std::thread(&Server::ProcessGameResults, this);
    game_results_thread.detach();


    std::cout << "Server started!\n";
    return true;

}


void Server::WaitForClients() {

    acceptor.async_accept([this](std::error_code ec, boost::asio::ip::tcp::socket socket) {

            if (!ec) {
                
                std::cout << "New connection " << socket.remote_endpoint() << "\n";
                std::shared_ptr<ClientConnection> new_connection = std::make_shared<ClientConnection>(std::move(socket), io_context, incoming_messages, 
                                                                                                      games_manager.GetGameMessagesReference());
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

    std::cout << "New message processing\n";

    std::string message_str(message->body.begin(), message->body.end());
    
    std::stringstream message_stream(message_str);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(message_stream, root);


    std::cout << message_str << "\n";


    if (root.empty()) {

        std::cout << "empty\n";

    } else {


        std::string action = root.get<std::string>("Action");


        if (action == "Login") {

            
            std::string nickname = root.get<std::string>("Nickname");
            std::string password = root.get<std::string>("Password");

            OnLogin(nickname, password, message->sender);             


        } else if (action == "Logout") {


            std::string nickname = root.get<std::string>("Nickname");
            OnLogout(nickname, message->sender);


        } else if (action == "Register") {

            
            std::string nickname = root.get<std::string>("Nickname");
            std::string password = root.get<std::string>("Password");

            OnRegister(nickname, password, message->sender);


        } else if (action == "Start_waiting") {

            
            if (message->sender->LoggedIn()) {

                std::cout << "Start waiting accepted!\n";

                games_manager.AddWaitingPlayer(message->sender);    
                
                boost::property_tree::ptree property_tree;
                property_tree.put("Action", "Start_waiting");
                property_tree.put("Code", "200");

                std::ostringstream json_stream;
                boost::property_tree::write_json(json_stream, property_tree);        
                std::string json_string = json_stream.str();
                
                std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

                std::shared_ptr<Message> new_message = std::make_shared<Message>();
                new_message->body = message_body;
                new_message->message_size = message_body.size();

                message->sender->SendMessage(new_message);         


            } else {

                
                std::cout << "Start waiting error!\n";
                
                boost::property_tree::ptree property_tree;
                property_tree.put("Action", "Start_waiting");
                property_tree.put("Code", "403");
                property_tree.put("Error_description", "User is not logged");

                std::ostringstream json_stream;
                boost::property_tree::write_json(json_stream, property_tree);        
                std::string json_string = json_stream.str();
                
                std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

                std::shared_ptr<Message> new_message = std::make_shared<Message>();
                new_message->body = message_body;
                new_message->message_size = message_body.size();

                message->sender->SendMessage(new_message);         


            }


        } else if (action == "Disconnect") {


            std::cout << message->sender->GetNickname() << " disconnected from server!\n";
            
            client_connections.delete_connection(message->sender->GetNickname());


        } 



    }



}


void Server::OnLogin(const std::string& nickname, const std::string& password, std::shared_ptr<ClientConnection>& client_connection) {


    if (client_connections.contains_nickname(nickname)) {


        boost::property_tree::ptree property_tree;
        property_tree.put("Action", "Login");
        property_tree.put("Code", "403");
        property_tree.put("Error_description", "User already logged");

        std::ostringstream json_stream;
        boost::property_tree::write_json(json_stream, property_tree);        
        std::string json_string = json_stream.str();
        
        std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

        std::shared_ptr<Message> message = std::make_shared<Message>();
        message->body = message_body;
        message->message_size = message_body.size();

        client_connection->SendMessage(message);         

        return;        

    }
    


    LoginResult result = sql_service.Login(nickname, password);

    if (result == LoginResult::SUCCESS) {
    
        
        std::cout << "Success login!\n";

        
        int rating = sql_service.GetPlayerRating(nickname);
        int games_played = sql_service.GetPlayersGamesPlayed(nickname);
        boost::property_tree::ptree top_players = sql_service.GetTopHundredPlayersRating();

        
        if (client_connection != nullptr) {
                
            std::cout << "Non empty client connection!\n";

        }
        
        client_connection->OnLoggedIn(nickname, rating, games_played);

        boost::property_tree::ptree property_tree;
        property_tree.put("Action", "Login");
        property_tree.put("Code", "200");
        property_tree.put("Nickname", nickname);
        property_tree.put("Rating", rating);
        property_tree.put("Games_played", games_played);
        property_tree.put_child("Top_players", top_players);

        std::cout << "Will create ostream\n";

        std::ostringstream json_stream;
        boost::property_tree::write_json(json_stream, property_tree);        
        std::string json_string = json_stream.str();
        
        
        std::cout << "Message body will be created\n";

        std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

        std::shared_ptr<Message> message = std::make_shared<Message>();
        message->body = message_body;
        message->message_size = message_body.size();

        std::cout << "message: " << json_string << "\n";

        client_connection->SendMessage(message); 
        


    } else if (result == LoginResult::INCORRECT_PASSWORD) {

        
        boost::property_tree::ptree property_tree;
        property_tree.put("Action", "Login");
        property_tree.put("Code", "403");
        property_tree.put("Error_description", "Incorrect password");

        std::ostringstream json_stream;
        boost::property_tree::write_json(json_stream, property_tree);        
        std::string json_string = json_stream.str();
        
        std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

        std::shared_ptr<Message> message = std::make_shared<Message>();
        message->body = message_body;
        message->message_size = message_body.size();

        client_connection->SendMessage(message); 
        


    } else if (result == LoginResult::NO_NICKNAME_IN_DATABASE) {


        boost::property_tree::ptree property_tree;
        property_tree.put("Action", "Login");
        property_tree.put("Code", "403");
        property_tree.put("Error_description", "Nickname is not registered");

        std::ostringstream json_stream;
        boost::property_tree::write_json(json_stream, property_tree);        
        std::string json_string = json_stream.str();
        
        std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

        std::shared_ptr<Message> message = std::make_shared<Message>();
        message->body = message_body;
        message->message_size = message_body.size();

        client_connection->SendMessage(message); 
 

    }


}

void Server::OnLogout(const std::string& nickname, std::shared_ptr<ClientConnection>& client_connection) {


    if (client_connection->LoggedIn() && client_connection->GetNickname() == nickname) {


        client_connection->Logout();        

        boost::property_tree::ptree property_tree;
        property_tree.put("Action", "Logout");
        property_tree.put("Code", "200");

        std::ostringstream json_stream;
        boost::property_tree::write_json(json_stream, property_tree);        
        std::string json_string = json_stream.str();
        
        std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

        std::shared_ptr<Message> message = std::make_shared<Message>();
        message->body = message_body;
        message->message_size = message_body.size();

        client_connection->SendMessage(message); 


    } else if (!client_connection->LoggedIn()) {


        boost::property_tree::ptree property_tree;
        property_tree.put("Action", "Logout");
        property_tree.put("Code", "403");
        property_tree.put("Error_description", "User is not logged in");

        std::ostringstream json_stream;
        boost::property_tree::write_json(json_stream, property_tree);        
        std::string json_string = json_stream.str();
        
        std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

        std::shared_ptr<Message> message = std::make_shared<Message>();
        message->body = message_body;
        message->message_size = message_body.size();

        client_connection->SendMessage(message); 
 

    } else if (client_connection->GetNickname() != nickname) {

        
        boost::property_tree::ptree property_tree;
        property_tree.put("Action", "Logout");
        property_tree.put("Code", "403");
        property_tree.put("Error_description", "Incorrect user nickname");

        std::ostringstream json_stream;
        boost::property_tree::write_json(json_stream, property_tree);        
        std::string json_string = json_stream.str();
        
        std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

        std::shared_ptr<Message> message = std::make_shared<Message>();
        message->body = message_body;
        message->message_size = message_body.size();

        client_connection->SendMessage(message); 
        
    
    }

}

void Server::OnDisconnect(const std::shared_ptr<ClientConnection>& client_connection) {

    client_connections.erase(client_connection);

}

void Server::OnRegister(const std::string& nickname, const std::string& password, std::shared_ptr<ClientConnection>& client_connection) {

    RegisterResult register_result = sql_service.Register(nickname, password); 

    if (register_result == RegisterResult::SUCCESS) {


        boost::property_tree::ptree property_tree;
        property_tree.put("Action", "Register");
        property_tree.put("Code", "200");
        property_tree.put("Nickname", nickname);

        std::ostringstream json_stream;
        boost::property_tree::write_json(json_stream, property_tree);        
        std::string json_string = json_stream.str();
        
        std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

        std::shared_ptr<Message> message = std::make_shared<Message>();
        message->body = message_body;
        message->message_size = message_body.size();

        client_connection->SendMessage(message); 
    
        
    } else if (register_result == RegisterResult::USER_ALREADY_EXISTS) {


        boost::property_tree::ptree property_tree;
        property_tree.put("Action", "Register");
        property_tree.put("Code", "403");
        property_tree.put("Error_description", "Nickname is already registered");

        std::ostringstream json_stream;
        boost::property_tree::write_json(json_stream, property_tree);        
        std::string json_string = json_stream.str();
        
        std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

        std::shared_ptr<Message> message = std::make_shared<Message>();
        message->body = message_body;
        message->message_size = message_body.size();

        client_connection->SendMessage(message); 


    }


}


void Server::ProcessGameResults() {

    
    while (true) {

        game_results.wait();

        while (!game_results.empty()) {

            std::shared_ptr<GameResult> result = game_results.pop_front();
             
            sql_service.AddGameResult(result->white_nickname, result->black_nickname, result->winner, result->number_of_moves, result->total_time);
 
            sql_service.UpdatePlayerRating(result->white_nickname, result->new_white_player_rating, result->white_player_games_played);
            sql_service.UpdatePlayerRating(result->black_nickname, result->new_black_player_rating, result->black_player_games_played);


            if (sql_service.CheckIfNeedToUpdateTopPlayersRating(result->white_nickname, result->black_nickname)) {

                boost::property_tree::ptree top_players = sql_service.GetTopHundredPlayersRating();

                boost::property_tree::ptree property_tree;
                property_tree.put("Action", "Update_top_players_rating");
                property_tree.put_child("Top_players", top_players);

                std::ostringstream json_stream;
                boost::property_tree::write_json(json_stream, property_tree);        
                std::string json_string = json_stream.str();
                
                std::vector<uint8_t> message_body(json_string.begin(), json_string.end());

                std::shared_ptr<Message> message = std::make_shared<Message>();
                message->body = message_body;
                message->message_size = message_body.size();

                client_connections.SendMessageToAllClients(message);

            }

            std::cout << "Updated ratings for " << result->white_nickname << " and " << result->black_nickname << "\n";

        }


    }


}


void Server::SetBestPlayersRating() {

    

}


void Server::OnStopWaiting(std::shared_ptr<ClientConnection>& client_connection) {


}

void Server::OnResign(const std::string& sender_nickname) {


}
    
void Server::OnOfferDraw(const std::string& sender_nickname) {


}
    
void Server::OnCancelDraw(const std::string& sender_nickname) {


}   



