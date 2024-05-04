#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    isMaximized = false, isOnTop = false, isSecondTab = false, isOperationSelection = false, isEqualsPressed = false, isTrigonometryPressed = false, isDegreeOperation = false, isRootOperation = false, blockNumberInput = false, blockOperationInput = false;
    value = 0, savedValue = 0;
    openBracketCount = 0, closeBracketCount = 0;
    reRoot.setPattern("(root(\\(.*?\\)|\\d+\\.\\d+|\\d+))+");
    reFact.setPattern("\\([^()]*?\\)(?=\\!)");
    reTrig.setPattern("(?<!Math\\.|[a])sin(\\(.*?\\))|(?<!Math\\.|[a])cos(\\(.*?\\))|(?<!Math\\.|[a])tan(\\(.*?\\))|(?<!Math\\.)asin(\\(.*?\\))|(?<!Math\\.)acos(\\(.*?\\))|(?<!Math\\.)atan(\\(.*?\\))");
    ui->lineEdit->setText("0");
    connectButtons();
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(onLineEditChanged(QString)));
}

MainWindow::~MainWindow() { delete ui; }

bool MainWindow::getMaximizedFlag() { return isMaximized; }

void MainWindow::setMaximizedFlag(bool flag) { isMaximized = flag; }

void MainWindow::resizeEvent(QResizeEvent *event)
{
    changeFont();
    QMainWindow::resizeEvent(event);
}

void MainWindow::onLineEditChanged(const QString &text)
{
    if (text.isEmpty()) return;

    if (ui->lineEdit->text().toDouble() != 0) {
        ui->buttonClear->setText("C");
    } else {
        ui->buttonClear->setText("AC");
    }

    // Изменение шрифта
    changeFont();

    // Проверка на максимум цифр
    QString currentText = text;
    if (currentText.contains("-")) {
        currentText.remove("-");
    }
    if (currentText.contains(".")) {
        currentText.remove(".");
    }
    if (currentText.length() >= 16) {
        blockNumberInput = true;
    }
}

void MainWindow::onButtonMaximize()
{
    QIcon icon;
    if (isMaximized) {
        showNormal();
        isMaximized = false;
        icon.addFile(":/icons/fullscreen_icon.svg", QSize(18, 18));
        ui->buttonMaximize->setIcon(icon);
        ui->window->setStyleSheet("QFrame {background-color: rgb(28, 31, 41); color: white; border: 1px solid rgb(47, 50, 60); border-radius: 10px;}");
    } else {
        showMaximized();
        isMaximized = true;
        icon.addFile(":/icons/fullscreen_exit_icon.svg", QSize(18, 18));
        ui->buttonMaximize->setIcon(icon);
        ui->window->setStyleSheet("QFrame {background-color: rgb(28, 31, 41); color: white;}");
    }
}

