import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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

    readonly property int speedMin: 1
    readonly property int speedMax: 100
    readonly property int speedDefault: 20

    readonly property int toolTipDelay: 500

    Frame
    {
        id: controlFrameLeft
        hoverEnabled: true
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

            Button
            {
                id: newButton
                text: qsTr("New")

                ToolTip.text: qsTr("Create a new world")
                ToolTip.visible: hovered
                ToolTip.delay: toolTipDelay
            }

            Shortcut
            {
                sequence: StandardKey.New
                onActivated: newButton.clicked()
            }

            Button
            {
                id: randomButton
                text: qsTr("Random")

                ToolTip.text: qsTr("Generate random cells")
                ToolTip.visible: hovered
                ToolTip.delay: toolTipDelay
            }

            Button
            {
                id: loadButton
                text: qsTr("Load")

                ToolTip.text: qsTr("Load world from file")
                ToolTip.visible: hovered
                ToolTip.delay: toolTipDelay
            }

            Shortcut
            {
                sequence: StandardKey.Open
                onActivated: loadButton.clicked()
            }

            Button
            {
                id: saveButton
                text: qsTr("Save")

                ToolTip.text: qsTr("Save world to file")
                ToolTip.visible: hovered
                ToolTip.delay: toolTipDelay
            }

            Shortcut
            {
                sequence: StandardKey.Save
                onActivated: saveButton.clicked()
            }

            Button
            {
                id: runButton

                readonly property string startText: qsTr("Start")
                readonly property string stopText: qsTr("Stop")

                text: startText

                ToolTip.text: qsTr("Start or stop the simulation")
                ToolTip.visible: hovered
                ToolTip.delay: toolTipDelay
            }

            Button
            {
                id: nextButton
                text: qsTr("Next")

                ToolTip.text: qsTr("Calculate and display the next generation")
                ToolTip.visible: hovered
                ToolTip.delay: toolTipDelay
            }

            Button
            {
                id: aboutButton
                text: qsTr("Help")

                ToolTip.text: qsTr("Show information about this app")
                ToolTip.visible: hovered
                ToolTip.delay: toolTipDelay
            }
        }
    }

    Frame
    {
        id: controlFrameRight
        hoverEnabled: true
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
                ToolTip.delay: toolTipDelay
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
