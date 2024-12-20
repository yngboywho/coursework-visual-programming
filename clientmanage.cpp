#include "clientmanage.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlError>

ClientManage::ClientManage(QWidget *parent, QVBoxLayout *layout)
    : QObject(parent), parentWidget(parent), layout(layout) {}

void ClientManage::loadClients()
{
    qDeleteAll(clientWidgets);
    clientWidgets.clear();
    clientDataMap.clear();

    QSqlQuery query("SELECT name, visit, rate, like, dislike FROM Clients");
    while (query.next())
    {
        QString name = query.value(0).toString();
        int visits = query.value(1).toInt();
        int likes = query.value(3).toInt();
        int dislikes = query.value(4).toInt();

        addClientBlock(name, visits, 0.0, likes, dislikes);
    }
}

void ClientManage::addClient()
{
    QString name = QInputDialog::getText(parentWidget, "Добавить клиента", "Введите имя:");
    if (name.isEmpty())
        return;

    int visits = 0;
    int likes = 0;
    int dislikes = 0;
    double rate = 0.0;

    QSqlQuery query;
    query.prepare("INSERT INTO Clients (name, visit, rate, like, dislike) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(name);
    query.addBindValue(visits);
    query.addBindValue(rate);
    query.addBindValue(likes);
    query.addBindValue(dislikes);

    if (query.exec())
    {
        addClientBlock(name, visits, rate, likes, dislikes);
        QMessageBox::information(parentWidget, "Успешно", "Клиент успешно добавлен!");
    }
    else
    {
        QMessageBox::critical(parentWidget, "Ошибка", query.lastError().text());
    }
}

void ClientManage::removeClient()
{
    QString name = QInputDialog::getText(parentWidget, "Удалить клиента", "Введите имя:");
    if (name.isEmpty())
        return;

    QSqlQuery query;
    query.prepare("DELETE FROM Clients WHERE name = ?");
    query.addBindValue(name);

    if (query.exec())
    {
        if (clientWidgets.contains(name))
        {
            QWidget *widget = clientWidgets[name];
            layout->removeWidget(widget);
            delete widget;
            clientWidgets.remove(name);
        }
        QMessageBox::information(parentWidget,  "Успешно", "Клиент успешно удален!");
    }
    else
    {
        QMessageBox::critical(parentWidget, "Error", query.lastError().text());
    }
}

void ClientManage::searchClient(const QString &query)
{
    QString lowerQuery = query.trimmed().toLower();
    for (const QString &name : clientWidgets.keys())
    {
        QWidget *widget = clientWidgets[name];
        widget->setVisible(name.toLower().contains(lowerQuery));
    }
}

void ClientManage::addClientBlock(const QString &name, int visits, double rate, int likes, int dislikes)
{
    QWidget *clientWidget = new QWidget(parentWidget);
    clientWidget->setFixedHeight(80);

    QHBoxLayout *hLayout = new QHBoxLayout(clientWidget);
    hLayout->setContentsMargins(5, 5, 5, 5);
    hLayout->setSpacing(10);

    QWidget *nameBlock = new QWidget();
    QHBoxLayout *nameLayout = new QHBoxLayout(nameBlock);
    QLabel *nameLabel = new QLabel(name);
    nameLabel->setObjectName("nameLabel");
    nameLabel->setStyleSheet("font-size: 12px;");
    nameLayout->addWidget(nameLabel);
    nameBlock->setLayout(nameLayout);
    nameBlock->setStyleSheet("border: 1px solid #cccccc; background-color: #fff; padding: 5px; color: black;");

    QWidget *visitBlock = new QWidget();
    QHBoxLayout *visitLayout = new QHBoxLayout(visitBlock);

    QLabel *visitsLabel = new QLabel(QString::number(visits));
    visitsLabel->setObjectName("visitsLabel");
    visitsLabel->setStyleSheet("font-size: 12px;");
    visitsLabel->setAlignment(Qt::AlignCenter);

    QPushButton *plusButton = new QPushButton("+");
    plusButton->setFixedSize(50, 50);
    visitsLabel->setObjectName("plusButton");
    plusButton->setStyleSheet("font-size: 12px;");
    plusButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #ffffff;"
        "   border: 1px solid #cccccc;"
        "   border-radius: 5px;"
        "   color: black;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e6f7ff;"
        "   border-color: #80d4ff;"
        "}");

    QPushButton *minusButton = new QPushButton("-");
    minusButton->setFixedSize(50, 50);
    visitsLabel->setObjectName("minusButton");
    minusButton->setStyleSheet("font-size: 12px;");
    minusButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #ffffff;"
        "   border: 1px solid #cccccc;"
        "   border-radius: 5px;"
        "   color: black;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e6f7ff;"
        "   border-color: #80d4ff;"
        "}");

    visitLayout->addWidget(minusButton);
    visitLayout->addWidget(visitsLabel);
    visitLayout->addWidget(plusButton);
    visitBlock->setLayout(visitLayout);
    visitBlock->setStyleSheet("border: 1px solid #cccccc; background-color: #fff; padding: 5px; color: black;");

    QWidget *rateBlock = new QWidget();
    QHBoxLayout *rateLayout = new QHBoxLayout(rateBlock);

    QPushButton *likeButton = new QPushButton();
    likeButton->setIcon(QIcon(":/icons/like.png"));
    likeButton->setObjectName("likeButton");
    likeButton->setIconSize(QSize(32, 32));
    likeButton->setFixedSize(40, 40);
    likeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #ffffff;"
        "   border: 1px solid #cccccc;"
        "   border-radius: 5px;"
        "   color: black;"
        "}"
        "QPushButton:hover {"
        "   background-color: #d4f7d4;"
        "   border-color: #80d4ff;"
        "}");

    QPushButton *dislikeButton = new QPushButton();
    dislikeButton->setIcon(QIcon(":/icons/dont-like.png"));
    dislikeButton->setIconSize(QSize(32, 32));
    dislikeButton->setObjectName("dislikeButton");
    dislikeButton->setFixedSize(40, 40);
    dislikeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #ffffff;"
        "   border: 1px solid #cccccc;"
        "   border-radius: 5px;"
        "   color: black;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ffe6e6;"
        "   border-color: #ff8080;"
        "}");

    QLabel *likesLabel = new QLabel(QString::number(likes));
    likesLabel->setObjectName("likesLabel");
    likesLabel->setObjectName("likesLabel");
    likesLabel->setStyleSheet("font-size: 12px;");
    likesLabel->setAlignment(Qt::AlignCenter);

    QLabel *dislikesLabel = new QLabel(QString::number(dislikes));
    dislikesLabel->setObjectName("dislikesLabel");
    dislikesLabel->setObjectName("dislikesLabel");
    dislikesLabel->setStyleSheet("font-size: 12px;");
    dislikesLabel->setAlignment(Qt::AlignCenter);

    QLabel *rateLabel = new QLabel("0.0");
    rateLabel->setObjectName("rateLabel");
    rateLabel->setObjectName("rateLabel");
    rateLabel->setStyleSheet("font-size: 12px;");
    rateLabel->setAlignment(Qt::AlignCenter);

    QLabel *rateIconLabel = new QLabel();
    rateIconLabel->setPixmap(QPixmap(":/icons/star.png").scaled(32, 32, Qt::KeepAspectRatio));
    rateIconLabel->setAlignment(Qt::AlignCenter);

    rateLayout->addWidget(likeButton);
    rateLayout->addWidget(likesLabel);
    rateLayout->addWidget(dislikeButton);
    rateLayout->addWidget(dislikesLabel);
    rateLayout->addWidget(rateIconLabel);
    rateLayout->addWidget(rateLabel);
    rateBlock->setLayout(rateLayout);
    rateBlock->setStyleSheet("border: 1px solid #cccccc; background-color: #fff; padding: 5px; color: black");

    hLayout->addWidget(nameBlock);
    hLayout->addWidget(visitBlock);
    hLayout->addWidget(rateBlock);

    layout->addWidget(clientWidget);

    ClientData data = {visitsLabel, likesLabel, dislikesLabel, rateLabel, visits, likes, dislikes};
    clientDataMap[name] = data;
    clientWidgets[name] = clientWidget;

    connect(plusButton, &QPushButton::clicked, [=]()
            {
                clientDataMap[name].visits++;
                visitsLabel->setText(QString::number(clientDataMap[name].visits));
                updateClientInDatabase(
                    name,
                    clientDataMap[name].visits,
                    clientDataMap[name].likes,
                    clientDataMap[name].dislikes,
                    clientDataMap[name].rateLabel->text().toDouble()); });

    connect(minusButton, &QPushButton::clicked, [=]()
            {
                if (clientDataMap[name].visits > 0)
                    clientDataMap[name].visits--;
                visitsLabel->setText(QString::number(clientDataMap[name].visits));
                updateClientInDatabase(
                    name,
                    clientDataMap[name].visits,
                    clientDataMap[name].likes,
                    clientDataMap[name].dislikes,
                    clientDataMap[name].rateLabel->text().toDouble()); });

    connect(likeButton, &QPushButton::clicked, [=]()
            {
                clientDataMap[name].likes++;
                likesLabel->setText(QString::number(clientDataMap[name].likes));
                updateRate(clientDataMap[name]);
                updateClientInDatabase(
                    name,
                    clientDataMap[name].visits,
                    clientDataMap[name].likes,
                    clientDataMap[name].dislikes,
                    clientDataMap[name].rateLabel->text().toDouble()
                    ); });

    connect(dislikeButton, &QPushButton::clicked, [=]()
            {
                clientDataMap[name].dislikes++;
                dislikesLabel->setText(QString::number(clientDataMap[name].dislikes));
                updateRate(clientDataMap[name]);
                updateClientInDatabase(
                    name,
                    clientDataMap[name].visits,
                    clientDataMap[name].likes,
                    clientDataMap[name].dislikes,
                    clientDataMap[name].rateLabel->text().toDouble()
                    ); });

    updateRate(clientDataMap[name]);
}

