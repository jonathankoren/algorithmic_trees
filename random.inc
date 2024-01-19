/**
 * Some useful functions for getting consistent random numbers out of OpenSCAD.
 *
 * Created:  Tue Jan  9 23:00:20 PST 2024   Jonathan Koren
 */

 RANDOM_SEED = 0;   // Set seed to > 0 to get stable numbers out of the PRNG.

/**
 * Calculates a random seed from a node ID and sequence number.
 *
 * @param nodeId  a unique number identifying which recursion element this is
 * @param seq     sequence number identifying which draw this is for nodeId
 * @return A random number on [low, high]
 */
function calcRandomSeed(nodeId, seq) =
    (RANDOM_SEED == 0) ?
      //echo("calcRandomSeed", "PURE RANDOM")
      rands(0,1,1)[0] :
      //echo("calcRandomSeed", "nodeId", nodeId, "seq", seq, "seed", (RANDOM_SEED + pow(nodeId, seq)))
      RANDOM_SEED + pow(nodeId, seq);


/**
 * Returns a random number on [low, high].
 *
 * @param low     lower bound on random number
 * @param high    upper bound on random number
 * @param nodeId  a unique number identifying which recursion element this is
 * @param seq     sequence number identifying which draw this is for nodeId
 * @return A random number on [low, high]
 *
 * Example
 * RANDOM_SEED = 10;
 * nodeId = 1;
 * rand1 = randss(0, 10, nodeId, 1);      // first random number
 * rand2 = randss(0, 10, nodeId, 2);      // will be different from rand1
 * rand3 = randss(0, 10, nodeId, 1);      // will be identical to rand1 since nodeId and seq are equal
 * rand4 = randss(0, 10, nodeId + 1, 1);  // will be different from rand1 since nodeIds are different
 */
function randss(low, high, nodeId, seq) =
    (RANDOM_SEED > 0) ? rands(low, high, 1, calcRandomSeed(nodeId, seq))[0] : rands(low, high, 1)[0];
