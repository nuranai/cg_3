#include "win.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

win::win()
{
    area = new Area( this ); //создаем холст
    startBtn = new QPushButton("Start", this);
    exitBtn = new QPushButton("Exit",this ); //создаем кнопку
    QHBoxLayout *Hlayout = new QHBoxLayout();
    QVBoxLayout *layout = new QVBoxLayout(this);
    Hlayout->addWidget(startBtn);
    layout->addWidget(area);
    layout->addLayout(Hlayout);
    layout->addWidget(exitBtn);
    connect(exitBtn,SIGNAL(clicked(bool)), this,SLOT(close()));
    connect(startBtn,SIGNAL(clicked(bool)), area,SLOT(start_prog()));

}



void win::start_prog() {
    area->start_prog();
}