void MainWindow::onButtonOnTop()
{
    QFont font;
    if (isOnTop) {
        ui->buttonOnTop->setStyleSheet("QPushButton {background-color: rgb(28, 31, 41);color: white;border-radius: 5px;} QPushButton:hover {background-color: rgb(47, 50, 60);} QPushButton:pressed {background-color: rgb(57, 60, 70);}");
        isOnTop = false;
        setWindowFlag(Qt::WindowStaysOnTopHint, false);
        show();
        setMinimumSize(350, 500);
        setMaximumSize(screen()->availableSize().width(), screen()->availableSize().height());
        // Возврат убранного
        ui->programName->show();
        ui->buttonMinimize->show();
        ui->buttonMaximize->show();
        // Увеличение шрифтов
        for (int i = 0; i < ui->saveButtons->count(); i++) {
            QPushButton *button = qobject_cast<QPushButton*>(ui->saveButtons->itemAt(i)->widget());
            font = button->font();
            font.setPointSize(12);
            button->setFont(font);
        }
        for (int i = 0; i < ui->buttonGrid->count(); i++) {
            QPushButton *button = qobject_cast<QPushButton*>(ui->buttonGrid->itemAt(i)->widget());
            font.setPointSize(12);
            if (!button->icon().isNull()) {
                if (button->iconSize() == QSize(12, 12)) {
                    button->setIconSize(QSize(16, 16));
                } else {
                    button->setIconSize(QSize(24, 24));
                }
            }
            // Цифровая панель
            for (int j = 0; j < 10; j++) {
                if (button->objectName() == "button" + QString::number(j)) {
                    font.setPointSize(14);
                }
            }
            // Кнопки базовой математики + "."
            if (button->objectName() == "buttonMultiply" || button->objectName() == "buttonDivide" || button->objectName() == "buttonAdd" || button->objectName() == "buttonSubtract" || button->objectName() == "buttonDot") font.setPointSize(14);
            button->setFont(font);
        }
        font.setPointSize(42);
        ui->lineEdit->setFont(font);
        font.setPointSize(14);
        ui->label->setFont(font);
    } else {
        ui->buttonOnTop->setStyleSheet("QPushButton {background-color: rgb(150, 60, 94);color: white;border-radius: 5px;} QPushButton:hover {background-color: rgb(140, 50, 84);} QPushButton:pressed {background-color: rgb(130, 40, 74);}");
        isOnTop = true;
        setWindowFlag(Qt::WindowStaysOnTopHint, true);
        show();
        setMinimumSize(250, 325);
        setMaximumSize(285, 380);
        setGeometry(x(), y(), 285, 380);
        // Скрываю кнопки
        ui->programName->hide();
        ui->buttonMinimize->hide();
        ui->buttonMaximize->hide();
        // Уменьшение шрифтов
        for (int i = 0; i < ui->saveButtons->count(); i++) {
            QPushButton *button = qobject_cast<QPushButton*>(ui->saveButtons->itemAt(i)->widget());
            font = button->font();
            font.setPointSize(8);
            button->setFont(font);
        }
        for (int i = 0; i < ui->buttonGrid->count(); i++) {
            QPushButton *button = qobject_cast<QPushButton*>(ui->buttonGrid->itemAt(i)->widget());
            font.setPointSize(8);
            if (!button->icon().isNull()) {
                if (button->iconSize() == QSize(16, 16)) {
                    button->setIconSize(QSize(12, 12));
                } else {
                    button->setIconSize(QSize(18, 18));
                }
            }
            // Цифровая панель
            for (int j = 0; j < 10; j++) {
                if (button->objectName() == "button" + QString::number(j)) {
                    font.setPointSize(10);
                }
            }
            // Кнопки базовой математики + "."
            if (button->objectName() == "buttonMultiply" || button->objectName() == "buttonDivide" || button->objectName() == "buttonAdd" || button->objectName() == "buttonSubtract" || button->objectName() == "buttonDot") font.setPointSize(10);
            button->setFont(font);
        }
        font.setPointSize(30);
        ui->lineEdit->setFont(font);
        font.setPointSize(10);
        ui->label->setFont(font);
    }
    changeFont();
}

void MainWindow::onButtonSecondTab()
{
    if (isSecondTab) {
        ui->buttonSecondTab->setStyleSheet("QPushButton {background-color: rgb(37, 40, 50);color: white;border-radius: 5px;} QPushButton:hover {background-color: rgb(47, 50, 60);} QPushButton:pressed {background-color: rgb(43, 46, 56);}");
        // Стиль для строки
        for (int i = 0; i < 3; i++) {
            ui->buttonGrid->itemAtPosition(0, i + 1)->widget()->setStyleSheet("QPushButton {background-color: rgb(37, 40, 50);color: white;border-radius: 5px;}QPushButton:hover {background-color: rgb(47, 50, 60);}QPushButton:pressed {background-color: rgb(43, 46, 56);}");
            QPushButton *button = qobject_cast<QPushButton*>(ui->buttonGrid->itemAtPosition(0, i + 1)->widget());
            button->setText(button->text().removeFirst());
        }
        // Стиль для столбца
        for (int i = 0; i < 2; i++) {
            ui->buttonGrid->itemAtPosition(i + 1, 0)->widget()->setStyleSheet("QPushButton {background-color: rgb(37, 40, 50);color: white;border-radius: 5px;}QPushButton:hover {background-color: rgb(47, 50, 60);}QPushButton:pressed {background-color: rgb(43, 46, 56);}");
        }
        // Установка иконок для кнопок
        QIcon icon1, icon2;
        icon1.addFile(":/icons/square_icon.png", QSize(24, 24));
        ui->buttonDegree->setIcon(icon1);
        icon2.addFile(":/icons/square_root_icon.png", QSize(24, 24));
        ui->buttonRoot->setIcon(icon2);
        isSecondTab = false;
    } else {
        ui->buttonSecondTab->setStyleSheet("QPushButton {background-color: rgb(150, 60, 94);color: white;border-radius: 5px;} QPushButton:hover {background-color: rgb(140, 50, 84);} QPushButton:pressed {background-color: rgb(130, 40, 74);}");
        // Стиль для строки
        for (int i = 0; i < 3; i++) {
            ui->buttonGrid->itemAtPosition(0, i + 1)->widget()->setStyleSheet("QPushButton {background-color: rgb(37, 40, 50);color: white;border-radius: 5px;} QPushButton:hover {background-color: rgb(140, 50, 84);} QPushButton:pressed {background-color: rgb(130, 40, 74);}");
            QPushButton *button = qobject_cast<QPushButton*>(ui->buttonGrid->itemAtPosition(0, i + 1)->widget());
            button->setText("a" + button->text());
        }
        // Стиль для столбца
        for (int i = 0; i < 2; i++) {
            ui->buttonGrid->itemAtPosition(i + 1, 0)->widget()->setStyleSheet("QPushButton {background-color: rgb(37, 40, 50);color: white;border-radius: 5px;} QPushButton:hover {background-color: rgb(140, 50, 84);} QPushButton:pressed {background-color: rgb(130, 40, 74);}");
        }
        // Установка иконок для кнопок
        QIcon icon1, icon2;
        icon1.addFile(":/icons/square_y_icon.png", QSize(24, 24));
        ui->buttonDegree->setIcon(icon1);
        icon2.addFile(":/icons/square_root_y_icon.png", QSize(24, 24));
        ui->buttonRoot->setIcon(icon2);
        isSecondTab = true;
    }
}

