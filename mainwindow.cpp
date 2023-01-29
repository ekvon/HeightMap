#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalLayout->setParent(this);
    //  QRect rect=this->geometry();
    //  ui->verticalLayout_2->setGeometry(QRect(rect.width()/3,0,2*rect.width()/3,rect.height()));
    ui->verticalLayout->setContentsMargins(3,3,3,3);
    ui->verticalLayout_2->setContentsMargins(3,3,3,3);
    ui->listWidget->addItem("scale");
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(4);
    QRect rect=ui->verticalLayout_2->geometry();
    fprintf(stdout,"vlayout2: (%d,%d,%d,%d)",
            rect.topLeft().x(),rect.topLeft().y(),rect.width(),rect.height());
    screen=new Widget(this);
    screen->setObjectName("screen");
    ui->verticalLayout_2->addWidget(screen);
    this->grabKeyboard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    QWidget::keyPressEvent(event);
    fprintf(stdout,"MainWindow: key is pressed (%d)\n",event->key());
    screen->keyPressEvent(event);
}
