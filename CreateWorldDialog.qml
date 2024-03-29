import QtQuick
import QtQuick.Controls

Dialog
{
    property alias grid: grid
    property int worldWidth: widthSpinBox.value
    property int worldHeight: heightSpinBox.value

    modal: true
    title: qsTr("Create a new world")

    standardButtons: Dialog.Ok | Dialog.Cancel

    Grid
    {
        id: grid

        columns: 2
        rows: 2

        Label
        {
            id: widthLabel
            text: qsTr("Width:")
        }

        SpinBox
        {
            id: widthSpinBox
            editable: true

            from: 1
            to: 100
            stepSize: 1

            value: 30
        }

        Label
        {
            id: heightLabel
            text: qsTr("Height:")
        }

        SpinBox
        {
            id: heightSpinBox
            editable: true

            from: widthSpinBox.from
            to: widthSpinBox.to
            stepSize: widthSpinBox.stepSize

            value: 30
        }
    }
}