void MainWindow::onButtonAngle() { (ui->buttonAngle->text() == "DEG") ? ui->buttonAngle->setText("RAD") : ui->buttonAngle->setText("DEG"); }

void MainWindow::onButtonNumber()
{
    if (blockNumberInput) {
        return;
    }
    if (blockOperationInput) {
        blockOperationInput = false;
    }
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (value == 0) {
        if ((isOnTop) ? (ui->lineEdit->font().pointSize() < 30) : (ui->lineEdit->font().pointSize() < 42)) {
            ui->lineEdit->setFont(QFont(ui->lineEdit->font().family(), (isOnTop) ? 30 : 42));
        }
        if (ui->lineEdit->text().contains(".")) {
            ui->lineEdit->setText(isOperationSelection ? button->text() : (ui->lineEdit->text() + button->text()));
        } else {
            ui->lineEdit->setText(button->text());
        }
        if (isEqualsPressed || isTrigonometryPressed) {
            ui->label->setText("");
            isEqualsPressed = false;
            isTrigonometryPressed = false;
        }
    } else {
        ui->lineEdit->setText(ui->lineEdit->text() + button->text());
    }
    makeFlagsFalse();
    if (isOperationSelection) {
        isOperationSelection = false;
    }
    value = ui->lineEdit->text().toDouble();
}

void MainWindow::onButtonBasicMath()
{
    if (blockOperationInput) {
        return;
    }
    if (blockNumberInput) {
        blockNumberInput = false;
    }
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (isEqualsPressed) {
        ui->label->setText("");
        value = ui->lineEdit->text().toDouble();
        isEqualsPressed = false;
    }
    makeFlagsFalse();
    if (ui->label->text().size() > 0) {
        if (ui->label->text().back() == '^' && value == 0) {
            ui->label->setText(ui->label->text().removeLast() + button->text());
            isOperationSelection = true;
        } else if (ui->label->text().size() > 1 && ui->label->text().last(2) == "^2") {
            ui->label->setText(ui->label->text() + button->text());
            isOperationSelection = true;
        } else if (ui->label->text().back() == ')' || ui->label->text().back() == '!') {
            ui->label->setText(ui->label->text() + button->text());
            isOperationSelection = true;
        } else if (ui->label->text().size() > 3 && ui->label->text().last(4) == "root" && value == 0) {
            ui->label->setText(ui->label->text().remove(ui->label->text().size() - 4, ui->label->text().size()) + button->text());
            isOperationSelection = true;
        }
    }
    if (isOperationSelection) {
        ui->label->setText(ui->label->text().removeLast() + button->text());
    } else {
        if (value < 0) {
            ui->label->setText(ui->label->text() + "(" + QString::number(value, 'g', 16) + ")" + button->text());
        } else if (value == 0) {
            (ui->label->text().size() == 0) ? ui->label->setText(QString::number(value, 'g', 16) + button->text()) : ui->label->setText(ui->label->text() + button->text());
        } else {
            ui->label->setText(ui->label->text() + QString::number(value, 'g', 16) + button->text());
        }
        isOperationSelection = true;
    }
    value = 0;
}

