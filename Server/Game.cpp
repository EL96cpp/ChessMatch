#include "Game.h"
#include "ClientConnection.h"
#include "ChessFigure.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "GameResult.h"
#include "ThreadSafeQueue.h"
#include "ThreadSafeQueue.cpp"


Game::Game(ThreadSafeQueue<GameResult>& game_results_queue) : white_player(nullptr), black_player(nullptr), draw_offered_by(Color::EMPTY),
                                                              current_turn_color(Color::WHITE), game_results_queue(game_results_queue), number_of_moves(0) {

    //Constructor for testing purposes

    CreateStartField();
    
    game_result_type = GameResultType::IN_PROCESS;

    start_timepoint = std::chrono::system_clock::now();

}


Game::Game(std::shared_ptr<ClientConnection>& white_player, 
           std::shared_ptr<ClientConnection>& black_player,
           ThreadSafeQueue<GameResult>& game_results_queue) : white_player(white_player), black_player(black_player),
                                                              current_turn_color(Color::WHITE), draw_offered_by(Color::EMPTY),
                                                              game_results_queue(game_results_queue), number_of_moves(0) {

    white_player->SetIsWaiting(false);
    black_player->SetIsWaiting(false);

    white_player->SetPlayerColor(Color::WHITE);
    black_player->SetPlayerColor(Color::BLACK);

    CreateStartField();
    SendStartGameMessages();
    
    game_result_type = GameResultType::IN_PROCESS;

    start_timepoint = std::chrono::system_clock::now();

}

void Game::CreateStartField() {

    //Black figures

    std::vector<std::shared_ptr<ChessFigure>> black_figures;

    black_figures.push_back(std::make_shared<Rook>(Rook(Color::BLACK, 0, 0)));    
    black_figures.push_back(std::make_shared<Knight>(Knight(Color::BLACK, 0, 1))); 
    black_figures.push_back(std::make_shared<Bishop>(Bishop(Color::BLACK, 0, 2)));
    black_figures.push_back(std::make_shared<Queen>(Queen(Color::BLACK, 0, 3))); 
    black_figures.push_back(std::make_shared<King>(King(Color::BLACK, 0, 4))); 
    black_figures.push_back(std::make_shared<Bishop>(Bishop(Color::BLACK, 0, 5))); 
    black_figures.push_back(std::make_shared<Knight>(Knight(Color::BLACK, 0, 6))); 
    black_figures.push_back(std::make_shared<Rook>(Rook(Color::BLACK, 0, 7))); 

    board_cells.push_back(black_figures);
    

    //Black pawns

    std::vector<std::shared_ptr<ChessFigure>> black_pawns;

    for (int i = 0; i < 8; ++i) {

        black_pawns.push_back(std::make_shared<Pawn>(Pawn(Color::BLACK, 1, i)));

    }
    
    board_cells.push_back(black_pawns);


    //Empty fields

    for (int i = 2; i < 6; ++i) {
        
        std::vector<std::shared_ptr<ChessFigure>> empty_line;

        for (int j = 0; j < 8; ++j) {

            empty_line.push_back(std::make_shared<ChessFigure>(Color::EMPTY, FigureType::EMPTY, i, j));

        }

        board_cells.push_back(empty_line);

    }
    
    
    //White pawns

    std::vector<std::shared_ptr<ChessFigure>> white_pawns;

    for (int i = 0; i < 8; ++i) {

        white_pawns.push_back(std::make_shared<Pawn>(Pawn(Color::WHITE, 6, i)));

    }

    board_cells.push_back(white_pawns);

    
    //White figures

    std::vector<std::shared_ptr<ChessFigure>> white_figures;
    
    white_figures.push_back(std::make_shared<Rook>(Rook(Color::WHITE, 7, 0)));
    white_figures.push_back(std::make_shared<Knight>(Knight(Color::WHITE, 7, 1)));
    white_figures.push_back(std::make_shared<Bishop>(Bishop(Color::WHITE, 7, 2)));
    white_figures.push_back(std::make_shared<Queen>(Queen(Color::WHITE, 7, 3)));
    white_figures.push_back(std::make_shared<King>(King(Color::WHITE, 7, 4)));
    white_figures.push_back(std::make_shared<Bishop>(Bishop(Color::WHITE, 7, 5)));
    white_figures.push_back(std::make_shared<Knight>(Knight(Color::WHITE, 7, 6)));
    white_figures.push_back(std::make_shared<Rook>(Rook(Color::WHITE, 7, 7)));
 
    board_cells.push_back(white_figures);


}

