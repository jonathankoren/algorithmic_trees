 include <random.inc>;
include <rotation_matrix.inc>;
include <polygons.inc>
include <subdivide2d.inc>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Smoothness
$fn = 100;

// Set to nonzero for consistent random numbers
RANDOM_SEED = 0;        

// Maximum angle a branch can grow at in degrees. Y rotation only.
MAXIMUM_Y_ANGLE = 90;             

// Maximum change in angle in degrees around the Y axis.
MAXIMUM_Y_ANGLE_DELTA = 60;       

// Maximum change in angle in degrees around the Z axis.
MAXIMUM_Z_ANGLE_DELTA = 60;

// +/- to randomly perturb the calculated angle to the target when performing a Y rotation.
Y_ANGLE_NOISE = 90;             

// +/- to randomly perturb the calculated angle to the target when performing a Z rotation.
Z_ANGLE_NOISE = 180;             

// Maximum length of a segment.
MAXIMUM_LENGTH = 10;            

// Standard length of a segment.
STANDARD_LENGTH = 5;

// Minimum length of a segment.
MINIMUM_LENGTH = 2;             

// +/- percentage to randomly perturb the calculated length of the segment.
LENGTH_NOISE_PCT_DELTA = 0.3;   

// Probability to modify the Y rotation angle.
Y_ANGLE_NOISE_PROB = 0;         

// Probability to modify the Z rotation angle.
Z_ANGLE_NOISE_PROB = 0;         

// Probability to modify the length of the segment.
LENGTH_NOISE_PROB = 0;          

// Probability of splitting a branch after each segment.
SPLIT_PROB = 0.2;

// Minium number of splits (Should be greater than 1.)
MINIMUM_SPLIT = 2;

// Maximum number of splits.
MAXIMUM_SPLIT = 5;

// Target plane length (X direction)
TARGET_PLANE_LENGTH = 100;

// Target plane width (Y direction)
TARGET_PLANE_WIDTH = 100;

// Target plane  height (Z direction)
TARGET_PLANE_HEIGHT = 100;

// Show terminal nodes
DEBUG_SHOW_TERMINAL_NODES = true;

// Show target locations
DEBUG_SHOW_TARGET_LOCATIONS = true;

// Show tree (You will want this on)
DEBUG_SHOW_TREE = true;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function appendPlaneHeightSingle(point) = 
    [ point[0], point[1], TARGET_PLANE_HEIGHT ];

function appendPlaneHeight(points) = 
    [ for (i = [0:len(points)-1]) [ points[i][0], points[i][1], TARGET_PLANE_HEIGHT ] ];
        
        
function appendPlaneHeightListOfLists(listOfLists) =
    [ for (i = [0:len(listOfLists)-1]) appendPlaneHeight(listOfLists[i]) ];
    
function removeZSingle(point) =
    [ point[0], point[1] ];
    
function removeZ(points) =
    [ for (i = [0:len(points)-1]) [ points[i][0], points[i][1] ] ];
        
/**
 * Draw a segment at [0, 0, 0] oriented in the +Z direction, with the radii and length.
 *
 * @param length
 * @param startRadius
 * @param endRadius
 */
module drawSegment(length, startRadius, endRadius) {
    cylinder(length, startRadius, endRadius);
    
    difference() {
        sphere(startRadius);
        cylinder(length, startRadius, startRadius);
    }
    
    difference() {
        translate([0, 0, length])
        sphere(endRadius);
        cylinder(length, endRadius, endRadius);
    }
}

function targetPlanePoints() =
    [ [ -TARGET_PLANE_LENGTH / 2, -TARGET_PLANE_WIDTH / 2, TARGET_PLANE_HEIGHT ],
      [ -TARGET_PLANE_LENGTH / 2,  TARGET_PLANE_WIDTH / 2, TARGET_PLANE_HEIGHT ],
      [  TARGET_PLANE_LENGTH / 2,  TARGET_PLANE_WIDTH / 2, TARGET_PLANE_HEIGHT ],
      [  TARGET_PLANE_LENGTH / 2, -TARGET_PLANE_WIDTH / 2, TARGET_PLANE_HEIGHT ] ];

