#include "waitingwidget.h"

WaitingWidget::WaitingWidget(QWidget *parent)
    : QWidget{parent}
{}

void WaitingWidget::update() {
    
    
    
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

}
