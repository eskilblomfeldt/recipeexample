import QtQuick 2.7
import QtQuick.Controls 2.0

Page {
    id: page

    property var selectedRecipe: recipes.at(selectedRecipeIndex)
    property alias selectedRecipeIndex: listView.currentIndex

    Label {
        id: label
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        font.pixelSize: page.height / 40
        text: qsTr("Recipes")
    }

    ListView {
        id: listView
        anchors.top: label.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        model: recipes
        clip: true

        delegate: ItemDelegate {
            text: display
            font.pixelSize: page.height / 50
            width: parent.width
            highlighted: ListView.isCurrentItem

            onClicked: listView.currentIndex = index
        }
    }
}
