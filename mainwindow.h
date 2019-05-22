#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include<QPushButton>
#include<QtSql/QSql>
#include <QTimer>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QItemSelectionModel>
#include <QLineEdit>
#include <QSqlQuery>
#include <QComboBox>
#include <QSpinBox>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QPushButton* pb_add;
    QPushButton* pb_query;
    QPushButton* pb_loadcsv;

    QTimer* timer;
    QLineEdit *le_date;

    QSqlDatabase db;
    QSqlQuery dbquery;

    QSqlTableModel *tableModel;
    QItemSelectionModel *selectModel;
    QItemSelectionModel* sModel;
    QSqlQueryModel* model;

    QLineEdit *le_cpmc;
    QLineEdit *le_cpgg;
    QLineEdit* le_lrrq;
    QLineEdit* le_bzjg;
    QComboBox *cbb_dw;
    QSpinBox* sb_sl;
    QLineEdit* le_gys;
    QLineEdit* le_lxfs;
    QLineEdit* le_bz;
    QLineEdit* le_content;






private:
     void initComboBox();
     void initSpinBox();
     void initTLdate();
     void updateTableView();



private slots:
    void method_add();
    void method_alert();
    void method_query();
    void method_timer();
    void method_contentReturn();
    void method_loadcsv();

    void keyPressEvent(QKeyEvent* event);

};

#endif // MAINWINDOW_H
