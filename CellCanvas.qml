import QtQuick 2.9

Item
{
    id: canvas
    width: parent.width
    height: parent.height

    property int columns: 0
    property int rows: 0
    property var cellArray: []

    function redraw(biotope)
    {
        const Cell = Qt.createComponent("Cell.qml");
        children = []; // remove all cells from canvas

        columns = biotope.width;
        rows = biotope.height;
        cellArray = new Array(columns * rows); // keep references of cells to modify them later

        const boxWidth = canvas.width / columns;
        const boxHeight = canvas.height / rows;

        for (var y = 0; y < rows; y++)
        {
            for (var x = 0; x < columns; x++)
            {
                var cell = Cell.createObject(canvas);

                cell.move(boxWidth * x, boxHeight * y);
                cell.resize(boxWidth, boxHeight);

                cellArray[offset(x, y)] = cell;
            }
        }

        refreshAll(biotope);
    }

    function refreshAll(biotope)
    {
        canvas.visible = false;

        for (var y = 0; y < rows; y++)
        {
            for (var x = 0; x < columns; x++)
            {
                refreshCell(biotope, x, y);
            }
        }

        canvas.visible = true;
    }

    function refreshCell(biotope, x, y)
    {
        var cell = cellArray[offset(x, y)];
        cell.setAlive(biotope.isAlive(x, y));
    }

    function offset(x, y)
    {
        return y * columns + x;
    }
}
