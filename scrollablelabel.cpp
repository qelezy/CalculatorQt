#include "scrollablelabel.h"

ScrollableLabel::ScrollableLabel(QWidget *parent, Qt::WindowFlags f) : QLabel(parent, f) {}

void ScrollableLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPosition = event->pos();
    }
}

void ScrollableLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if (QScrollArea *scrollArea = qobject_cast<QScrollArea*>(parentWidget()->parentWidget()->parentWidget())) {
            scrollArea->horizontalScrollBar()->setValue(scrollArea->horizontalScrollBar()->value() + (event->pos() - lastPosition).x());
        }
    }
}

void ScrollableLabel::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    if (cursor() != Qt::ArrowCursor)
        setCursor(Qt::ArrowCursor);
}
