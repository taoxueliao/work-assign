import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import "pane"
import "theme"

ApplicationWindow {
    id: window
    minimumWidth: 800
    minimumHeight: 520
    title: "单词分配"
    color: Theme.background

    Component.onCompleted: {
        var availW = Math.min(Screen.desktopAvailableWidth, Screen.width)
        var availH = Math.min(Screen.desktopAvailableHeight, Screen.height)
        var targetWidth = Math.max(minimumWidth, Math.round(availW * 0.8))
        var targetHeight = Math.max(minimumHeight, Math.round(availH * 0.8))
        targetWidth = Math.min(targetWidth, availW)
        targetHeight = Math.min(targetHeight, availH)
        width = targetWidth
        height = targetHeight
        x = Screen.virtualX + Math.round((Screen.width - targetWidth) / 2)
        y = Screen.virtualY + Math.round((Screen.height - targetHeight) / 2)
        visible = true
    }

    LeftPane {
        id: leftPane
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width * 2 / 10

        onPublished: assignController.assign(wordCount, month, round)
    }

    RightPane {
        anchors.left: leftPane.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }
}
