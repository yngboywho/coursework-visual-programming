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


    employeeManager = new EmployeeManager(this, ui->verticalLayoutEmployees);
    connect(ui->buttonAdd, &QPushButton::clicked, employeeManager, &EmployeeManager::addEmployee);
    connect(ui->buttonRemove, &QPushButton::clicked, employeeManager, &EmployeeManager::removeEmployee);
    connect(ui->Search, &QLineEdit::textChanged, employeeManager, &EmployeeManager::searchEmployee);
    connect(ui->buttonEdit, &QPushButton::clicked, employeeManager, &EmployeeManager::editEmployee);

    inventoryManager = new InventoryManager(this, ui->verticalLayoutEmployees_4);
    connect(ui->Search_6, &QLineEdit::textChanged, inventoryManager, &InventoryManager::searchInventory);
    connect(ui->buttonInventory, &QPushButton::clicked, this, &MainWindow::on_buttonInventory_clicked);
    connect(ui->buttonAdd_3, &QPushButton::clicked, [this]()
            { inventoryManager->addInventoryItem(); });
    connect(ui->buttonRemove_3, &QPushButton::clicked, [this]()
            {
                QString name = QInputDialog::getText(this, "Удалить элемент", "Введите наименование элемента для удаления:");
                inventoryManager->deleteInventoryItem(name); });
    inventoryManager->loadInventory();
    connect(ui->buttonEdit_3, &QPushButton::clicked, inventoryManager, &InventoryManager::editInventoryItem);


    clientManager = new ClientManager(this, ui->verticalLayoutEmployees_3);
    connect(ui->buttonAdd_2, &QPushButton::clicked, clientManager, &ClientManager::addClient);
    connect(ui->buttonRemove_2, &QPushButton::clicked, clientManager, &ClientManager::removeClient);
    connect(ui->Search_5, &QLineEdit::textChanged, clientManager, &ClientManager::searchClient);
    connect(ui->buttonEdit_2, &QPushButton::clicked, clientManager, &ClientManager::editClient);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonEmployees_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    employeeManager->loadEmployees();
    ui->Search->clear();
}

void MainWindow::on_buttonClients_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    clientManager->loadClients();
    ui->Search_5->clear();
}

void MainWindow::on_buttonInventory_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    inventoryManager->loadInventory();
    ui->Search_6->clear();
}
