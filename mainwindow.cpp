#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->setAttribute(Qt::WA_TranslucentBackground);
    Qt::WindowFlags flags = this->windowFlags();
    this->setWindowFlags(flags | Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);

    settings = new QSettings("settings.ini", QSettings::IniFormat);
    if(settings->contains("colorWindowText")) {
        QColor colorInput(settings->value("colorWindowText").toString());
        if(colorInput.isValid()) {
            QPalette palette = this->ui->lcdClock->palette();
            palette.setColor(QPalette::WindowText, colorInput);
            this->ui->lcdClock->setPalette(palette);
        }
    }
    if(settings->contains("scaleMainWindow")) {
        double scale = settings->value("scaleMainWindow").toDouble();
        if(!(scale < 0.4 || scale > 10.0))
            this->resize(240 * scale, 102 * scale);
    }
    if(settings->contains("posMainWindow")) {
        this->move(settings->value("posMainWindow").toPoint());
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
}

void MainWindow::on_lcdClock_customContextMenuRequested(const QPoint &pos) {
    QMenu contextMenu;

    QAction colorsAction("Farbe", this);
    connect(&colorsAction, SIGNAL(triggered()), this, SLOT(onColorChange()));
    contextMenu.addAction(&colorsAction);

    QAction resizeAction("Größe", this);
    connect(&resizeAction, SIGNAL(triggered()), this, SLOT(onResize()));
    contextMenu.addAction(&resizeAction);

    QAction closeAction("Beenden", this);
    connect(&closeAction, SIGNAL(triggered()), this, SLOT(onClose()));
    contextMenu.addAction(&closeAction);

    contextMenu.exec(mapToGlobal(pos));
}

void MainWindow::onColorChange() {
    bool ok;
    QColor colorNow = this->ui->lcdClock->palette().color(QPalette::WindowText);
    QString input = QInputDialog::getText(this, "Farbe", "ARGB Hex:", QLineEdit::Normal, colorNow.name(QColor::HexArgb), &ok);
    if(ok) {
        QColor colorInput(input);
        if(colorInput.isValid()) {
            QPalette palette = this->ui->lcdClock->palette();
            palette.setColor(QPalette::WindowText, colorInput);
            this->ui->lcdClock->setPalette(palette);
        }
    }
}

void MainWindow::onResize() {
    bool ok;
    double scale = QInputDialog::getDouble(this, "Größe", "Multiplikator:", this->width() / 240.0, 0.4, 10.0, 1, &ok);
    if(ok) {
        this->resize(240 * scale, 102 * scale);
    }
}

void MainWindow::onClose() {
    settings->setValue("colorWindowText", this->ui->lcdClock->palette().color(QPalette::WindowText).name(QColor::HexArgb));
    settings->setValue("scaleMainWindow", this->width() / 240.0);
    settings->setValue("posMainWindow", this->pos());
    settings->sync();
    QApplication::quit();
}
