import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import QtQuick.Controls 1.4 //for calendar

import Tasks 1.0

Window {
    width: calendar.width + frameList.width + 30 //30 - всякие там margins
    height: calendar.height + frameButtons.height + buttonClose.height + 30
    visible: true
    title: qsTr("qt_9")
    Frame{
        id: frameMain
        anchors.fill: parent

        Calendar{
            id: calendar
            onClicked: {
                labelDate.text = Qt.formatDate(calendar.selectedDate, "dd.MM.yyyy")
//                listView.model.list.funcToUpdateModelFromSQLite(DateTime(calendar.selected))

            }
        }
        Label {
            id: labelDate
            anchors.left: calendar.right
            anchors.leftMargin: 10
            anchors.topMargin: 10
            text: Qt.formatDate(new Date(), "dd.MM.yyyy")
        }

        Frame{
            id: frameList
            anchors.left: calendar.right
            anchors.top: labelDate.bottom
            anchors.topMargin: 5
            anchors.leftMargin: 10

            ListView{
                id: listView
                implicitWidth: 170
                implicitHeight: 200
                clip: true

                model: TasksModel {
                    list: tasksList
                }

                delegate: RowLayout{
                    width: parent.width

                    CheckBox{
                        checked: model.done
                        onClicked: model.done = checked
                    }
                    TextField{
                        text: model.description
                        onEditingFinished: model.description = text
                        Layout.fillWidth: true
                    }
                }
            }
        }
        Frame{
            id: frameButtons
            anchors.top: calendar.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            RowLayout{

                Button{
                    id: btnAdd
                    //text: qsTr("Add new task")
                    text: qsTr("Добавить задание")
                    //          Layout.fillWidth: true
                    onClicked: tasksList.appendItem()
                }
                Button {
                    id: btnRemove
                    //text: qsTr("Remove completed tasks")
                    text: qsTr("Удалить выполненные задания")
                    onClicked: tasksList.removeCompletedItems()
                }
            }
        }
        Button{
            id: buttonClose
            anchors.top: frameButtons.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Покинуть это гостеприимное место")
            onClicked: {
                listView.model.list.writeDataToSQLiteBase()
                close()
            }
        }
    }
}
