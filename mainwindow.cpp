#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractItemView>
#include <QDateTime>
#include <QKeyEvent>
#include <QSplashScreen>
#include<QSqlError>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //init ui
    pb_add=ui->pb_add;
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

    le_cpmc->setPlaceholderText("- ?");
    le_cpgg->setPlaceholderText("-");
    le_bzjg->setPlaceholderText("-");
    le_gys->setPlaceholderText("=");
    le_lxfs->setPlaceholderText("=");
    le_bz->setPlaceholderText("=");




    pb_add->setEnabled(false);


//    signal slot
    connect(this->pb_add,SIGNAL(clicked()),this,SLOT(method_add()));
    connect(this->pb_query,SIGNAL(clicked()),this,SLOT(method_query()));
    connect(this->le_content,SIGNAL(returnPressed()),this,SLOT(method_contentReturn()));

    le_content->setPlaceholderText("产品名称 / 产品规格 / upd 1 / del 1 ");


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


    this->setWindowTitle("PMS 0.0.1 made by  mengtianwxs");


    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
//    ui->tableView->setSelectionModel(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);

    //hotkey setting
    this->setFocusPolicy(Qt::StrongFocus);

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

    ui->tableView->setColumnWidth(0,50);
    ui->tableView->setColumnWidth(1,100);
    ui->tableView->setColumnWidth(2,240);
    ui->tableView->setColumnWidth(5,50);
    ui->tableView->setColumnWidth(6,50);
    ui->tableView->setColumnWidth(9,300);


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

    if(s_cpmc==""){
        //query last data
        QString sql="select * from tb_main order by id desc limit 1";
        QSqlQuery sq;
        sq.exec(sql);
        QString scpmc="";
        while (sq.next()) {

            scpmc=sq.value("cpmc").toString();

        }

       s_cpmc=scpmc;

    }

    if(s_cpgg=="" && s_bzjg=="")
    {
        ui->statusBar->showMessage("- items cannot null value");
    }
    else
    {

        bool q=dbquery.exec("insert into tb_main (cpmc,cpgg,lrrq,bzjg,dw,sl,gys,lxfs,bz) values ('"+s_cpmc+"','"+s_cpgg+"','"+s_lrrq+"','"+s_bzjg+"','"+s_dw+"','"+s_sl+"','"+s_gys+"','"+s_lxfs+"','"+s_bz+"');");
        if(q){
            ui->statusBar->showMessage("success insert into data ");
        }else{
            ui->statusBar->showMessage("faild to insert into "+dbquery.lastError().text());
        }

    }



    updateTableView();

}



void MainWindow::method_alert()
{

}

