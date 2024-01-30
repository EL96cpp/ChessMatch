#include "SqlService.h"

SqlService::SqlService() : sql_connection("postgresql://accounting@localhost/chessmatch") {}

bool SqlService::CheckIfUserExists(const std::string& nickname) {

    sql_connection.prepare("nickname", "SELECT EXISTS(SELECT * FROM users WHERE nickname = $1)");
    pqxx::work work{sql_connection};
    pqxx::result result = work.exec_prepared("nickname", nickname);

    bool user_exists = result[0][0].as<bool>();
    return user_exists;
    
}

LoginResult SqlService::Login(const std::string& nickname, const std::string& password) {

    if (!CheckIfUserExists(nickname)) {

        sql_connection.prepare("nickname", "SELECT password FROM users WHERE nickname = $1");
        pqxx::work work{sql_connection};
        pqxx::result result = work.exec_prepared("nickname", nickname);

        std::string correct_password = result[0]["password"].c_str();
        
        if (password == correct_password) {

            return LoginResult::SUCCESS;

        } else {

            return LoginResult::INCORRECT_PASSWORD;

        }

    } else {

        return LoginResult::NO_NICKNAME_IN_DATABASE;

    }    

}
    

RegisterResult SqlService::Register(const std::string& nickname, const std::string& password) {

    if (!CheckIfUserExists(nickname)) {

        sql_connection.prepare("insert", "INSERT INTO users VALUES($1, $2)");
        pqxx::work work{sql_connection};

        work.exec_prepared("insert", nickname, password);
        work.commit();

        return RegisterResult::SUCCESS; 
        

    } else {

        return RegisterResult::USER_ALREADY_EXISTS;

    }    




}


