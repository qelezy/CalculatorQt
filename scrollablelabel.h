#ifndef SCROLLABLELABEL_H
#define SCROLLABLELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QJSEngine>

class ScrollableLabel : public QLabel
{
public:
    ScrollableLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;

private:
    QPoint lastPosition;
};

#endif // SCROLLABLELABEL_H
