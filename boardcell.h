#ifndef BOARDCELL_H
#define BOARDCELL_H

#include <QObject>
#include <QGraphicsRectItem>

class BoardCell : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    BoardCell(const int& y_coordinate, const int& x_coordinate, const int& width, const int& height);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    int GetX();
    int GetY();

signals:
    void BoardCellClicked(BoardCell* cell);

private:
    int y;
    int x;
};

#endif // BOARDCELL_H