function perturbPercentage(rate, base, lowPercentage, highPercentage, randSeed) =
    let (trigVal = (randSeed == 0) ?
            rands(0, 1, 1)[0] :
            rands(0, 1, 1, randSeed)[0],
         pct = (randSeed == 0) ?
            rands(lowPercentage, highPercentage, 1)[0] :
            rands(lowPercentage, highPercentage, 1, randSeed)[0])
    (trigVal <  rate) ?
        pct * base :
        base;

function perturbAbsolute(rate, base, lowDelta, highDelta, randSeed) =
    let (trigVal = (randSeed == 0) ?
            rands(0, 1, 1)[0] :
            rands(0, 1, 1, randSeed)[0],
         delta = (randSeed == 0) ?
            rands(lowDelta, highDelta, 1)[0] :
            rands(lowDelta, highDelta, 1, randSeed)[0])
    (trigVal <  rate) ?
        delta + base :
        base;

/**
 * @param nodeId        Unique integer id for the segment
 * @param srcPoint      3D point indicating the center of the base of the segment
 * @param targetPoint   3D point indicating where the branch is trying to grow to
 * @param startRadius   Radius of the segment at the bottom
 * @param recursionTTL  Time To Live for recursion (i.e. Recursion stops at 0)
 * @param seq           Sequence id for generating random numbers
 */
function calcSegmentEndRadius(id, srcPoint, targetPoint, startRadius, recursionTTL, seq) =
    // FIXME: write
    startRadius;

/**
 * Calculates the typical length of a segment.
 *
 * @param srcPoint      3D point indicating the center of the base of the branch
 * @param targetPoint   3D point indicating where the branch is trying to grow to
 * @param recursionTTL  Time To Live for recursion (i.e. Recursion stops at 0)
 * @return The base length of the Segment
 */
function calcSegmentBaseLength(srcPoint, targetPoint, recursionTTL) =
    clamp(0, euclidDistance(srcPoint, targetPoint), MAXIMUM_LENGTH);
    
/**
 * Calcualtes the actual length of a segment.
 *
 * @param id            unique integer id for the Segment
 * @param srcPoint      3D point indicating the center of the base of the Segment
 * @param targetPoint   3D point indicating where the branch is trying to grow to
 * @param recursionTTL  Time To Live for recursion (i.e. Recursion stops at 0)
 * @param seq           Sequence id for generating random numbers
 */
function calcSegmentLength(nodeId, srcPoint, targetPoint, recursionTTL, seq) =
    let (seed = calcRandomSeed(nodeId, seq),
         baseLength = calcSegmentBaseLength(srcPoint, targetPoint, recursionTTL))
        clamp(MINIMUM_LENGTH,
            perturbPercentage(LENGTH_NOISE_PROB, baseLength, 1 - LENGTH_NOISE_PCT_DELTA, 1 + LENGTH_NOISE_PCT_DELTA, seed),
            MAXIMUM_LENGTH); 


function calcSegmentTargetYangle(srcPoint, targetPoint) =
    let (centTarget = subVects(targetPoint, srcPoint))
        acos(centTarget[2] / simpleEuclid(centTarget));
        

function calcSegmentTargetZangle(srcPoint, targetPoint) =
    let (centTarget = subVects(targetPoint, srcPoint))
        (centTarget[0] == 0) ? 
            0 :
            (srcPoint[0] <= targetPoint[0]) ?
                atan(centTarget[1] / centTarget[0]) :
                atan(centTarget[1] / centTarget[0]) + 180;

/**
 * @param id            unique integer id for the Segment
 * @param srcPoint      3D point indicating the center of the base of the Segment
 * @param targetPoint   3D point indicating where the branch is trying to grow to
 * @param startYangle   Angle in degrees (+Z = 0 deg) the Segment is initally rotated around the Y axis
 * @param recursionTTL  Time To Live for recursion (i.e. Recursion stops at 0)
 * @param seq           Sequence id for generating random numbers
 */
function calcSegmentYangle(nodeId, srcPoint, targetPoint, startYangle, recursionTTL, seq) =
    let (seed = calcRandomSeed(nodeId, seq),
         targetAngle = calcSegmentTargetYangle(srcPoint, targetPoint),
         deltaToTarget = targetAngle - startYangle,
         // FIXME: change this to take into account the recusionTTL and the distance to the target
         actualDelta = perturbAbsolute(Y_ANGLE_NOISE_PROB, deltaToTarget, -MAXIMUM_Y_ANGLE_DELTA, MAXIMUM_Y_ANGLE_DELTA, seed))
        clamp(0, startYangle + actualDelta, MAXIMUM_Y_ANGLE);
         
