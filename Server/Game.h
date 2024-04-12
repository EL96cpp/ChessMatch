#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <memory>
#include <vector>
#include <map>
#include <cmath>


class ChessFigure;
class ClientConnection;
class Message;
class GameResult;
enum class Color;
enum class FigureType;

enum class PlayersGameResult {

    WINNER,
    LOOSER,
    DRAW

};

enum class GameResultType {

    IN_PROCESS,
    WHITE_WINS,
    BLACK_WINS,
    DRAW

};


class Game {

public:
    Game(ThreadSafeQueue<GameResult>& game_results_queue);
    Game(std::shared_ptr<ClientConnection>& white_player, std::shared_ptr<ClientConnection>& black_player, ThreadSafeQueue<GameResult>& game_results_queue);

    bool MakeMove(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, const Color& player_color);
    bool EatFigure(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, 
                   const Color& player_color, const std::string& transformation_type = "");
    bool MakeCastling(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, const Color& player_color);
    bool TransformPawn(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, 
                       const Color& player_color, const std::string& figure_type);
    bool AcceptDraw(std::shared_ptr<ClientConnection>& player);
    void Resign(std::shared_ptr<ClientConnection>& player);

    Color DrawOfferedBy();
    void SetDrawOfferedBy(const Color& color);
    bool CheckIfPlayerIsAGameMember(std::shared_ptr<ClientConnection>& player);
    Color GetCurrentTurnPlayerColor();
    void ChangeCurrentTurnPlayerColorAndIncrementNumberOfMoves();
    void SendMessageToAll(std::shared_ptr<Message>& message);
    FigureType GetFigureTypeFromString(const std::string& figure_type);
    std::shared_ptr<ChessFigure> CreateFigure(const Color& color, const FigureType& type, const size_t& y, const size_t& x);
    GameResultType GetGameResultType();


private:
    void SwapFigures(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to);
    void CreateStartField();
    void SendStartGameMessages();
    void MakeMove(const char& letter_from, const size_t& index_from, const char& letter_to, const size_t& index_to);

    bool CheckIfMoveIsCorrect(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, const Color& player_color);
    bool CheckIfEatFigureIsCorrect(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, const Color& player_color);
    bool CheckIfCastlingIsCorrect(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, const Color& player_color);
    bool CheckIfPawnTransformationIsCorrect(const size_t& y_from, const size_t& x_from, const size_t& y_to, const size_t& x_to, 
                                            const Color& player_color, const std::string& figure_type);
    void OnGameOver();
    size_t CalculateNewEloRating(std::shared_ptr<ClientConnection>& player, std::shared_ptr<ClientConnection>& opponent, const PlayersGameResult& game_result);

private:
    std::shared_ptr<ClientConnection> white_player;
    std::shared_ptr<ClientConnection> black_player;
       
    Color draw_offered_by;

    Color current_turn_color;

    GameResultType game_result_type;

    ThreadSafeQueue<GameResult>& game_results_queue;

    std::chrono::time_point<std::chrono::system_clock> start_timepoint;
    std::chrono::time_point<std::chrono::system_clock> end_timepoint;
    size_t number_of_moves;

    std::vector<std::vector<std::shared_ptr<ChessFigure>>> board_cells;
    std::vector<std::shared_ptr<ChessFigure>> taken_by_white_player;
    std::vector<std::shared_ptr<ChessFigure>> taken_by_black_player;

};



