#include "waitingwidget.h"

WaitingWidget::WaitingWidget(QWidget *parent)
    : QWidget{parent}, rect_size(170), margin_size(30),
      first_color(129, 144, 135), second_color(238,238,210), current_rect(1)
{

    first_rect = QRect(0, 0, rect_size, rect_size);
    second_rect = QRect(rect_size+margin_size, 0, rect_size, rect_size);
    third_rect = QRect(0, rect_size+margin_size, rect_size, rect_size);
    fourth_rect = QRect(rect_size+margin_size, rect_size+margin_size, rect_size, rect_size);

}

void WaitingWidget::updateRectCount() {
    
    qDebug() << "update";
    if (current_rect < 4) {

        ++current_rect;

    } else {

        current_rect = 1;

    }

    update();
    
}

void WaitingWidget::SetAnimationInterval(const int &animation_interval)
{
    this->animation_interval = animation_interval;
}

void WaitingWidget::SetRectSize(const int &rect_size)
{
    this->rect_size = rect_size;
}

void WaitingWidget::SetFirstColor(const QColor &first_color)
{
    this->first_color = first_color;
}

void WaitingWidget::SetSecondColor(const QColor &second_color)
{
    this->second_color = second_color;
}

void WaitingWidget::paintEvent(QPaintEvent *event)
{
    qDebug() << "current rect " << current_rect;

    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    if (current_rect == 1) {


        painter.setRenderHint(QPainter::Antialiasing);
        painter.save();

        painter.setBrush(first_color);
        painter.drawRect(first_rect);

        QColor first_color_transparent(first_color);
        first_color_transparent.setAlpha(10);
        painter.setBrush(first_color_transparent);
        painter.drawRect(fourth_rect);

        QColor second_color_transparent(second_color);
        second_color_transparent.setAlpha(10);
        painter.setBrush(second_color_transparent);
        painter.drawRect(second_rect);
        painter.drawRect(third_rect);


    } else if (current_rect == 2) {


        painter.setRenderHint(QPainter::Antialiasing);
        painter.save();

        QColor first_color_transparent(first_color);
        first_color_transparent.setAlpha(10);
        painter.setBrush(first_color_transparent);

        painter.setBrush(first_color_transparent);
        painter.drawRect(first_rect);
        painter.drawRect(fourth_rect);


        painter.setBrush(second_color);
        painter.drawRect(second_rect);

        QColor second_color_transparent(second_color);
        second_color_transparent.setAlpha(10);
        painter.setBrush(second_color_transparent);
        painter.drawRect(third_rect);


    } else if (current_rect == 3) {

        painter.setRenderHint(QPainter::Antialiasing);
        painter.save();

        painter.setBrush(first_color);
        painter.drawRect(fourth_rect);

        QColor first_color_transparent(first_color);
        first_color_transparent.setAlpha(10);
        painter.setBrush(first_color_transparent);
        painter.drawRect(first_rect);

        QColor second_color_transparent(second_color);
        second_color_transparent.setAlpha(10);
        painter.setBrush(second_color_transparent);
        painter.drawRect(second_rect);
        painter.drawRect(third_rect);


    } else if (current_rect == 4) {


        painter.setRenderHint(QPainter::Antialiasing);
        painter.save();

        painter.setBrush(second_color);
        painter.drawRect(third_rect);

        QColor first_color_transparent(first_color);
        first_color_transparent.setAlpha(10);
        painter.setBrush(first_color_transparent);
        painter.drawRect(first_rect);
        painter.drawRect(fourth_rect);

        QColor second_color_transparent(second_color);
        second_color_transparent.setAlpha(10);
        painter.setBrush(second_color_transparent);
        painter.drawRect(second_rect);


    }

}
