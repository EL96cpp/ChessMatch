#include "SqlService.h"


SqlService::SqlService() : sql_connection("host=localhost user=postgres password=postgres dbname=chessmatch") {}   


bool SqlService::CheckIfUserExists(const std::string& nickname) {
    
    sql_connection.prepare("check_exists", "SELECT EXISTS(SELECT * FROM users WHERE nickname = $1)");
    pqxx::work work{sql_connection};
    pqxx::result result = work.exec_prepared("check_exists", nickname);

    bool user_exists = result[0][0].as<bool>();

    std::cout << user_exists << " result\n";

    return user_exists;
    
}


LoginResult SqlService::Login(const std::string& nickname, const std::string& password) {

    std::cout << "sql service login " << nickname << " " << password << "\n";

    if (CheckIfUserExists(nickname)) {

        std::cout << "before prepeare\n";

        sql_connection.prepare("get_password", "SELECT password FROM users WHERE nickname = $1");
        
        std::cout << "before work creation\n";

        pqxx::work work{sql_connection};
        pqxx::result result = work.exec_prepared("get_password", nickname);

        std::string correct_password = result[0]["password"].c_str();
        
        if (password == correct_password) {

            std::cout << "Correct password" << correct_password << "\n";

            return LoginResult::SUCCESS;

        } else {

            std::cout << "Incorrect password " << password << "\n";

            return LoginResult::INCORRECT_PASSWORD;

        }

    } else {

        std::cout << "No nickname in database " << nickname << "\n";

        return LoginResult::NO_NICKNAME_IN_DATABASE;

    }    

}
    

RegisterResult SqlService::Register(const std::string& nickname, const std::string& password) {

    if (!CheckIfUserExists(nickname)) {

        sql_connection.prepare("register", "INSERT INTO users VALUES($1, $2, 0, 0)");
        pqxx::work work{sql_connection};

        work.exec_prepared("register", nickname, password);
        work.commit();

        return RegisterResult::SUCCESS; 
        

    } else {

        return RegisterResult::USER_ALREADY_EXISTS;

    }    


}


void SqlService::AddGameResult(const std::string& white_nickname, const std::string& black_nickname, const std::string& winner, const int& number_of_moves, const int& total_time) {

    sql_connection.prepare("insert_game_result", "INSERT INTO games VALUES($1, $2, $3, $4, $5)");
    pqxx::work work{sql_connection};

    work.exec_prepared("insert_game_result", white_nickname, black_nickname, winner, number_of_moves, total_time);
    work.commit();
 

}


UpdateRatingResult SqlService::UpdatePlayerRating(const std::string& nickname, const size_t& new_rating, const size_t& games_played) {

    if (CheckIfUserExists(nickname)) {

        std::cout << "Sql service update " << nickname << " new rating " << new_rating << " " << games_played << "\n";

        sql_connection.prepare("update_rating", "UPDATE users SET rating = $1, games_played = $2 WHERE nickname = $3");
        pqxx::work work{sql_connection};
        
        work.exec_prepared("update_rating", new_rating, games_played, nickname);
        work.commit();

        return UpdateRatingResult::SUCCESS; 
        

    } else {

        return UpdateRatingResult::NO_NICKNAME_IN_DATABASE;

    }    


}



int SqlService::GetPlayerRating(const std::string& nickname) {

    sql_connection.prepare("get_player_rating", "SELECT rating FROM users WHERE nickname = $1");
    pqxx::work work{sql_connection};

    pqxx::result result = work.exec_prepared("get_player_rating", nickname);
    int rating = result[0]["rating"].as<int>();

    return rating;

} 


int SqlService::GetPlayersGamesPlayed(const std::string& nickname) {

    sql_connection.prepare("get_player_games_played", "SELECT games_played FROM users WHERE nickname = $1");
    pqxx::work work{sql_connection};

    pqxx::result result = work.exec_prepared("get_player_games_played", nickname);
    int games_played = result[0]["games_played"].as<int>();

    return games_played;
 
} 



boost::property_tree::ptree SqlService::GetTopHundredPlayersRating() {

    pqxx::work work{sql_connection};

    pqxx::result result = work.exec("SELECT nickname, rating FROM users ORDER BY rating DESC LIMIT 100");

    boost::property_tree::ptree ratings;

    for (int i = 0; i < result.size(); ++i) {

        boost::property_tree::ptree line;
        line.put(result[i][0].as<std::string>(), result[i][1].as<int>());
        
        ratings.push_back(std::make_pair("", line));        
        
        std::cout << "Rating " << result[i][1].as<int>() << "\n";

    }

    return ratings;
 
}


bool SqlService::CheckIfNeedToUpdateTopPlayersRating(const std::string& first_nickname, const std::string& second_nickname) {

    pqxx::work work{sql_connection};

    pqxx::result result = work.exec("SELECT nickname, rating FROM users ORDER BY rating DESC LIMIT 100");

    boost::property_tree::ptree ratings;

    for (int i = 0; i < result.size(); ++i) {

        if (result[i][0].as<std::string>() == first_nickname || result[i][0].as<std::string>() == second_nickname) {

            std::cout << "Need to update top hundred ratings for " << first_nickname << " " << second_nickname << "\n";
            
            return true;

        }
    
    }

    return false;

}