void Game::SendStartGameMessages() {


    boost::property_tree::ptree property_tree_white;
    property_tree_white.put("Method", "POST");
    property_tree_white.put("Action", "Start_game");
    property_tree_white.put("Player_color", "White");
    property_tree_white.put("Opponent_nickname", black_player->GetNickname());

    std::ostringstream json_stream_white;
    boost::property_tree::write_json(json_stream_white, property_tree_white);        
    std::string json_string_white = json_stream_white.str();
    
    std::vector<uint8_t> message_body_white(json_string_white.begin(), json_string_white.end());

    std::shared_ptr<Message> new_message_white = std::make_shared<Message>();
    new_message_white->body = message_body_white;
    new_message_white->message_size = message_body_white.size();

    white_player->SendMessage(new_message_white);
    

    boost::property_tree::ptree property_tree_black;
    property_tree_black.put("Method", "POST");
    property_tree_black.put("Action", "Start_game");
    property_tree_black.put("Player_color", "Black");
    property_tree_black.put("Opponent_nickname", white_player->GetNickname());

    std::ostringstream json_stream_black;
    boost::property_tree::write_json(json_stream_black, property_tree_black);        
    std::string json_string_black = json_stream_black.str();
    
    std::vector<uint8_t> message_body_black(json_string_black.begin(), json_string_black.end());

    std::shared_ptr<Message> new_message_black = std::make_shared<Message>();
    new_message_black->body = message_body_black;
    new_message_black->message_size = message_body_black.size();

    black_player->SendMessage(new_message_black);

}

Color Game::DrawOfferedBy() {

    return draw_offered_by;

}


void Game::SetDrawOfferedBy(const Color& color) {

    draw_offered_by = color;

}

std::shared_ptr<ClientConnection> Game::GetWhitePlayer() {

    return white_player;

}

std::shared_ptr<ClientConnection> Game::GetBlackPlayer() {

    return black_player;

}


Color Game::GetCurrentTurnPlayerColor() {

    return current_turn_color;

}

void Game::ChangeCurrentTurnPlayerColorAndIncrementNumberOfMoves() {

    if (current_turn_color == Color::WHITE) {

        current_turn_color = Color::BLACK;

    } else {

        current_turn_color = Color::WHITE;

    }

    ++number_of_moves;

}

void Game::SendMessageToAll(std::shared_ptr<Message>& message) {

    white_player->SendMessage(message);
    black_player->SendMessage(message);

}

std::shared_ptr<ChessFigure> Game::CreateFigure(const Color& color, const FigureType& type, const size_t& y, const size_t& x) {

    if (type == FigureType::ROOK) {
    
        return std::make_shared<Rook>(color, y, x);
    
    } else if (type == FigureType::KNIGHT) {
    
        return std::make_shared<Knight>(color, y, x);
    
    } else if (type == FigureType::BISHOP) {
    
        return std::make_shared<Bishop>(color, y, x);
    
    } else if (type == FigureType::QUEEN) {
        
        return std::make_shared<Queen>(color, y, x);
    
    } else {
    
        return std::make_shared<ChessFigure>(color, type, y, x);
        
    }

}

FigureType Game::GetFigureTypeFromString(const std::string& figure_type) {
    
    if (figure_type == "Rook") {
    
        return FigureType::ROOK;
    
    } else if (figure_type == "Knight") {
    
        return FigureType::KNIGHT;
    
    } else if (figure_type == "Bishop") {
    
        return FigureType::BISHOP;
    
    } else if (figure_type == "Queen") {
    
        return FigureType::QUEEN;
    
    } else {
    
        return FigureType::EMPTY;

    }

}

