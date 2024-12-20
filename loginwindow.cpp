#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    ui->passLineEdit->setEchoMode(QLineEdit::Password);
    ui->passLineEdit_2->setEchoMode(QLineEdit::Password);

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
        // Проверка на наличие таблицы Users
        QSqlQuery testQuery;
        if (!testQuery.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='Users';")) {
            qDebug() << "Error checking Users table:" << testQuery.lastError().text();
        } else if (!testQuery.next()) {
            qDebug() << "Table Users does not exist in the database!";
        } else {
            qDebug() << "Table Users exists.";
        }
    }
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

// Кнопка входа
void LoginWindow::on_loginButton_clicked()
{
    QString username = ui->loginLineEdit->text();
    QString password = ui->passLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка ввода", "Пожалуйста, заполните все поля!");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT id FROM Users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        int userId = query.value("id").toInt();

        QSettings settings("MyCompany", "Fitness");
        settings.setValue("user_id", userId);

        qDebug() << userId;

        QMessageBox::information(this, "Успех", "Вход в систему успешный!");

        // Открываем главное окно
        MainWindow *main = new MainWindow();
        main->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Ошибка входа", "Неверное имя пользователя или пароль.");
    }
    // Проверка содержимого таблицы (диагностика)
    QSqlQuery testQuery("SELECT * FROM Users");
    if (!testQuery.exec()) {
        qDebug() << "Error fetching Users table data:" << testQuery.lastError().text();
    } else {
        while (testQuery.next()) {
            qDebug() << "ID:" << testQuery.value("id").toInt()
            << "Username:" << testQuery.value("username").toString()
            << "Password:" << testQuery.value("password").toString();
        }
    }
}

// Кнопка регистрации
void LoginWindow::on_regButton_clicked()
{
    QString username = ui->loginLineEdit_2->text();
    QString password = ui->passLineEdit_2->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка ввода", "Пожалуйста, заполните все поля!");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        QMessageBox::information(this, "Успех", "Регистрация прошла успешно!");
        ui->stackedWidget->setCurrentIndex(0);  // Перейти на страницу входа
    } else {
        QMessageBox::warning(this, "Ошибка регистрации", "Не удалось зарегистрировать пользователя: " + query.lastError().text());
    }
}

// Кнопка назад
void LoginWindow::on_back_login_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// Переход к форме регистрации
void LoginWindow::on_signupButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

