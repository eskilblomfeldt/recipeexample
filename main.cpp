#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "recipemodel.h"
#include "foodimageprovider.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    RecipeModel model;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("recipes"), QVariant::fromValue(&model));
    engine.addImageProvider(QStringLiteral("food"), new FoodImageProvider(&model));
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