void Game::SwapFigures(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to) {
    
    std::swap(board_cells[y_from][x_from], board_cells[y_to][x_to]);
    
    board_cells[y_from][x_from]->SetCoordinates(y_from, x_from);
    board_cells[y_to][x_to]->SetCoordinates(y_to, x_to);

    board_cells[y_from][x_from]->SetMadeFirstStep(true);
    board_cells[y_to][x_to]->SetMadeFirstStep(true);

}

bool Game::MakeMove(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, const Color& player_color) {

    if (CheckIfMoveIsCorrect(y_from, x_from, y_to, x_to, player_color)) {
        
        SwapFigures(y_from, x_from, y_to, x_to);
    
        ChangeCurrentTurnPlayerColorAndIncrementNumberOfMoves();

        return true;
        
    } else {
        
        return false;

    }

}
    
bool Game::EatFigure(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, 
                     const Color& player_color, const std::string& transformation_type) {

    if (CheckIfEatFigureIsCorrect(y_from, x_from, y_to, x_to, player_color)) {

        if (board_cells[y_from][x_from]->GetType() == FigureType::PAWN && ((player_color == Color::WHITE && y_to == 0) || (player_color == Color::BLACK && y_to == 7))) {
        
            FigureType figure_type = GetFigureTypeFromString(transformation_type);

            if (figure_type != FigureType::EMPTY) {

                if (player_color == Color::WHITE) {

                    taken_by_white_player.push_back(board_cells[y_to][x_to]);

                } else {

                    taken_by_black_player.push_back(board_cells[y_to][x_to]);

                }

                if (board_cells[y_to][x_to]->GetType() != FigureType::KING) {

                    board_cells[y_from][x_from] = std::make_shared<ChessFigure>(Color::EMPTY, FigureType::EMPTY, y_from, x_from);
                    board_cells[y_to][x_to] = CreateFigure(player_color, figure_type, y_to, x_to);
                    board_cells[y_to][x_to]->SetMadeFirstStep(true);

                    ChangeCurrentTurnPlayerColorAndIncrementNumberOfMoves();

                    return true;
                
                } else {

                    
                    end_timepoint = std::chrono::system_clock::now();
                    std::string winner;
                    size_t new_white_player_rating;
                    size_t new_black_player_rating;

                    //Game over
                    if (board_cells[y_from][x_from]->GetColor() == Color::WHITE) {

                        game_result_type = GameResultType::WHITE_WINS;
                        winner = white_player->GetNickname();
                        new_white_player_rating = CalculateNewEloRating(white_player, black_player, PlayersGameResult::WINNER);
                        new_black_player_rating = CalculateNewEloRating(black_player, white_player, PlayersGameResult::LOOSER);

                    } else if (board_cells[y_from][x_from]->GetColor() == Color::BLACK) {

                        game_result_type = GameResultType::BLACK_WINS;
                        winner = black_player->GetNickname();
                        new_black_player_rating = CalculateNewEloRating(black_player, white_player, PlayersGameResult::WINNER);
                        new_white_player_rating = CalculateNewEloRating(white_player, black_player, PlayersGameResult::LOOSER);

                    }

                    white_player->SetNewRatingAndIncrementGamesPlayed(new_white_player_rating);
                    black_player->SetNewRatingAndIncrementGamesPlayed(new_black_player_rating);

                    size_t total_time = std::chrono::duration_cast<std::chrono::seconds>(end_timepoint - start_timepoint).count();
                    std::shared_ptr<GameResult> result = std::make_shared<GameResult>(white_player->GetNickname(), black_player->GetNickname(), winner, number_of_moves, 
                                                                                      total_time, new_white_player_rating, white_player->GetGamesPlayed(), 
                                                                                      new_black_player_rating, black_player->GetGamesPlayed());

                    game_results_queue.push_back(result);


                    return true;

                }
               

            } else {

                return false;

            }
        
        } else {
        

            if (player_color == Color::WHITE) {

                taken_by_white_player.push_back(board_cells[y_to][x_to]);

            } else {

                taken_by_black_player.push_back(board_cells[y_to][x_to]);

            }   

            if (board_cells[y_to][x_to]->GetType() != FigureType::KING) {

                
                SwapFigures(y_from, x_from, y_to, x_to);
                board_cells[y_from][x_from] = std::make_shared<ChessFigure>(Color::EMPTY, FigureType::EMPTY, y_from, x_from);
           
                ChangeCurrentTurnPlayerColorAndIncrementNumberOfMoves();

                return true;
                

            } else {

                
                end_timepoint = std::chrono::system_clock::now();
                std::string winner;
                size_t new_white_player_rating;
                size_t new_black_player_rating;

                //Game over
                if (board_cells[y_from][x_from]->GetColor() == Color::WHITE) {

                    game_result_type = GameResultType::WHITE_WINS;
                    winner = white_player->GetNickname();
                    new_white_player_rating = CalculateNewEloRating(white_player, black_player, PlayersGameResult::WINNER);
                    new_black_player_rating = CalculateNewEloRating(black_player, white_player, PlayersGameResult::LOOSER);

                } else if (board_cells[y_from][x_from]->GetColor() == Color::BLACK) {

                    game_result_type = GameResultType::BLACK_WINS;
                    winner = black_player->GetNickname();
                    new_black_player_rating = CalculateNewEloRating(black_player, white_player, PlayersGameResult::WINNER);
                    new_white_player_rating = CalculateNewEloRating(white_player, black_player, PlayersGameResult::LOOSER);

                }

                white_player->SetNewRatingAndIncrementGamesPlayed(new_white_player_rating);
                black_player->SetNewRatingAndIncrementGamesPlayed(new_black_player_rating);


                size_t total_time = std::chrono::duration_cast<std::chrono::seconds>(end_timepoint - start_timepoint).count();
                std::shared_ptr<GameResult> result = std::make_shared<GameResult>(white_player->GetNickname(), black_player->GetNickname(), winner, number_of_moves, 
                                                                                  total_time, new_white_player_rating, white_player->GetGamesPlayed(), 
                                                                                  new_black_player_rating, black_player->GetGamesPlayed());

                game_results_queue.push_back(result);


                return true;

            }

        }

    } else {
        
        return false;

    }


}
    
