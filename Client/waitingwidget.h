#ifndef WAITINGWIDGET_H
#define WAITINGWIDGET_H

#include <QObject>
#include <QWidget>
#include <QRect>
#include <QColor>
#include <QPainter>
#include <QPropertyAnimation>


class WaitingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WaitingWidget(QWidget *parent = nullptr);

    void SetAnimationInterval(const int& animation_interval);
    void SetRectSize(const int& rect_size);
    void SetFirstColor(const QColor& first_color);
    void SetSecondColor(const QColor& second_color);

public slots:
    void updateRectCount();

signals:

private:

    QRect first_rect;
    QRect second_rect;
    QRect third_rect;
    QRect fourth_rect;

    int current_rect;
    int animation_interval;
    int rect_size;
    int margin_size;
    QColor first_color;
    QColor second_color;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // WAITINGWIDGET_H
