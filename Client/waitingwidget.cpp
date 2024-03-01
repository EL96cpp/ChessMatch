#include "waitingwidget.h"

WaitingWidget::WaitingWidget(QWidget *parent)
    : QWidget{parent}, rect_size(170), margin_size(30), background_color(173, 188, 159),
      first_color(67, 104, 80), second_color(251, 250, 218), current_rect(1), alpha_value(90)
{

    background_rect = QRect(0, 0, rect_size*2 + margin_size*3, rect_size*2 + margin_size*3);
    first_rect = QRect(margin_size, margin_size, rect_size, rect_size);
    second_rect = QRect(rect_size+margin_size*2, margin_size, rect_size, rect_size);
    third_rect = QRect(margin_size, rect_size+margin_size*2, rect_size, rect_size);
    fourth_rect = QRect(rect_size+margin_size*2, rect_size+margin_size*2, rect_size, rect_size);

}

void WaitingWidget::updateRectCount() {
    
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

    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(background_color);
    painter.drawRect(background_rect);


    if (current_rect == 1) {


        painter.setBrush(first_color);
        painter.drawRect(first_rect);

        QColor first_color_transparent(first_color);
        first_color_transparent.setAlpha(alpha_value);
        painter.setBrush(first_color_transparent);
        painter.drawRect(fourth_rect);

        QColor second_color_transparent(second_color);
        second_color_transparent.setAlpha(alpha_value);
        painter.setBrush(second_color_transparent);
        painter.drawRect(second_rect);
        painter.drawRect(third_rect);


    } else if (current_rect == 2) {


        QColor first_color_transparent(first_color);
        first_color_transparent.setAlpha(alpha_value);
        painter.setBrush(first_color_transparent);

        painter.setBrush(first_color_transparent);
        painter.drawRect(first_rect);
        painter.drawRect(fourth_rect);


        painter.setBrush(second_color);
        painter.drawRect(second_rect);

        QColor second_color_transparent(second_color);
        second_color_transparent.setAlpha(alpha_value);
        painter.setBrush(second_color_transparent);
        painter.drawRect(third_rect);


    } else if (current_rect == 3) {


        painter.setBrush(first_color);
        painter.drawRect(fourth_rect);

        QColor first_color_transparent(first_color);
        first_color_transparent.setAlpha(alpha_value);
        painter.setBrush(first_color_transparent);
        painter.drawRect(first_rect);

        QColor second_color_transparent(second_color);
        second_color_transparent.setAlpha(alpha_value);
        painter.setBrush(second_color_transparent);
        painter.drawRect(second_rect);
        painter.drawRect(third_rect);


    } else if (current_rect == 4) {


        painter.setBrush(second_color);
        painter.drawRect(third_rect);

        QColor first_color_transparent(first_color);
        first_color_transparent.setAlpha(alpha_value);
        painter.setBrush(first_color_transparent);
        painter.drawRect(first_rect);
        painter.drawRect(fourth_rect);

        QColor second_color_transparent(second_color);
        second_color_transparent.setAlpha(alpha_value);
        painter.setBrush(second_color_transparent);
        painter.drawRect(second_rect);


    }

}