void MainWindow::method_query()
{

//简单查询
    QString txt=le_content->text();


    QString sql="select * from tb_main where cpmc like '%"+txt+"%';";
    model=new QSqlQueryModel(this);





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


    QString s_cpmc=le_cpmc->text();
    QString s_cpgg=le_cpgg->text();
    QString s_lrrq=le_lrrq->text();
    QString s_bzjg=le_bzjg->text();
    QString s_dw=cbb_dw->currentText();
    QString s_sl=sb_sl->text();
    QString s_gys=le_gys->text();
    QString s_lxfs=le_lxfs->text();
    QString s_bz=le_bz->text();


//   删除 修改
    if(txt.split(" ").length()>=2){
        QString txt0=txt.split(" ")[0];
        QString txt1=txt.split(" ")[1];

        if(txt0=="del"){
            sql="delete from tb_main where id ="+txt1;
            model->setQuery(sql);
            updateTableView();
            ui->statusBar->showMessage("delete data success");

        }

        if(txt0=="del" && txt1=="all"){
            sql="delete from tb_main;";
            model->setQuery(sql);
            sql="update sqlite_sequence SET seq = 0 where name ='tb_main'; ";
            model->setQuery(sql);
            updateTableView();
            ui->statusBar->showMessage("delete ALL data success");

        }

        if(txt0=="upd"){
            if(s_cpmc!=""){
                sql="update tb_main set cpmc='"+s_cpmc+"' where id ="+txt1;
                model->setQuery(sql);
                updateTableView();
            }
            if(s_cpgg!=""){
                sql="update tb_main set cpgg='"+s_cpgg+"' where id ="+txt1;
                model->setQuery(sql);
                updateTableView();
            }
//            if(s_lrrq!=""){
//                sql="update tb_main set lrrq='"+s_lrrq+"' where id ="+txt1;
//                model->setQuery(sql);
//                updateTableView();
//            }
            if(s_bzjg!=""){
                sql="update tb_main set bzjg='"+s_bzjg+"' where id ="+txt1;
                model->setQuery(sql);
                updateTableView();
            }
            if(s_dw!=""){
                sql="update tb_main set dw='"+s_dw+"' where id ="+txt1;
                model->setQuery(sql);
                updateTableView();
            }
            if(s_sl!=""){
                sql="update tb_main set sl='"+s_sl+"' where id ="+txt1;
                model->setQuery(sql);
                updateTableView();
            }
            if(s_gys!=""){
                sql="update tb_main set gys='"+s_gys+"' where id ="+txt1;
                model->setQuery(sql);
                updateTableView();
            }
            if(s_lxfs!=""){
                sql="update tb_main set lxfs='"+s_lxfs+"' where id ="+txt1;
                model->setQuery(sql);
                updateTableView();
            }
            if(s_bz!=""){
                sql="update tb_main set bz='"+s_bz+"' where id ="+txt1;
                model->setQuery(sql);
                updateTableView();
            }
        }


        if(txt0=="add"){

            QString add_sql="";
            int txt_len=txt1.split(",").length();



            if(txt_len==4){
                QString txt_cpmc=txt1.split(",")[0];
                QString txt_cpgg=txt1.split(",")[1];
                QString txt_bzjg=txt1.split(",")[2];
                QString txt_gys=txt1.split(",")[3];


                add_sql="insert into tb_main (cpmc,cpgg,lrrq,bzjg,dw,sl,gys,lxfs,bz) values ('"+txt_cpmc+"','"+txt_cpgg+"','"+le_date->text()+"','"+txt_bzjg+
                        "','台','1','"+txt_gys+"','-','-')";
                model->setQuery(add_sql);
                updateTableView();
            }


            if(txt_len==9){
                QString txt_cpmc=txt1.split(",")[0];
                QString txt_cpgg=txt1.split(",")[1];
                QString txt_lrrq=txt1.split(",")[2];
                QString txt_bzjg=txt1.split(",")[3];
                QString txt_dw=txt1.split(",")[4];
                QString txt_sl=txt1.split(",")[5];
                QString txt_gys=txt1.split(",")[6];
                QString txt_lxfs=txt1.split(",")[7];
                QString txt_bz=txt1.split(",")[8];

                if(txt_lrrq==""){
                    txt_lrrq=le_date->text();
                }
                if(txt_dw==""){
                    txt_dw="台";
                }
                if(txt_sl==""){
                    txt_sl="1";
                }
                if(txt_gys==""){
                    txt_gys="-";
                }
                if(txt_lxfs==""){
                    txt_lxfs="-";
                }
                if(txt_bz==""){
                    txt_bz="-";
                }

                add_sql="insert into tb_main (cpmc,cpgg,lrrq,bzjg,dw,sl,gys,lxfs,bz) values ('"+txt_cpmc+"','"+txt_cpgg+"','"+txt_lrrq+"','"+txt_bzjg+
                        "','"+txt_dw+"','"+txt_sl+"','"+txt_gys+"','"+txt_lxfs+"','"+txt_bz+"')";
                model->setQuery(add_sql);
                updateTableView();

            }



        }


    }




}

void MainWindow::method_timer()
{
     initTLdate();

}

void MainWindow::method_contentReturn()
{
    qDebug()<<"click query";

    method_query();
}


// hotkey
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if((event->key()==Qt::Key_E) && (event->modifiers()==Qt::ControlModifier)){
        le_content->clear();
    }

    if((event->key()==Qt::Key_W) && (event->modifiers()==Qt::ControlModifier)){
        le_content->setFocus();
    }

    if((event->key()==Qt::Key_S) && (event->modifiers()==Qt::ControlModifier)){
        le_cpgg->setFocus();
    }

    if((event->key()==Qt::Key_D) && (event->modifiers()==Qt::ControlModifier)){
        le_bzjg->setFocus();
    }

    if((event->key()==Qt::Key_F) && (event->modifiers()==Qt::ControlModifier)){
        le_bzjg->clear();
        le_cpmc->clear();
        le_cpgg->clear();
        le_gys->clear();
        le_lxfs->clear();
        le_bz->clear();
        initSpinBox();
        le_cpmc->setFocus();
    }

    if((event->key()==Qt::Key_Q) && (event->modifiers()==Qt::ControlModifier)){
        method_query();
    }

    if((event->key()==Qt::Key_R) && (event->modifiers()==Qt::ControlModifier)){
        method_add();
    }
}



