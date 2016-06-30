import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    width: 1080
    height: 100
    title: qsTr("QRecipe")

    SwipeView {
        id: swipeView
        anchors.fill: parent

        RecipeList {
            id: recipeList
            onSelectedRecipeIndexChanged: {
                if (swipeView.currentIndex == 0)
                    swipeView.contentItem.incrementCurrentIndex()
            }
        }

        RecipeDetails {
            id: recipeDetails
            recipe: recipeList.selectedRecipe
        }
    }
}
