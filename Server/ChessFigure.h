#pragma once


enum class FigureColor {

    WHITE,
    BLACK,
    EMPTY

};

enum class FigureType {

    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    EMPTY

};


class ChessFigure {

public:
    ChessFigure(const FigureColor& color, const FigureType& type);

    FigureColor GetColor();
    FigureType GetType();

private:
    FigureColor color;
    FigureType type;

};
