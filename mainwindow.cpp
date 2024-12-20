#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../../database.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Cannot open database: " + db.lastError().text());
        qDebug() << "Database Error:" << db.lastError().text();
    }
    else
    {
        qDebug() << "Database opened successfully!";
    }

    connect(ui->buttonEmployees, &QPushButton::clicked, this, &MainWindow::on_buttonEmployees_clicked);


    employeeManage = new EmployeeManage(this, ui->verticalLayoutEmployees);
    connect(ui->buttonAdd, &QPushButton::clicked, employeeManage, &EmployeeManage::addEmployee);
    connect(ui->buttonRemove, &QPushButton::clicked, employeeManage, &EmployeeManage::removeEmployee);
    connect(ui->Search, &QLineEdit::textChanged, employeeManage, &EmployeeManage::searchEmployee);
    connect(ui->buttonEdit, &QPushButton::clicked, employeeManage, &EmployeeManage::editEmployee);

    inventoryScreen = new InventoryScreen(this, ui->verticalLayoutEmployees_4);
    connect(ui->Search_6, &QLineEdit::textChanged, inventoryScreen, &InventoryScreen::searchInventory);
    connect(ui->buttonInventory, &QPushButton::clicked, this, &MainWindow::on_buttonInventory_clicked);
    connect(ui->buttonAdd_3, &QPushButton::clicked, [this]()
            { inventoryScreen->addInventoryItem(); });
    connect(ui->buttonRemove_3, &QPushButton::clicked, [this]()
            {
                QString name = QInputDialog::getText(this, "Удалить элемент", "Введите наименование элемента для удаления:");
                inventoryScreen->deleteInventoryItem(name); });
    inventoryScreen->loadInventory();
    connect(ui->buttonEdit_3, &QPushButton::clicked, inventoryScreen, &InventoryScreen::editInventoryItem);


    clientManage = new ClientManage(this, ui->verticalLayoutEmployees_3);
    connect(ui->buttonAdd_2, &QPushButton::clicked, clientManage, &ClientManage::addClient);
    connect(ui->buttonRemove_2, &QPushButton::clicked, clientManage, &ClientManage::removeClient);
    connect(ui->Search_5, &QLineEdit::textChanged, clientManage, &ClientManage::searchClient);
    connect(ui->buttonEdit_2, &QPushButton::clicked, clientManage, &ClientManage::editClient);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonEmployees_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    employeeManage->loadEmployees();
    ui->Search->clear();
}

void MainWindow::on_buttonClients_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    clientManage->loadClients();
    ui->Search_5->clear();
}

void MainWindow::on_buttonInventory_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    inventoryScreen->loadInventory();
    ui->Search_6->clear();
}
