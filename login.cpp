#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"

LoginWidget::LoginWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    ui->start_pass_field->setEchoMode(QLineEdit::Password);
    ui->pass_field->setEchoMode(QLineEdit::Password);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Cannot open database: " + db.lastError().text());
        qDebug() << "Database Error:" << db.lastError().text();
    }
    else
    {
        qDebug() << "Database opened successfully!";
    }
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

// Кнопка входа
void LoginWidget::on_login_btn_clicked()
{
    QString username = ui->start_login_field->text();
    QString password = ui->start_pass_field->text();

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
}

// Кнопка регистрации
void LoginWidget::on_reg_btn_clicked()
{
    QString username = ui->login_field->text();
    QString password = ui->pass_field->text();

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
        ui->stackedWidget->setCurrentIndex(1);  // Перейти на страницу входа
    } else {
        QMessageBox::warning(this, "Ошибка регистрации", "Не удалось зарегистрировать пользователя: " + query.lastError().text());
    }
}

// Кнопка назад
void LoginWidget::on_back_login_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Переход к форме регистрации
void LoginWidget::on_regist_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
