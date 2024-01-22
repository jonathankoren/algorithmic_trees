function findIntersectionHelp(center, scaledDivPoint, polygon, idx) =
  (idx >= (2 * len(polygon))) ?
    echo("!!!!! FAIL INTERSECT", center, scaledDivPoint, polygon, idx)
    [0, 0] :
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
         z = echo("FINDING INTERSECT", numParts, center, "points", points, "divPoints", divPoints, "scaledDivPoints", scaledDivPoints),
         intersections = [ for (i = [0:len(divPoints)-1]) findIntersection(center, scaledDivPoints[i], points) ]
        )
        (numParts == 1) ?
          [points] :
          [for (iii = [0:(numParts - 1)])
              subDividePointsBetween(center, intersections, points, iii) ];
