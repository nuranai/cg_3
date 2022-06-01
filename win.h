#ifndef WIN_H
#define WIN_H

#include <QtGui>
#include <QPushButton>
#include <QObject>
#include "area.h"

class win: public QWidget
{
    Q_OBJECT
protected:
    Area * area;
    QPushButton * startBtn;
    QPushButton * exitBtn;
public:
    win();
public slots:
    void start_prog();
};

#endif // WIN_H
