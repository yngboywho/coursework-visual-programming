#include "employeemanage.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QSqlError>

EmployeeManage::EmployeeManage(QWidget *parent, QVBoxLayout *layout)
    : QObject(parent), parentWidget(parent), layout(layout)
{
}

void EmployeeManage::loadEmployees()
{
    qDeleteAll(employeeWidgets);
    employeeWidgets.clear();

    QSqlQuery query("SELECT name, education, job_title, salary FROM Employees");
    while (query.next())
    {
        QString name = query.value(0).toString();
        QString education = query.value(1).toString();
        QString jobTitle = query.value(2).toString();
        int salary = query.value(3).toInt();
        addEmployeeBlock(name, education, jobTitle, salary);
    }
}

void EmployeeManage::addEmployee()
{
    QString name = QInputDialog::getText(parentWidget, "Добавить сотрудника", "Введите ФИО:");
    if (name.isEmpty())
        return;

    QString education = QInputDialog::getText(parentWidget, "Добавить сотрудника", "Введите образование:");

    QString jobTitle = QInputDialog::getText(parentWidget, "Добавить сотрудника", "Введите должность:");
    if (jobTitle.isEmpty())
        return;

    bool ok;
    int salary = QInputDialog::getInt(parentWidget, "Добавить сотрудника", "Введите оклад:", 0, 0, 1000000, 1, &ok);
    if (!ok)
        return;

    QSqlQuery query;
    query.prepare("INSERT INTO Employees (name, education, job_title, salary) VALUES (?, ?, ?, ?)");
    query.addBindValue(name);
    query.addBindValue(education);
    query.addBindValue(jobTitle);
    query.addBindValue(salary);

    if (query.exec())
    {
        QMessageBox::information(parentWidget, "Успешно", "Сотрудник успешно добавлен!");
        addEmployeeBlock(name, education, jobTitle, salary);
    }
    else
    {
        QMessageBox::critical(parentWidget, tr("Ошибка"),
                              tr("Не удалось добавить сотрудника: %1").arg(query.lastError().text()));
    }
}