void MainWindow::onButtonChangeSign()
{
    if (isOperationSelection) {
        value = ui->lineEdit->text().toDouble();
        isOperationSelection = false;
    }
    if (isEqualsPressed) {
        ui->label->setText("");
        value = ui->lineEdit->text().toDouble();
        isEqualsPressed = false;
    }
    ui->lineEdit->setText(QString::number(value *= -1, 'g', 16));
}

void MainWindow::onButtonEquals()
{
    if (!ui->label->text().contains("=")) {
        if (ui->label->text().size() > 0 && ui->label->text().back() != ')' && !ui->label->text().back().isDigit() && ui->label->text().back() != '!') {
            ui->label->setText(ui->label->text() + ((value < 0) ? ("(" + ui->lineEdit->text() + ")") : ui->lineEdit->text()));
        }
        if (openBracketCount > closeBracketCount) {
            for (int i = 0; i < openBracketCount - closeBracketCount; i++) {
                ui->label->setText(ui->label->text() + ")");
            }
        }
        if (ui->label->text().isEmpty()) {
            ui->label->setText(ui->lineEdit->text());
        }

        QString str = ui->label->text();
        if (ui->label->text().contains("×")) {
            str.replace("×", "*");
        }
        if (ui->label->text().contains("÷")) {
            str.replace("÷", "/");
        }
        if (ui->label->text().contains("^")) {
            str.replace("^", "**");
        }
        if (ui->label->text().contains("√")) {
            str.replace("√", "Math.sqrt");
        }
        if (ui->label->text().contains("root")) {
            str = parseRoot(str);
        }
        if (ui->label->text().contains("!")) {
            str = parseFactorial(str);
        }
        if (ui->label->text().contains("sin") || ui->label->text().contains("cos") || ui->label->text().contains("tan") || ui->label->text().contains("asin") || ui->label->text().contains("acos") || ui->label->text().contains("atan")) {
            str = parseTrigonometry(str);
        }

        QJSEngine engine;
        QJSValue answer = engine.evaluate(str);
        ui->label->setText(ui->label->text() + "=");
        qDebug() << answer.toNumber();
        if (QString::number(answer.toNumber()) != "nan") {
            ui->lineEdit->setText(QString::number(answer.toNumber(), 'g', 16));
            value = answer.toNumber();
        } else {
            ui->lineEdit->setText("Ошибка");
            value = 0;
        }

        ui->buttonClear->setText("AC");
        openBracketCount = 0, closeBracketCount = 0;
        isEqualsPressed = true;
        makeFlagsFalse();
        if (isOperationSelection) {
            isOperationSelection = false;
        }
        if (blockNumberInput) {
            blockNumberInput = false;
        }
        if (blockOperationInput) {
            blockOperationInput = false;
        }
    }
}

void MainWindow::onButtonDot()
{
    if (blockNumberInput || isEqualsPressed) {
        return;
    }
    if (!ui->lineEdit->text().contains(".")) {
        if (isOperationSelection) {
            ui->lineEdit->setText("0.");
            isOperationSelection = false;
        } else {
            ui->lineEdit->setText(ui->lineEdit->text() + ".");
        }
        value = ui->lineEdit->text().toDouble();
    }
}

void MainWindow::onButtonBracket()
{
    if (isEqualsPressed) {
        return;
    }
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button->text() == "(") {
        if (blockNumberInput) {
            blockNumberInput = false;
        }
        if (ui->label->text().isEmpty() && value != 0) {
            ui->label->setText(ui->lineEdit->text() + "×(");
        } else {
            if (value != 0) {
                ui->label->setText(ui->label->text() + ((value < 0) ? "(" + QString::number(value, 'g', 16) + ")" : QString::number(value, 'g', 16)));
            }
            (ui->label->text().size() > 0 && (ui->label->text().back().isDigit() || ui->label->text().back() == ')' || ui->label->text().back() == '!')) ? ui->label->setText(ui->label->text() + "×(") : ui->label->setText(ui->label->text() + "(");
        }
        openBracketCount++;
        blockOperationInput = true;
    } else {
        if (openBracketCount > closeBracketCount) {
            if (ui->label->text().size() > 0) {
                if (ui->label->text().back() == '!' || ui->label->text().back() == ')' || ui->label->text().back().isDigit()) {
                    ui->label->setText(ui->label->text() + ")");
                } else {
                    ui->label->setText(ui->label->text() + ((value < 0) ? "(" + QString::number(value, 'g', 16) + ")" : QString::number(value, 'g', 16)) + ")");
                }
            }
            closeBracketCount++;
            // blockNumberInput = true;
        }
    }
    value = 0;
}

