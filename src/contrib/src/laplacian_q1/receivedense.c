#ifdef PETSC_RCS_HEADER
static char vcid[] = "$Id: receivedense.c,v 1.6 1998/04/03 21:00:37 bsmith Exp $";
#endif
/* 
   This is part of the MatlabSockettool Package. It is called by 
 the receive.mex4 Matlab program. 
  
        Written by Barry Smith, bsmith@mcs.anl.gov 4/14/92

  Since this is called from Matlab it cannot be compiled with C++.
*/

#include <stdio.h>
#include "sys.h"
#include "mex.h"
#define ERROR(a) {fprintf(stderr,"RECEIVE %s \n",a); return -1;}
/*-----------------------------------------------------------------*/
#undef __FUNC__  
#define __FUNC__ "ReceiveDenseMatrix"
int ReceiveDenseMatrix(Matrix *plhs[],int t)
{
  int    m,n,compx = 0,i;
  
  /* get size of matrix */
  if (PetscBinaryRead(t,&m,1,PETSC_INT))   ERROR("reading number columns"); 
  if (PetscBinaryRead(t,&n,1,PETSC_INT))   ERROR("reading number rows"); 
  if (PetscBinaryRead(t,&compx,1,PETSC_INT))   ERROR("reading number rows"); 
  
  /*allocate matrix */
  plhs[0]  = mxCreateFull(m, n, compx);
  /* read in matrix */
  if (!compx) {
    if (PetscBinaryRead(t,mxGetPr(plhs[0]),n*m,PETSC_DOUBLE)) ERROR("read dense matrix");
  } else {
    for ( i=0; i<n*m; i++ ) {
      if (PetscBinaryRead(t,mxGetPr(plhs[0])+i,1,PETSC_DOUBLE))ERROR("read dense matrix");
      if (PetscBinaryRead(t,mxGetPi(plhs[0])+i,1,PETSC_DOUBLE))ERROR("read dense matrix");
    }
  }
  return 0;
}

#undef __FUNC__  
#define __FUNC__ "ReceiveIntDenseMatrix"
int ReceiveDenseIntMatrix(Matrix *plhs[],int t)
{
  int    m,compx = 0,i,*array;
  double *values;
  
  /* get size of matrix */
  if (PetscBinaryRead(t,&m,1,PETSC_INT))   ERROR("reading number columns"); 
  
  /*allocate matrix */
  plhs[0]  = mxCreateFull(m, 1, 0);

  /* read in matrix */
  array = (int *) malloc(m*sizeof(int)); if (!array) ERROR("reading allocating space");
  if (PetscBinaryRead(t,array,m,PETSC_INT)) ERROR("read dense matrix");

  values = mxGetPr(plhs[0]);
  for ( i =0; i<m; i++ ) {
    values[i] = array[i];
  }
  free(array);

  return 0;
}
    
 
