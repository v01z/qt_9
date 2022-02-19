import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import QtQuick.Controls 1.4 //for calendar

import Tasks 1.0

Window {
    width: 580
    height: 285
    visible: true
    title: qsTr("qt_9")
    Calendar{
        id: calendar
        onClicked: {
            labelDate.text = calendar.selectedDate

        }
    }
    Label {
        id: labelDate
        anchors.left: calendar.right
        anchors.leftMargin: 10
        anchors.topMargin: 10
        //    text: calendar. currentDate ?
        text: "  "
    }

    Frame{
        id: frame
        anchors.left: calendar.right
        anchors.top: labelDate.bottom
        anchors.topMargin: 5
        anchors.leftMargin: 10
    //    Layout.fillWidth: true
        ListView{
            implicitWidth: 170
            implicitHeight: 200
            clip: true
//            anchors.fill: parent

            model: TasksModel {
                list: tasksList
            }

            delegate: RowLayout{
                //if TextField.text has text then show the element
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

    RowLayout{
        anchors.left: frame.right
        Button{
            id: btnAdd
            text: qsTr("Add new item       ")
  //          Layout.fillWidth: true
            onClicked: tasksList.appendItem()
        }
        Button {
            id: btnRemove
            anchors.top: btnAdd.bottom
            anchors.left: parent.left
            text: qsTr("Remove completed")
 //           Layout.fillWidth: true
            onClicked: tasksList.removeCompletedItems()
        }

    }
}
