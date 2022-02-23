import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import QtQuick.Controls 1.4 //for calendar
import ru.geekbrains 1.0

Window {
    width: calendar.width + frameList.width + 30
    height: calendar.height + frameButtons.height + buttonClose.height + 30
    visible: true
    title: qsTr("Organizer")
    Frame{
        id: frameMain
        anchors.fill: parent

        Calendar{
            id: calendar
            //            width: frameList.height

            onClicked: {
                labelDate.text = qsTr("Выбранная дата: ") +
                        Qt.formatDate(calendar.selectedDate, "dd.MM.yyyy")

                listView.model.list.updateCurrentItems(calendar.selectedDate)
                listView.model.list = tasksList
                labelTotalCount.text = qsTr("Всего заданий: ")
                        + tasksList.getTotalTasksCount()
            }
        }
        Label {
            id: labelDate
            anchors.left: calendar.right
            anchors.leftMargin: 10
            anchors.topMargin: 10
            text: qsTr("Выбранная дата: ") +
                  Qt.formatDate(new Date(), "dd.MM.yyyy")
        }

        Label {
            id: labelTotalCount
            anchors.top: labelDate.bottom
            anchors.left: calendar.right
            anchors.leftMargin: 10
            text: qsTr("Всего заданий: ") + tasksList.getTotalTasksCount()
        }
        /*
        RoundButton {
            id: btnIncrListViewItems
            anchors.top: labelDate.bottom
            anchors.right: parent.right
            //            anchors.left: labelDate.right
            //            anchors.left: calendar.right + listView.width
            //            anchors.leftMargin: 10
            text: "+"
            palette {
                button: "#aaddaa"
            }
            //            background: "palegreen"
            //            color: "palegreen"
            //width: 10
        }
        */

        Frame{
            id: frameList
            anchors.left: calendar.right
            anchors.top: labelTotalCount.bottom
            anchors.topMargin: 5
            anchors.leftMargin: 10
            height: calendar.height - labelDate.height
                    - labelDate.anchors.topMargin -
                    labelTotalCount.height

            ListView{
                id: listView
                implicitWidth: 270
                implicitHeight: 200
                clip: true

                model: TasksModel {
                    list: tasksList
                }

                delegate: RowLayout{
                    width: listView.width

                    CheckBox{
                        checked: model.done
                        onClicked: model.done = checked
                    }
                    TextField{
                        id: textFieldDescr
                        text: model.description
                        onEditingFinished: model.description = text
                        Layout.fillWidth: true
                        onAccepted: {
                            console.log("accepted " + text)
                            tasksList.increaseTotalTasksCount()
                            /*
                            tasksList.setTotalTasksCount(
                                        tasksList.getTotalTasksCount() + 1)
                                        */
                            labelTotalCount.text = qsTr("Всего заданий: ")
                                  + tasksList.getTotalTasksCount()

                        }
                        onTextChanged: {
                            console.log("text changed to " + text)
                            }
                        /*
                        MouseArea{ //move it to 'onAccepted' in order to use newTaskIsAccepted()
                            anchors.fill: parent
                            onClicked: {
                                //we can find index of currentItems using text.
                                //and remove or not remove item according text's valid:w
                                //See new func newTaskIsAccepted()
                                console.log("clicked on " + parent.text)

                                enabled = false //
//                                console.log(listView.indexAt())

//                                parent.selectAll()
//                                parent.focus = true
                            }
                        }
                        */

                        /*
                        onAccepted: {
                            tasksList.setTotalTasksCount(tasksList.getTotalTasksCount() + 1)
                            if (text.length < 1)
                            {
                                text = qsTr("Задание номер ")
                                        + tasksList.getTotalTasksCount()
                                //textFieldDescr.accepted(true)
                                //debug

                                labelTotalCount.text = qsTr("Всего заданий: ")
                                        + tasksList.getTotalTasksCount()
                                //end debug
                            }

                        }
                        */

                    }
                    TextField{
                        id: textFieldDate
                        text: model.date
                        visible: false
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
                    text: qsTr("Добавить задание")
                    onClicked: {
                        tasksList.appendItem(calendar.selectedDate)
                        labelTotalCount.text =  qsTr("Всего заданий: ")
                                + tasksList.getTotalTasksCount() + " + 1"
                    }

                }
                Button {
                    id: btnRemove
                    text: qsTr("Удалить отмеченные задания")
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Удалить выполненные либо ошибочно введённые")
                    onClicked:  {
                        tasksList.removeCompletedItems()
                        labelTotalCount.text = qsTr("Всего заданий: ")
                                  + tasksList.getTotalTasksCount()
                    }

                }
            }
        }
        Button{
            id: buttonClose
            anchors.top: frameButtons.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Выход")
            onClicked: {
                listView.model.list.writeDataToSQLiteBase()
                close()
            }
        }

        RoundButton {
            id: btnIncrListViewItems
            anchors.top: labelDate.bottom
            anchors.right: parent.right
            text: "+"
            palette {
                button: "#aaddaa"
            }

//            onClicked: tasksList.appendItem(calendar.selectedDate)
        }
    }
}
