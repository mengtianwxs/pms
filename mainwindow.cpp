#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractItemView>
#include <QDateTime>
#include <QSplashScreen>
#include<QSqlError>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pb_add=ui->pb_add;
    pb_test=ui->pb_test;
    pb_alert=ui->pb_alert;
    pb_query=ui->pb_query;

    le_cpgg=ui->le_cpgg;
    le_cpmc=ui->le_cpmc;
    le_lrrq=ui->le_date;
    le_bzjg=ui->le_bzjg;
    cbb_dw=ui->cbb_dw;
    sb_sl=ui->sb_sl;
    le_gys=ui->le_gys;
    le_lxfs=ui->le_lxfs;
    le_bz=ui->le_bz;
    le_content=ui->le_content;

    le_cpmc->setPlaceholderText("-");
    le_cpgg->setPlaceholderText("-");
    le_bzjg->setPlaceholderText("- ￥");
    le_gys->setPlaceholderText("=");
    le_lxfs->setPlaceholderText("=");
    le_bz->setPlaceholderText("=");




    pb_add->setEnabled(false);


    connect(this->pb_test,SIGNAL(clicked()),this,SLOT(method_test()));
    connect(this->pb_add,SIGNAL(clicked()),this,SLOT(method_add()));
    connect(this->pb_query,SIGNAL(clicked()),this,SLOT(method_query()));


    initComboBox();
    initSpinBox();


    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dbmt.db");
    if(!db.open())
    {
        ui->statusBar->showMessage("Failed to connect database "+db.lastError().text());
    }else{
        ui->statusBar->showMessage("success to connect database");
        pb_add->setEnabled(true);
    }

    QSqlQuery sql_query;
    sql_query.exec("create table if not exists tb_main (id integer primary key autoincrement, cpmc text, cpgg text,lrrq text,bzjg text,dw text,sl text,gys text,lxfs text,bz text);");
    dbquery=sql_query;


    updateTableView();

    timer=new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    connect(timer,SIGNAL(timeout()),this,SLOT(method_timer()));
    le_date=ui->le_date;
    le_date->setEnabled(false);


    this->setWindowTitle("PMS 0.0.1");


    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
//    ui->tableView->setSelectionModel(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);

    ui->statusBar->showMessage("'-' means must write data , '=' means need write data by yourself ...");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initComboBox()
{
    ui->cbb_dw->clear();
    QStringList strlist;
    strlist<<"台"<<"个"<<"套"<<"只"<<"米"<<"把"<<"袋"<<"盘"<<"包";
    ui->cbb_dw->addItems(strlist);
}

void MainWindow::initSpinBox()
{
    ui->sb_sl->clear();
    ui->sb_sl->setValue(1);
}

void MainWindow::initTLdate()
{
    QDateTime curdatetime=QDateTime::currentDateTime();
    ui->le_date->setText(curdatetime.toString("yyyy-MM-dd hh:mm"));


}

void MainWindow::updateTableView()
{





    tableModel=new QSqlTableModel(this,db);
    tableModel->setTable("tb_main");

//    tableModel->setSort(tableModel->fieldIndex("id"),Qt::AscendingOrder);
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModel->sort(0,Qt::DescendingOrder);

    tableModel->select();
//    cpmc text, cpgg text,lrrq text,bzjg text,dw text,sl text,gys text,lxfs text,bz text);");
    tableModel->setHeaderData(tableModel->fieldIndex("id"),Qt::Horizontal,"序号");
    tableModel->setHeaderData(tableModel->fieldIndex("cpmc"),Qt::Horizontal,"产品名称");
    tableModel->setHeaderData(tableModel->fieldIndex("cpgg"),Qt::Horizontal,"产品规格");
    tableModel->setHeaderData(tableModel->fieldIndex("lrrq"),Qt::Horizontal,"录入日期");
    tableModel->setHeaderData(tableModel->fieldIndex("bzjg"),Qt::Horizontal,"标准价格");
    tableModel->setHeaderData(tableModel->fieldIndex("dw"),Qt::Horizontal,"单位");
    tableModel->setHeaderData(tableModel->fieldIndex("sl"),Qt::Horizontal,"数量");
    tableModel->setHeaderData(tableModel->fieldIndex("gys"),Qt::Horizontal,"供应商");
    tableModel->setHeaderData(tableModel->fieldIndex("lxfs"),Qt::Horizontal,"联系方式");
    tableModel->setHeaderData(tableModel->fieldIndex("bz"),Qt::Horizontal,"备注");



    selectModel=new QItemSelectionModel(tableModel);
    ui->tableView->setModel(tableModel);
    ui->tableView->setSelectionModel(selectModel);
    ui->tableView->setColumnHidden(tableModel->fieldIndex("rowid"),true);
//    ui->tableView->sortByColumn(1,Qt::DescendingOrder);


}



void MainWindow::method_add()
{


    QString s_cpmc=le_cpmc->text();
    QString s_cpgg=le_cpgg->text();
    QString s_lrrq=le_lrrq->text();
    QString s_bzjg=le_bzjg->text();
    QString s_dw=cbb_dw->currentText();
    QString s_sl=sb_sl->text();
    QString s_gys=le_gys->text();
    QString s_lxfs=le_lxfs->text();
    QString s_bz=le_bz->text();


    if(s_gys==""){
        le_gys->setText("-");
    }
    if(s_lxfs==""){
        le_lxfs->setText("-");
    }
    if(s_bz==""){
        le_bz->setText("-");
    }

    if(s_cpmc=="" || s_cpgg=="" || s_bzjg==""){
        ui->statusBar->showMessage("- items cannot null value");
    }else{

        bool q=dbquery.exec("insert into tb_main (cpmc,cpgg,lrrq,bzjg,dw,sl,gys,lxfs,bz) values ('"+s_cpmc+"','"+s_cpgg+"','"+s_lrrq+"','"+s_bzjg+"','"+s_dw+"','"+s_sl+"','"+s_gys+"','"+s_lxfs+"','"+s_bz+"');");
        if(q){
            ui->statusBar->showMessage("success insert into data ");
        }else{
            ui->statusBar->showMessage("faild to insert into "+dbquery.lastError().text());
        }

    }



    updateTableView();

}

void MainWindow::method_test()
{
    qDebug("this is test");
    updateTableView();
}

void MainWindow::method_alert()
{

}

void MainWindow::method_query()
{


    QString txt=le_content->text();
    QString sql="select * from tb_main where cpmc like '%"+txt+"%';";
    qDebug()<<"success query"<< sql;

    QSqlQueryModel *model=new QSqlQueryModel(this);
    model->setQuery(sql);
    model->setHeaderData(0,Qt::Horizontal,"序号");
    model->setHeaderData(1,Qt::Horizontal,"产品名称");
    model->setHeaderData(2,Qt::Horizontal,"产品规格");
    model->setHeaderData(3,Qt::Horizontal,"录入日期");

    model->setHeaderData(4,Qt::Horizontal,"标准价格");

    model->setHeaderData(5,Qt::Horizontal,"单位");
    model->setHeaderData(6,Qt::Horizontal,"数量");
    model->setHeaderData(7,Qt::Horizontal,"供应商");

    model->setHeaderData(8,Qt::Horizontal,"联系方式");
    model->setHeaderData(9,Qt::Horizontal,"备注");


    sModel=new QItemSelectionModel(model);

    ui->tableView->setModel(model);
    ui->tableView->setSelectionModel(sModel);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);


}

void MainWindow::method_timer()
{
     initTLdate();

}



