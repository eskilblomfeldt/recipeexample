#include "recipemodel.h"
#include "recipe.h"

#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlError>
#include <QSqlRecord>
#include <QImage>
#include <QDebug>
#include <QFile>

RecipeModel::RecipeModel(QObject *parent)
    : QSqlQueryModel(parent)
{
    m_database = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    m_database.setDatabaseName("recipes.db");
    if (!m_database.open(QStringLiteral("recipeuser"), QStringLiteral("recipepassword"))) {
        qWarning() << "Failed to open database";
        return;
    }

    QSqlQuery query(QStringLiteral("SELECT * FROM recipes"));
    if (!query.isActive()) {
        QSqlQuery createQuery(QString::fromLatin1("CREATE TABLE recipes"
                                                  "     (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                  "     time INTEGER,"
                                                  "     title TEXT,"
                                                  "     directions TEXT,"
                                                  "     image BLOB)"));
        if (!createQuery.isActive())
            qWarning() << "Creating table failed:" << m_database.lastError().text();

        {
            QFile imageFile(":/lumberjackstack.jpg");
            if (!imageFile.open(QIODevice::ReadOnly))
                qWarning() << "Can't open image file";

            QSqlQuery insertQuery;
            insertQuery.prepare(QStringLiteral("INSERT INTO recipes (time, title, directions, image) VALUES (:time, :title, :directions, :image)"));
            insertQuery.bindValue(QStringLiteral(":time"), 25);
            insertQuery.bindValue(QStringLiteral(":title"), QStringLiteral("Lumber jack stack"));
            insertQuery.bindValue(QStringLiteral(":directions"), QStringLiteral("1. Make scrambled eggs<br />2. Make waffles<br />3. Make bacon<br />4. Put them on top of each other"));
            insertQuery.bindValue(QStringLiteral(":image"), imageFile.readAll());
            if (!insertQuery.exec())
                qWarning() << "Inserting failed:" << m_database.lastError().text();
        }

        {
            QFile imageFile(":/water.jpg");
            if (!imageFile.open(QIODevice::ReadOnly))
                qWarning() << "Can't open image file";

            QSqlQuery insertQuery;
            insertQuery.prepare(QStringLiteral("INSERT INTO recipes (time, title, directions, image) VALUES (:time, :title, :directions, :image)"));
            insertQuery.bindValue(QStringLiteral(":time"), 25);
            insertQuery.bindValue(QStringLiteral(":title"), QStringLiteral("Boiling water"));
            insertQuery.bindValue(QStringLiteral(":directions"), QStringLiteral("1. Add water<br />2. Heat to 100 degrees Celsius"));
            insertQuery.bindValue(QStringLiteral(":image"), imageFile.readAll());
            if (!insertQuery.exec())
                qWarning() << "Inserting failed:" << m_database.lastError().text();
        }
    }

    setQuery(QStringLiteral("SELECT title, directions, time, id FROM recipes"));
}

RecipeModel::~RecipeModel()
{
    QSqlDatabase::removeDatabase(m_database.connectionName());
}

QObject *RecipeModel::at(int index) const
{
    if (index < 0 || index >= rowCount())
        return nullptr;

    QSqlRecord sqlRecord = record(index);
    return new Recipe(sqlRecord.value(3).toInt(), sqlRecord.value(0).toString(), sqlRecord.value(2).toInt(), sqlRecord.value(1).toString());
}

QImage RecipeModel::recipeImage(int id) const
{
    QSqlQuery query(QStringLiteral("SELECT id, image FROM recipes WHERE id=%1").arg(id));
    if (query.first()) {
        QByteArray ba = query.value(1).toByteArray();
        return QImage::fromData(ba, "jpg");
    }
    return QImage();
}
