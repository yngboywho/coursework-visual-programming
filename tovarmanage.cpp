#include "tovarmanage.h"
#include <QHBoxLayout>

TovarManage::TovarManage(QWidget *parent, QVBoxLayout *layout)
    : QObject(parent), parentWidget(parent), layout(layout)
{}

void TovarManage::loadProduct()
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }
    productWidgets.clear();

    QSqlQuery query("SELECT id, name, count_training, count_warehouse FROM Products");
    while (query.next())
    {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        int countTraining = query.value(2).toInt();
        int countWarehouse = query.value(3).toInt();

        QFrame *nameBlock = new QFrame;
        QHBoxLayout *nameLayout = new QHBoxLayout(nameBlock);
        QLabel *nameLabel = new QLabel(name);
        nameLabel->setObjectName("nameLabel");
        nameLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #495057; padding: 10px; background-color: #fff; border-radius: 5px; border: 1px solid #ced4da;");
        nameLayout->addWidget(nameLabel);
        nameBlock->setFrameShape(QFrame::StyledPanel);
        nameBlock->setFixedHeight(80);
        nameBlock->setStyleSheet("background-color: #fff; border: 1px solid #ccc; border-radius: 5px; padding: 5px; color: black;");

        QFrame *trainingBlock = new QFrame;
        QHBoxLayout *trainingLayout = new QHBoxLayout(trainingBlock);
        QPushButton *addTrainingButton = new QPushButton("+");
        QLabel *countTrainingLabel = new QLabel(QString::number(countTraining));
        QPushButton *removeTrainingButton = new QPushButton("-");

        countTrainingLabel->setAlignment(Qt::AlignCenter);
        addTrainingButton->setFixedSize(50, 50);
        addTrainingButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #ffffff;"
            "   border: 1px solid #cccccc;"
            "   border-radius: 5px;"
            "} "
            "QPushButton:hover {"
            "   background-color: #e6f7ff;"
            "   border-color: #80d4ff;"
            "} ");
        countTrainingLabel->setStyleSheet("font-size: 12px; color: #000000; padding: 10px; background-color: #fff; border-radius: 5px; border: 1px solid #ced4da;");
        removeTrainingButton->setFixedSize(50, 50);
        removeTrainingButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #ffffff;"
            "   border: 1px solid #cccccc;"
            "   border-radius: 5px;"
            "} "
            "QPushButton:hover {"
            "   background-color: #e6f7ff;"
            "   border-color: #80d4ff;"
            "} ");
        trainingLayout->addWidget(removeTrainingButton);
        trainingLayout->addWidget(countTrainingLabel);
        trainingLayout->addWidget(addTrainingButton);
        trainingBlock->setFrameShape(QFrame::StyledPanel);
        trainingBlock->setFixedHeight(80);
        trainingBlock->setStyleSheet("background-color: #fff; border: 1px solid #ccc; border-radius: 5px; padding: 5px; color: black;");

        QFrame *warehouseBlock = new QFrame;
        QHBoxLayout *warehouseLayout = new QHBoxLayout(warehouseBlock);
        QPushButton *addWarehouseButton = new QPushButton("+");
        QLabel *countWarehouseLabel = new QLabel(QString::number(countWarehouse));
        QPushButton *removeWarehouseButton = new QPushButton("-");

        countWarehouseLabel->setStyleSheet("font-size: 12px; color: #000000; padding: 10px; background-color: #fff; border-radius: 5px; border: 1px solid #ced4da;");
        countWarehouseLabel->setAlignment(Qt::AlignCenter);
        addWarehouseButton->setFixedSize(50, 50);
        addWarehouseButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #ffffff;"
            "   border: 1px solid #cccccc;"
            "   border-radius: 5px;"
            "} "
            "QPushButton:hover {"
            "   background-color: #e6f7ff;"
            "   border-color: #80d4ff;"
            "} ");
        removeWarehouseButton->setFixedSize(50, 50);
        removeWarehouseButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #ffffff;"
            "   border: 1px solid #cccccc;"
            "   border-radius: 5px;"
            "} "
            "QPushButton:hover {"
            "   background-color: #e6f7ff;"
            "   border-color: #80d4ff;"
            "} ");
        warehouseLayout->addWidget(removeWarehouseButton);
        warehouseLayout->addWidget(countWarehouseLabel);
        warehouseLayout->addWidget(addWarehouseButton);
        warehouseBlock->setFrameShape(QFrame::StyledPanel);
        warehouseBlock->setFixedHeight(80);
        warehouseBlock->setStyleSheet("background-color: #fff; border: 1px solid #ccc; border-radius: 5px; padding: 5px; color: black;");

        connect(addTrainingButton, &QPushButton::clicked, [=]()
                { updateCount(id, "count_training", 1, countTrainingLabel); });
        connect(removeTrainingButton, &QPushButton::clicked, [=]()
                { updateCount(id, "count_training", -1, countTrainingLabel); });

        connect(addWarehouseButton, &QPushButton::clicked, [=]()
                { updateCount(id, "count_warehouse", 1, countWarehouseLabel); });
        connect(removeWarehouseButton, &QPushButton::clicked, [=]()
                { updateCount(id, "count_warehouse", -1, countWarehouseLabel); });

        QHBoxLayout *itemLayout = new QHBoxLayout;
        itemLayout->addWidget(nameBlock);
        itemLayout->addWidget(trainingBlock);
        itemLayout->addWidget(warehouseBlock);

        QWidget *itemWidget = new QWidget;
        itemWidget->setLayout(itemLayout);
        layout->addWidget(itemWidget);

        itemWidget->setFixedHeight(90);
        productWidgets.append(itemWidget);
    }
}