void MainWindow::onButtonConstant()
{
    if (blockNumberInput) {
        return;
    }
    if (isEqualsPressed || isTrigonometryPressed) {
        ui->label->setText("");
        isEqualsPressed = false;
        isTrigonometryPressed = false;
    }
    if (isOperationSelection)
        isOperationSelection = false;
    (sender()->objectName() == "buttonPi") ? ui->lineEdit->setText(QString::number(M_PI, 'g', 16)) : ui->lineEdit->setText(QString::number(M_E, 'g', 16));
    value = ui->lineEdit->text().toDouble();
}

void MainWindow::onButtonClear()
{
    if (ui->buttonClear->text() == "C") {
        ui->lineEdit->setText("0");
    } else {
        ui->label->setText("");
        ui->lineEdit->setText("0");
        makeFlagsFalse();
        isOperationSelection = false, isEqualsPressed = false, blockNumberInput = false, blockOperationInput = false;
        openBracketCount = 0, closeBracketCount = 0;
    }
    value = 0;
    if ((isOnTop) ? (ui->lineEdit->font().pointSize() < 30) : (ui->lineEdit->font().pointSize() < 42)) {
        (isOnTop) ? ui->lineEdit->setFont(QFont(ui->lineEdit->font().family(), 30)) : ui->lineEdit->setFont(QFont(ui->lineEdit->font().family(), 42));
    }
}

void MainWindow::onButtonBackspace()
{
    if (isOperationSelection || isTrigonometryPressed || blockNumberInput)
        return;
    if (ui->lineEdit->text().length() > 1){
        ui->lineEdit->setText(ui->lineEdit->text().removeLast());
        if (ui->lineEdit->text().length() == 1 && value < 0) {
            ui->lineEdit->setText("0");
        }
    } else {
        ui->lineEdit->setText("0");
    }
    if (blockOperationInput) {
        blockOperationInput = false;
    }
    value = ui->lineEdit->text().toDouble();
}

void MainWindow::onButtonTrigonometry()
{
    if (isEqualsPressed) {
        return;
    }
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (ui->label->text().size() > 0 && ui->label->text().back() != ')' && ui->label->text().back() != '!' && !ui->label->text().back().isDigit()) {
        ui->label->setText(ui->label->text() + button->text() + "(" + QString::number(value, 'g', 16) + ")");
    } else if (ui->label->text().isEmpty()) {
        ui->label->setText(ui->label->text() + button->text() + "(" + QString::number(value, 'g', 16) + ")");
    }
    isTrigonometryPressed = true;
    value = 0;
}

void MainWindow::onButtonDegree()
{
    if (blockOperationInput || isEqualsPressed) {
        return;
    }
    if (isRootOperation && ui->label->text().size() > 3) {
        ui->label->setText(ui->label->text().remove(ui->label->text().size() - 4, ui->label->text().size()));
    }
    if (isSecondTab) {
        if (ui->label->text().isEmpty()) {
            ui->label->setText(ui->label->text() + ((value < 0) ? ("(" + QString::number(value, 'g', 16) + ")") : QString::number(value, 'g', 16)));
        }
        if (ui->label->text().size() > 0 && (ui->label->text().back() == '(' || ui->label->text().back() == '^')) {
            ui->label->setText(ui->label->text() + ui->lineEdit->text());
        }
        if (isOperationSelection && !ui->label->text().back().isDigit()) {
            ui->label->setText(ui->label->text().removeLast());
        }
        ui->label->setText(ui->label->text() + "^");
        isDegreeOperation = true;
    } else {
        if (ui->label->text().size() > 0 && ui->label->text().back() != ')' && ui->label->text().back() != '!' && !ui->label->text().back().isDigit()) {
            ui->label->setText(ui->label->text() + ((value < 0) ? ("(" + QString::number(value, 'g', 16) + ")") : QString::number(value, 'g', 16)) + "^2");
        } else {
            if (ui->label->text().isEmpty()) {
                ui->label->setText(ui->label->text() + ((value < 0) ? ("(" + QString::number(value, 'g', 16) + ")") : QString::number(value, 'g', 16)));
            }
            ui->label->setText(ui->label->text() + "^2");
        }
        blockNumberInput = true;
    }
    value = 0;
}

