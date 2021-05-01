import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

RowLayout
{
    property alias newButton: newButton
    property alias randomButton: randomButton
    property alias loadButton: loadButton
    property alias saveButton: saveButton
    property alias runButton: runButton
    property alias nextButton: nextButton
    property alias aboutButton: aboutButton
    property alias cellLabel: cellLabel
    property alias generationsLabel: generationsLabel
    property alias speedSlider: speedSlider
    property alias speedSpinBox: speedSpinBox

    property int speedMin: 1
    property int speedMax: 100
    property int speedDefault: 20

    Frame
    {
        id: controlFrameLeft
        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

        background: Rectangle
        {
            color: "transparent"
            border.color: "transparent"
        }

        leftPadding: 10
        bottomPadding: 10

        RowLayout
        {
            id: controlBoxLeft

            RoundButton
            {
                id: newButton
                text: "🌍"

                ToolTip.text: qsTr("Create a new world")
                ToolTip.visible: hovered
            }

            Shortcut
            {
                sequence: StandardKey.New
                onActivated: newButton.clicked()
            }

            RoundButton
            {
                id: randomButton
                text: "🔀"

                ToolTip.text: qsTr("Generate random cells")
                ToolTip.visible: hovered
            }

            RoundButton
            {
                id: loadButton
                text: "📂"

                ToolTip.text: qsTr("Load world from file")
                ToolTip.visible: hovered
            }

            Shortcut
            {
                sequence: StandardKey.Open
                onActivated: loadButton.clicked()
            }

            RoundButton
            {
                id: saveButton
                text: "💾"

                ToolTip.text: qsTr("Save world to file")
                ToolTip.visible: hovered
            }

            Shortcut
            {
                sequence: StandardKey.Save
                onActivated: saveButton.clicked()
            }

            RoundButton
            {
                id: runButton

                property string startSymbol: "▶"
                property string stopSymbol: "⏸"

                text: startSymbol

                ToolTip.text: qsTr("Start or stop the simulation")
                ToolTip.visible: hovered
            }

            RoundButton
            {
                id: nextButton
                text: "⏩"

                ToolTip.text: qsTr("Calculate and display the next generation")
                ToolTip.visible: hovered
            }

            RoundButton
            {
                id: aboutButton
                text: "？"

                ToolTip.text: qsTr("Show information about this app")
                ToolTip.visible: hovered
            }
        }
    }

    Frame
    {
        id: controlFrameRight
        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

        background: Rectangle
        {
            color: "transparent"
            border.color: "transparent"
        }

        rightPadding: 10
        bottomPadding: 10

        RowLayout
        {
            id: controlBoxRight
            spacing: 15

            Label
            {
                id: cellLabel
                text: "0 " + qsTr("cells")
            }

            Label
            {
                id: generationsLabel
                text: "0 " + qsTr("generations")
            }

            SpinBox
            {
                id: speedSpinBox
                width: parent.width

                editable: true

                from: speedMin
                to: speedMax
                value: speedDefault
                stepSize: 1

                ToolTip.text: qsTr("Speed in generations per second")
                ToolTip.visible: hovered
            }

            Slider
            {
                id: speedSlider
                width: parent.width

                from: speedMin
                to: speedMax
                value: speedDefault
                stepSize: 1
            }
        }
    }
}
