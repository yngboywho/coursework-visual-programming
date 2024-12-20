#ifndef LOGINW_H
#define LOGIN_H

#include <QMainWindow>
#include <QSettings>
#include <QDebug>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

private slots:
    void on_reg_btn_clicked();
    void on_back_login_clicked();
    void on_regist_btn_clicked();
    void on_login_btn_clicked();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGIN_H