void TovarManage::addProductItem()
{
    QString name = QInputDialog::getText(parentWidget, "Добавить элемент", "Введите название:");
    if (name.isEmpty())
        return;

    QSqlQuery query;
    query.prepare("INSERT INTO Products (name, count_training, count_warehouse) VALUES (:name, 0, 0)");
    query.bindValue(":name", name);

    if (!query.exec())
    {
        QMessageBox::critical(parentWidget, "Ошибка", "Не удалось добавить элемент: " + query.lastError().text());
        return;
    }
    loadProduct();
}

void TovarManage::deleteProductItem(const QString &name)
{
    if (name.isEmpty())
    {
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM Products WHERE name = :name");
    checkQuery.bindValue(":name", name);

    if (!checkQuery.exec())
    {
        QMessageBox::critical(parentWidget, "Ошибка", "Не удалось выполнить проверку наличия элемента: " + checkQuery.lastError().text());
        return;
    }

    checkQuery.next();
    int count = checkQuery.value(0).toInt();
    if (count == 0)
    {
        QMessageBox::warning(parentWidget, "Предупреждение", "Элемент с таким наименованием не найден в базе данных.");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM Products WHERE name = :name");
    query.bindValue(":name", name);
    if (!query.exec())
    {
        QMessageBox::critical(parentWidget, "Ошибка", "Не удалось удалить элемент: " + query.lastError().text());
        return;
    }

    loadProduct();
}

void TovarManage::updateCount(int id, const QString &column, int change, QLabel *label)
{
    int currentValue = label->text().toInt();
    int newValue = currentValue + change;
    if (newValue < 0)
    {
        QMessageBox::warning(parentWidget, "Ошибка", "Количество не может быть отрицательным!");
        return;
    }

    QSqlQuery query;
    query.prepare(QString("UPDATE Products SET %1 = :value WHERE id = :id").arg(column));
    query.bindValue(":value", newValue);
    query.bindValue(":id", id);

    if (!query.exec())
    {
        QMessageBox::critical(parentWidget, "Ошибка", "Не удалось обновить данные: " + query.lastError().text());
        return;
    }

    label->setText(QString::number(newValue));
}

void TovarManage::searchProduct(const QString &query)
{
    QString lowerQuery = query.trimmed().toLower();
    for (QWidget *itemWidget : productWidgets)
    {
        QLabel *nameLabel = itemWidget->findChild<QLabel *>();
        if (nameLabel)
        {
            QString itemName = nameLabel->text().toLower();
            if (itemName.contains(lowerQuery))
            {
                itemWidget->setVisible(true);
            }
            else
            {
                itemWidget->setVisible(false);
            }
        }
    }
}

void TovarManage::editProductItem()
{
    bool ok;
    QString originalName = QInputDialog::getText(parentWidget,
                                                 tr("Редактировать продукт"),
                                                 tr("Введите название продукта для редактирования:"),
                                                 QLineEdit::Normal, "", &ok);

    if (!ok || originalName.isEmpty())
    {
        QMessageBox::warning(parentWidget, tr("Отмена"), tr("Редактирование отменено или введено пустое значение."));
        return;
    }

    QSqlQuery queryFind;
    queryFind.prepare("SELECT id, name, count_training, count_warehouse FROM Products WHERE name = ?");
    queryFind.addBindValue(originalName);

    if (!queryFind.exec() || !queryFind.next())
    {
        QMessageBox::warning(parentWidget, tr("Не найдено"),
                             tr("Продукт с таким названием не найден в базе данных."));
        return;
    }

    int currentId = queryFind.value(0).toInt();
    QString currentName = queryFind.value(1).toString();
    int currentCountTraining = queryFind.value(2).toInt();
    int currentCountWarehouse = queryFind.value(3).toInt();

    QString newName = QInputDialog::getText(parentWidget, tr("Редактировать продукт"),
                                            tr("Введите новое название:"), QLineEdit::Normal, currentName, &ok);
    if (!ok)
        return;

    int newCountTraining = QInputDialog::getInt(parentWidget, tr("Редактировать продукт"),
                                                tr("Введите количество для тренировки:"),
                                                currentCountTraining, 0, 1000000, 1, &ok);
    if (!ok)
        return;

    int newCountWarehouse = QInputDialog::getInt(parentWidget, tr("Редактировать продукт"),
                                                 tr("Введите количество на складе:"),
                                                 currentCountWarehouse, 0, 1000000, 1, &ok);
    if (!ok)
        return;

    QSqlQuery queryUpdate;
    queryUpdate.prepare("UPDATE Products SET name = ?, count_training = ?, count_warehouse = ? WHERE id = ?");
    queryUpdate.addBindValue(newName);
    queryUpdate.addBindValue(newCountTraining);
    queryUpdate.addBindValue(newCountWarehouse);
    queryUpdate.addBindValue(currentId);

    if (queryUpdate.exec())
    {
        loadProduct();
        QMessageBox::information(parentWidget, tr("Успешно"), tr("Данные продукта успешно обновлены!"));
    }
    else
    {
        QMessageBox::critical(parentWidget, tr("Ошибка"),
                              tr("Не удалось обновить данные продукта: %1").arg(queryUpdate.lastError().text()));
    }
}

