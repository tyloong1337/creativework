#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myglwidget.h"
#include <QPainter>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QLabel>
MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this); // Растянуть окно вправо на 200 пикселей при запуске
    //widgets
    //extern QLineEdit* ent1;
    //extern QLineEdit* ent2;
    ent1=new QLineEdit;
    ent2=new QLineEdit;
    grid=new QGridLayout();
    openGLW=new myGLWidget(this);
    btn_calc=new QPushButton;
    for (int i=0;i<6;i++)
    {
        QPushButton* btn=new QPushButton;
        btn->setFixedSize(200,50);
        grid->addWidget(btn,i,1,1,2);
        connect(btn,&QPushButton::clicked,this,&MainWindow::Fswitch);
        G->addButton(btn,i);
    }
    //texts
    G->button(0)->setText("Режим добавления вершин");
    G->button(1)->setText("Добавить\nоднонаправленное ребро\nс заданным весом");
    G->button(2)->setText("Добавить\nдвунаправленное ребро\nс заданным весом");
    G->button(3)->setText("Режим перемещения");
    G->button(4)->setText("Режим удаления");
    G->button(5)->setText("Отменить текущий режим");
    btn_calc->setText("Решить задачу коммивояжёра!");
    G->button(0)->setShortcut(Qt::Key_A);
    G->button(1)->setShortcut(Qt::Key_Q);
    G->button(2)->setShortcut(Qt::Key_E);
    G->button(3)->setShortcut(Qt::Key_M);
    G->button(4)->setShortcut(Qt::Key_D);
    G->button(5)->setShortcut(Qt::Key_Escape);
    //sizes
    G->button(1)->setFixedSize(150,50);
    G->button(2)->setFixedSize(150,50);
    btn_calc->setFixedSize(200,50);
    ent1->setFixedSize(40,50);
    ent2->setFixedSize(40,50);
    //locations
    grid->addWidget(openGLW,0,0,8,1);
    grid->addWidget(G->button(1),1,2);
    grid->addWidget(G->button(2),2,2);
    grid->addWidget(ent1,1,1);
    grid->addWidget(ent2,2,1);
    grid->addWidget(G->button(1),1,2);
    grid->addWidget(G->button(2),2,2);
    grid->addWidget(btn_calc,7,1,1,2);
    connect(btn_calc,&QPushButton::clicked,this,&MainWindow::Fcheck);
    ui->centralwidget->setLayout(grid);
}
void MainWindow::Fswitch()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    int buttonID = G->id(button);
    for (int i=0;i<6;i++)
    {
        G->button(i)->setStyleSheet("QPushButton { background-color: rgb(240,240,240); }\n");
    }
    if (buttonID!=5) G->button(buttonID)->setStyleSheet("QPushButton { background-color: rgb(200,200,200); }\n");
    extern QString status;
    extern QString text_for_arrows;
    extern QString statusBook[7];
    if (buttonID==1) text_for_arrows=ent1->text();
    if (buttonID==2) text_for_arrows=ent2->text();
    status=statusBook[buttonID];
}
void MainWindow::Fcheck()
{
    try
    {
        F();
    }
    catch(...)
    {
        QMessageBox::warning(this, "Внимание","невозможно вычислить");
    }
}
void MainWindow::F()
{
    extern int len;
    extern QVector <QVector<int>> matrix;
    QVector <int> result_arr_1;
    QVector <int> result_arr_2;
    int sum=0;
    bool flag=true;
    int M=1000;
    QVector <QVector<int>> data=matrix;
    for (int i=0;i<len;i++) for (int k=0;k<len;k++) if (i==k || data[i][k]==0) data[i][k]=M;
    while (flag)
    {
        QVector <int> row(len);
        row.fill(M);
        QVector <int> column(len);
        column.fill(M);
        for (int i=0;i<len;i++) for (int k=0;k<len;k++) row[i]=qMin(row[i],data[i][k]);
        for (int i=0;i<len;i++) for (int k=0;k<len;k++) if (row[i]!=M && data[i][k]!=M) data[i][k]-=row[i];
        for (int i=0;i<len;i++) for (int k=0;k<len;k++) column[k]=qMin(column[k],data[i][k]);
        for (int i=0;i<len;i++) for (int k=0;k<len;k++) if (column[k]!=M && data[i][k]!=M) data[i][k]-=column[k];
        int max_mark_x;
        int max_mark_y;
        int max_mark=-1;
        for (int i=0;i<len;i++) for (int k=0;k<len;k++) if (data[i][k]==0)
        {
            int min1=M;
            int min2=M;
            for (int j=0;j<len;j++) if (j!=k) min1=qMin(min1,data[i][j]);
            for (int j=0;j<len;j++) if (j!=i) min2=qMin(min2,data[j][k]);
            if (min1+min2>max_mark)
            {
                max_mark=min1+min2;
                max_mark_x=i;
                max_mark_y=k;
            }
        }
        sum+=matrix[max_mark_x][max_mark_y];
        data[max_mark_y][max_mark_x]=M;
        for (int i=0;i<len;i++)
        {
            data[max_mark_x][i]=M;
            data[i][max_mark_y]=M;
        }
        result_arr_1.push_back(max_mark_x+1);
        result_arr_2.push_back(max_mark_y+1);
        flag=false;
        for (int i=0;i<len;i++) for (int k=0;k<len;k++) if (data[i][k]!=M) flag=true;
    }
    QString result = "->" + QString::number(result_arr_1[0] - 1) + "->" + QString::number(result_arr_2[0] - 1);
    int x = result_arr_2[0] - 1;
    result_arr_1.erase(result_arr_1.begin());
    result_arr_2.erase(result_arr_2.begin());
    while (result_arr_1.size() != 0) {
        for (int i = 0; i < result_arr_1.size(); i++) {
            if (result_arr_1[i] - 1 == x) {
        result += "->" + QString::number(result_arr_2[i] - 1);
        x = result_arr_2[i] - 1;
        result_arr_1.erase(result_arr_1.begin() + i);
        result_arr_2.erase(result_arr_2.begin() + i);
        break;
            }
        }
    }
    result[0]=' ';
    result[1]=' ';
    result+=" | "+QString::number(sum);
    QMessageBox::information(this, "Результат",result);
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_triggered()
{
    QMessageBox msgBox;
    QLabel *label = new QLabel(&msgBox);
    label->setText("<a href=\"https://github.com/tyloong1337\">GitHub</a>");
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->setOpenExternalLinks(true);
    msgBox.setText("Симулятор решения задачи коммивояжёра\nВыполнил Шальнев Алексей\nСтудент РИС-22-2Б");
    msgBox.setWindowTitle("О программе");
    msgBox.layout()->addWidget(label);
    msgBox.exec();
}


void MainWindow::on_action_2_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Режим добавления вершин - 'A'\nДобавление однонаправленного ребра - 'Q'\nДобавление двунаправленного ребра - 'E'\nРежим перемещения - 'M'\nРежим удаления - 'D'\nОтмена текущего режима - 'ESC'");
    msgBox.setWindowTitle("Горячие клавиши");
    msgBox.exec();
}

