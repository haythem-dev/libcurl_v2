#ifndef PRINT_H
#define PRINT_H

#include <QtWidgets/QMainWindow>
#include "ui_print.h"

class print : public QMainWindow
{
    Q_OBJECT

public:
    print(QWidget *parent = 0);
    ~print();

private:
    Ui::printClass ui;
};

#endif // PRINT_H
