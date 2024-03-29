import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

import Qt.labs.platform as Labs

import Headway;

ApplicationWindow
{
    id: window
    visible: true
    minimumWidth: 1000
    minimumHeight: 700

    Labs.MenuBar
    {
        id: globalMenuBar

        Labs.Menu
        {
            id: appMenu
            Labs.MenuItem
            {
                role: Labs.MenuItem.AboutRole
                onTriggered: aboutDialog.open()
            }
        }
    }

    World
    {
        id: biotope
        onError:
        {
            errorDialog.text = message;
            errorDialog.open();
        }
    }

    Labs.MessageDialog
    {
        id: errorDialog
        title: qsTr("Error")
        buttons: Labs.MessageDialog.Close
    }

    AboutDialog
    {
        id: aboutDialog

        x: 10
        y: 10
        width: parent.width - 20
        height: parent.height - 20
    }

    CreateWorldDialog
    {
        id: createWorldDialog

        x: (parent.width / 2) - (width / 2)
        y: (parent.height / 2) - (height / 2)

        grid.columnSpacing: 20
        grid.rowSpacing: 5

        grid.verticalItemAlignment: Grid.AlignVCenter

        onAccepted:
        {
            biotope.createWorld(worldWidth, worldHeight, 0);
            redrawTimer.start();
        }
    }

    CreateRandomCellsDialog
    {
        id: createRandomCellsDialog

        x: (parent.width / 2) - (width / 2)
        y: (parent.height / 2) - (height / 2)

        grid.columnSpacing: 20
        grid.rowSpacing: 5

        grid.verticalItemAlignment: Grid.AlignVCenter

        onAccepted:
        {
            biotope.random(randomSpinBox.value);
            cellCanvas.redraw(biotope);
        }
    }

    Labs.FileDialog
    {
        id: openDialog
        fileMode: Labs.FileDialog.OpenFile
        options: Labs.FileDialog.ReadOnly
        nameFilters: [qsTr("All files (*)"), qsTr("XML files (*.xml)"), qsTr("JSON files (*.json)")]
        onAccepted:
        {
            biotope.loadFile(openDialog.file);
            cellCanvas.refreshAll(biotope);
        }
    }

    Labs.FileDialog
    {
        id: saveDialog
        fileMode: Labs.FileDialog.SaveFile
        nameFilters: openDialog.nameFilters
        onAccepted: biotope.saveFile(saveDialog.file)
    }

    MouseArea
    {
        id: mouseArea
        width: contentItem.width
        height: contentItem.height

        CellCanvas
        {
            id: cellCanvas
        }

        onClicked: mouse =>
        {
            const x = Math.floor((mouse.x * biotope.width) / (cellCanvas.width));
            const y = Math.floor((mouse.y * biotope.height) / (cellCanvas.height));

            if (biotope.isAlive(x, y))
                biotope.killCell(x, y);
            else
                biotope.createCell(x, y);

            cellCanvas.refreshCell(biotope, x, y);
        }
    }

    footer: ControlToolBar
    {
        id: controlToolBar
        width: parent.width

        newButton.onClicked: createWorldDialog.open()
        randomButton.onClicked:
        {
            const space = biotope.width * biotope.height - biotope.count;

            createRandomCellsDialog.randomSpinBox.to = space;
            createRandomCellsDialog.randomSpinBox.value = space / 2;
            createRandomCellsDialog.open();
        }

        loadButton.onClicked: openDialog.open()
        saveButton.onClicked: saveDialog.open()

        runButton.onClicked:
        {
            if (!simulationTimer.running)
            {
                newButton.enabled = false;
                randomButton.enabled = false;
                loadButton.enabled = false;
                saveButton.enabled = false;
                nextButton.enabled = false;
                mouseArea.enabled = false;

                simulationTimer.start();
                refreshTimer.start();

                runButton.text = runButton.stopText;
            }

            else
            {
                simulationTimer.stop();
                refreshTimer.stop();

                cellCanvas.refreshAll(biotope);

                newButton.enabled = true;
                randomButton.enabled = true;
                loadButton.enabled = true;
                saveButton.enabled = true;
                nextButton.enabled = true;
                mouseArea.enabled = true;

                runButton.text = runButton.startText;
            }
        }

        nextButton.onClicked:
        {
            biotope.next();
            cellCanvas.refreshAll(biotope);
        }

        aboutButton.onClicked: aboutDialog.open()
        aboutButton.visible: !HEADWAY_GLOBAL_MENUBAR

        cellLabel.text: biotope.count + (biotope.count === 1 ? qsTr(" cell") : qsTr(" cells"))
        generationsLabel.text: biotope.generations + (biotope.generations === 1 ? qsTr(" generation") : qsTr(" generations"))

        speedSpinBox.onValueChanged:
        {
            if (speedSlider.value != speedSpinBox.value)
            {
                simulationTimer.interval = 1000 / speedSpinBox.value;
                speedSlider.value = speedSpinBox.value;
            }
        }


        speedSlider.onValueChanged:
        {
            if (speedSpinBox.value != speedSlider.value)
            {
                simulationTimer.interval = 1000 / speedSlider.value;
                speedSpinBox.value = speedSlider.value;
            }
        }
    }

    Shortcut
    {
        sequences: [StandardKey.Close, "Ctrl+W"]
        context: Qt.WindowShortcut
        onActivated: window.close()
    }

    Shortcut
    {
        sequences: [StandardKey.Quit, "Ctrl+Q"]
        context: Qt.ApplicationShortcut
        onActivated: Qt.quit()
    }

    Timer
    {
        id: simulationTimer
        repeat: true
        interval: 50

        onTriggered: biotope.next()
    }

    Timer
    {
        id: refreshTimer
        repeat: true
        interval: 200

        onTriggered: cellCanvas.refreshAll(biotope)
    }

    Timer
    {
        id: redrawTimer
        repeat: false
        interval: 50

        onTriggered:
        {
            cellCanvas.redraw(biotope);
            setTitle(biotope.width + "x" + biotope.height);
        }
    }

    onWidthChanged: redrawTimer.start()
    onHeightChanged: redrawTimer.start()
    Component.onCompleted:
    {
        if (!HEADWAY_GLOBAL_MENUBAR) globalMenuBar.window = null;
        redrawTimer.start();
    }
}
