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
                    ListElement { text: "Онлайн"}
                    ListElement { text: "Название"}
                }
                onActivated: {
                    API.onMainPageModelChanged();
                }
            }
        }
        ListView {
            id: listMainPage
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
                if(sortBy === "Название")
                {
                    sortList.sort(function(a, b){
                        if(a.name < b.name) { return -1; }
                        if(a.name > b.name) { return 1; }
                        return 0;
                    })
                    console.log("Sorted by Названию");
                }
                else if(sortBy === "Онлайн")
                {
                    sortList.sort(function(a, b){
                        if(Number(a.viewers) > Number(b.viewers)) { return -1; }
                        if(Number(a.viewers) < Number(b.viewers)) { return 1; }
                        return 0;
                    })
                    console.log("Sorted by Подписчики");
                }
                return sortList;
            }
            model: ListModel {
                id: modelMainPage
            }
            Component.onCompleted: {
                API.updateChannels();
                API.updateMainPageModel();
                API.onMainPageModelChanged();
            }
            Connections {
                target: API
                function onMainPageModelChanged() {
                    var list = listMainPage.sortModel(API.mainPageListModel, comboBoxSortType.currentText);
                    modelMainPage.clear();
                    modelMainPage.append(list);
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
                        font.pointSize: 16
                        Layout.fillWidth: true
                    }
                    ColumnLayout {
                        height: parent.height
                        RowLayout {
                            Rectangle {
                                height: viewersText.height / 2
                                width: height
                                radius: 10
                                color: "#EB0400"
                                opacity: model.isLive ? 1.0 : 0
                            }
                            Label {
                                id: viewersText
                                text: model.viewers
                                font.pointSize: 16
                                opacity: model.isLive ? 1.0 : 0
                            }
                        }
                        Label {
                            text: ""
                            font.pointSize: 16
                            Timer {
                                id: startedTimer
                                interval: 100
                                running: model.isLive
                                repeat: true
                                onTriggered: {
                                    var date = new Date()
                                    date.setSeconds(date.getSeconds() + 2)
                                    var start = new Date(model.time)
                                    var diff = new Date(date - start)
                                    parent.text = Qt.formatTime(diff, "hh:mm:ss")
                                }
                            }
                        }
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
                }
                onClicked: {
                    menuChannel.open()
                }
            }
        }
    }
}