/**
 * @param id            unique integer id for the Segment
 * @param srcPoint      3D point indicating the center of the base of the Segment
 * @param targetPoint   3D point indicating where the branch is trying to grow to
 * @param startZangle   Angle in degrees (+X = 0 deg) the Segment is initially rotated around the Z axis
 * @param recursionTTL  Time To Live for recursion (i.e. Recursion stops at 0)
 * @param seq           Sequence id for generating random numbers
 */
function calcSegmentZangle(nodeId, srcPoint, targetPoint, startZangle, recursionTTL, seq) =
    let (seed = calcRandomSeed(nodeId, seq),
         targetAngle = calcSegmentTargetZangle(srcPoint, targetPoint),
         deltaToTarget = targetAngle - startZangle,
         // FIXME: change this to take into account the recusionTTL and the distance to the target
         actualDelta = perturbAbsolute(Z_ANGLE_NOISE_PROB, deltaToTarget, -MAXIMUM_Z_ANGLE_DELTA, MAXIMUM_Z_ANGLE_DELTA, seed))
        startZangle + actualDelta;
         
/**
 * Returns the end point of branch.
 *
 * @param mrot          A 4x4 rotation/translation matrix
 * @param length        Length of Segment
 * @return The 3D point where the branch ends.
 */
function calcSegmentEndPoint(mrot, length) =
    vec3matrixMult3([0, 0, length], mrot);

/**
 * Determines if the branch should stop growing.
 *
 * @param nodeId
 * @param endPoint
 * @param targetPoint
 * @param recursionTT
 * @param seq
 * @return True if the branch should stop grouwing.
 */
function branchRecursionStop(nodeId, endPoint, targetPoint, recursionTTL, seq) = 
    (recursionTTL <= 0) || (euclidDistance(endPoint, targetPoint) < MINIMUM_LENGTH);
    

/**
 * Determines the of branches the branch should split into. 
 * 1 indicates no split.
 *
 * @param nodeId
 * @param endPoint
 * @param targetPoint
 * @param recursionTT
 * @param seq
 * @return the number of splits the branch undergows. (1 indicates no split)
 */
 /*
function numSplits(nodeId, endPoint, targetPoint, recursionTTL, seq) = 
    (randss(0, 1, nodeId, seq) < SPLIT_PROB) ?
        round(randss(MINIMUM_SPLIT, MAXIMUM_SPLIT, nodeId, seq + 1))  : 
        1;
*/
//function numSplits(nodeId, endPoint, targetPoint, recursionTTL, seq) = 
//    ((recursionTTL >= 4) && (recursionTTL % 2 == 0)) ? 2 : 1;
function numSplits(nodeId, endPoint, targetPoint, recursionTTL, seq) = 
    (recursionTTL == 8) ? 
        4 :
    (recursionTTL == 6) ? 
        4 :
        (recursionTTL == 4) ? 
            4 :
            1;

function calculateNewTargetPlanes3D(numOfSplits, targetPlanePoints) =
    appendPlaneHeightListOfLists(subDivideFunct(numOfSplits, removeZ(targetPlanePoints), oldCenter2D));

/**
 * @param nodeId        unique integer id for the Segment
 * @param srcPoint      3D point indicating the center of the base of the Segment
 * @param targetPoint   3D point indicating where the branch is trying to grow to
 * @param startRadius   Radius of the Segment at the bottom.drawSegment
 * @param startYangle   Angle in degrees (+Z = 0 deg) the Segment is initally rotated around the Y axis
 * @param startZangle   Angle in degrees (+X = 0 deg) the Segment is initially rotated around the Z axis
 * @param recursionTTL  Time To Live for recursion (i.e. Recursion stops at 0)
 */
