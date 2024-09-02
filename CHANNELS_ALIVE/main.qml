import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

ApplicationWindow {
    id: window
    width: 360
    height: 520
    visible: true
    title: "Channels A-Live"

    Timer {
        interval: 5000
        running: true
        repeat: true
        onTriggered: {
            API.updateChannels();
            API.updateMainPageModel();
            console.log("Update");
        }
    }

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: navigateBackAction.trigger()
    }

    Action {
        id: navigateBackAction
        icon.source: stackView.depth > 1 ? "qrc:/icons/chevron-left.svg" : "qrc:/icons/menu.svg"
        onTriggered: {
            if (stackView.depth > 1) {
                stackView.pop()
                listView.currentIndex = -1
            } else {
                drawer.open()
            }
        }
    }

    Shortcut {
        sequence: "Menu"
        onActivated: optionsMenuAction.trigger()
    }

    Action {
        id: optionsMenuAction
        icon.source: "qrc:/icons/settings.svg"
        onTriggered: optionsMenu.open()
    }

    header: ToolBar {
        id: toolbarHeader
        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                action: navigateBackAction
            }

            Label {
                id: titleLabel
                text: listView.currentItem ? listView.currentItem.textItem : "Главная"
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                action: optionsMenuAction

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    Action {
                        text: "Настройки"
                        onTriggered: settingsDialog.open()
                    }
                    Action {
                        text: "О программе"
                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    Drawer {
        id: drawer
        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height
        interactive: stackView.depth === 1
        ColumnLayout {
            anchors.fill: parent
            Item {
                Layout.fillWidth: true
                height: 50
                Image {
                    id: twitchLogo
                    source: "qrc:/icons/twitch.svg"
                    anchors.centerIn: parent
                    height: parent.height - 10
                    width: height
                }
            }
            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                id: listView
                focus: true
                currentIndex: -1
                delegate: ItemDelegate {
                    width: listView.width
                    property alias textItem: textTitle.text
                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        spacing: 10
                        Image {
                            height: parent.height - 5
                            width: height
                            source: model.imgSource
                        }
                        Label {
                            id: textTitle
                            text: model.title
                            font.pointSize: 16
                            Layout.fillWidth: true
                        }
                    }
                    onClicked: {
                        listView.currentIndex = index
                        stackView.push(model.source)
                        drawer.close()
                    }
                }
                model: ListModel {
                    ListElement { title: "Каналы"; source: "qrc:/pages/ChannelsPage.qml"; imgSource: "qrc:/icons/tv.svg" }
                    ListElement { title: "Добавить канал"; source: "qrc:/pages/AddChannelPage.qml"; imgSource: "qrc:/icons/plus-square.svg" }
                }
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "qrc:/pages/MainPage.qml"
    }

    Dialog {
        id: settingsDialog
        x: Math.round((window.width - width) / 2)
        y: Math.round(window.height / 6)
        width: Math.round(Math.min(window.width, window.height) / 3 * 2)
        focus: true
        title: "Настройки"
        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20
            Switch {
                text: "Оповещения"
            }
        }
    }

    Dialog {
        id: aboutDialog
        modal: true
        focus: true
        title: "О программе"
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        Label {
            width: aboutDialog.availableWidth
            text: "Программа позволяет отслеживать активность каналов с Twitch."
            wrapMode: Label.Wrap
            font.pixelSize: 12
        }
    }

}
