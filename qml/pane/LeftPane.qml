import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import "../theme"

Rectangle {
    id: root
    color: Theme.background

    readonly property var months: ["1月", "2月", "3月", "4月", "5月", "6月", "7月", "8月", "9月", "10月", "11月", "12月"]
    readonly property var rounds: ["1", "2", "3", "4", "end"]

    signal published(string month, string round, int wordCount)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Math.max(12, Math.min(Math.round(28 * Theme.scale), root.width * 0.1))
        spacing: Theme.spacing + 4

        Label {
            text: "发布设置"
            font.pixelSize: Theme.titleSize
            font.bold: true
            color: Theme.text
            Layout.fillWidth: true
            wrapMode: Text.Wrap
        }

        ColumnLayout {
            spacing: 8
            Layout.fillWidth: true

            Label {
                text: "月份"
                color: Theme.muted
            }

            ComboBox {
                id: monthBox
                Layout.fillWidth: true
                model: root.months
            }
        }

        ColumnLayout {
            spacing: 8
            Layout.fillWidth: true

            Label {
                text: "月次"
                color: Theme.muted
            }

            ComboBox {
                id: roundBox
                Layout.fillWidth: true
                model: root.rounds
            }
        }

        ColumnLayout {
            spacing: 8
            Layout.fillWidth: true

            Label {
                text: "单词量"
                color: Theme.muted
            }

            TextField {
                id: countField
                objectName: "countField"
                Layout.fillWidth: true
                placeholderText: "1 - 300"
                selectByMouse: true
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator {
                    bottom: 1
                    top: 300
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: Math.max(8, Theme.spacing)

            Button {
                id: publishButton
                objectName: "publishButton"
                text: "发布"
                Layout.fillWidth: true
                Layout.preferredHeight: Math.round(40 * Theme.scale)
                enabled: countField.acceptableInput

                background: Rectangle {
                    radius: Theme.radius
                    color: publishButton.down ? "#24573D" : (publishButton.enabled ? Theme.accent : "#C8C4BC")
                }

                contentItem: Text {
                    text: publishButton.text
                    color: "white"
                    font.pixelSize: Theme.buttonTextSize
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: root.published(monthBox.currentText, roundBox.currentText, parseInt(countField.text))
            }

            Button {
                id: exportButton
                objectName: "exportButton"
                text: "导出"
                Layout.fillWidth: true
                Layout.preferredHeight: Math.round(40 * Theme.scale)
                enabled: assignController.hasResult

                background: Rectangle {
                    radius: Theme.radius
                    color: exportButton.down ? "#E7F0EA" : "white"
                    border.width: 1
                    border.color: exportButton.enabled ? Theme.accent : "#C8C4BC"
                }

                contentItem: Text {
                    text: exportButton.text
                    color: exportButton.enabled ? Theme.accent : "#C8C4BC"
                    font.pixelSize: Theme.buttonTextSize
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    exportDialog.folder = assignController.exportDirectory
                    exportDialog.open()
                }
            }
        }

        Label {
            visible: assignController.hasResult
            text: assignController.exportBaseName + ".txt    " + assignController.exportBaseName + "_translation.txt"
            color: Theme.muted
            font.pixelSize: Theme.bodySize
            wrapMode: Text.Wrap
            Layout.fillWidth: true
        }

        Item {
            Layout.fillHeight: true
        }
    }

    FolderDialog {
        id: exportDialog
        objectName: "exportDialog"
        title: "选择导出文件夹"
        onAccepted: assignController.exportTo(currentFolder)
    }
}
