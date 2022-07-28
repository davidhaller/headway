import QtQuick
import QtQuick.Controls

Dialog
{
    width: parent.width
    height: parent.height
    modal: true
    title: qsTr("About Headway")
    standardButtons: Dialog.Close

    ScrollView
    {
        anchors.fill: parent

        TextArea
        {
            readOnly: true
            selectByMouse: true

            wrapMode: TextEdit.WordWrap
            textFormat: TextEdit.RichText

            font.pointSize: 12
            text: "<h1>Headway "+ HEADWAY_VERSION + "</h1>"
                  + "<p>Copyright © 2022 David Haller (<a href=\"mailto:davidhaller@mailbox.org\">davidhaller@mailbox.org</a>)</p>"
                  + "<p>Licensed under the terms and conditions of the<br/>Apache License, Version 2.0.</p>"
                  + "<p><a href=\"https://davidhaller.github.io/headway\">https://davidhaller.github.io/headway</a></p>"
                  + "<h2>Description</h2>"
                  + "<p>In 1970, the British mathematician John Conway invented a cellular automaton called *Game Of Life*. The simulation takes place in a two-dimensional grid of cells represented as rectangles. A cell can either be *alive* or *dead*. This state can change from one generation to another, depending on the amount of neighbor cells. If a cell has two neighbors, it keeps its state, three neighbors means that the cell will be revived when it was dead before. A cell dies when it has zero, one or more than four neighbors. Therefore, life is threatened by overpopulation and aloneness.</p>"
                  + "<p>At every generation transition, the neighbors of each cell are counted and, depending on the result, the rules mentioned before are applied to each cell. There are several possible simulation outcomes which can occur, e.g:</p>"
                  + "<p><ul><li>death of all cells</li><li>all cells turn stable, so in every future generation no cell dies or gets revived</li><li>periodic movement patterns</li><li>chaotic fluctuation, no recognizable pattern</li></ul></p>"
                  + "<p>Although the simulation may appear to be chaotic, it's actually deterministic, so the same starting parameters will always lead to the same simulation progress.</p>"

            onLinkActivated: Qt.openUrlExternally(link)
        }
    }
}
