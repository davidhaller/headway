import QtQuick 6.0

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

        for (let y = 0; y < rows; y++)
        {
            for (let x = 0; x < columns; x++)
            {
                let cell = Cell.createObject(canvas);

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

        for (let y = 0; y < rows; y++)
        {
            for (let x = 0; x < columns; x++)
            {
                refreshCell(biotope, x, y);
            }
        }

        canvas.visible = true;
    }

    function refreshCell(biotope, x, y)
    {
        let cell = cellArray[offset(x, y)];
        cell.setAlive(biotope.isAlive(x, y));
    }

    function offset(x, y)
    {
        return y * columns + x;
    }
}
