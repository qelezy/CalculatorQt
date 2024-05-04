#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QScrollBar>
#include <QJSEngine>
#include <QRegularExpression>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool getMaximizedFlag();
    void setMaximizedFlag(bool flag);

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onLineEditChanged(const QString &text);
    void onButtonMaximize();
    void onButtonOnTop();
    void onButtonSecondTab();
    void onButtonAngle();
    void onButtonNumber();
    void onButtonBasicMath();
    void onButtonChangeSign();
    void onButtonEquals();
    void onButtonDot();
    void onButtonBracket();
    void onButtonConstant();
    void onButtonClear();
    void onButtonBackspace();
    void onButtonTrigonometry();
    void onButtonDegree();
    void onButtonRoot();
    void onButtonFactorial();
    void onButtonExponent();
    void onButtonPercent();
    void onButtonMC();
    void onButtonMR();
    void onButtonMP();
    void onButtonMM();

private:
    Ui::MainWindow *ui;
    bool isMaximized, isOnTop, isSecondTab, isOperationSelection, isEqualsPressed, isTrigonometryPressed, isDegreeOperation, isRootOperation, blockNumberInput, blockOperationInput;
    double value, savedValue;
    int openBracketCount, closeBracketCount;
    QPoint dragPosition;
    QRegularExpression reRoot, reFact, reTrig;
    void connectButtons();
    void makeFlagsFalse();
    void changeFont();
    QString parseRoot(const QString& expression);
    QString parseFactorial(const QString& expression);
    unsigned long long factorial(unsigned n);
    QString parseTrigonometry(const QString& expression);
};
#endif // MAINWINDOW_H