bool Game::MakeCastling(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, const Color& player_color) {
    
    if (CheckIfCastlingIsCorrect(y_from, x_from, y_to, x_to, player_color)) {
        
        SwapFigures(y_from, x_from, y_to, x_to);
        
        if (x_from > x_to) {
        
            board_cells[y_from][0]->SetMadeFirstStep(true);
            SwapFigures(y_from, 0, y_to, x_to+1);
        
        } else {
        
            board_cells[y_from][7]->SetMadeFirstStep(true);
            SwapFigures(y_from, 7, y_to, x_to-1);
        
        }
        
        board_cells[y_from][x_from]->SetMadeFirstStep(true);
        board_cells[y_to][x_to]->SetMadeFirstStep(true);
    
        ChangeCurrentTurnPlayerColorAndIncrementNumberOfMoves();

        return true;
    
    } else {
        
        return false;
    
    }
    
}
    
bool Game::TransformPawn(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, 
                         const Color& player_color, const std::string& figure_type) {

    if (CheckIfPawnTransformationIsCorrect(y_from, x_from, y_to, x_to, player_color, figure_type)) {
    
        SwapFigures(y_from, x_from, y_to, x_to);
        FigureType transformation_type = GetFigureTypeFromString(figure_type);
        board_cells[y_to][x_to] = CreateFigure(player_color, transformation_type, y_to, x_to);
        board_cells[y_to][x_to]->SetMadeFirstStep(true);

        ChangeCurrentTurnPlayerColorAndIncrementNumberOfMoves();

        return true;
    
    } else {
        
        return false;

    }

}