void MainWindow::onButtonRoot()
{
    if (blockOperationInput || isEqualsPressed) {
        return;
    }
    if (isDegreeOperation && ui->label->text().size() > 0) {
        ui->label->setText(ui->label->text().removeLast());
    }
    if (isRootOperation && value == 0) {
        return;
    }
    if (isSecondTab) {
        if (isOperationSelection && !ui->label->text().back().isDigit()) {
            ui->label->setText(ui->label->text().removeLast());
        }
        if (isDegreeOperation || (ui->label->text().size() > 0 && (ui->label->text().back() == ')' || ui->label->text().back().isDigit()))) {
            ui->label->setText(ui->label->text() + "root");
        } else {
            ui->label->setText(ui->label->text() + ((value < 0) ? ("(" + QString::number(value, 'g', 16) + ")") : QString::number(value, 'g', 16)) + "root");
        }
        isRootOperation = true;
        isOperationSelection = true;
    } else {
        if (ui->label->text().isEmpty()) {
            ui->label->setText(ui->label->text() + "√(" + QString::number(value, 'g', 16) + ")");
        }
        if (ui->label->text().size() > 0 && ui->label->text().back() != ')' && ui->label->text().back() != '!' && !ui->label->text().back().isDigit()) {
            ui->label->setText(ui->label->text() + "√(" + QString::number(value, 'g', 16) + ")");
        }

        if (blockOperationInput) {
           blockOperationInput = false;
        }
        blockNumberInput = true;
    }
    value = 0;
}

void MainWindow::onButtonFactorial()
{
    if (isEqualsPressed) {
        return;
    }
    if (ui->label->text().size() > 0 && ui->label->text().back() == '!') {
        ui->label->setText(ui->label->text() + "×(" + ui->lineEdit->text() + ")!");
    } else {
        if (ui->label->text().size() > 0 && !ui->label->text().back().isDigit()) {
            (ui->label->text().size() > 0 && ui->label->text().back() == ')') ? ui->label->setText(ui->label->text() + "!") : ui->label->setText(ui->label->text() + "(" + ui->lineEdit->text() + ")!");
        }
    }
    if (isOperationSelection) {
        isOperationSelection = false;
    }
    if (blockOperationInput) {
        blockOperationInput = false;
    }
    blockNumberInput = true;
    makeFlagsFalse();
    value = 0;
}

void MainWindow::onButtonExponent()
{
    if (isEqualsPressed) {
        return;
    }
    if (ui->label->text().size() > 0 && ui->label->text().back() == ')') {
        ui->label->setText(ui->label->text() + "×10^(" + ui->lineEdit->text() + ")");
    } else {
        if (ui->label->text().size() > 0 && !ui->label->text().back().isDigit() && ui->label->text().back() != '!') {
            ui->label->setText(ui->label->text() + "10^(" + ui->lineEdit->text() + ")");
        }
    }
    if (blockOperationInput) {
        blockOperationInput = false;
    }
    blockNumberInput = true;
    value = 0;
}

void MainWindow::onButtonPercent()
{
    if (isEqualsPressed) {
        return;
    }
    if (isOperationSelection) {
        ui->label->setText(ui->label->text() + QString::number(value /= 100));
        ui->lineEdit->setText(QString::number(value));
        isOperationSelection = false;
    } else if (isEqualsPressed) {
        ui->label->setText(QString::number(value /= 100));
        ui->lineEdit->setText(QString::number(value));
    } else {
        if (value == 0) {
            return;
        }
        ui->lineEdit->setText(QString::number(value /= 100));
    }
}

void MainWindow::onButtonMC()
{
    savedValue = 0;
}

void MainWindow::onButtonMR()
{
    if (isOperationSelection) {
        isOperationSelection = false;
    }
    if (blockOperationInput) {
        blockOperationInput = false;
    }
    ui->lineEdit->setText(QString::number(savedValue, 'g', 16));
    value = savedValue;
}

void MainWindow::onButtonMP()
{
    savedValue += value;
}

void MainWindow::onButtonMM()
{
    savedValue -= value;
}

