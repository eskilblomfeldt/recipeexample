/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

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
