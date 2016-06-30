#ifndef RECIPEMODEL_H
#define RECIPEMODEL_H

#include <QAbstractItemModel>
#include <QSqlDatabase>
#include <QSqlQueryModel>

class QSqlError;
class RecipeModel: public QSqlQueryModel
{
    Q_OBJECT
public:
    RecipeModel(QObject *parent = nullptr);
    ~RecipeModel();

    Q_INVOKABLE QObject *at(int index) const;
    QImage recipeImage(int id) const;

private:
    QSqlDatabase m_database;
};

#endif // RECIPEMODEL_H
