#ifndef TOVARMANAGE_H
#define TOVARMANAGE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>

class TovarManage : public QObject
{
    Q_OBJECT

public:
    explicit TovarManage(QWidget *parent = nullptr, QVBoxLayout *layout = nullptr);

    void loadProduct();
    void addProductItem();
    void deleteProductItem(const QString &name);
    void updateCount(int id, const QString &column, int change, QLabel *label);
    void searchProduct(const QString &query);
    void editProductItem();

private:
    QWidget *parentWidget;
    QVBoxLayout *layout;
    QList<QWidget*> productWidgets;
};

#endif // TOVARMANAGE_H