bool Game::AcceptDraw(std::shared_ptr<ClientConnection>& player) {

    if (draw_offered_by != Color::EMPTY && draw_offered_by != player->GetPlayerColor()) {

        size_t new_white_player_rating = CalculateNewEloRating(white_player, black_player, PlayersGameResult::DRAW);
        size_t new_black_player_rating = CalculateNewEloRating(black_player, white_player, PlayersGameResult::DRAW);

        white_player->SetNewRatingAndIncrementGamesPlayed(new_white_player_rating);
        black_player->SetNewRatingAndIncrementGamesPlayed(new_black_player_rating);


        end_timepoint = std::chrono::system_clock::now();
        size_t total_time = std::chrono::duration_cast<std::chrono::seconds>(end_timepoint - start_timepoint).count();
        std::shared_ptr<GameResult> result = std::make_shared<GameResult>(white_player->GetNickname(), black_player->GetNickname(), "Draw", number_of_moves, 
                                                                          total_time, new_white_player_rating, white_player->GetGamesPlayed(), 
                                                                          new_black_player_rating, black_player->GetGamesPlayed());

        game_results_queue.push_back(result);

        return true;

    } else {

        return false;

    }

}

void Game::Resign(std::shared_ptr<ClientConnection>& player) {

    std::string winner;
    size_t new_white_player_rating;
    size_t new_black_player_rating;


    if (player->GetNickname() == white_player->GetNickname()) {

        winner = black_player->GetNickname();
        new_white_player_rating = CalculateNewEloRating(white_player, black_player, PlayersGameResult::LOOSER);
        new_black_player_rating = CalculateNewEloRating(black_player, white_player, PlayersGameResult::WINNER);

    } else if (player->GetNickname() == black_player->GetNickname()) {

        winner = white_player->GetNickname();
        new_white_player_rating = CalculateNewEloRating(white_player, black_player, PlayersGameResult::WINNER);
        new_black_player_rating = CalculateNewEloRating(black_player, white_player, PlayersGameResult::LOOSER);

    }


    white_player->SetNewRatingAndIncrementGamesPlayed(new_white_player_rating);
    black_player->SetNewRatingAndIncrementGamesPlayed(new_black_player_rating);


    end_timepoint = std::chrono::system_clock::now();
    size_t total_time = std::chrono::duration_cast<std::chrono::seconds>(end_timepoint - start_timepoint).count();
    std::shared_ptr<GameResult> result = std::make_shared<GameResult>(white_player->GetNickname(), black_player->GetNickname(), winner, number_of_moves, 
                                                                      total_time, new_white_player_rating, white_player->GetGamesPlayed(), 
                                                                      new_black_player_rating, black_player->GetGamesPlayed());

    game_results_queue.push_back(result);

}

bool Game::CheckIfPlayerIsAGameMember(std::shared_ptr<ClientConnection>& player) {

    return (white_player == player || black_player == player);

}

bool Game::CheckIfMoveIsCorrect(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, const Color& player_color) {


    if (y_from > 7 || y_from < 0 || y_to > 7 || y_to < 0 ||
        x_from > 7 || x_from < 0 || x_to > 7 || x_to < 0) {

        return false;

    }



    if (board_cells[y_from][x_from]->GetColor() == player_color && player_color == current_turn_color) {

        std::vector<std::pair<size_t, size_t>> possible_moves = board_cells[y_from][x_from]->CalculatePossibleMoves(board_cells);

        if (std::find(possible_moves.begin(), possible_moves.end(), std::pair<size_t, size_t>(y_to, x_to)) != possible_moves.end() && 
            board_cells[y_to][x_to]->GetColor() == Color::EMPTY) {
            
            return true;    

        } else {

            return false;

        }


    } else {

        return false;

    } 

}

bool Game::CheckIfEatFigureIsCorrect(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, const Color& player_color) {
    
    if (y_from > 7 || y_from < 0 || y_to > 7 || y_to < 0 ||
        x_from > 7 || x_from < 0 || x_to > 7 || x_to < 0) {
    
        return false;
    
    }
    
    if (board_cells[y_from][x_from]->GetColor() == player_color && player_color == current_turn_color) {
    
        std::vector<std::pair<size_t, size_t>> possible_moves = board_cells[y_from][x_from]->CalculatePossibleMoves(board_cells);
    
        if (std::find(possible_moves.begin(), possible_moves.end(), std::pair<size_t, size_t>(y_to, x_to)) != possible_moves.end() && 
            board_cells[y_to][x_to]->GetColor() != Color::EMPTY && board_cells[y_to][x_to]->GetColor() != player_color) {
        
            return true;    
    
        } else {

            return false;
    
        }
    
    } else {

        return false;
    
    } 
     
}

