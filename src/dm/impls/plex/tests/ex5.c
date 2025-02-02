static char help[] = "Tests for creation of hybrid meshes\n\n";

/* TODO
 - Propagate hybridSize with distribution
 - Test with multiple fault segments
 - Test with embedded fault
 - Test with multiple faults
 - Move over all PyLith tests?
*/

#include <petscdmplex.h>
#include <petscds.h>
#include <petsc/private/dmpleximpl.h>
/* List of test meshes

Triangle
--------
Test 0:
Two triangles sharing a face

        4
      / | \
     8  |  9
    /   |   \
   2  0 7 1  5
    \   |   /
     6  |  10
      \ | /
        3

should become two triangles separated by a zero-volume cell with 4 vertices

        5--16--8              4--12--6 3
      / |      | \          / |      | | \
    11  |      |  12       9  |      | |  4
    /   |      |   \      /   |      | |   \
   3  0 10  2 14 1  6    2  0 8  1  10 6 0  1
    \   |      |   /      \   |      | |   /
     9  |      |  13       7  |      | |  5
      \ |      | /          \ |      | | /
        4--15--7              3--11--5 2

Test 1:
Four triangles sharing two faces which are oriented against each other

          9
         / \
        /   \
      17  2  16
      /       \
     /         \
    8-----15----5
     \         /|\
      \       / | \
      18  3  12 |  14
        \   /   |   \
         \ /    |    \
          4  0 11  1  7
           \    |    /
            \   |   /
            10  |  13
              \ | /
               \|/
                6

Fault mesh

0 --> 0
1 --> 1
2 --> 2
3 --> 3
4 --> 5
5 --> 6
6 --> 8
7 --> 11
8 --> 15

       2
       |
  6----8----4
       |    |
       3    |
          0-7-1
            |
            |
            5

should become four triangles separated by two zero-volume cells with 4 vertices

          11
          / \
         /   \
        /     \
      22   2   21
      /         \
     /           \
   10-----20------7
28  |     5    26/ \
   14----25----12   \
     \         /|   |\
      \       / |   | \
      23  3  17 |   |  19
        \   /   |   |   \
         \ /    |   |    \
          6  0 24 4 16 1  9
           \    |   |    /
            \   |   |   /
            15  |   |  18
              \ |   | /
               \|   |/
               13---8
                 27

Tetrahedron
-----------
Test 0:
Two tets sharing a face

 cell   5 _______    cell
 0    / | \      \       1
    16  |  18     22
    /8 19 10\      \
   2-15-|----4--21--6
    \  9| 7 /      /
    14  |  17     20
      \ | /      /
        3-------

should become two tetrahedrons separated by a zero-volume cell with 3 faces/3 edges/6 vertices

 cell   6 ___36___10______    cell
 0    / | \        |\      \     1
    24  |  26      | 32     30
    /12 27 14\    33  \      \
   3-23-|----5--35-|---9--29--7
    \ 13| 11/      |18 /      /
    22  |  25      | 31     28
      \ | /        |/      /
        4----34----8------
         cell 2

In parallel,

 cell   5 ___28____8      4______    cell
 0    / | \        |\     |\      \     0
    19  |   21     | 24   | 13  6  11
    /10 22 12\    25  \   |8 \      \
   2-18-|----4--27-|---7  14  3--10--1
    \ 11| 9 /      |13 /  |  /      /
    17  |  20      | 23   | 12  5  9
      \ | /        |/     |/      /
        3----26----6      2------
         cell 1

Test 1:
Four tets sharing two faces

Cells:    0-3,4-5
Vertices: 6-15
Faces:    16-29,30-34
Edges:    35-52,53-56

Quadrilateral
-------------
Test 0:
Two quads sharing a face

   5--10---4--14---7
   |       |       |
  11   0   9   1  13
   |       |       |
   2---8---3--12---6

should become two quads separated by a zero-volume cell with 4 vertices

   6--13---5-20-10--17---8    5--10---4-14--7  4---7---2
   |       |     |       |    |       |     |  |       |
  14   0  12  2 18   1  16   11   0   9  1 12  8   0   6
   |       |     |       |    |       |     |  |       |
   3--11---4-19--9--15---7    2---8---3-13--6  3---5---1

Test 1:

Original mesh with 9 cells,

  9 ----10 ----11 ----12
  |      |      |      |
  |      |      |      |
  |      |      |      |
  |      |      |      |
 13 ----14 ----15 ----16
  |      |      |      |
  |      |      |      |
  |      |      |      |
  |      |      |      |
 17 ----18 ----19 ----20
  |      |      |      |
  |      |      |      |
  |      |      |      |
  |      |      |      |
 21 ----22 ----23 ----24

After first fault,

 12 ----13 ----14-28 ----15
  |      |      |  |      |
  |  0   |  1   | 9|  2   |
  |      |      |  |      |
  |      |      |  |      |
 16 ----17 ----18-29 ----19
  |      |      |  |      |
  |  3   |  4   |10|  5   |
  |      |      |  |      |
  |      |      |  |      |
 20 ----21-----22-30 ----23
  |      |      |  \--11- |
  |  6   |  7   |     8   |
  |      |      |         |
  |      |      |         |
 24 ----25 ----26--------27

After second fault,

 14 ----15 ----16-30 ----17
  |      |      |  |      |
  |  0   |  1   | 9|  2   |
  |      |      |  |      |
  |      |      |  |      |
 18 ----19 ----20-31 ----21
  |      |      |  |      |
  |  3   |  4   |10|  5   |
  |      |      |  |      |
  |      |      |  |      |
 33 ----34-----24-32 ----25
  |  12  | 13 / |  \-11-- |
 22 ----23---/  |         |
  |      |   7  |     8   |
  |  6   |      |         |
  |      |      |         |
  |      |      |         |
 26 ----27 ----28--------29

Hexahedron
----------
Test 0:
Two hexes sharing a face

cell   9-----31------8-----42------13 cell
0     /|            /|            /|     1
    32 |   15      30|   21      41|
    /  |          /  |          /  |
   6-----29------7-----40------12  |
   |   |     18  |   |     24  |   |
   |  36         |  35         |   44
   |19 |         |17 |         |23 |
  33   |  16    34   |   22   43   |
   |   5-----27--|---4-----39--|---11
   |  /          |  /          |  /
   | 28   14     | 26    20    | 38
   |/            |/            |/
   2-----25------3-----37------10

should become two hexes separated by a zero-volume cell with 8 vertices

                         cell 2
cell  10-----41------9-----62------18----52------14 cell
0     /|            /|            /|            /|     1
    42 |   20      40|  32       56|   26      51|
    /  |          /  |          /  |          /  |
   7-----39------8-----61------17--|-50------13  |
   |   |     23  |   |         |   |     29  |   |
   |  46         |  45         |   58        |   54
   |24 |         |22 |         |30 |         |28 |
  43   |  21    44   |        57   |   27   53   |
   |   6-----37--|---5-----60--|---16----49--|---12
   |  /          |  /          |  /          |  /
   | 38   19     | 36   31     | 55    25    | 48
   |/            |/            |/            |/
   3-----35------4-----59------15----47------11

In parallel,

                         cell 2
cell   9-----31------8-----44------13     8----20------4  cell
0     /|            /|            /|     /|           /|     1
    32 |   15      30|  22       38|   24 |  10      19|
    /  |          /  |          /  |   /  |         /  |
   6-----29------7-----43------12  |  7----18------3   |
   |   |     18  |   |         |   |  |   |    13  |   |
   |  36         |  35         |   40 |  26        |   22
   |19 |         |17 |         |20 |  |14 |        |12 |
  33   |  16    34   |        39   |  25  |  11   21   |
   |   5-----27--|---4-----42--|---11 |   6----17--|---2
   |  /          |  /          |  /   |  /         |  /
   | 28   14     | 26   21     | 37   |23     9    | 16
   |/            |/            |/     |/           |/
   2-----25------3-----41------10     5----15------1

Test 1:

*/

