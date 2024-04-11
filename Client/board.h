#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QMap>

#include "chessfigure.h"
#include "boardcell.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "king.h"
#include "queen.h"
#include "emptyfigure.h"


enum class PawnTransformation {

    NONE,
    TRANSFORM,
    EAT_AND_TRANSFORM

};

class Board : public QObject
{
    Q_OBJECT
public:
    Board(QGraphicsScene* board_scene, QObject *parent = nullptr);
    void SetGameOver(const bool& game_over);

signals:
    void MakeMove(const QString& letter_from, const QString& index_from, const QString& letter_to, const QString& index_to);
    void EatFigure(const QString& letter_from, const QString& index_from, const QString& letter_to,
                   const QString& index_to, const QString& transformation_type = "");
    void MakeCastling(const QString& letter_from, const QString& index_from, const QString& letter_to, const QString& index_to);
    void TransformPawn(const QString& letter_from, const QString& index_from, const QString& letter_to,
                       const QString& index_to, const QString& figure_type);

    void PlayerFigureTaken(const FigureType& figure_type, const FigureColor& figure_color);
    void OpponentFigureTaken(const FigureType& figure_type, const FigureColor& figure_color);
    void SetMainWindowPlayerTurn(const QString& turn);
    void GameOver(const QString& winner_color);
    void ShowTransformPawnChoice(const FigureColor& pawn_color);
    void HideTransformPawnChoice();

public slots:
    void OnMakeMoveAccepted(const QString& letter_from, const QString& index_from, const QString& letter_to, const QString& index_to);
    void OnEatFigureAccepted(const QString& letter_from, const QString& index_from,
                             const QString& letter_to, const QString& index_to,
                             const QString& transformation_figure_type);
    void OnMakeCastlingAccepted(const QString& letter_from, const QString& index_from, const QString& letter_to, const QString& index_to);
    void OnTransformPawnAccepted(const QString& letter_from, const QString& index_from, const QString& letter_to,
                                 const QString& index_to, const FigureType& figure_type);

    void BoardCellClicked(BoardCell* cell);
    void FigureClicked(ChessFigure* figure);
    void TransformFigureClicked(ChessFigure* figure);
    void StartNewGame();
    void SetPlayerColor(const FigureColor& player_color);
    void SetBrushes(const QColor& light_color, const QColor& dark_color, const QColor& outline_color, const QColor& select_color);
    void SetPawnTransformChoice(const FigureType& figure_type);

private:
    void SetBoardNavigationMaps();
    void MoveFigureToEmptyCell(const int& y_from, const int& x_from, const int& y_to, const int& x_to);
    void MakeFiguresCastling(const int& y_from, const int& x_from, const int& y_to, const int& x_to);
    void SelectedFigureTakesFigure(const int& taken_y, const int& taken_x);
    void ChangeCurrentPlayer();
    QString GetStringValueOfFigureType(const FigureType& figure_type);
    FigureType GetFigureTypeFromString(const QString& figure_type);
    QString GetStringValueOfColor(const FigureColor& figure_color);
    ChessFigure *CreateFigure(const size_t& y, const size_t& x, const FigureType& figure_type, const FigureColor& figure_color);

    void SetBoardOutlineCells();
    void SetBoardOutlineText();
    void SetBoardCells();
    void PaintBoardCells();
    void SetFigures();
    void SetOpponentFigures();
    void SetEmptyFigures();
    void SetPlayerFigures();
    void PaintSelectedFigureMoves();
    void UnpaintSelectedFigureMoves();

private:
    FigureColor player_color;
    FigureColor current_player;
    ChessFigure* selected_figure;
    ChessFigure* transformation_pawn;
    QVector<std::pair<int, int>> selected_figure_moves;
    QMap<int, QString> white_board_navigation_map_x;
    QMap<int, QString> white_board_navigation_map_y;
    QMap<int, QString> black_board_navigation_map_x;
    QMap<int, QString> black_board_navigation_map_y;
    QBrush light_brush;
    QBrush dark_brush;
    QBrush outline_brush;
    QBrush select_brush;
    QGraphicsScene* board_scene;
    QVector<QVector<ChessFigure*>> figures;
    QVector<QVector<BoardCell*>> board_cells;
    QVector<QVector<QGraphicsRectItem*>> board_outline_cells;
    QVector<QVector<QGraphicsTextItem*>> board_outline_text;
    bool game_over = false;

    PawnTransformation transformation_type;
    size_t pawn_transformation_y;   //Stores coordinates of the cell, where pawn moves when
    size_t pawn_transformation_x;   //eats figure and transforms simultaneously

};

#endif // BOARD_H