void MainWindow::connectButtons()
{
    // Кнопки окна приложения
    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->buttonMinimize, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(ui->buttonMaximize, SIGNAL(clicked()), this, SLOT(onButtonMaximize()));
    connect(ui->buttonOnTop, SIGNAL(clicked()), this, SLOT(onButtonOnTop()));
    // Вспомогательные кнопки работы с калькулятором
    connect(ui->buttonSecondTab, SIGNAL(clicked()), this, SLOT(onButtonSecondTab()));
    connect(ui->buttonAngle, SIGNAL(clicked()), this, SLOT(onButtonAngle()));
    connect(ui->buttonChangeSign, SIGNAL(clicked()), this, SLOT(onButtonChangeSign()));
    connect(ui->buttonEquals, SIGNAL(clicked()), this, SLOT(onButtonEquals()));
    connect(ui->buttonClear, SIGNAL(clicked()), this, SLOT(onButtonClear()));
    connect(ui->buttonErase, SIGNAL(clicked()), this, SLOT(onButtonBackspace()));
    connect(ui->buttonDot, SIGNAL(clicked()), this, SLOT(onButtonDot()));
    connect(ui->buttonBracketOpen, SIGNAL(clicked()), this, SLOT(onButtonBracket()));
    connect(ui->buttonBracketClose, SIGNAL(clicked()), this, SLOT(onButtonBracket()));
    // Константы
    connect(ui->buttonPi, SIGNAL(clicked()), this, SLOT(onButtonConstant()));
    connect(ui->buttonE, SIGNAL(clicked()), this, SLOT(onButtonConstant()));
    // Кнопки цифровой панели
    for (int i = 0; i < 10; i++) {
        QPushButton *button = MainWindow::findChild<QPushButton*>("button" + QString::number(i));
        connect(button, SIGNAL(clicked()), this, SLOT(onButtonNumber()));
    }
    // Кнопки базовой математики
    connect(ui->buttonAdd, SIGNAL(clicked()), this, SLOT(onButtonBasicMath()));
    connect(ui->buttonSubtract, SIGNAL(clicked()), this, SLOT(onButtonBasicMath()));
    connect(ui->buttonMultiply, SIGNAL(clicked()), this, SLOT(onButtonBasicMath()));
    connect(ui->buttonDivide, SIGNAL(clicked()), this, SLOT(onButtonBasicMath()));
    // Кнопки тригонометрических операций
    connect(ui->buttonSin, SIGNAL(clicked()), this, SLOT(onButtonTrigonometry()));
    connect(ui->buttonCos, SIGNAL(clicked()), this, SLOT(onButtonTrigonometry()));
    connect(ui->buttonTan, SIGNAL(clicked()), this, SLOT(onButtonTrigonometry()));
    // Кнопки математических операций
    connect(ui->buttonDegree, SIGNAL(clicked()), this, SLOT(onButtonDegree()));
    connect(ui->buttonRoot, SIGNAL(clicked()), this, SLOT(onButtonRoot()));
    connect(ui->buttonFactorial, SIGNAL(clicked()), this, SLOT(onButtonFactorial()));
    connect(ui->buttonExponent, SIGNAL(clicked()), this, SLOT(onButtonExponent()));
    connect(ui->buttonPercent, SIGNAL(clicked()), this, SLOT(onButtonPercent()));
    // Кнопки для работы с памятью
    connect(ui->buttonMC, SIGNAL(clicked()), this, SLOT(onButtonMC()));
    connect(ui->buttonMR, SIGNAL(clicked()), this, SLOT(onButtonMR()));
    connect(ui->buttonMPlus, SIGNAL(clicked()), this, SLOT(onButtonMP()));
    connect(ui->buttonMMinus, SIGNAL(clicked()), this, SLOT(onButtonMM()));
}

void MainWindow::makeFlagsFalse()
{
    if (isTrigonometryPressed)
        isTrigonometryPressed = false;
    if (isDegreeOperation)
        isDegreeOperation = false;
    if (isRootOperation)
        isRootOperation = false;
}

void MainWindow::changeFont()
{
    QFont font = ui->lineEdit->font();
    QFontMetrics fontMetrics(font);

    int textWidth = fontMetrics.horizontalAdvance(ui->lineEdit->text()), availableWidth = ui->lineEdit->width(), charWidth = fontMetrics.averageCharWidth();

    // Изменение размера шрифта
    if (textWidth + charWidth / 2 > availableWidth) {
        while (textWidth + charWidth / 2 > availableWidth && font.pointSize() > 1) {
            font.setPointSize(font.pointSize() - 1);
            fontMetrics = QFontMetrics(font);
            textWidth = fontMetrics.horizontalAdvance(ui->lineEdit->text());
            charWidth = fontMetrics.averageCharWidth();
        }
    } else {
        while (textWidth + charWidth / 2 < availableWidth && ((isOnTop) ? (font.pointSize() < 30) : (font.pointSize() < 42))) {
            font.setPointSize(font.pointSize() + 1);
            fontMetrics = QFontMetrics(font);
            textWidth = fontMetrics.horizontalAdvance(ui->lineEdit->text());
            charWidth = fontMetrics.averageCharWidth();
        }
        if (textWidth + charWidth / 2 > availableWidth) {
            font.setPointSize(font.pointSize() - 1);
        }
    }
    ui->lineEdit->setFont(font);
}