typedef struct {
  PetscInt  debug;         /* The debugging level */
  PetscInt  dim;           /* The topological mesh dimension */
  PetscBool cellSimplex;   /* Use simplices or hexes */
  PetscBool testPartition; /* Use a fixed partitioning for testing */
  PetscInt  testNum;       /* The particular mesh to test */
} AppCtx;

static PetscErrorCode ProcessOptions(MPI_Comm comm, AppCtx *options)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  options->debug         = 0;
  options->dim           = 2;
  options->cellSimplex   = PETSC_TRUE;
  options->testPartition = PETSC_TRUE;
  options->testNum       = 0;

  ierr = PetscOptionsBegin(comm, "", "Meshing Problem Options", "DMPLEX");CHKERRQ(ierr);
  ierr = PetscOptionsBoundedInt("-debug", "The debugging level", "ex5.c", options->debug, &options->debug, NULL,0);CHKERRQ(ierr);
  ierr = PetscOptionsRangeInt("-dim", "The topological mesh dimension", "ex5.c", options->dim, &options->dim, NULL,1,3);CHKERRQ(ierr);
  ierr = PetscOptionsBool("-cell_simplex", "Use simplices if true, otherwise hexes", "ex5.c", options->cellSimplex, &options->cellSimplex, NULL);CHKERRQ(ierr);
  ierr = PetscOptionsBool("-test_partition", "Use a fixed partition for testing", "ex5.c", options->testPartition, &options->testPartition, NULL);CHKERRQ(ierr);
  ierr = PetscOptionsBoundedInt("-test_num", "The particular mesh to test", "ex5.c", options->testNum, &options->testNum, NULL,0);CHKERRQ(ierr);
  ierr = PetscOptionsEnd();CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