module branch(nodeId, srcPoint, targetPlane3D, startRadius, startYangle, startZangle, recursionTTL, oldNodeId) {
    kolor = COLORS[recursionTTL % len(COLORS)];
    targetPoint = centroid(targetPlane3D);
//    echo(nodeId, "oldNodeId", oldNodeId);
//    echo(nodeId, kolor, recursionTTL);
//    echo(nodeId, "srcPoint", srcPoint);
//    echo(nodeId, "targetPlane3D", targetPlane3D);
//    echo(nodeId, "targetPoint", targetPoint);
    endRadius = calcSegmentEndRadius(nodeId, srcPoint, targetPoint, startRadius, recursionTTL, 0);
//    echo(nodeId, "distance", euclidDistance(srcPoint, targetPoint));
    length = calcSegmentLength(nodeId, srcPoint, targetPoint, recursionTTL, 1);
//    echo(nodeId, "length", length);
    
//    echo(nodeId, "startYangle", startYangle);
    yangleTarget = calcSegmentTargetYangle(srcPoint, targetPoint);
//    echo(nodeId, "yangleTarget", yangleTarget);
    
    yangle = calcSegmentYangle(nodeId, srcPoint, targetPoint, startYangle, recursionTTL, 2);
//    echo(nodeId, "yangle", yangle);
    zangle = calcSegmentZangle(nodeId, srcPoint, targetPoint, startZangle, recursionTTL, 3);
//    echo(nodeId, "zangle", zangle);
    
    mrot = makeRotationMatrix(yangle, zangle, srcPoint);
    endPoint = addVects(calcSegmentEndPoint(mrot, length), srcPoint);
//    echo(nodeId, "mrot", mrot);
//    echo(nodeId, "endPoint", endPoint);

    debugColor = ((srcPoint[0] < targetPoint[0]) && (srcPoint[1] < targetPoint[1])) ? "black" :
                 ((srcPoint[0] > targetPoint[0]) && (srcPoint[1] < targetPoint[1])) ? "red" :
                 ((srcPoint[0] > targetPoint[0]) && (srcPoint[1] > targetPoint[1])) ? "blue" :
                 ((srcPoint[0] < targetPoint[0]) && (srcPoint[1] > targetPoint[1])) ? "green" :
                 "pink";
//    echo(nodeId, "debugColor", debugColor, srcPoint, targetPoint);
    

    if (DEBUG_SHOW_TREE) {
        multmatrix(mrot)
        color(debugColor)
        drawSegment(length, startRadius, endRadius);
    }
    
    if (!branchRecursionStop(nodeId, endPoint, targetPoint, recursionTTL, 4)) {
        numOfSplits = numSplits(nodeId, endPoint, targetPoint, recursionTTL, 5);
//        echo(nodeId, "numOfSplits", numOfSplits); 
        newTargetPlanes3D = calculateNewTargetPlanes3D(numOfSplits, targetPlane3D);
//        echo(nodeId, "newTargetPlanes3D", newTargetPlanes3D);

        if ((numOfSplits > 1) && DEBUG_SHOW_TARGET_LOCATIONS) {
            for (i = [0:len(newTargetPlanes3D)-1]) {
                translate(centroid(newTargetPlanes3D[i]))
                color(COLORS[recursionTTL  % len(COLORS)])
                sphere(1);
            }
            echo(nodeId, "targets are", COLORS[recursionTTL % len(COLORS)]);
        }

        for (i = [1:numOfSplits]) {
            newNodeId = (nodeId * pow(10, i - 1)) + 1;
//            echo("NEW", newNodeId, "<-", nodeId, i, newTargetPlanes3D[i - 1]);            
            branch(newNodeId, endPoint, newTargetPlanes3D[i - 1], endRadius, yangle, zangle, recursionTTL - 1, nodeId);
        }

    } 
    else { 
//        echo(nodeId, "!!!!!", (recursionTTL < 1)? "EXHAUSTED" : "STOP", "!!!!!");
        if (DEBUG_SHOW_TERMINAL_NODES) {
//        translate(endPoint) color((recursionTTL < 1)? kolor : "black") cube([3*startRadius, 3*startRadius, 3*startRadius], true);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

COLORS = ["yellow", "blue", "lime", "purple", "green", "orange", "red"];

nodeId = 2;
srcPoint = [0, 0, 0];
startRadius = 1;
startYangle = 0;
startZangle = 0;
recursionTTL = 10;
oldCenter2D = [0, 1000];

branch(nodeId, srcPoint, targetPlanePoints(), startRadius, startYangle, startZangle, recursionTTL, -1);


