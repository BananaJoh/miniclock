#include "lcdclock.h"

LCDClock::LCDClock(QWidget *parent) : QLCDNumber(parent) {
    display("88:88");
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);
//    updateTime();
}

void LCDClock::updateTime() {
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
//    if(time.second() % 2 == 0)
//        text[2] = ' ';
    display(text);
}
