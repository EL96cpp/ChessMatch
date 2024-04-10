#include "takenfiguresmanager.h"

TakenFiguresManager::TakenFiguresManager(QGraphicsScene* opponent_taken_scene, QGraphicsScene* player_taken_scene, QObject *parent)
    : opponent_taken_scene(opponent_taken_scene), player_taken_scene(player_taken_scene), QObject{parent}
{

}

void TakenFiguresManager::StartNewGame() {

    x_opponent_taken = -130;
    y_opponent_taken = -100;
    x_player_taken = -130;
    y_player_taken = -100;

    player_taken_scene->clear();
    opponent_taken_scene->clear();
    player_taken_figures.clear();
    opponent_taken_figures.clear();

}

void TakenFiguresManager::AddPlayerTakenFigure(const FigureType& figure_type, const FigureColor& figure_color) {

    ChessFigure* figure = CreateTakenFigure(figure_type, figure_color);

    figure->setScale(0.45);
    figure->setY(y_player_taken);
    figure->setX(x_player_taken);
    player_taken_scene->addItem(figure);
    player_taken_figures.push_back(figure);

    if (player_taken_figures.size() == 8) {

        y_player_taken += 60;
        x_player_taken = -130;

    } else {

        x_player_taken += 30;

    }

}

void TakenFiguresManager::AddOpponentTakenFigure(const FigureType& figure_type, const FigureColor& figure_color) {

    ChessFigure* figure = CreateTakenFigure(figure_type, figure_color);

    figure->setScale(0.45);
    figure->setY(y_opponent_taken);
    figure->setX(x_opponent_taken);
    opponent_taken_scene->addItem(figure);
    opponent_taken_figures.push_back(figure);

    if (opponent_taken_figures.size() == 8) {

        y_opponent_taken += 60;
        x_opponent_taken = -130;

    } else {

        x_opponent_taken += 30;

    }

}

ChessFigure *TakenFiguresManager::CreateTakenFigure(const FigureType &figure_type, const FigureColor &figure_color) {

    if (figure_type == FigureType::PAWN) {

        return new Pawn(0, 0, figure_color, FigureOwner::OPPONENT, this);

    } else if (figure_type == FigureType::ROOK) {

        return new Rook(0, 0, figure_color, FigureOwner::OPPONENT, this);

    } else if (figure_type == FigureType::KNIGHT) {

        return new Knight(0, 0, figure_color, FigureOwner::OPPONENT, this);

    } else if (figure_type == FigureType::BISHOP) {

        return new Bishop(0, 0, figure_color, FigureOwner::OPPONENT, this);

    } else if (figure_type == FigureType::QUEEN) {

        return new Queen(0, 0, figure_color, FigureOwner::OPPONENT, this);

    }

}