void EmployeeManage::removeEmployee()
{
    bool ok;
    QString name = QInputDialog::getText(parentWidget,
                                         tr("Удалить сотрудника"),
                                         tr("Введите имя сотрудника для удаления:"),
                                         QLineEdit::Normal, "", &ok);

    if (!ok || name.isEmpty())
    {
        QMessageBox::warning(parentWidget, tr("Отмена"), tr("Удаление отменено или введено пустое значение."));
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM Employees WHERE name = ?");
    query.addBindValue(name);

    if (query.exec())
    {
        if (query.numRowsAffected() > 0)
        {
            QMessageBox::information(parentWidget, tr("Успех"), tr("Сотрудник успешно удалён."));
            if (employeeWidgets.contains(name))
            {
                QWidget *widget = employeeWidgets.value(name, nullptr);
                if (widget)
                {
                    layout->removeWidget(widget);
                    delete widget;
                }
                employeeWidgets.remove(name);
            }
        }
        else
        {
            QMessageBox::warning(parentWidget, tr("Не найдено"),
                                 tr("Сотрудник с таким именем не найден в базе данных."));
        }
    }
    else
    {
        QMessageBox::critical(parentWidget, tr("Ошибка"),
                              tr("Не удалось удалить сотрудника: %1").arg(query.lastError().text()));
    }
}

void EmployeeManage::searchEmployee(const QString &query)
{
    QString lowerQuery = query.trimmed().toLower();

    for (const QString &name : employeeWidgets.keys())
    {
        QWidget *widget = employeeWidgets[name];
        if (name.toLower().contains(lowerQuery))
        {
            widget->setVisible(true);
        }
        else
        {
            widget->setVisible(false);
        }
    }
}

void EmployeeManage::addEmployeeBlock(const QString &name, const QString &education, const QString &jobTitle, int salary)
{
    if (employeeWidgets.contains(name))
    {
        return;
    }

    QWidget *rowWidget = new QWidget();
    rowWidget->setStyleSheet(
        "QWidget {"
        "   background-color: transparent;"
        "}");

    QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    rowLayout->setSpacing(20);

    auto createBlock = [](const QString &text)
    {
        QWidget *blockWidget = new QWidget();
        blockWidget->setStyleSheet(
            "QWidget {"
            "   background-color: #ffffff;"
            "   border: 1px solid #e0e0e0;"
            "   border-radius: 5px;"
            "   padding: 20px;"
            "}"
            "QLabel {"
            "   color: #333333;"
            "   font-size: 14px;"
            "   font-weight: bold;"
            "   text-align: center;"
            "}");

        QVBoxLayout *blockLayout = new QVBoxLayout(blockWidget);
        blockLayout->setContentsMargins(20, 10, 20, 10);

        QLabel *label = new QLabel(text, blockWidget);
        label->setAlignment(Qt::AlignCenter);
        blockLayout->addWidget(label);

        return blockWidget;
    };

    QWidget *nameBlock = createBlock(name);
    QWidget *educationBlock = createBlock(education);
    QWidget *jobTitleBlock = createBlock(jobTitle);
    QWidget *salaryBlock = createBlock(QString::number(salary) + " ₽");

    rowLayout->addWidget(nameBlock);
    rowLayout->addWidget(educationBlock);
    rowLayout->addWidget(jobTitleBlock);
    rowLayout->addWidget(salaryBlock);

    this->layout->addWidget(rowWidget);
    employeeWidgets[name] = rowWidget;
}

void EmployeeManage::editEmployee()
{
    bool ok;
    QString originalName = QInputDialog::getText(parentWidget,
                                                 tr("Редактировать сотрудника"),
                                                 tr("Введите имя сотрудника для редактирования:"),
                                                 QLineEdit::Normal, "", &ok);

    if (!ok || originalName.isEmpty())
    {
        QMessageBox::warning(parentWidget, tr("Отмена"), tr("Редактирование отменено или введено пустое значение."));
        return;
    }

    QSqlQuery queryFind;
    queryFind.prepare("SELECT id, name, education, job_title, salary FROM Employees WHERE name = ?");
    queryFind.addBindValue(originalName);

    if (!queryFind.exec() || !queryFind.next())
    {
        QMessageBox::warning(parentWidget, tr("Не найдено"),
                             tr("Сотрудник с таким именем не найден в базе данных."));
        return;
    }

    int currentId = queryFind.value(0).toInt();
    QString currentName = queryFind.value(1).toString();
    QString currentEducation = queryFind.value(2).toString();
    QString currentJobTitle = queryFind.value(3).toString();
    int currentSalary = queryFind.value(4).toInt();

    QString newName = QInputDialog::getText(parentWidget, tr("Редактировать сотрудника"),
                                            tr("Введите новое имя:"), QLineEdit::Normal, currentName, &ok);
    if (!ok)
        return;

    QString newEducation = QInputDialog::getText(parentWidget, tr("Редактировать сотрудника"),
                                                 tr("Введите новое образование:"), QLineEdit::Normal, currentEducation, &ok);
    if (!ok)
        return;

    QString newJobTitle = QInputDialog::getText(parentWidget, tr("Редактировать сотрудника"),
                                                tr("Введите новую должность:"), QLineEdit::Normal, currentJobTitle, &ok);
    if (!ok)
        return;

    int newSalary = QInputDialog::getInt(parentWidget, tr("Редактировать сотрудника"),
                                         tr("Введите новый оклад:"), currentSalary, 0, 1000000, 1, &ok);
    if (!ok)
        return;

    QSqlQuery queryUpdate;
    queryUpdate.prepare("UPDATE Employees SET name = ?, education = ?, job_title = ?, salary = ? WHERE id = ?");
    queryUpdate.addBindValue(newName);
    queryUpdate.addBindValue(newEducation);
    queryUpdate.addBindValue(newJobTitle);
    queryUpdate.addBindValue(newSalary);
    queryUpdate.addBindValue(currentId);

    if (queryUpdate.exec())
    {
        if (employeeWidgets.contains(originalName))
        {
            QWidget *widget = employeeWidgets[originalName];

            auto labels = widget->findChildren<QLabel *>();
            for (QLabel *label : labels)
            {
                if (label->objectName() == "nameLabel")
                    label->setText(newName);
                else if (label->objectName() == "educationLabel")
                    label->setText(newEducation);
                else if (label->objectName() == "jobTitleLabel")
                    label->setText(newJobTitle);
                else if (label->objectName() == "salaryLabel")
                    label->setText(QString::number(newSalary) + " ₽");
            }

            employeeWidgets.remove(originalName);
            employeeWidgets.insert(newName, widget);
        }

        QMessageBox::information(parentWidget, tr("Успешно"), tr("Данные сотрудника успешно обновлены!"));
        loadEmployees();
    }
    else
    {
        QMessageBox::critical(parentWidget, tr("Ошибка"),
                              tr("Не удалось обновить данные сотрудника: %1").arg(queryUpdate.lastError().text()));
    }
}
