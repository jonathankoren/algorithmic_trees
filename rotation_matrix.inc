/**
 * Some useful rotation matrix functions.
 *
 * Created:  Tue Jan  9 23:00:20 PST 2024   Jonathan Koren
 */

/**
 * Multiply a 3D vector by a 3x3 matrix.
 * (Note, this can also work with a 4x4 matrix, because we just ignore the fourth row/col.)
 *
 * @param v   3D vector
 * @param m   3x3 (or 4x4) matrix
 * @return a 3D vector
 */
function vec3matrixMult3(v, m) =
    [
        (v[0] * m[0][0]) + (v[1] * m[0][1]) + (v[2] * m[0][2]),
        (v[0] * m[1][0]) + (v[1] * m[1][1]) + (v[2] * m[1][2]),
        (v[0] * m[2][0]) + (v[1] * m[2][1]) + (v[2] * m[2][2]),
    ];

/**
 * Multiply a 4x4 matrix by another 4x4 matrix.
 *
 * @param x   4x4 matrix
 * @param y   4x4 matrix
 * @return a 4x4 matrix
 */
function matrix4Mult(x, y) =
    [
        [
            (x[0][0] * y[0][0]) + (x[0][1] * y[1][0]) + (x[0][2] * y[2][0]) + (x[0][3] * y[3][0]),
            (x[0][0] * y[0][1]) + (x[0][1] * y[1][1]) + (x[0][2] * y[2][1]) + (x[0][3] * y[3][1]),
            (x[0][0] * y[0][2]) + (x[0][1] * y[1][2]) + (x[0][2] * y[2][2]) + (x[0][3] * y[3][2]),
            (x[0][0] * y[0][3]) + (x[0][1] * y[1][3]) + (x[0][2] * y[2][3]) + (x[0][3] * y[3][3]),
        ],
        [
            (x[1][0] * y[0][0]) + (x[1][1] * y[1][0]) + (x[1][2] * y[2][0]) + (x[1][3] * y[3][0]),
            (x[1][0] * y[0][1]) + (x[1][1] * y[1][1]) + (x[1][2] * y[2][1]) + (x[1][3] * y[3][1]),
            (x[1][0] * y[0][2]) + (x[1][1] * y[1][2]) + (x[1][2] * y[2][2]) + (x[1][3] * y[3][2]),
            (x[1][0] * y[0][3]) + (x[1][1] * y[1][3]) + (x[1][2] * y[2][3]) + (x[1][3] * y[3][3]),
        ],
        [
            (x[2][0] * y[0][0]) + (x[2][1] * y[1][0]) + (x[2][2] * y[2][0]) + (x[2][3] * y[3][0]),
            (x[2][0] * y[0][1]) + (x[2][1] * y[1][1]) + (x[2][2] * y[2][1]) + (x[2][3] * y[3][1]),
            (x[2][0] * y[0][2]) + (x[2][1] * y[1][2]) + (x[2][2] * y[2][2]) + (x[2][3] * y[3][2]),
            (x[2][0] * y[0][3]) + (x[2][1] * y[1][3]) + (x[2][2] * y[2][3]) + (x[2][3] * y[3][3]),

        ],
        [
            (x[3][0] * y[0][0]) + (x[3][1] * y[1][0]) + (x[3][2] * y[2][0]) + (x[3][3] * y[3][0]),
            (x[3][0] * y[0][1]) + (x[3][1] * y[1][1]) + (x[3][2] * y[2][1]) + (x[3][3] * y[3][1]),
            (x[3][0] * y[0][2]) + (x[3][1] * y[1][2]) + (x[3][2] * y[2][2]) + (x[3][3] * y[3][2]),
            (x[3][0] * y[0][3]) + (x[3][1] * y[1][3]) + (x[3][2] * y[2][3]) + (x[3][3] * y[3][3]),

        ],
    ];

/**
 * Add a 4x4 matrix to another 4x4 matrix.
 *
 * @param x   4x4 matrix
 * @param y   4x4 matrix
 * @return a 4x4 matrix
 */
function matrix4add(x, y) =
    [
        [ x[0][0] + y[0][0], x[0][1] + y[0][1], x[0][2] + y[0][2], x[0][3] + y[0][3] ],
        [ x[1][0] + y[1][0], x[1][1] + y[1][1], x[1][2] + y[1][2], x[1][3] + y[1][3] ],
        [ x[2][0] + y[2][0], x[2][1] + y[2][1], x[2][2] + y[2][2], x[2][3] + y[2][3] ],
        [ x[3][0] + y[3][0], x[3][1] + y[3][1], x[3][2] + y[3][2], x[3][3] + y[3][3] ],
    ];

/**
 * Construct a 4x4 translation matrix.
 *
 * @param vect    3D vector indicating the direction and magnitude of the translation
 * @return a 4x4 translation matrix
 */
function trans(vect) =
    [ [1, 0, 0, vect[0]],
      [0, 1, 0, vect[1]],
      [0, 0, 1, vect[2]],
      [0, 0, 0, 1]
    ];

/**
 * Construct a partial 4x4 translation matrix
 * Like trans(), but it has zeros on the main diagonal. This makes it useful to
 * be added to a rotation matrix.
 *
 * @param vect    3D vector indicating the direction and magnitude of the translation
 * @return a 4x4 translation matrix but with all zeros on the main diagonal
 */
function transFrag(vect) =
    [ [0, 0, 0, vect[0]],
      [0, 0, 0, vect[1]],
      [0, 0, 0, vect[2]],
      [0, 0, 0, 0]
    ];

/**
 * Construct a 4x4 rotation matrix for rotating around the Y axis.
 *
 * @param y_ang   Angle to rotate in degrees. 0 degrees is along +Z
 * @return a 4x4 matrix for rotating around the Y axis.
 */
function yrot(y_ang) =
    [ [ cos(y_ang), 0,  sin(y_ang), 0],
      [         0,  1,           0, 0],
      [-sin(y_ang), 0,  cos(y_ang), 0],
      [         0,  0,           0, 1]
    ];

/**
 * Construct a 4x4 rotation matrix for rotating around the Z axis.
 *
 * @param z_ang   Angle to rotate in degrees. 0 degrees is along +Y
 * @return a 4x4 matrix for rotating around the Z axis.
 */
function zrot(z_ang) =
    [ [ cos(z_ang), -sin(z_ang), 0, 0],
      [ sin(z_ang),  cos(z_ang), 0, 0],
      [         0,            0, 1, 0],
      [         0,            0, 0, 1]
    ];

/**
 * Creates a 4x4 combination rotation and translation matrix.
 *
 * @param y_ang         angle in degrees to rotate around the Y axis (0 deg = +Z axis)
 * @param z_ang         angle in degrees to rotate around the Z axis (0 deg = +Y axis)
 * @param translation   3D vector for the translation
 * @return a 4x4 rotation matrix for rotating and translation
 */
function makeRotationMatrix(y_ang, z_ang, translation) =
    matrix4add(matrix4Mult(zrot(z_ang), yrot(y_ang)), transFrag(translation));
