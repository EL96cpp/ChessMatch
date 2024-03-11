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

        sql_connection.prepare("insert", "INSERT INTO users VALUES($1, $2, 0, 0)");
        pqxx::work work{sql_connection};

        work.exec_prepared("insert", nickname, password);
        work.commit();

        return RegisterResult::SUCCESS; 
        

    } else {

        return RegisterResult::USER_ALREADY_EXISTS;

    }    


}



UpdateRatingResult SqlService::UpdatePlayerRating(const std::string& nickname, const size_t& new_rating) {

    if (CheckIfUserExists(nickname)) {

        sql_connection.prepare("insert", "UPDATE users SET rating = $1 WHERE nickname = $2");
        pqxx::work work{sql_connection};

        work.exec_prepared("insert", new_rating,  nickname);
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



std::map<std::string, int> SqlService::GetTopHundredPlayersRating() {

    pqxx::work work{sql_connection};

    pqxx::result result = work.exec("SELECT nickname, rating FROM users ORDER BY rating DESC LIMIT 100");

    std::map<std::string, int> ratings;

    for (int i = 0; i < result.size(); ++i) {

        ratings.emplace(result[i][0].as<std::string>(), result[i][1].as<int>());

    }

    for (auto& pair : ratings) {

        std::cout << "Ratings " << pair.first << " " << pair.second << "\n";

    }

    return ratings;
 
}

