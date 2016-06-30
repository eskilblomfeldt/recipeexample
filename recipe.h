#ifndef RECIPE_H
#define RECIPE_H

#include <QObject>

class Recipe : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(int time READ time CONSTANT)
    Q_PROPERTY(QString directions READ directions CONSTANT)
public:
    explicit Recipe(int id,
                    const QString &title,
                    int time,
                    const QString &directions,
                    QObject *parent = 0);

    int id() const { return m_id; }
    QString title() const { return m_title; }
    int time() const { return m_time; }
    QString directions() const { return m_directions; }

    Q_INVOKABLE void createTimer(const QString &message) const;

private:
    int m_id;
    QString m_title;
    int m_time;
    QString m_directions;
};

#endif // RECIPE_H
