#ifndef CLIENTMANAGE_H
#define CLIENTMANAGE_H

#include <QObject>
#include <QVBoxLayout>
#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QLabel>

class ClientManage : public QObject
{
    Q_OBJECT
public:
    explicit ClientManage(QWidget *parent, QVBoxLayout *layout);

    void loadClients();
    void addClient();
    void removeClient();
    void searchClient(const QString &query);
    void editClient();
    void updateClientInDatabase(const QString &name, int visits, int likes, int dislikes, double rate);

private:
    QWidget *parentWidget;
    QVBoxLayout *layout;
    QMap<QString, QWidget *> clientWidgets;

    struct ClientData
    {
        QLabel *visitsLabel;
        QLabel *likesLabel;
        QLabel *dislikesLabel;
        QLabel *rateLabel;
        int visits;
        int likes;
        int dislikes;
    };

    QMap<QString, ClientData> clientDataMap;

    void addClientBlock(const QString &name, int visits, double rate, int likes, int dislikes);
    void updateRate(ClientData &data);
    void deleteCellContent(int row, int column);

};

#endif // CLIENTMANAGE_H
