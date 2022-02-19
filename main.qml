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
//    calendar.style:
//    navigationBar.
    onClicked: {
        labelDate.text = calendar.selectedDate

    }
}
Label {
    id: labelDate
    //anchors.top: calendar.bottom
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
//   anchors.topMargin: 10
   ListView{
       implicitWidth: 170
       implicitHeight: 200

//       model: 100
       model: 10
       delegate: RowLayout{
           //if TextField.text has text then show the element
           CheckBox{}
           TextField{
        //       width: 250
           }
       }
   }
}
}
