import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import QtQuick.Controls 1.4 //for calendar

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
   ListView{
       implicitWidth: 170
       implicitHeight: 200
       clip: true
//       model: 10
       model: ListModel {
           ListElement{
               done: true
               description: "Wash the car"
           }
           ListElement{
               done: false
               description: "Fix the sink"
           }
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
}
