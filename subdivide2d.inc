function findIntersectionHelp(center, scaledDivPoint, polygon, idx) =
    (intersectHappens(center, scaledDivPoint, polygon[idx], polygon[(idx + 1) % len(polygon)])) ?
        intersection(center, scaledDivPoint, polygon[idx], polygon[(idx + 1) % len(polygon)]) :
        findIntersectionHelp(center, scaledDivPoint, polygon, idx + 1);

function findIntersection(center, scaledDivPoint, polygon) =
   findIntersectionHelp(center, scaledDivPoint, polygon, 0);


function divisionAngleStep(order) =
    360 / order;

function divisionAngle(i, order) =
    divisionAngle(order) * i;


function subDividePointsBetweenInterior(center, intersections, polyPoints, idx) =
    let (startIntersectIdx = idx,
         endIntersectIdx = (idx + 1) % len(intersections),
         validAngle = angle(center, intersections[startIntersectIdx], intersections[endIntersectIdx])
        )

        [ for (i = [0:len(polyPoints) - 1])
            if ((crossProd2DPoints(center, intersections[startIntersectIdx], polyPoints[i]) < 0) &&
                (angle(center, intersections[startIntersectIdx], polyPoints[i]) < validAngle))
            polyPoints[i]
        ];

function subDividePointsBetween(center, intersections, polyPoints, idx) =
    let (startIntersectIdx = idx,
         endIntersectIdx = (idx + 1) % len(intersections),
         validAngle = angle(center, intersections[startIntersectIdx], intersections[endIntersectIdx])
        )
        concat(concat([center, intersections[startIntersectIdx]], subDividePointsBetweenInterior(center, intersections, polyPoints, idx)),
            [intersections[endIntersectIdx]]);

function subDivideFunct(numParts, points, oldCenter) =
    let (angle = divisionAngleStep(numParts),
         center = centroid(points),
         cp = subVects(oldCenter, center),
         divPoints = [for (i = [0:numParts-1]) addVects(rotate2D(cp, -angle * i), center)],
         scaledDivPoints = [ for (i = [0:len(divPoints)-1]) scaleVector(10, center, divPoints[i]) ],
         intersections = [ for (i = [0:len(divPoints)-1]) findIntersection(center, scaledDivPoints[i], points) ]
        )
        (numParts == 1) ?
          [points] :
          [for (iii = [0:(numParts - 1)])
              subDividePointsBetween(center, intersections, points, iii) ];


module subDivideHelp(level, color, maxDepth, boundaryPoints, oldCenter) {
    echo("subDivideHelp", level, color, maxDepth, boundaryPoints, oldCenter);
    if (level <= maxDepth) {
        drawPoly(level, color, boundaryPoints, oldCenter);
        numParts = ((level % 2) == 0) ? 3 : 4;
        center = centroid(boundaryPoints);
        subDivisions = subDivideFunct(numParts, boundaryPoints, oldCenter);
        echo("subDivisions", subDivisions);
        //for (iii = [0 : numParts - 1])
        //    subDivideHelp(level + 1, COLORS[(iii + level)  % len(COLORS)], maxDepth, subDivisions[iii], center);
        subDivideHelp(level + 1, COLORS[level % len(COLORS)], maxDepth, subDivisions[0], center);
    }
}

module subDivide(maxDepth, boundaryPoints, oldCenter) {
    subDivideHelp(1, "gold", maxDepth, boundaryPoints, oldCenter);
}
