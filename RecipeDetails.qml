import QtQuick 2.0
import QtQuick.Controls 2.0

Page {
    id: page
    property var recipe: null

    Label {
        id: title
        text: recipe.title
        anchors.margins: page.height / 100
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: page.height / 40
    }

    Label {
        id: time
        anchors.margins: page.height / 100
        anchors.top: title.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: recipe.time + " minutes"
        font.pixelSize: page.height / 70
    }

    Image {
        id: image
        anchors.margins: page.height / 100
        anchors.top: time.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        source: "image://food/" + recipe.id
    }

    Text {
        id: directions
        anchors.margins: page.height / 100
        text: recipe.directions
        anchors.top: image.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: page.height / 60
    }

    Item {
        anchors.bottom: parent.bottom
        anchors.top: directions.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
            id: timerButton
            text: qsTr("Set timer")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            onClicked: recipe.createTimer(recipe.title)
        }
    }
}
