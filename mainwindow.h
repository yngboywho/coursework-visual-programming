#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QMap>
#include <QLabel>
#include <QMessageBox>
#include <QSqlError>
#include "employeemanage.h"
#include "clientmanage.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonEmployees_clicked();
    void on_buttonClients_clicked();
    void on_buttonInventory_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QString, QWidget*> employeeWidgets;
    EmployeeManage *employeeManager;
    InventoryManager *inventoryManager;
    ProductManager *productManager;
    ClientManage *clientManager;

};
#endif // MAINWINDOW_H
