/*
       Defines some simple data structures for writing cell (element) based PDE codes.
*/

#if !defined(__APPCTX_H)
#define __APPCTX_H

#include <string.h>
#include "ao.h"
#include "math.h"
#include "sles.h"
#include "snes.h"

typedef struct {

  /* the cell-oriented cell-coords, 
     i.e the coords of each of the 4 vertices corresponding to each cell */
  double *cell_coords;

  /* the index  for each of the 4 vertices of a given cell 
     (these will be in a local numbering - all calls use setlocal, with the
     loca to global mapping set by ltog)*/
  int *cell_vertex;

  /* the aforementioned local to global mapping */
  ISLocalToGlobalMapping ltog;

  /* counts */
  /* the number of cells on this processor,*/
  int cell_n; 
  /* all the vertices on local cells */
  int vertex_count;
  /* only those vertices (on local cells)  which actually belong to this processor */
  int vertex_local_count;
 
/********* Data sttructures for the boundary conditions ************/
  /* the local indices of vertices on the boundary */
  IS  vertex_boundary;
  int boundary_count; /* size */
  double *boundary_values;  /* space for the boundary values */
  double *boundary_coords;  /* the coords of the boundary points */

  /* for the global draw routine */
  IS cell_global;

} AppGrid;


typedef struct {
  Vec b; /* Global vector for the rhs */
  Vec x; /* Global vector for the solution */
  Mat A; /* for the stiffness */
} AppAlgebra;

typedef struct {

  int matlabgraphics;
  int show_vector;
  int show_matrix;
  int show_ao;
  int show_is;

  Draw       drawlocal;
  Draw       drawglobal;
  int        shownumbers;
  int        showvertices;
  int        showelements;
  int        showboundary;
  int        showboundaryvertices;
 
  int        showsomething;       
} AppView;



/* 
Additional structure for the discretization.
Values at the gauss points of the bilinear basis functions
*/

typedef struct {
  /* values of the reference interpolating functions at the gauss pts */
  double RefVal[4][4];
  double RefDx[4][4];
  double RefDy[4][4];

  /* values of the local interpolating fns at the gauss pts */
  double dx[4][4], dy[4][4];
  double detDh[4];

  /* the images of the gauss pts in the local element */
  double x[4], y[4];

  /* results of local integrations */
  double rhsresult[4];
  double stiffnessresult[4][4];

  /* quadrature weights */
  double  weights[4];

  /* pointer to coords of current cell */
  double *coords;

} AppElement;


/*
      comm   - MPI communictor where grid etc are stored
      aodata - grid database
*/
typedef struct {
  MPI_Comm   comm;
  AOData     aodata;
  AppGrid    grid;
  AppAlgebra algebra;  
  AppView    view;
  AppElement element;
} AppCtx;


/* function declarations */

extern int AppCtxViewGrid(Draw,void*);
extern int AppCtxGraphics(AppCtx *appctx);

extern int AppCtxCreate(MPI_Comm,AppCtx **);
extern int AppCtxDestroy(AppCtx *);
extern int AppCtxSetLocal(AppCtx *);
extern int AppCtxSolve(AppCtx*);

double f(double, double);
double bc(double, double);
double u(double, double);
double v(double, double);

extern int AODataPartitionAndSetupLocal(AOData ao, char *keyname,  char *segmentname, IS *iscell, IS *issegment, ISLocalToGlobalMapping *ltog);

extern int AppCtxCreateRhs(AppCtx*);
extern int AppCtxCreateMatrix(AppCtx*);

extern int AppCtxSetMatrix(AppCtx*);
extern int AppCtxSetRhs(AppCtx*);
extern int SetBoundaryConditions(AppCtx*);
extern int SetMatrixBoundaryConditions(AppCtx *appctx);

extern int ComputeRHS( AppElement *phi );
extern int ComputeStiffness( AppElement *phi );

extern int SetReferenceElement(AppCtx* appctx);
extern int SetLocalElement(AppElement *phi );
extern double InterpolatingFunctions(int partial, int node, double x, double y);

extern int AppCtxViewMatlab(AppCtx*);

#endif
