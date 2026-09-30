import QtQuick 2.15
import QtQuick.Controls 2.15
import "../theme"

Rectangle {
    id: root
    color: Theme.card

    Label {
        id: title
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: Theme.pageMargin
        text: "当前输出"
        font.pixelSize: Theme.titleSize
        font.bold: true
        color: Theme.text
    }

    Rectangle {
        id: header
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: title.bottom
        anchors.leftMargin: Theme.pageMargin
        anchors.rightMargin: Theme.pageMargin
        anchors.topMargin: Theme.spacing + 4
        height: Math.round(40 * Theme.scale)
        radius: Math.round(8 * Theme.scale)
        color: "#E7F0EA"

        readonly property int columnInset: Math.round(16 * Theme.scale)
        readonly property real wordRatio: 0.36
        readonly property real innerWidth: Math.max(0, width - columnInset * 2)
        readonly property real wordColumnWidth: innerWidth * wordRatio
        readonly property real translationColumnWidth: innerWidth - wordColumnWidth

        Label {
            x: header.columnInset
            width: header.wordColumnWidth
            anchors.verticalCenter: parent.verticalCenter
            text: "内容"
            font.pixelSize: Theme.bodySize
            font.bold: true
            color: Theme.text
            horizontalAlignment: Text.AlignLeft
            elide: Text.ElideRight
        }

        Label {
            x: header.columnInset + header.wordColumnWidth
            width: header.translationColumnWidth
            anchors.verticalCenter: parent.verticalCenter
            text: "翻译"
            font.pixelSize: Theme.bodySize
            font.bold: true
            color: Theme.text
            horizontalAlignment: Text.AlignLeft
            elide: Text.ElideRight
        }
    }

    ListView {
        id: list
        objectName: "resultList"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        anchors.leftMargin: Theme.pageMargin
        anchors.rightMargin: Theme.pageMargin
        anchors.topMargin: Theme.spacing
        anchors.bottomMargin: Theme.pageMargin
        clip: true
        spacing: Theme.spacing
        model: assignController.assignments

        delegate: Rectangle {
            width: list.width
            height: Math.max(Math.round(52 * Theme.scale), Math.max(wordLabel.implicitHeight, translationLabel.implicitHeight) + header.columnInset)
            radius: Theme.radius
            color: "#F7F6F2"

            Label {
                id: wordLabel
                x: header.columnInset
                y: header.columnInset / 2
                width: header.wordColumnWidth
                text: word
                color: Theme.text
                font.pixelSize: Theme.bodySize
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignLeft
            }

            Label {
                id: translationLabel
                x: header.columnInset + header.wordColumnWidth
                y: header.columnInset / 2
                width: header.translationColumnWidth
                text: translation
                color: Theme.muted
                font.pixelSize: Theme.bodySize
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignLeft
            }
        }
    }
}
