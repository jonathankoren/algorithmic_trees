/**
 * Sum all the idx coordinates from a list of points
 *
 * @param i       Recursion index. Set as 0 for the first call.
 * @param idx     Index of the coordinate to add (e.g. 0 = X, 1 = Y, 2 = Z)
 * @param points  List of points. Each point is an n-dimensional array.
 * @return The sum of all the idx coordinates in points, starting with point i.
 */
function sumCoords(i, idx, points) =
    (i >= len(points)) ? 0 : points[i][idx] + sumCoords(i + 1, idx, points);

/**
 * Calculate the centroid of a polygon.
 *
 * @param points    A list of n-dimensional points. Each point is an array of length 2.
 * @return The 2D point defining the centroid of the polyhedral defined by points.
 */
function centroid(points) =
  (len(points) == 0) ?
    0 :
    [ for (idx = [0 : len(points[0]) - 1]) sumCoords(0, idx, points) / len(points) ];

/**
 * Calculates a partial dot product of two vectors.
 *
 * @param v1      An n-dimensional vector starting at [0, 0]
 * @param v2      Another n-dimensional vector starting at [0, 0]
 * @param idx     Index of the coordinate to add (0 = X, 1 = Y, 2 = Z)
 * @returns The partial dot product of v1 and v2 by summing the coordinates starting at idx.
 */
function dotProductHelp(v1, v2, idx) =
    (idx >= len(v1)) ? 0 : (v1[idx] * v2[idx]) + dotProductHelp(v1, v2, idx + 1);

/**
 * Calculates the dot product of two n-dimensional vectors that start at the origin.
 *
 * @param v1      An n-dimensional vector starting at the origin
 * @param v2      Another n-dimensional vector starting at the origin
 * @return The dot product of v1 and v2.
 */
function dotProduct(v1, v2) =
    dotProductHelp(v1, v2, 0);

/**
 * Find the largest `idx`th coordinate in a list a points starting with point i.
 *
 * @param
 * @param idx         The cooordinate to find the maximum of (0 = X, 1 = Y, 2 = Z)
 * @param points      A list of n-dimensional points
 * @return The largest `idx`th coordinate in the list of points starting with point i
 */
function maxCoordHelp(i, idx, points) =
    (i == len(points) - 1) ? points[i][idx] :
        max(points[i][idx], maxCoord(i + 1, idx, points));

/**
 * Find the largest `idx`th coordinate in a list a points.
 *
 * @param idx         The cooordinate to find the maximum of (0 = X, 1 = Y, 2 = Z)
 * @param points      A list of n-dimensional points
 * @return The largest `idx`th coordinate in the list of points.
 */
function maxCoord(idx, points) =
    maxCoordHelp(0, idx, points);

/**
 * Determine of two points are coincident.
 *
 * @param p1      An n-dimensional point
 * @param p2      Another n-dimensional point
 * @param idx     Coordinate to check (0 = X, 1 = Y, 2 = Z)
 * @returns True of the two points are coincident, otherwise False
 */
function pointsEqualHelp(p1, p2, idx) =
  (idx >= len(p1)) ?
    true :
    (p1[idx] == p2[idx]) && pointsEqualHelp(p1, p2, idx + 1);

/**
 * Determine of two points are coincident.
 *
 * @param p1      An n-dimensional point
 * @param p2      Another n-dimensional point
 * @returns True of the two points are coincident, otherwise False
 */
function pointsEqual(p1, p2) =
    pointsEqualHelp(p1, p2, 0);

/**
 * Calculates the determinate of a 2x2 matrix.
 * | a b |
 * | c d |
 *
 * @param a
 * @param b
 * @param c
 * @param d
 * @return The determinate of the 2x2 matrix
 */
function det2D(a, b, c ,d) =
  (a * d) - (b * c);

/**
 * Calculates the determinate of a 2x2 matrix.
 *
 * @param m     A 2x2 matrix [[a, b], [c, d]]
 * @return The determinate of the 2x2 matrix
 */
function det2Dmatrix(m) =
  det2D(m[0][0], m[0][1], m[1][0], m[1][1]);

/**
 * Scalar multiplies a vector
 *
 * @param k     Scalar to multiply the vector by
 * @param v     An n-dimensional vector
 * @return V but with every dimension k-times bigger.
 */
function scalarMult(k, v) =
  [ for (i = [0:len(v) - 1]) v[i] * k ];

/**
 * Constrains a value to a closed interval.
 *
 * @param mmin    minmum acceptable value
 * @param v       the value to constrain
 * @param mmax    maximum acceptable value
 *
 * @return mmin if v < min, mmax if v > mmax, otherwise v
 */
function clamp(mmin, v, mmax) =
  min(max(v, mmin), mmax);

/**
 * Adds v2 from v1
 *
 * @param v1      An n-dimensional vector
 * @param v2      Another n-dimensional vector
 * @return v1 + v2
 */
function addVects(v1, v2) =
  [ for (i = [0:len(v1)-1]) v1[i] + v2[i] ];

/**
 * Subtracts v2 from v1
 *
 * @param v1      An n-dimensional vector
 * @param v2      Another n-dimensional vector
 * @return v1 - v2
 */
