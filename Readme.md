# Point in Polygon
## Description
Point in Polygon is a simple application,</br>
which allows to define polygon and external points</br>
and indicating which of them are inside and outside of polygon

<img src="./notes/main.png" width="640" height="400">

## Edit polygon
To edit polygon click on the first button.</br>
It will indicate activation of it's function by changing it's color. </br>
Next you can define your polygon by choosing points over left mouse button</br>

<img src="./notes/editing_polygon.png" width="640" height="400">

You can remove current last point from polygon, by clicking right mouse button.</br>
To finish editing polygon click one more time on the corresponding button or click on the first point while editing. </br>
Polygon can be modified letter on

## Points

To add external point click on the second button. </br>
Points which are within polygon will be marked with orange color </br>
Points outside of the polygon will be marked with blueish color </br>

<img src="./notes/computing_points.png" width="640" height="400">

Computation of the point position is done during it's creation or after polygon is stop being edited. </br>
Points cannot be removed

## Bugs

There is bug in the computation, when point has exact same y value as one of the lines. 

<img src="./notes/bug_01.png" width="640" height="400">