QString MainWindow::parseRoot(const QString &expression)
{
    QString result = expression;
    QRegularExpressionMatch match = reRoot.match(result);
    int expCount = 0;
    QString last = "";
    while (match.hasMatch()) {
        QString str = match.captured(0), base = "";
        int matchStart = match.capturedStart(), matchLength = match.capturedLength(), openBracket = 0, closeBracket = 0, ind = 0;
        for (int i = matchStart - 1; i >= 0; i--) {
            if (result[i] == '(') {
                openBracket++;
            } else if (result[i] == ')') {
                closeBracket++;
            }
            if (openBracket == closeBracket && (result[i] == '+' || result[i] == '-' || result[i] == '/' || result[i] == '*' || result[i] == '^')) {
                break;
            }
            base = result[i] + base;
            ind = i;
        }
        QRegularExpressionMatch subMatch = reRoot.match(str);
        while (subMatch.hasMatch()) {
            QString exponent = subMatch.captured(2);
            if (!expCount) {
                last = ",1/" + exponent + ")";
            } else {
                last = ",1/" + exponent + ")" + last;
            }
            expCount++;
            int index = str.lastIndexOf("root");
            str.replace(index, str.size(), last);
            subMatch = reRoot.match(str);
            if (!subMatch.hasMatch()) {
                last = base + last;
                for (int i = 0; i < expCount; i++) {
                    last = "Math.pow(" + last;
                }
                result.replace(ind, matchLength + matchStart - ind, last);
                last = "";
                expCount = 0;
                break;
            }
        }
        match = reRoot.match(result);
    }
    return result;
}

QString MainWindow::parseFactorial(const QString &expression)
{
    QString result = expression;
    QRegularExpressionMatch match = reFact.match(result);
    while (match.hasMatch()) {
        QString str = match.captured(0);
        str.removeFirst();
        str.removeLast();
        QRegularExpressionMatch subMatch = reFact.match(str);
        int matchStart = match.capturedStart(), matchLength = match.capturedLength();
        while (subMatch.hasMatch()) {
            QString subStr = subMatch.captured(0);
            int subMatchStart = subMatch.capturedStart(), subMatchLength = subMatch.capturedLength();
            subStr.removeFirst();
            subStr.removeLast();
            unsigned long long answer = factorial(subStr.toUInt());
            result.replace(subMatchStart, subMatchLength + 1, QString::number(answer));
            subMatch = reFact.match(str);
        }
        unsigned long long answer = factorial(str.toUInt());
        result.replace(matchStart, matchLength + 1, QString::number(answer));
        match = reFact.match(result);
    }
    return result;
}

unsigned long long MainWindow::factorial(unsigned n)
{
    unsigned long long result = 1;
    for (unsigned i = 0; i <= n; i++) {
        if (i != 0) {
            result *= i;
        }
    }
    return result;
}

QString MainWindow::parseTrigonometry(const QString &expression)
{
    QString result = expression;
    QRegularExpressionMatch match = reTrig.match(result);
    QJSEngine engine;
    QJSValue answer;
    while (match.hasMatch()) {
        int matchStart = match.capturedStart(), matchLength = match.capturedLength();
        if (ui->buttonAngle->text() == "DEG") {
            int index = matchStart;
            for (int i = matchStart; i < matchStart + matchLength; i++) {
                if (result[i] == '(') {
                    index = i;
                    break;
                }
            }
            QString function = result.sliced(matchStart, index - matchStart), arg, str;
            if (!match.captured(1).isEmpty()) {
                arg = match.captured(1);
            } else if (!match.captured(2).isEmpty()) {
                arg = match.captured(2);
            } else if (!match.captured(3).isEmpty()) {
                arg = match.captured(3);
            } else if (!match.captured(4).isEmpty()) {
                arg = match.captured(4);
            } else if (!match.captured(5).isEmpty()) {
                arg = match.captured(5);
            } else if (!match.captured(6).isEmpty()) {
                arg = match.captured(6);
            }
            if (function.front() != 'a') {
                arg += "*Math.PI/180";
                str = "Math." + function + "(" + arg + ")";
            } else {
                str = "Math." + function + arg + "*180/Math.PI";
            }
            answer = engine.evaluate(str);
            result.replace(matchStart, matchLength, QString::number(answer.toNumber()));
        } else {
            answer = engine.evaluate("Math." + match.captured(0));
            result.replace(matchStart, matchLength, QString::number(answer.toNumber()));
        }
        match = reTrig.match(result);
    }
    return result;
}