function subVects(v1, v2) =
  [ for (i = [0:len(v1)-1]) v1[i] - v2[i] ];


/**
 * Internal helper function for finding a 2D intersection point
 *
 * @param p1    Point 1 on line 1
 * @param p2    Point 2 on line 1
 * @param p3    Point 1 on line 2
 * @param p4    Point 2 on line 2
 */
function intersectHelpT(p1, p2, p3, p4) =
    det2D(p1[0] - p3[0], p3[0] - p4[0],
          p1[1] - p3[1], p3[1] - p4[1]) /
    det2D(p1[0] - p2[0], p3[0] - p4[0],
          p1[1] - p2[1], p3[1] - p4[1]);

/**
 * Internal helper function for finding a 2D intersection point
 *
 * @param p1    Point 1 on line 1
 * @param p2    Point 2 on line 1
 * @param p3    Point 1 on line 2
 * @param p4    Point 2 on line 2
 */
function intersectHelpU(p1, p2, p3, p4) =
    det2D(p1[0] - p3[0], p1[0] - p2[0],
          p1[1] - p3[1], p1[1] - p2[1]) /
    det2D(p1[0] - p2[0], p3[0] - p4[0],
          p1[1] - p2[1], p3[1] - p4[1]);

/**
 * Checks if a line segment defined by 2D points p1 and p2, intersects with another
 * line segment defined by 2D points p3 and p4.
 *
 * @see intersection()
 *
 * @param p1    Point 1 on line 1
 * @param p2    Point 2 on line 1
 * @param p3    Point 1 on line 2
 * @param p4    Point 2 on line 2
 * @return True if (p1, p2) intersects with (p3, p4).
 */
function intersectHappens(p1, p2, p3, p4) =
    (0 <= intersectHelpT(p1, p2, p3, p4)) &&
    (intersectHelpT(p1, p2, p3, p4) <= 1) &&
    (0 <= intersectHelpU(p1, p2, p3, p4)) &&
    (intersectHelpU(p1, p2, p3, p4) <= 1);

/**
 * Returns the 2D point where the line segment defined 2D points p1 and p2, intersects
 * with another line segment defined by 2D points p3 and p4.
 *
 * @note This point *may not* lie on either line segment if the line segments do
 * not actually intersect.
 * @see intersectHappens()
 *
 * @param p1    Point 1 on line 1
 * @param p2    Point 2 on line 1
 * @param p3    Point 1 on line 2
 * @param p4    Point 2 on line 2
 * @return The 2D point where (p1, p2) intersects (p3, p4).
 */
 function intersection(p1, p2, p3, p4) =
    [ (p1[0] + (intersectHelpT(p1, p2, p3, p4) * (p2[0] - p1[0]))),
      (p1[1] + (intersectHelpT(p1, p2, p3, p4) * (p2[1] - p1[1]))) ];


/**
 * Scales a line segment.
 *
 * @param k     Scalar to multiply by
 * @param p1    n-dimensional point indicating the start of the line segment
 * @param p2    n-dimeinstional point indicating the end of the line segment
 * @return
 */
function scaleVector(k, p1, p2) =
    addVects(scalarMult(k, subVects(p2, p1)), p1);

/**
 * Rotate a 2D point around the origin.
 *
 * @param p       A 2D point
 * @param angle   number of degrees to rotate around origin. Positive is clockwise.
 * @return a 2D point that has been rotated around the origin
 */
function rotate2D(p, angle) =
    [ (p[0] * cos(angle)) - (p[1] * sin(angle)),
      (p[0] * sin(angle)) + (p[1] * cos(angle)) ];

function euclidDistanceHelp(p1, p2, idx) =
  (idx >= len(p1)) ? 0 : pow(subVects(p1, p2)[idx], 2) + euclidDistanceHelp(p1, p2, idx + 1);

/**
 * Calculates the distance between two points.
 *
 * @param p1      An n-dimensional point
 * @param p2      Another n-dimensional point
 * @return The Euclidian distance between these two points.
 */
function euclidDistance(p1, p2) =
  sqrt(euclidDistanceHelp(p1, p2, 0));

function simpleEuclidHelp(v, idx) =
  (idx >= len(v)) ? 0 : (v[idx] * v[idx]) + simpleEuclidHelp(v, idx + 1);

  /**
   * Calculates the length of a vector starting at the origin.
   *
   * @param v      An n-dimensional vector starting the origin
   * @return The Euclidian length (L2 norm) of the vector.
   */
function simpleEuclid(v) =
  sqrt(simpleEuclidHelp(v, 0));

function cosPoints(center, p1, p2) =
    let (v1 = subVects(p1, center),
         v2 = subVects(p2, center))
        dotProduct(v1, v2) / (simpleEuclid(v1) * simpleEuclid(v2));

function angle(center, p1, p2) =
    acos(clamp(-1, cosPoints(center, p1, p2), 1));

function crossProd2D(p1, p2) =
    (p1[0] * p2[1]) - (p1[1] * p2[0]);

function crossProd2DPoints(center, p1, p2) =
    crossProd2D(subVects(p1, center), subVects(p2, center));