static PetscErrorCode CreateSimplex_2D(MPI_Comm comm, PetscInt testNum, DM *dm)
{
  DM             idm;
  PetscInt       p;
  PetscMPIInt    rank;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = MPI_Comm_rank(comm, &rank);CHKERRMPI(ierr);
  if (rank == 0) {
    switch (testNum) {
    case 0:
    {
      PetscInt    numPoints[2]        = {4, 2};
      PetscInt    coneSize[6]         = {3, 3, 0, 0, 0, 0};
      PetscInt    cones[6]            = {2, 3, 4,  5, 4, 3};
      PetscInt    coneOrientations[6] = {0, 0, 0,  0, 0, 0};
      PetscScalar vertexCoords[8]     = {-0.5, 0.5, 0.0, 0.0, 0.0, 1.0, 0.5, 0.5};
      PetscInt    markerPoints[8]     = {2, 1, 3, 1, 4, 1, 5, 1};
      PetscInt    faultPoints[2]      = {3, 4};

      ierr = DMPlexCreateFromDAG(*dm, 1, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
      for (p = 0; p < 4; ++p) {ierr = DMSetLabelValue(*dm, "marker", markerPoints[p*2], markerPoints[p*2+1]);CHKERRQ(ierr);}
      for (p = 0; p < 2; ++p) {ierr = DMSetLabelValue(*dm, "fault", faultPoints[p], 1);CHKERRQ(ierr);}
      ierr = DMSetLabelValue(*dm, "material", 0, 1);CHKERRQ(ierr);
      ierr = DMSetLabelValue(*dm, "material", 1, 2);CHKERRQ(ierr);
    }
    break;
    case 1:
    {
      PetscInt    numPoints[2]         = {6, 4};
      PetscInt    coneSize[10]         = {3, 3, 3, 3, 0, 0, 0, 0, 0, 0};
      PetscInt    cones[12]            = {4, 6, 5,  5, 6, 7,  8, 5, 9,  8, 4, 5};
      PetscInt    coneOrientations[12] = {0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0};
      PetscScalar vertexCoords[12]     = {-1.0, 0.0, 0.0, 1.0, 0.0, -1.0, 1.0, 0.0, -2.0, 1.0, -1.0, 2.0};
      PetscInt    markerPoints[6]      = {4, 1, 6, 1, 8, 1};
      PetscInt    faultPoints[3]       = {5, 6, 8};

      ierr = DMPlexCreateFromDAG(*dm, 1, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
      for (p = 0; p < 3; ++p) {ierr = DMSetLabelValue(*dm, "marker", markerPoints[p*2], markerPoints[p*2+1]);CHKERRQ(ierr);}
      for (p = 0; p < 3; ++p) {ierr = DMSetLabelValue(*dm, "fault", faultPoints[p], 1);CHKERRQ(ierr);}
      ierr = DMSetLabelValue(*dm, "material", 0, 1);CHKERRQ(ierr);ierr = DMSetLabelValue(*dm, "material", 3, 1);CHKERRQ(ierr);
      ierr = DMSetLabelValue(*dm, "material", 1, 2);CHKERRQ(ierr);ierr = DMSetLabelValue(*dm, "material", 2, 2);CHKERRQ(ierr);
    }
    break;
    default:
      SETERRQ1(comm, PETSC_ERR_ARG_OUTOFRANGE, "No test mesh %d", testNum);
    }
  } else {
    PetscInt numPoints[3] = {0, 0, 0};

    ierr = DMPlexCreateFromDAG(*dm, 1, numPoints, NULL, NULL, NULL, NULL);CHKERRQ(ierr);
    ierr = DMCreateLabel(*dm, "fault");CHKERRQ(ierr);
  }
  ierr = DMPlexInterpolate(*dm, &idm);CHKERRQ(ierr);
  ierr = DMViewFromOptions(idm, NULL, "-in_dm_view");CHKERRQ(ierr);
  ierr = DMDestroy(dm);CHKERRQ(ierr);
  *dm  = idm;
  PetscFunctionReturn(0);
}

static PetscErrorCode CreateSimplex_3D(MPI_Comm comm, AppCtx *user, DM dm)
{
  PetscInt       depth = 3, testNum  = user->testNum, p;
  PetscMPIInt    rank;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = MPI_Comm_rank(comm, &rank);CHKERRMPI(ierr);
  if (rank == 0) {
    switch (testNum) {
    case 0:
    {
      PetscInt    numPoints[4]         = {5, 7, 9, 2};
      PetscInt    coneSize[23]         = {4, 4, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2};
      PetscInt    cones[47]            = {7, 8, 9, 10,  11, 10, 13, 12,  15, 17, 14,  16, 18, 15,  14, 19, 16,  17, 18, 19,  17, 21, 20,  18, 22, 21,  22, 19, 20,   2, 3,  2, 4,  2, 5,  3, 4,  4, 5,  5, 3,  3, 6,  4, 6,  5, 6};
      PetscInt    coneOrientations[47] = {0, 0, 0,  0,   0, -2,  2,  2,   0, -1, -1,   0, -1, -1,   0, -1, -1,   0,  0,  0,   0,  0, -1,   0,  0, -1,  -1,  0,  0,   0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0};
      PetscScalar vertexCoords[15]     = {0.0, 0.0, -0.5,  0.0, -0.5, 0.0,  1.0, 0.0, 0.0,  0.0, 0.5, 0.0,  0.0, 0.0, 0.5};
      PetscInt    markerPoints[20]     = {2, 1, 3, 1, 4, 1, 5, 1, 14, 1, 15, 1, 16, 1, 17, 1, 18, 1, 19, 1};
      PetscInt    faultPoints[3]      = {3, 4, 5};

      ierr = DMPlexCreateFromDAG(dm, depth, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
      for (p = 0; p < 10; ++p) {
        ierr = DMSetLabelValue(dm, "marker", markerPoints[p*2], markerPoints[p*2+1]);CHKERRQ(ierr);
      }
      for (p = 0; p < 3; ++p) {
        ierr = DMSetLabelValue(dm, "fault", faultPoints[p], 1);CHKERRQ(ierr);
      }
    }
    break;
    case 1:
    {
      PetscInt    numPoints[4]         = {6, 13, 12, 4};
      PetscInt    coneSize[35]         = {4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
      PetscInt    cones[78]            = {10, 11, 12, 13,  10, 15, 16, 14,  17, 18, 14, 19,  20, 13, 19, 21,  22, 23, 24,  25, 26, 22,  24, 27, 25,  23, 26, 27,  28, 29, 23,  24, 30, 28,  22, 29, 30,   31, 32, 28,  29, 33, 31,  32, 33, 23,  26, 34, 33,  34, 27, 32,  6, 5,  5, 7,  7, 6,  6, 4,  4, 5,  7, 4,  7, 9,  9, 5,  6, 9,  9, 8,  8, 7,  5, 8,  4, 8};
      PetscInt    coneOrientations[78] = { 0,  0,  0,  0,  -2,  1,  0,  2,   0,  0, -3,  0,   0, -3, -1,  0,   0,  0,  0,   0,  0, -1,  -1,  0, -1,  -1, -1, -1,   0,  0,  0,   0,  0, -1,   0, -1, -1,    0,  0,  0,   0,  0, -1,  -1, -1,  0,  -1,  0, -1,  -1, -1, -1,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0};
      PetscScalar vertexCoords[18]     = {-1.0, 0.0, 0.0,  0.0, -1.0, 0.0,  0.0, 0.0, 1.0,  0.0, 1.0, 0.0,  0.0, 0.0, -1.0,  1.0, 0.0, 0.0};
      PetscInt    markerPoints[14]     = {5, 1, 6, 1, 7, 1, 10, 1, 22, 1, 23, 1, 24, 1};
      PetscInt    faultPoints[4]       = {5, 6, 7, 8};

      ierr = DMPlexCreateFromDAG(dm, depth, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
      for (p = 0; p < 7; ++p) {
        ierr = DMSetLabelValue(dm, "marker", markerPoints[p*2], markerPoints[p*2+1]);CHKERRQ(ierr);
      }
      for (p = 0; p < 4; ++p) {
        ierr = DMSetLabelValue(dm, "fault", faultPoints[p], 1);CHKERRQ(ierr);
      }
    }
    break;
    default:
      SETERRQ1(comm, PETSC_ERR_ARG_OUTOFRANGE, "No test mesh %d", testNum);
    }
  } else {
    PetscInt numPoints[4] = {0, 0, 0, 0};

    ierr = DMPlexCreateFromDAG(dm, depth, numPoints, NULL, NULL, NULL, NULL);CHKERRQ(ierr);
    ierr = DMCreateLabel(dm, "fault");CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

static PetscErrorCode CreateQuad_2D(MPI_Comm comm, PetscInt testNum, DM *dm)
{
  DM             idm;
  PetscInt       p;
  PetscMPIInt    rank;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = MPI_Comm_rank(comm, &rank);CHKERRMPI(ierr);
  if (rank == 0) {
    switch (testNum) {
    case 0:
    case 2:
    {
      PetscInt    numPoints[2]        = {6, 2};
      PetscInt    coneSize[8]         = {4, 4, 0, 0, 0, 0, 0, 0};
      PetscInt    cones[8]            = {2, 3, 4, 5,  3, 6, 7, 4};
      PetscInt    coneOrientations[8] = {0, 0, 0, 0,  0, 0, 0, 0};
      PetscScalar vertexCoords[12]    = {-0.5, 0.0, 0.0, 0.0, 0.0, 1.0, -0.5, 1.0, 0.5, 0.0, 0.5, 1.0};
      PetscInt    markerPoints[12]    = {2, 1, 3, 1, 4, 1, 5, 1, 6, 1, 7, 1};
      PetscInt    faultPoints[2]      = {3, 4};

      ierr = DMPlexCreateFromDAG(*dm, 1, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
      for (p = 0; p < 6; ++p) {ierr = DMSetLabelValue(*dm, "marker", markerPoints[p*2], markerPoints[p*2+1]);CHKERRQ(ierr);}
      if (testNum == 0) for (p = 0; p < 2; ++p) {ierr = DMSetLabelValue(*dm, "fault", faultPoints[p], 1);CHKERRQ(ierr);}
      if (testNum == 2) for (p = 0; p < 2; ++p) {ierr = DMSetLabelValue(*dm, "pfault", faultPoints[p], 1);CHKERRQ(ierr);}
    }
    break;
    case 1:
    {
      PetscInt    numPoints[2]         = {16, 9};
      PetscInt    coneSize[25]         = {4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      PetscInt    cones[36]            = {9,  13, 14, 10,
                                          10, 14, 15, 11,
                                          11, 15, 16, 12,
                                          13, 17, 18, 14,
                                          14, 18, 19, 15,
                                          15, 19, 20, 16,
                                          17, 21, 22, 18,
                                          18, 22, 23, 19,
                                          19, 23, 24, 20};
      PetscInt    coneOrientations[36] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      PetscScalar vertexCoords[32]     = {-3.0,  3.0,  -1.0,  3.0,  1.0,  3.0,  3.0,  3.0,  -3.0,  1.0,  -1.0,  1.0,  1.0,  1.0,  3.0,  1.0,
                                          -3.0, -1.0,  -1.0, -1.0,  1.0, -1.0,  3.0, -1.0,  -3.0, -3.0,  -1.0, -3.0,  1.0, -3.0,  3.0, -3.0};
      PetscInt    faultPoints[3]       = {11, 15, 19};
      PetscInt    fault2Points[2]      = {17, 18};

      ierr = DMPlexCreateFromDAG(*dm, 1, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
      for (p = 0; p < 3; ++p) {ierr = DMSetLabelValue(*dm, "fault",  faultPoints[p], 1);CHKERRQ(ierr);}
      for (p = 0; p < 2; ++p) {ierr = DMSetLabelValue(*dm, "fault2", fault2Points[p], 1);CHKERRQ(ierr);}
    }
    break;
    default:
      SETERRQ1(comm, PETSC_ERR_ARG_OUTOFRANGE, "No test mesh %d", testNum);
    }
  } else {
    PetscInt numPoints[3] = {0, 0, 0};

    ierr = DMPlexCreateFromDAG(*dm, 1, numPoints, NULL, NULL, NULL, NULL);CHKERRQ(ierr);
    if (testNum == 2) {ierr = DMCreateLabel(*dm, "pfault");CHKERRQ(ierr);}
    else              {ierr = DMCreateLabel(*dm, "fault");CHKERRQ(ierr);}
  }
  ierr = DMPlexInterpolate(*dm, &idm);CHKERRQ(ierr);
  ierr = DMViewFromOptions(idm, NULL, "-in_dm_view");CHKERRQ(ierr);
  ierr = DMDestroy(dm);CHKERRQ(ierr);
  *dm  = idm;
  PetscFunctionReturn(0);
}

static PetscErrorCode CreateHex_3D(MPI_Comm comm, PetscInt testNum, DM *dm)
{
  DM             idm;
  PetscInt       depth = 3, p;
  PetscMPIInt    rank;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = MPI_Comm_rank(comm, &rank);CHKERRMPI(ierr);
  if (rank == 0) {
    switch (testNum) {
    case 0:
    {
      PetscInt    numPoints[2]         = {12, 2};
      PetscInt    coneSize[14]         = {8,8, 0,0,0,0,0,0,0,0,0,0,0,0};
      PetscInt    cones[16]            = {2,5,4,3,6,7,8,9,  3,4,11,10,7,12,13,8};
      PetscInt    coneOrientations[16] = {0,0,0,0,0,0,0,0,  0,0, 0,0 ,0, 0, 0,0};
      PetscScalar vertexCoords[36]     = {-0.5,0.0,0.0, 0.0,0.0,0.0, 0.0,1.0,0.0, -0.5,1.0,0.0,
                                          -0.5,0.0,1.0, 0.0,0.0,1.0, 0.0,1.0,1.0, -0.5,1.0,1.0,
                                           0.5,0.0,0.0, 0.5,1.0,0.0, 0.5,0.0,1.0,  0.5,1.0,1.0};
      PetscInt    markerPoints[52]     = {2,1,3,1,4,1,5,1,6,1,7,1,8,1,9,1};
      PetscInt    faultPoints[4]       = {3, 4, 7, 8};

      ierr = DMPlexCreateFromDAG(*dm, 1, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
      ierr = DMPlexInterpolate(*dm, &idm);CHKERRQ(ierr);
      for (p = 0; p < 8; ++p) {ierr = DMSetLabelValue(idm, "marker", markerPoints[p*2], markerPoints[p*2+1]);CHKERRQ(ierr);}
      for (p = 0; p < 4; ++p) {ierr = DMSetLabelValue(idm, "fault", faultPoints[p], 1);CHKERRQ(ierr);}
    }
    break;
    case 1:
    {
      /* Cell Adjacency Graph:
        0 -- { 8, 13, 21, 24} --> 1
        0 -- {20, 21, 23, 24} --> 5 F
        1 -- {10, 15, 21, 24} --> 2
        1 -- {13, 14, 15, 24} --> 6
        2 -- {21, 22, 24, 25} --> 4 F
        3 -- {21, 24, 30, 35} --> 4
        3 -- {21, 24, 28, 33} --> 5
       */
      PetscInt    numPoints[2]         = {30, 7};
      PetscInt    coneSize[37]         = {8,8,8,8,8,8,8, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      PetscInt    cones[56]            = { 8, 21, 20,  7, 13, 12, 23, 24,
                                          14, 15, 10,  9, 13,  8, 21, 24,
                                          15, 16, 11, 10, 24, 21, 22, 25,
                                          30, 29, 28, 21, 35, 24, 33, 34,
                                          24, 21, 30, 35, 25, 36, 31, 22,
                                          27, 20, 21, 28, 32, 33, 24, 23,
                                          15, 24, 13, 14, 19, 18, 17, 26};
      PetscInt    coneOrientations[56] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      PetscScalar vertexCoords[90]     = {-2.0, -2.0, -2.0,  -2.0, -1.0, -2.0,  -3.0,  0.0, -2.0,  -2.0,  1.0, -2.0,  -2.0,  2.0, -2.0,  -2.0, -2.0,  0.0,
                                          -2.0, -1.0,  0.0,  -3.0,  0.0,  0.0,  -2.0,  1.0,  0.0,  -2.0,  2.0,  0.0,  -2.0, -1.0,  2.0,  -3.0,  0.0,  2.0,
                                          -2.0,  1.0,  2.0,   0.0, -2.0, -2.0,   0.0,  0.0, -2.0,   0.0,  2.0, -2.0,   0.0, -2.0,  0.0,   0.0,  0.0,  0.0,
                                           0.0,  2.0,  0.0,   0.0,  0.0,  2.0,   2.0, -2.0, -2.0,   2.0, -1.0, -2.0,   3.0,  0.0, -2.0,   2.0,  1.0, -2.0,
                                           2.0,  2.0, -2.0,   2.0, -2.0,  0.0,   2.0, -1.0,  0.0,   3.0,  0.0,  0.0,   2.0,  1.0,  0.0,   2.0,  2.0,  0.0};
      PetscInt    faultPoints[6]       = {20, 21, 22, 23, 24, 25};

      ierr = DMPlexCreateFromDAG(*dm, 1, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
      ierr = DMPlexInterpolate(*dm, &idm);CHKERRQ(ierr);
      for (p = 0; p < 6; ++p) {ierr = DMSetLabelValue(idm, "fault", faultPoints[p], 1);CHKERRQ(ierr);}
    }
    break;
    case 2:
    {
      /* Buried fault edge */
      PetscInt    numPoints[2]         = {18, 4};
      PetscInt    coneSize[22]         = {8,8,8,8, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      PetscInt    cones[32]            = { 4,  5,  8,  7, 13, 16, 17, 14,
                                           5,  6,  9,  8, 14, 17, 18, 15,
                                           7,  8, 11, 10, 16, 19, 20, 17,
                                           8,  9, 12, 11, 17, 20, 21, 18};
      PetscInt    coneOrientations[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      PetscScalar vertexCoords[54]     = {-2.0, -2.0,  0.0,  -2.0,  0.0,  0.0,  -2.0,  2.0,  0.0,   0.0, -2.0,  0.0,   0.0,  0.0,  0.0,   0.0,  2.0,  0.0,
                                           2.0, -2.0,  0.0,   2.0,  0.0,  0.0,   2.0,  2.0,  0.0,  -2.0, -2.0,  2.0,  -2.0,  0.0,  2.0,  -2.0,  2.0,  2.0,
                                           0.0, -2.0,  2.0,   0.0,  0.0,  2.0,   0.0,  2.0,  2.0,   2.0, -2.0,  2.0,   2.0,  0.0,  2.0,   2.0,  2.0,  2.0};
      PetscInt    faultPoints[4]       = {7, 8, 16, 17};

      ierr = DMPlexCreateFromDAG(*dm, 1, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
      ierr = DMPlexInterpolate(*dm, &idm);CHKERRQ(ierr);
      for (p = 0; p < 4; ++p) {ierr = DMSetLabelValue(idm, "fault", faultPoints[p], 1);CHKERRQ(ierr);}
    }
    break;
    default: SETERRQ1(comm, PETSC_ERR_ARG_OUTOFRANGE, "No test mesh %d", testNum);
    }
  } else {
    PetscInt numPoints[4] = {0, 0, 0, 0};

    ierr = DMPlexCreateFromDAG(*dm, depth, numPoints, NULL, NULL, NULL, NULL);CHKERRQ(ierr);
    ierr = DMPlexInterpolate(*dm, &idm);CHKERRQ(ierr);
    ierr = DMCreateLabel(idm, "fault");CHKERRQ(ierr);
  }
  ierr = DMViewFromOptions(idm, NULL, "-in_dm_view");CHKERRQ(ierr);
  ierr = DMDestroy(dm);CHKERRQ(ierr);
  *dm  = idm;
  PetscFunctionReturn(0);
}

static PetscErrorCode CreateFaultLabel(DM dm)
{
  DMLabel        label;
  PetscInt       dim, h, pStart, pEnd, pMax, p;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = DMGetDimension(dm, &dim);CHKERRQ(ierr);
  ierr = DMCreateLabel(dm, "cohesive");CHKERRQ(ierr);
  ierr = DMGetLabel(dm, "cohesive", &label);CHKERRQ(ierr);
  for (h = 0; h <= dim; ++h) {
    ierr = DMPlexGetSimplexOrBoxCells(dm, h, NULL, &pMax);CHKERRQ(ierr);
    ierr = DMPlexGetHeightStratum(dm, h, &pStart, &pEnd);CHKERRQ(ierr);
    for (p = pMax; p < pEnd; ++p) {ierr = DMLabelSetValue(label, p, 1);CHKERRQ(ierr);}
  }
  PetscFunctionReturn(0);
}

static PetscErrorCode CreateDiscretization(DM dm, AppCtx *user)
{
  PetscFE        fe;
  DMLabel        fault;
  PetscInt       dim;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = DMGetDimension(dm, &dim);CHKERRQ(ierr);
  ierr = DMGetLabel(dm, "cohesive", &fault);CHKERRQ(ierr);
  ierr = DMLabelView(fault, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

  ierr = PetscFECreateDefault(PETSC_COMM_SELF, dim, dim, user->cellSimplex, "displacement_", PETSC_DETERMINE, &fe);CHKERRQ(ierr);
  ierr = PetscFESetName(fe, "displacement");CHKERRQ(ierr);
  ierr = DMAddField(dm, NULL, (PetscObject) fe);CHKERRQ(ierr);
  ierr = PetscFEDestroy(&fe);CHKERRQ(ierr);

  ierr = PetscFECreateDefault(PETSC_COMM_SELF, dim-1, dim, user->cellSimplex, "faulttraction_", PETSC_DETERMINE, &fe);CHKERRQ(ierr);
  ierr = PetscFESetName(fe, "fault traction");CHKERRQ(ierr);
  ierr = DMAddField(dm, fault, (PetscObject) fe);CHKERRQ(ierr);
  ierr = PetscFEDestroy(&fe);CHKERRQ(ierr);

  ierr = DMCreateDS(dm);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

static PetscErrorCode CreateMesh(MPI_Comm comm, AppCtx *user, DM *dm)
{
  PetscInt       dim          = user->dim;
  PetscBool      cellSimplex  = user->cellSimplex, hasFault, hasFault2, hasParallelFault;
  PetscMPIInt    rank, size;
  DMLabel        matLabel;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = MPI_Comm_rank(comm, &rank);CHKERRMPI(ierr);
  ierr = MPI_Comm_size(comm, &size);CHKERRMPI(ierr);
  ierr = DMCreate(comm, dm);CHKERRQ(ierr);
  ierr = DMSetType(*dm, DMPLEX);CHKERRQ(ierr);
  ierr = DMSetDimension(*dm, dim);CHKERRQ(ierr);
  switch (dim) {
  case 2:
    if (cellSimplex) {
      ierr = CreateSimplex_2D(comm, user->testNum, dm);CHKERRQ(ierr);
    } else {
      ierr = CreateQuad_2D(comm, user->testNum, dm);CHKERRQ(ierr);
    }
    break;
  case 3:
    if (cellSimplex) {
      ierr = CreateSimplex_3D(comm, user, *dm);CHKERRQ(ierr);
    } else {
      ierr = CreateHex_3D(comm, user->testNum, dm);CHKERRQ(ierr);
    }
    break;
  default:
    SETERRQ1(comm, PETSC_ERR_ARG_OUTOFRANGE, "Cannot make hybrid meshes for dimension %d", dim);
  }
  ierr = PetscObjectSetOptionsPrefix((PetscObject) *dm, "orig_");CHKERRQ(ierr);
  ierr = DMSetFromOptions(*dm);CHKERRQ(ierr);
  ierr = DMGetLabel(*dm, "material", &matLabel);CHKERRQ(ierr);
  if (matLabel) {
    ierr = DMPlexLabelComplete(*dm, matLabel);CHKERRQ(ierr);
  }
  ierr = DMViewFromOptions(*dm, NULL, "-dm_view");CHKERRQ(ierr);
  ierr = DMHasLabel(*dm, "fault", &hasFault);CHKERRQ(ierr);
  if (hasFault) {
    DM      dmHybrid = NULL, dmInterface = NULL;
    DMLabel faultLabel, faultBdLabel, hybridLabel, splitLabel;

    ierr = DMGetLabel(*dm, "fault", &faultLabel);CHKERRQ(ierr);
    ierr = DMGetLabel(*dm, "faultBd", &faultBdLabel);CHKERRQ(ierr);
    ierr = DMPlexCreateHybridMesh(*dm, faultLabel, faultBdLabel, &hybridLabel, &splitLabel, &dmInterface, &dmHybrid);CHKERRQ(ierr);
    ierr = DMLabelView(hybridLabel, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    ierr = DMLabelDestroy(&hybridLabel);CHKERRQ(ierr);
    ierr = DMLabelView(splitLabel, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    ierr = DMLabelDestroy(&splitLabel);CHKERRQ(ierr);
    ierr = DMViewFromOptions(dmInterface, NULL, "-dm_interface_view");CHKERRQ(ierr);
    ierr = DMDestroy(&dmInterface);CHKERRQ(ierr);
    ierr = DMDestroy(dm);CHKERRQ(ierr);
    *dm  = dmHybrid;
  }
  ierr = DMHasLabel(*dm, "fault2", &hasFault2);CHKERRQ(ierr);
  if (hasFault2) {
    DM      dmHybrid = NULL;
    DMLabel faultLabel, faultBdLabel, hybridLabel;

    ierr = PetscObjectSetOptionsPrefix((PetscObject) *dm, "faulted_");CHKERRQ(ierr);
    ierr = DMViewFromOptions(*dm, NULL, "-dm_view_pre");CHKERRQ(ierr);
    ierr = DMSetFromOptions(*dm);CHKERRQ(ierr);
    ierr = DMViewFromOptions(*dm, NULL, "-dm_view");CHKERRQ(ierr);
    ierr = DMGetLabel(*dm, "fault2", &faultLabel);CHKERRQ(ierr);
    ierr = DMGetLabel(*dm, "fault2Bd", &faultBdLabel);CHKERRQ(ierr);
    ierr = DMPlexCreateHybridMesh(*dm, faultLabel, faultBdLabel, &hybridLabel, NULL, NULL, &dmHybrid);CHKERRQ(ierr);
    ierr = DMLabelView(hybridLabel, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    ierr = DMLabelDestroy(&hybridLabel);CHKERRQ(ierr);
    ierr = DMDestroy(dm);CHKERRQ(ierr);
    *dm  = dmHybrid;
  }
  if (user->testPartition && size > 1) {
    PetscPartitioner part;
    PetscInt *sizes  = NULL;
    PetscInt *points = NULL;

    if (rank == 0) {
      if (dim == 2 && cellSimplex && size == 2) {
        switch (user->testNum) {
        case 0: {
          PetscInt triSizes_p2[2]  = {1, 2};
          PetscInt triPoints_p2[3] = {0, 1, 2};

          ierr = PetscMalloc2(2, &sizes, 3, &points);CHKERRQ(ierr);
          ierr = PetscArraycpy(sizes,  triSizes_p2, 2);CHKERRQ(ierr);
          ierr = PetscArraycpy(points, triPoints_p2, 3);CHKERRQ(ierr);break;}
        default:
          SETERRQ1(PETSC_COMM_WORLD, PETSC_ERR_ARG_WRONG, "Could not find matching test number %d for triangular mesh on 2 procs", user->testNum);
        }
      } else if (dim == 2 && !cellSimplex && size == 2) {
        switch (user->testNum) {
        case 0: {
          PetscInt quadSizes_p2[2]  = {1, 2};
          PetscInt quadPoints_p2[3] = {0, 1, 2};

          ierr = PetscMalloc2(2, &sizes, 3, &points);CHKERRQ(ierr);
          ierr = PetscArraycpy(sizes,  quadSizes_p2, 2);CHKERRQ(ierr);
          ierr = PetscArraycpy(points, quadPoints_p2, 3);CHKERRQ(ierr);break;}
        case 2: {
          PetscInt quadSizes_p2[2]  = {1, 1};
          PetscInt quadPoints_p2[2] = {0, 1};

          ierr = PetscMalloc2(2, &sizes, 2, &points);CHKERRQ(ierr);
          ierr = PetscArraycpy(sizes,  quadSizes_p2, 2);CHKERRQ(ierr);
          ierr = PetscArraycpy(points, quadPoints_p2, 2);CHKERRQ(ierr);break;}
        default:
          SETERRQ1(PETSC_COMM_WORLD, PETSC_ERR_ARG_WRONG, "Could not find matching test number %d for quadrilateral mesh on 2 procs", user->testNum);
        }
      } else if (dim == 3 && cellSimplex && size == 2) {
        switch (user->testNum) {
        case 0: {
          PetscInt tetSizes_p2[2]  = {1, 2};
          PetscInt tetPoints_p2[3] = {0, 1, 2};

          ierr = PetscMalloc2(2, &sizes, 3, &points);CHKERRQ(ierr);
          ierr = PetscArraycpy(sizes,  tetSizes_p2, 2);CHKERRQ(ierr);
          ierr = PetscArraycpy(points, tetPoints_p2, 3);CHKERRQ(ierr);break;}
        default:
          SETERRQ1(PETSC_COMM_WORLD, PETSC_ERR_ARG_WRONG, "Could not find matching test number %d for triangular mesh on 2 procs", user->testNum);
        }
      } else if (dim == 3 && !cellSimplex && size == 2) {
        switch (user->testNum) {
        case 0: {
          PetscInt hexSizes_p2[2]  = {1, 2};
          PetscInt hexPoints_p2[3] = {0, 1, 2};

          ierr = PetscMalloc2(2, &sizes, 3, &points);CHKERRQ(ierr);
          ierr = PetscArraycpy(sizes,  hexSizes_p2, 2);CHKERRQ(ierr);
          ierr = PetscArraycpy(points, hexPoints_p2, 3);CHKERRQ(ierr);break;}
        default:
          SETERRQ1(PETSC_COMM_WORLD, PETSC_ERR_ARG_WRONG, "Could not find matching test number %d for triangular mesh on 2 procs", user->testNum);
        }
      } else SETERRQ(PETSC_COMM_WORLD, PETSC_ERR_ARG_WRONG, "Could not find matching test partition");
    }
    ierr = DMPlexGetPartitioner(*dm, &part);CHKERRQ(ierr);
    ierr = PetscPartitionerSetType(part, PETSCPARTITIONERSHELL);CHKERRQ(ierr);
    ierr = PetscPartitionerShellSetPartition(part, size, sizes, points);CHKERRQ(ierr);
    ierr = PetscFree2(sizes, points);CHKERRQ(ierr);
  }
  {
    DM pdm = NULL;

    /* Distribute mesh over processes */
    ierr = DMPlexDistribute(*dm, 0, NULL, &pdm);CHKERRQ(ierr);
    if (pdm) {
      ierr = DMViewFromOptions(pdm, NULL, "-dm_view");CHKERRQ(ierr);
      ierr = DMDestroy(dm);CHKERRQ(ierr);
      *dm  = pdm;
    }
  }
  ierr = DMHasLabel(*dm, "pfault", &hasParallelFault);CHKERRQ(ierr);
  if (hasParallelFault) {
    DM      dmHybrid = NULL;
    DMLabel faultLabel, faultBdLabel, hybridLabel;

    ierr = DMGetLabel(*dm, "pfault", &faultLabel);CHKERRQ(ierr);
    ierr = DMGetLabel(*dm, "pfaultBd", &faultBdLabel);CHKERRQ(ierr);
    ierr = DMPlexCreateHybridMesh(*dm, faultLabel, faultBdLabel, &hybridLabel, NULL, NULL, &dmHybrid);CHKERRQ(ierr);
    ierr = DMLabelView(hybridLabel, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    ierr = DMLabelDestroy(&hybridLabel);CHKERRQ(ierr);
    ierr = DMDestroy(dm);CHKERRQ(ierr);
    *dm  = dmHybrid;
  }
  ierr = PetscObjectSetName((PetscObject) *dm, "Hybrid Mesh");CHKERRQ(ierr);
  ierr = CreateFaultLabel(*dm);CHKERRQ(ierr);
  ierr = CreateDiscretization(*dm, user);CHKERRQ(ierr);
  ierr = DMViewFromOptions(*dm, NULL, "-dm_view_pre");CHKERRQ(ierr);
  ierr = DMSetFromOptions(*dm);CHKERRQ(ierr);
  ierr = DMViewFromOptions(*dm, NULL, "-dm_view");CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

static PetscErrorCode TestMesh(DM dm, AppCtx *user)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = DMPlexCheckSymmetry(dm);CHKERRQ(ierr);
  ierr = DMPlexCheckSkeleton(dm, 0);CHKERRQ(ierr);
  ierr = DMPlexCheckFaces(dm, 0);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

static PetscErrorCode TestDiscretization(DM dm, AppCtx *user)
{
  PetscSection   s;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = DMGetSection(dm, &s);CHKERRQ(ierr);
  ierr = PetscObjectViewFromOptions((PetscObject) s, NULL, "-local_section_view");CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

static PetscErrorCode r(PetscInt dim, PetscReal time, const PetscReal x[], PetscInt Nc, PetscScalar *u, void *ctx)
{
  PetscInt d;
  for (d = 0; d < dim; ++d) u[d] = x[d];
  return 0;
}

static PetscErrorCode rp1(PetscInt dim, PetscReal time, const PetscReal x[], PetscInt Nc, PetscScalar *u, void *ctx)
{
  PetscInt d;
  for (d = 0; d < dim; ++d) u[d] = x[d] + (d > 0 ? 1.0 : 0.0);
  return 0;
}

static PetscErrorCode phi(PetscInt dim, PetscReal time, const PetscReal x[], PetscInt Nc, PetscScalar *u, void *ctx)
{
  PetscInt d;
  u[0] = -x[1];
  u[1] =  x[0];
  for (d = 2; d < dim; ++d) u[d] = x[d];
  return 0;
}

/* \lambda \cdot (\psi_u^- - \psi_u^+) */
static void f0_bd_u(PetscInt dim, PetscInt Nf, PetscInt NfAux,
                    const PetscInt uOff[], const PetscInt uOff_x[], const PetscScalar u[], const PetscScalar u_t[], const PetscScalar u_x[],
                    const PetscInt aOff[], const PetscInt aOff_x[], const PetscScalar a[], const PetscScalar a_t[], const PetscScalar a_x[],
                    PetscReal t, const PetscReal x[], const PetscReal n[], PetscInt numConstants, const PetscScalar constants[], PetscScalar f0[])
{
  const PetscInt Nc = uOff[1]-uOff[0];
  PetscInt       c;
  for (c = 0;  c < Nc;   ++c) {
    f0[c]    =   u[Nc*2+c] + x[Nc-c-1];
    f0[Nc+c] = -(u[Nc*2+c] + x[Nc-c-1]);
  }
}

/* (d - u^+ + u^-) \cdot \psi_\lambda */
static void f0_bd_l(PetscInt dim, PetscInt Nf, PetscInt NfAux,
                    const PetscInt uOff[], const PetscInt uOff_x[], const PetscScalar u[], const PetscScalar u_t[], const PetscScalar u_x[],
                    const PetscInt aOff[], const PetscInt aOff_x[], const PetscScalar a[], const PetscScalar a_t[], const PetscScalar a_x[],
                    PetscReal t, const PetscReal x[], const PetscReal n[], PetscInt numConstants, const PetscScalar constants[], PetscScalar f0[])
{
  const PetscInt Nc = uOff[2]-uOff[1];
  PetscInt       c;

  for (c = 0; c < Nc; ++c) f0[c] = (c > 0 ? 1.0 : 0.0) + u[c] - u[Nc+c];
}

/* \psi_lambda \cdot (\psi_u^- - \psi_u^+) */
static void g0_bd_ul(PetscInt dim, PetscInt Nf, PetscInt NfAux,
                     const PetscInt uOff[], const PetscInt uOff_x[], const PetscScalar u[], const PetscScalar u_t[], const PetscScalar u_x[],
                     const PetscInt aOff[], const PetscInt aOff_x[], const PetscScalar a[], const PetscScalar a_t[], const PetscScalar a_x[],
                     PetscReal t, PetscReal u_tShift, const PetscReal x[], const PetscReal n[], PetscInt numConstants, const PetscScalar constants[], PetscScalar g0[])
{
  const PetscInt Nc = uOff[1]-uOff[0];
  PetscInt       c;

  for (c = 0; c < Nc; ++c) {
    g0[(0 +c)*Nc+c] =  1.0;
    g0[(Nc+c)*Nc+c] = -1.0;
  }
}

/* (-\psi_u^+ + \psi_u^-) \cdot \psi_\lambda */
static void g0_bd_lu(PetscInt dim, PetscInt Nf, PetscInt NfAux,
                     const PetscInt uOff[], const PetscInt uOff_x[], const PetscScalar u[], const PetscScalar u_t[], const PetscScalar u_x[],
                     const PetscInt aOff[], const PetscInt aOff_x[], const PetscScalar a[], const PetscScalar a_t[], const PetscScalar a_x[],
                     PetscReal t, PetscReal u_tShift, const PetscReal x[], const PetscReal n[], PetscInt numConstants, const PetscScalar constants[], PetscScalar g0[])
{
  const PetscInt Nc = uOff[2]-uOff[1];
  PetscInt       c;

  for (c = 0; c < Nc; ++c) {
    g0[c*Nc*2+c]    =  1.0;
    g0[c*Nc*2+Nc+c] = -1.0;
  }
}

static PetscErrorCode TestAssembly(DM dm, AppCtx *user)
{
  Mat              J;
  Vec              locX, locF;
  PetscDS          probh;
  DMLabel          fault, material;
  IS               cohesiveCells;
  PetscFormKey     keys[3];
  PetscErrorCode (*initialGuess[2])(PetscInt dim, PetscReal time, const PetscReal x[], PetscInt Nc, PetscScalar u[], void *ctx);
  PetscInt         dim, Nf, cMax, cEnd, id;
  PetscErrorCode   ierr;

  PetscFunctionBegin;
  ierr = DMGetDimension(dm, &dim);CHKERRQ(ierr);
  ierr = DMPlexGetSimplexOrBoxCells(dm, 0, NULL, &cMax);CHKERRQ(ierr);
  ierr = DMPlexGetHeightStratum(dm, 0, NULL, &cEnd);CHKERRQ(ierr);
  ierr = ISCreateStride(PETSC_COMM_SELF, cEnd - cMax, cMax, 1, &cohesiveCells);CHKERRQ(ierr);
  ierr = DMGetLabel(dm, "cohesive", &fault);CHKERRQ(ierr);
  ierr = DMGetLocalVector(dm, &locX);CHKERRQ(ierr);
  ierr = PetscObjectSetName((PetscObject) locX, "Local Solution");CHKERRQ(ierr);
  ierr = DMGetLocalVector(dm, &locF);CHKERRQ(ierr);
  ierr = PetscObjectSetName((PetscObject) locF, "Local Residual");CHKERRQ(ierr);
  ierr = DMCreateMatrix(dm, &J);CHKERRQ(ierr);
  ierr = PetscObjectSetName((PetscObject) J, "Jacobian");CHKERRQ(ierr);

  /* The initial guess has displacement shifted by one unit in each fault parallel direction across the fault */
  ierr = DMGetLabel(dm, "material", &material);CHKERRQ(ierr);
  id   = 1;
  initialGuess[0] = r;
  initialGuess[1] = NULL;
  ierr = DMProjectFunctionLabelLocal(dm, 0.0, material, 1, &id, PETSC_DETERMINE, NULL, initialGuess, NULL, INSERT_VALUES, locX);CHKERRQ(ierr);
  id   = 2;
  initialGuess[0] = rp1;
  initialGuess[1] = NULL;
  ierr = DMProjectFunctionLabelLocal(dm, 0.0, material, 1, &id, PETSC_DETERMINE, NULL, initialGuess, NULL, INSERT_VALUES, locX);CHKERRQ(ierr);
  id   = 1;
  initialGuess[0] = NULL;
  initialGuess[1] = phi;
  ierr = DMProjectFunctionLabelLocal(dm, 0.0, fault, 1, &id, PETSC_DETERMINE, NULL, initialGuess, NULL, INSERT_VALUES, locX);CHKERRQ(ierr);
  ierr = VecViewFromOptions(locX, NULL, "-local_solution_view");CHKERRQ(ierr);

  ierr = DMGetCellDS(dm, cMax, &probh);CHKERRQ(ierr);
  ierr = PetscDSGetNumFields(probh, &Nf);CHKERRQ(ierr);
  ierr = PetscDSSetBdResidual(probh, 0, f0_bd_u, NULL);CHKERRQ(ierr);
  if (Nf > 1) {ierr = PetscDSSetBdResidual(probh, 1, f0_bd_l, NULL);CHKERRQ(ierr);}
  ierr = PetscDSSetBdJacobian(probh, 0, 1, g0_bd_ul, NULL, NULL, NULL);CHKERRQ(ierr);
  if (Nf > 1) {ierr = PetscDSSetBdJacobian(probh, 1, 0, g0_bd_lu, NULL, NULL, NULL);CHKERRQ(ierr);}

  keys[0].label = NULL;
  keys[0].value = 0;
  keys[0].field = 0;
  keys[0].part  = 0;
  keys[1].label = NULL;
  keys[1].value = 0;
  keys[1].field = 0;
  keys[1].part  = 0;
  keys[2].label = NULL;
  keys[2].value = 0;
  keys[2].field = 0;
  keys[2].part  = 0;
  ierr = VecSet(locF, 0.);CHKERRQ(ierr);
  ierr = DMPlexComputeResidual_Hybrid_Internal(dm, keys, cohesiveCells, 0.0, locX, NULL, 0.0, locF, user);CHKERRQ(ierr);
  ierr = VecViewFromOptions(locF, NULL, "-local_residual_view");CHKERRQ(ierr);
  ierr = MatZeroEntries(J);CHKERRQ(ierr);
  ierr = DMPlexComputeJacobian_Hybrid_Internal(dm, keys, cohesiveCells, 0.0, 0.0, locX, NULL, J, J, user);CHKERRQ(ierr);
  ierr = MatViewFromOptions(J, NULL, "-local_jacobian_view");CHKERRQ(ierr);

  ierr = DMRestoreLocalVector(dm, &locX);CHKERRQ(ierr);
  ierr = DMRestoreLocalVector(dm, &locF);CHKERRQ(ierr);
  ierr = MatDestroy(&J);CHKERRQ(ierr);
  ierr = ISDestroy(&cohesiveCells);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

int main(int argc, char **argv)
{
  DM             dm;
  AppCtx         user;                 /* user-defined work context */
  PetscErrorCode ierr;

  ierr = PetscInitialize(&argc, &argv, NULL,help);if (ierr) return ierr;
  ierr = ProcessOptions(PETSC_COMM_WORLD, &user);CHKERRQ(ierr);
  ierr = CreateMesh(PETSC_COMM_WORLD, &user, &dm);CHKERRQ(ierr);
  ierr = TestMesh(dm, &user);CHKERRQ(ierr);
  ierr = TestDiscretization(dm, &user);CHKERRQ(ierr);
  ierr = TestAssembly(dm, &user);CHKERRQ(ierr);
  ierr = DMDestroy(&dm);CHKERRQ(ierr);
  ierr = PetscFinalize();
  return ierr;
}

/*TEST
  testset:
    args: -orig_dm_plex_check_all -dm_plex_check_all \
          -displacement_petscspace_degree 1 -faulttraction_petscspace_degree 1 -dm_petscds_view -local_section_view \
          -local_solution_view -local_residual_view -local_jacobian_view
    test:
      suffix: tri_0
      args: -dim 2
    test:
      suffix: tri_t1_0
      args: -dim 2 -test_num 1
    test:
      suffix: tet_0
      args: -dim 3
    test:
      suffix: tet_t1_0
      args: -dim 3 -test_num 1

  testset:
    args: -orig_dm_plex_check_all -dm_plex_check_all \
          -displacement_petscspace_degree 1 -faulttraction_petscspace_degree 1 -dm_petscds_view
    test:
      suffix: tet_1
      nsize: 2
      args: -dim 3
    test:
      suffix: tri_1
      nsize: 2
      args: -dim 2

  testset:
    args: -orig_dm_plex_check_all -dm_plex_check_all \
          -displacement_petscspace_degree 1 -faulttraction_petscspace_degree 1 -dm_petscds_view
    # 2D Quads
    test:
      suffix: quad_0
      args: -dim 2 -cell_simplex 0
    test:
      suffix: quad_1
      nsize: 2
      args: -dim 2 -cell_simplex 0
    test:
      suffix: quad_t1_0
      args: -dim 2 -cell_simplex 0 -test_num 1 -faulted_dm_plex_check_all
    # 3D Hex
    test:
      suffix: hex_0
      args: -dim 3 -cell_simplex 0
    test:
      suffix: hex_1
      nsize: 2
      args: -dim 3 -cell_simplex 0
    test:
      suffix: hex_t1_0
      args: -dim 3 -cell_simplex 0 -test_num 1
    test:
      suffix: hex_t2_0
      args: -dim 3 -cell_simplex 0 -test_num 2

TEST*/