bool Game::CheckIfCastlingIsCorrect(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, const Color& player_color) {

    if (y_from > 7 || y_from < 0 || y_to > 7 || y_to < 0 ||
        x_from > 7 || x_from < 0 || x_to > 7 || x_to < 0) {

        return false;

    }

    if (x_from > x_to) {
        
        if (!board_cells[y_from][x_from]->MadeFirstStep() && !board_cells[y_from][0]->MadeFirstStep() &&
            board_cells[y_from][x_from]->GetType() == FigureType::KING && board_cells[y_from][0]->GetType() == FigureType::ROOK &&
            board_cells[y_from][x_from]->GetColor() == player_color && board_cells[y_from][0]->GetColor() == player_color && player_color == current_turn_color) {
           
            for (int i = 1; i < x_from; ++i) {
            
                if (board_cells[y_from][i]->GetColor() != Color::EMPTY) {

                    return false;

                }

            }

            return true;
            
        } else {

            return false;

        }
            

    } else {

        if (!board_cells[y_from][x_from]->MadeFirstStep() && !board_cells[y_from][7]->MadeFirstStep() &&
            board_cells[y_from][x_from]->GetType() == FigureType::KING && board_cells[y_from][7]->GetType() == FigureType::ROOK &&
            board_cells[y_from][x_from]->GetColor() == player_color && board_cells[y_from][7]->GetColor() == player_color && player_color == current_turn_color) {
            
            for (int i = x_from+1; i < 7; ++i) {
            
                if (board_cells[y_from][i]->GetColor() != Color::EMPTY) {

                    return false;

                }

            }

            return true;
            
        } else {

            return false;

        }
 
    }

}

bool Game::CheckIfPawnTransformationIsCorrect(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, 
                                              const Color& player_color, const std::string& figure_type) {
        
    if (y_from > 7 || y_from < 0 || y_to > 7 || y_to < 0 ||
        x_from > 7 || x_from < 0 || x_to > 7 || x_to < 0) {

        return false;

    }

    if (GetFigureTypeFromString(figure_type) == FigureType::EMPTY) {

        return true;

    }
    

    if (board_cells[y_from][x_from]->GetColor() == player_color && player_color == current_turn_color) {

        if (board_cells[y_from][x_from]->GetColor() == Color::WHITE && y_to == 0 ||
            board_cells[y_from][x_from]->GetColor() == Color::BLACK && y_to == 7) {

            std::vector<std::pair<size_t, size_t>> possible_moves = board_cells[y_from][x_from]->CalculatePossibleMoves(board_cells);

            if (std::find(possible_moves.begin(), possible_moves.end(), std::pair<size_t, size_t>(y_to, x_to)) != possible_moves.end()) {

                return true;

            } else {

                return false;

            }

        } else {

            return false;

        }

    } else {

        return false;

    } 
   
}

size_t Game::CalculateNewEloRating(std::shared_ptr<ClientConnection>& player, std::shared_ptr<ClientConnection>& opponent, const PlayersGameResult& game_result) {

    size_t player_rating = player->GetRating();
    size_t player_games_played = player->GetGamesPlayed();
    size_t opponent_rating = opponent->GetRating();

    double e = 1.0 / (1.0 + std::pow(10.0, std::fabs(opponent_rating - player_rating)/400.0));    
    
    double s;

    if (game_result == PlayersGameResult::WINNER) {

        s = 1.0;

    } else if (game_result == PlayersGameResult::DRAW) {

        s = 0.5;

    } else if (game_result == PlayersGameResult::LOOSER) {

        s = 0.0;

    }

    double k;

    if (player_rating >= 2400) {

        k = 10.0;        

    } else if (player_rating < 2400 && player_games_played >= 30) {

        k = 20.0;

    } else {

        k = 40.0;

    }

    double new_player_rating = player_rating + k * (s - e);

    new_player_rating = (new_player_rating < 0) ? 0 : new_player_rating;
    
    return std::floor(new_player_rating);

}

GameResultType Game::GetGameResultType() {

    return game_result_type;

}


