#ifndef LCDCLOCK_H
#define LCDCLOCK_H

#include<QLCDNumber>
#include<QTimer>
#include<QTime>

class LCDClock : public QLCDNumber {
    Q_OBJECT

public:
    LCDClock(QWidget *parent = 0);

private slots:
    void updateTime();
};

#endif // LCDCLOCK_H
