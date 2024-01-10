#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QBrush>

#include "chessfigure.h"
#include "boardcell.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "king.h"
#include "queen.h"
#include "emptyfigure.h"

class Board : public QObject
{
    Q_OBJECT
public:
    Board(QGraphicsScene* board_scene, QObject *parent = nullptr);

signals:
    void PlayerFigureTaken(ChessFigure* fiugre);
    void OpponentFigureTaken(ChessFigure* figure);
    void SetMainWindowPlayerTurn(const QString& turn);
    void GameOver(const QString& winner_color);
    void ShowTransformPawnChoice(const QString& pawn_color);

public slots:
    void BoardCellClicked(BoardCell* cell);
    void FigureClicked(ChessFigure* figure);
    void StartNewGame();
    void SetPlayerColor(const FigureColor& player_color);
    void SetBrushes(const QColor& light_color, const QColor& dark_color, const QColor& outline_color, const QColor& select_color);
    void SetPawnTransformChoice(const FigureType& figure_type);

private:
    void MoveSelectedFigureToEmptyCell(const int& cell_y, const int& cell_x);
    void SelectedFigureTakesFigure(const int& taken_y, const int& taken_x);
    void ChangeCurrentPlayer();
    void MakeLeftCastling(const int& cell_y, const int& cell_x);
    void MakeRightCastling(const int& cell_y, const int& cell_x);

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
    bool waiting_for_pawn_transformation = false;

};

#endif // BOARD_H
