#ifndef FOODIMAGEPROVIDER_H
#define FOODIMAGEPROVIDER_H

#include <QQuickImageProvider>

class RecipeModel;
class FoodImageProvider : public QQuickImageProvider
{
public:
    FoodImageProvider(RecipeModel *recipeModel);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    RecipeModel *m_recipeModel;
};

#endif // FOODIMAGEPROVIDER_H
