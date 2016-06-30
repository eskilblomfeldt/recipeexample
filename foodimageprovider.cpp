#include "foodimageprovider.h"
#include "recipemodel.h"

#include <QDebug>

FoodImageProvider::FoodImageProvider(RecipeModel *recipeModel)
    : QQuickImageProvider(QQuickImageProvider::Image)
    , m_recipeModel(recipeModel)
{
}

QImage FoodImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage image = m_recipeModel->recipeImage(id.toInt());
    if (size != nullptr)
        *size = image.size();

    if (requestedSize.isValid())
        return image.scaled(requestedSize);
    else
        return image;
}
