import QtQuick

Rectangle
{
    function move(newX, newY)
    {
        x = newX;
        y = newY;
    }

    function resize(newWidth, newHeight)
    {
        width = newWidth;
        height = newHeight;
    }

    function setAlive(value)
    {
        if (value === true)
            color = "black";
        else
            color = "white";
    }

    border.color: "lightgray"
}
