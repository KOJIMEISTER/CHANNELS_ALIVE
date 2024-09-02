import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Page {
    ColumnLayout {
        spacing: 30
        anchors.margins: 30
        anchors.fill: parent
        RowLayout {
            Layout.fillWidth: true
            Label {
                Layout.fillWidth: true
                text: "Сортировать по"
                font.pointSize: 16
            }
            ComboBox {
                id: comboBoxSortType
                Layout.fillWidth: true
                model: ListModel {
                    ListElement { text: "Подписчики"}
                    ListElement { text: "Названию"}
                }
                onActivated: {
                    API.onChannelsModelChanged();
                }
            }
        }
        ListView {
            id: listChannels
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 30
            clip: true
            function sortModel (refModel, sortBy)
            {
                var sortList = []
                for(var i = 0; i < refModel.length; ++i)
                {
                    sortList.push(refModel[i]);
                }
                if(sortBy === "Названию")
                {
                    sortList.sort(function(a, b){
                        if(a.name < b.name) { return -1; }
                        if(a.name > b.name) { return 1; }
                        return 0;
                    })
                    console.log("Sorted by Названию");
                }
                else if(sortBy === "Подписчики")
                {
                    sortList.sort(function(a, b){
                        if(Number(a.subscribers) > Number(b.subscribers)) { return -1; }
                        if(Number(a.subscribers) < Number(b.subscribers)) { return 1; }
                        return 0;
                    })
                    console.log("Sorted by Подписчики");
                }
                return sortList;
            }
            model: ListModel {
                id: modelChannels
            }
            Component.onCompleted: {
                API.updateChannelsPageModel();
                API.onChannelsModelChanged();
            }
            Connections {
                target: API
                function onChannelsModelChanged() {
                    var items = listChannels.sortModel(API.channelsListModel, comboBoxSortType.currentText);
                    modelChannels.clear();
                    modelChannels.append(items);
                }
            }
            delegate: ItemDelegate {
                id: itemDelegate
                width: parent.width
                height: 75
                RowLayout {
                    anchors.fill: parent
                    spacing: 10
                    Image {
                        id: profileImage
                        height: itemDelegate.height
                        width: height
                        sourceSize.width: height
                        sourceSize.height: height
                        source: model.image
                        layer.enabled: true
                        layer.effect: OpacityMask {
                            maskSource: Item {
                                width: profileImage.width
                                height: profileImage.height
                                Rectangle {
                                    anchors.centerIn: parent
                                    width: profileImage.width
                                    height: profileImage.height
                                    radius: Math.min(width, height)
                                }
                            }
                        }
                    }
                    Label {
                        text: name
                        wrapMode: Text.WrapAnywhere
                        font.pointSize: 16
                        Layout.fillWidth: true
                        clip: true
                    }
                    Image {
                        height: parent.height
                        width: height
                        source: "qrc:/icons/user.svg"
                    }
                    Label {
                        font.pointSize: 16
                        wrapMode: Text.WrapAnywhere
                        text: subscribers
                        clip: true
                    }
                    Image {
                        height: parent.height
                        width: height
                        source: model.isNotified ? "qrc:/icons/bell.svg" : "qrc:/icons/bell-off.svg"
                    }
                }
                Dialog {
                    id: deleteDialog
                    x: (parent.width - width) / 2
                    y: (parent.height - height) / 2
                    parent: Overlay.overlay
                    modal: true
                    title: "Удалить канал " + model.name
                    standardButtons: Dialog.Yes | Dialog.No
                    onAccepted: {
                        API.deleteChannel(model.name);
                        API.updateChannelsPageModel();
                        listChannels.model.remove(index)
                    }
                }
                Menu {
                    id: menuChannel
                    Action {
                        text: "Перейти"
                        onTriggered: {
                            var url = "https://www.twitch.tv/" + model.name
                            Qt.openUrlExternally(url)
                        }
                    }
                    Action {
                        text: model.isNotified ? "Выкл. оповещение" : "Вкл. оповещение"
                        onTriggered: {
                            if(model.isNotified)
                            {
                                API.turnOffNotify(model.name);
                                model.isNotified = false;
                            }
                            else
                            {
                                API.turnOnNotify(model.name);
                                model.isNotified = true;
                            }
                        }
                    }
                    Action {
                        text: "Удалить"
                        onTriggered: {
                            deleteDialog.open()
                        }
                    }
                }
                onClicked: {
                    menuChannel.open()
                }
            }
        }
    }
}
