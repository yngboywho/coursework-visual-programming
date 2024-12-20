#ifndef EMPLOYEEMANAGE_H
#define EMPLOYEEMANAGE_H

#include <QObject>
#include <QWidget>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QMap>

class EmployeeManage : public QObject
{
    Q_OBJECT

public:
    explicit EmployeeManage(QWidget *parent = nullptr, QVBoxLayout *layout = nullptr);
    void loadEmployees();
    void addEmployee();
    void removeEmployee();
    void searchEmployee(const QString &query);
    void editEmployee();

private:
    void addEmployeeBlock(const QString &name, const QString &education, const QString &jobTitle, int salary);

    QWidget *parentWidget;
    QVBoxLayout *layout;
    QMap<QString, QWidget *> employeeWidgets;
};

#endif // EMPLOYEEMANAGE_H
