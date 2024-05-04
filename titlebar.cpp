#include "titlebar.h"
#include "mainwindow.h"

TitleBar::TitleBar(QWidget *parent) : QFrame(parent)
{
    isDragging = false;
    setMouseTracking(true);
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (dynamic_cast<QPushButton*>(childAt(event->pos())) == nullptr)
        {
            dragPosition = event->globalPosition().toPoint() - window()->pos();
            isDragging = true;
        }
    }
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging) {
        window()->move(event->globalPosition().toPoint() - dragPosition);
    }
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (isDragging) {
        isDragging = false;
    }
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        MainWindow *mainWindow = dynamic_cast<MainWindow*>(window());
        QIcon icon;
        if (mainWindow->getMaximizedFlag()) {
            mainWindow->showNormal();
            mainWindow->setMaximizedFlag(false);
            icon.addFile(":/icons/fullscreen_icon.svg", QSize(18, 18));
            findChild<QPushButton*>("buttonMaximize")->setIcon(icon);
            dynamic_cast<QFrame*>(parent())->setStyleSheet("QFrame {background-color: rgb(28, 31, 41); color: white; border: 1px solid rgb(47, 50, 60); border-radius: 10px;}");
        } else {
            mainWindow->showMaximized();
            mainWindow->setMaximizedFlag(true);
            icon.addFile(":/icons/fullscreen_exit_icon.svg", QSize(18, 18));
            findChild<QPushButton*>("buttonMaximize")->setIcon(icon);
            dynamic_cast<QFrame*>(parent())->setStyleSheet("QFrame {background-color: rgb(28, 31, 41); color: white;}");
        }
    }
}

void TitleBar::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    if (cursor() != Qt::ArrowCursor) {
        setCursor(Qt::ArrowCursor);
    }
}