void ClientManage::updateRate(ClientData &data)
{
    double rate = 0.0;
    int totalVotes = data.likes + data.dislikes;
    if (totalVotes > 0)
    {
        rate = static_cast<double>(data.likes) / totalVotes * 5.0;
    }
    data.rateLabel->setText(QString::number(rate, 'f', 2));
}

void ClientManage::updateClientInDatabase(const QString &name, int visits, int likes, int dislikes, double rate)
{
    QSqlQuery query;
    query.prepare("UPDATE Clients SET visit = ?, like = ?, dislike = ?, rate = ? WHERE name = ?");
    query.addBindValue(visits);
    query.addBindValue(likes);
    query.addBindValue(dislikes);
    query.addBindValue(rate);
    query.addBindValue(name);

    if (!query.exec())
    {
        QMessageBox::critical(parentWidget, "Database Error", query.lastError().text());
    }
}

void ClientManage::editClient()
{
    bool ok;
    QString originalName = QInputDialog::getText(parentWidget,
                                                 tr("Редактировать клиента"),
                                                 tr("Введите имя клиента для редактирования:"),
                                                 QLineEdit::Normal, "", &ok);

    if (!ok || originalName.isEmpty())
    {
        QMessageBox::warning(parentWidget, tr("Отмена"), tr("Редактирование отменено или введено пустое значение."));
        return;
    }

    QSqlQuery queryFind;
    queryFind.prepare("SELECT name, visit, rate, like, dislike FROM Clients WHERE name = ?");
    queryFind.addBindValue(originalName);

    if (!queryFind.exec() || !queryFind.next())
    {
        QMessageBox::warning(parentWidget, tr("Не найдено"),
                             tr("Клиент с таким именем не найден в базе данных."));
        return;
    }

    QString currentName = queryFind.value(0).toString();
    int currentVisits = queryFind.value(1).toInt();
    int currentLikes = queryFind.value(3).toInt();
    int currentDislikes = queryFind.value(4).toInt();

    QString newName = QInputDialog::getText(parentWidget, tr("Редактировать клиента"),
                                            tr("Введите новое имя:"), QLineEdit::Normal, currentName, &ok);
    if (!ok || newName.isEmpty())
        return;

    int newVisits = QInputDialog::getInt(parentWidget, tr("Редактировать клиента"),
                                         tr("Введите количество посещений:"), currentVisits, 0, 10000, 1, &ok);
    if (!ok)
        return;

    int newLikes = QInputDialog::getInt(parentWidget, tr("Редактировать клиента"),
                                        tr("Введите количество лайков:"), currentLikes, 0, 10000, 1, &ok);
    if (!ok)
        return;

    int newDislikes = QInputDialog::getInt(parentWidget, tr("Редактировать клиента"),
                                           tr("Введите количество дизлайков:"), currentDislikes, 0, 10000, 1, &ok);
    if (!ok)
        return;

    QSqlQuery queryUpdate;
    queryUpdate.prepare("UPDATE Clients SET name = ?, visit = ?, like = ?, dislike = ? WHERE name = ?");
    queryUpdate.addBindValue(newName);
    queryUpdate.addBindValue(newVisits);
    queryUpdate.addBindValue(newLikes);
    queryUpdate.addBindValue(newDislikes);
    queryUpdate.addBindValue(currentName);

    if (queryUpdate.exec())
    {
        if (clientWidgets.contains(currentName))
        {
            QWidget *widget = clientWidgets[currentName];

            auto labels = widget->findChildren<QLabel *>();
            for (QLabel *label : labels)
            {
                if (label->objectName() == "nameLabel")
                    label->setText(newName);
                else if (label->objectName() == "visitsLabel")
                    label->setText(QString::number(newVisits));
                else if (label->objectName() == "likesLabel")
                    label->setText(QString::number(newLikes));
                else if (label->objectName() == "dislikesLabel")
                    label->setText(QString::number(newDislikes));
            }
            clientWidgets.remove(currentName);
            clientWidgets.insert(newName, widget);
        }
        else
        {
            QMessageBox::warning(parentWidget, tr("Ошибка"),
                                 tr("Не найден виджет для клиента, данные не обновлены в интерфейсе."));
        }
        if (clientDataMap.contains(newName))
        {
            ClientData &data = clientDataMap[newName];
            data.visits = newVisits;
            data.likes = newLikes;
            data.dislikes = newDislikes;
            updateRate(data);
        }

        QMessageBox::information(parentWidget, tr("Успешно"), tr("Данные клиента успешно обновлены!"));
        loadClients();
    }
    else
    {
        QMessageBox::critical(parentWidget, tr("Ошибка"),
                              tr("Не удалось обновить данные клиента: %1").arg(queryUpdate.lastError().text()));
    }
}

