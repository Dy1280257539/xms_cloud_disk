#pragma once

#include <QDialog>
#include "ui_xlogingui.h"

class XLoginGUI : public QDialog
{
    Q_OBJECT

public:
    XLoginGUI(QWidget *parent = Q_NULLPTR);
    ~XLoginGUI();
public slots:
    void Login();
private:
    Ui::XLoginGUI ui;
};
