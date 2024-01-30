#include "boardcell.h"

BoardCell::BoardCell(const int& y_coordinate, const int& x_coordinate, const int& width, const int& height) :
    QGraphicsRectItem(50 + x_coordinate * 100, 50 + y_coordinate * 100, width, height), y(y_coordinate), x(x_coordinate) {}

void BoardCell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit BoardCellClicked(this);
}

int BoardCell::GetX()
{
    return x;
}

int BoardCell::GetY()
{
    return y;
}
