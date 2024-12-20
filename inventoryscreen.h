#ifndef INVENTORYSCREEN_H
#define INVENTORYSCREEN_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>

class InventoryScreen : public QObject
{
    Q_OBJECT

public:
    InventoryScreen(QWidget *parent = nullptr, QVBoxLayout *layout = nullptr);

    void loadInventory();
    void addInventoryItem();
    void deleteInventoryItem(const QString &name);
    void updateCount(int id, const QString &column, int change, QLabel *label);
    void searchInventory(const QString &query);
    void editInventoryItem();


private:
    QWidget *parentWidget;
    QVBoxLayout *layout;
    QList<QWidget *> inventoryWidgets;
};

#endif // INVENTORYSCREEN_H
