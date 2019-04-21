#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "md5.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent,Qt::FramelessWindowHint),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    //设置透明、无框、置顶

    QPixmap pixw(":/img/UI.png");
    ui->label->resize(pixw.width(),pixw.height());
    ui->label->setPixmap(pixw);
    //设置UI

    textOn=false;//输入框展开状态
    transOn=false;//翻译结果状态

    //ui->textEdit->grabKeyboard();
    ui->textEdit->installEventFilter(this);//设置完后自动调用其eventFilter函数

    /* //新建QSystemTrayIcon对象
        QSystemTrayIcon mSysTrayIcon = new QSystemTrayIcon(this);
        //新建托盘要显示的icon
        QIcon icon = QIcon(":/img/GG64.png");
        //将icon设到QSystemTrayIcon对象中
        mSysTrayIcon->setIcon(icon);
        //当鼠标移动到托盘上的图标时，会显示此处设置的内容
        mSysTrayIcon->setToolTip(QObject::trUtf8("测试系统托盘图标"));
        //在系统托盘显示此对象
        mSysTrayIcon->show();*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){//左键拖动窗体
        relativePos = this->pos()- event->globalPos();
    }
    if(event->button()==Qt::MidButton){//中键关闭窗体
        this->close();
    }
    if(event->button()==Qt::RightButton){//右键弹出/收起输入框
        QPropertyAnimation *pScaleAnimation1 = new QPropertyAnimation(ui->textEdit, "geometry");
        if(textOn){
            pScaleAnimation1->setDuration(100);
            pScaleAnimation1->setStartValue(QRect(95, 35,161, 31));
            pScaleAnimation1->setEndValue(QRect(95,35,0,31));
            pScaleAnimation1->start();
            textOn=false;
        }
        else{
            pScaleAnimation1->setDuration(100);
            pScaleAnimation1->setStartValue(QRect(95, 35, 0, 31));
            pScaleAnimation1->setEndValue(QRect(95,35,161,31));
            pScaleAnimation1->start();
            textOn=true;
        }

        //QDesktopServices::openUrl(QUrl("https://ranchoturing.com/", QUrl::TolerantMode));
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)//移动窗体
{
    this->move(event->globalPos() + relativePos);
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{

    if(target == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            if(k->key() == Qt::Key_Enter || k->key() == Qt::Key_Return)//Enter小键盘；Return回车
            {
                if(ui->textEdit->text().isEmpty()){
                    transOn=false;
                    //qDebug()<<"empty"<<endl;
                }
                else{
                    if(transOn == false){
                        QString text = ui->textEdit->text();
                        string getSrc=BDTApi(text.toStdString());
                        text = QString::fromStdString(getSrc);//得到请求链接的QString格式

                        QString data = TT::getHtml(text);
                        QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toLocal8Bit().data());
                        QJsonObject jsonObject = jsonDocument.object();//JO中的dst已被转换成中文字符


                        QString TText;
                        if(jsonObject.contains("trans_result"))
                        {
                            QJsonArray Array = jsonObject["trans_result"].toArray();
                            QJsonObject subObject = Array.at(0).toObject();
                            TText = subObject["dst"].toString();
                            //   qDebug()<<jsonObject<<endl<<Array<<endl<<subObject<<endl<<TText;
                        }

                        ui->textEdit->setText(TText);

                        transOn=true;
                        //qDebug()<<"1"<<endl;
                        return true;
                    }
                    else{
                        ui->textEdit->clear();
                        transOn=false;
                        //qDebug()<<"2"<<endl;
                    }
                }
            }
            if(k->key() == Qt::Key_Backspace)//退格
            {
                transOn=false;
                //qDebug()<<"B"<<endl;
            }
        }


    }
    return false;//MainWindow::eventFilter(target,event);
}



