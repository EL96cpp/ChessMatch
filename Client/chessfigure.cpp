#include "chessfigure.h"

ChessFigure::ChessFigure(const int &y_index,
                         const int &x_index,
                         QObject *parent,
                         const bool& is_transform_pawn_choice_figure) : x_board_index(x_index),
                                                                        y_board_index(y_index),
                                                                        QObject(parent)
{

    this->is_transform_pawn_choice_figure = is_transform_pawn_choice_figure;
    setPos(50 + x_board_index*100, 50 + y_board_index*100);

}

ChessFigure::ChessFigure(ChessFigure &figure) {

    x_board_index = figure.x_board_index;
    y_board_index = figure.y_board_index;
    figure_type = figure.figure_type;
    figure_color = figure.figure_color;
    made_first_step = figure.made_first_step;

}

int ChessFigure::GetX()
{
    return x_board_index;
}

int ChessFigure::GetY()
{
    return y_board_index;
}

void ChessFigure::SetX(const int &x)
{
    x_board_index = x;
}

void ChessFigure::SetY(const int &y)
{
    y_board_index = y;
}

void ChessFigure::SetMadeFirstStep(const bool &value)
{
    made_first_step = value;
}

bool ChessFigure::MadeFirstStep()
{
    return made_first_step;
}

FigureType ChessFigure::GetType()
{
    return figure_type;
}

FigureColor ChessFigure::GetColor()
{
    return figure_color;
}

FigureOwner ChessFigure::GetOwner()
{
    return figure_owner;
}


void ChessFigure::Move(const int &y_coordinate, const int &x_coordinate)
{
    made_first_step = true;
    moveBy((x_coordinate - x_board_index)*100, (y_coordinate - y_board_index)*100);
    update();
    y_board_index = y_coordinate;
    x_board_index = x_coordinate;
}

void ChessFigure::SwapCoordinatesAndMovePixmaps(ChessFigure *figure)
{
    QPointF figure_pos = figure->pos();
    figure->setPos(pos());
    setPos(figure_pos);


    int y = figure->GetY(), x = figure->GetX();
    figure->SetY(GetY());
    figure->SetX(GetX());
    SetY(y);
    SetX(x);
    made_first_step = true;
}

void ChessFigure::Debug()
{
    if (figure_color == FigureColor::WHITE) {

        qDebug() << "white figure " << y_board_index << " " << x_board_index;

    } else if (figure_color == FigureColor::BLACK) {

        qDebug() << "black figure " << y_board_index << " " << x_board_index;

    } else if (figure_color == FigureColor::EMPTY) {

        qDebug() << "empty figure " << y_board_index << " " << x_board_index;

    }


    switch(figure_type) {
    case FigureType::PAWN:
        qDebug() << "pawn figure " << y_board_index << " " << x_board_index;
        break;

    case FigureType::ROOK:
        qDebug() << "rook figure " << y_board_index << " " << x_board_index;
        break;

    case FigureType::BISHOP:
        qDebug() << "bishop figure " << y_board_index << " " << x_board_index;
        break;

    case FigureType::KNIGHT:
        qDebug() << "knight figure " << y_board_index << " " << x_board_index;
        break;

    case FigureType::QUEEN:
        qDebug() << "queen figure " << y_board_index << " " << x_board_index;
        break;

    case FigureType::KING:
        qDebug() << "king figure " << y_board_index << " " << x_board_index;
        break;

    case FigureType::EMPTY:
        qDebug() << "empty figure " << y_board_index << " " << x_board_index;
        break;
    }

}


void ChessFigure::ChessFigure::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    if (is_transform_pawn_choice_figure) {

        emit TransformFigureClicked(this);

    } else {

        emit FigureClicked(this);

    }

}
