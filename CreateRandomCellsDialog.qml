import QtQuick 2.9;
import QtQuick.Controls 2.2

Dialog
{
    property alias grid: grid
    property alias randomSpinBox: randomSpinBox

    modal: true
    title: qsTr("Create random cells")

    standardButtons: Dialog.Ok | Dialog.Cancel

    Grid
    {
        id: grid

        columns: 2
        rows: 1

        Label
        {
            id: randomLabel
            text: qsTr("Cells:")
        }

        SpinBox
        {
            id: randomSpinBox
            editable: true

            from: 1
            to: 100
            stepSize: 10

            value: 30
        }
    }
}
