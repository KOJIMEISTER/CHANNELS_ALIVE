import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    Column {
        anchors.margins: 30
        anchors.fill: parent
        spacing: 30
        Item {
            height: 70
            width: parent.width
            TextField {
                id: textField
                font.pointSize: 24
                placeholderText: "ID канала"
                width: parent.width
            }
        }
        Button {
            property bool isSuccess: false
            id: buttonAdd
            text: "Добавить"
            highlighted: true
            height: 50
            width: 140
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                if(API.addChannel(textField.text))
                {
                    isSuccess = true;
                    API.updateMainPageModel();
                }
                else
                {
                    isSuccess = false;
                }
                addInfo.open()
            }
            Dialog {
                id: addInfo
                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                parent: Overlay.overlay
                width: 300
                height: 100
                modal: true
                focus: true
                title: buttonAdd.isSuccess ? "Канал добавлен" : "Канал не найден"
                standardButtons: Dialog.Ok
            }
        }
    }
}
