
static char help[] = "Test the use of MatSetValuesBlocked(), MatZeroRows() for rectangular MatBAIJ matrix, test MatSetValuesBlocked() for MatSBAIJ matrix (-test_mat_sbaij).";

#include "petscmat.h"

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **args)
{
  Mat         A;
  int         bs=3,m=4,n=6,i,j,val = 10,ierr,size,rank,rstart;
  PetscScalar x[6][9],y[3][3],one=1.0;
  int         row[2],col[3],eval;
  IS          is;
  PetscTruth  flg,testsbaij=PETSC_FALSE;

  PetscInitialize(&argc,&args,(char *)0,help);

  ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);CHKERRQ(ierr);
  ierr = MPI_Comm_rank(PETSC_COMM_WORLD,&rank);CHKERRQ(ierr);

  ierr = PetscOptionsHasName(PETSC_NULL,"-test_mat_sbaij",&testsbaij);CHKERRQ(ierr);
  
  if (testsbaij){
    if (size == 1) {
      ierr = MatCreateSeqSBAIJ(PETSC_COMM_SELF,bs,m*bs,n*bs,1,PETSC_NULL,&A);CHKERRQ(ierr);
    } else {
      ierr = MatCreateMPISBAIJ(PETSC_COMM_WORLD,bs,m*bs,n*bs,PETSC_DECIDE,PETSC_DECIDE,1,
                            PETSC_NULL,1,PETSC_NULL,&A);CHKERRQ(ierr);
    }
  } else {
    if (size == 1) {
      ierr = MatCreateSeqBAIJ(PETSC_COMM_SELF,bs,m*bs,n*bs,1,PETSC_NULL,&A);CHKERRQ(ierr);
    } else {
      ierr = MatCreateMPIBAIJ(PETSC_COMM_WORLD,bs,m*bs,n*bs,PETSC_DECIDE,PETSC_DECIDE,1,
                            PETSC_NULL,1,PETSC_NULL,&A);CHKERRQ(ierr);
    }
  }
  ierr = PetscOptionsHasName(PETSC_NULL,"-column_oriented",&flg);CHKERRQ(ierr);
  if (flg) { 
    ierr = MatSetOption(A,MAT_COLUMN_ORIENTED);CHKERRQ(ierr); 
    eval = 6;
  } else {
    eval = 9;
  }

  ierr = PetscOptionsHasName(PETSC_NULL,"-ass_extern",&flg);CHKERRQ(ierr);
  if (flg && (size != 1))    rstart = m*((rank+1)%size);
  else                       rstart = m*(rank);

  row[0] =rstart+0;  row[1] =rstart+2;
  col[0] =rstart+0;  col[1] =rstart+1;  col[2] =rstart+3;
  for (i=0; i<6; i++) {
    for (j =0; j< 9; j++) x[i][j] = (PetscScalar)val++;
  }

  ierr = MatSetValuesBlocked(A,2,row,3,col,&x[0][0],INSERT_VALUES);CHKERRQ(ierr);



  ierr = MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

  /*
  This option does not work for rectangular matrices
  ierr = MatSetOption(A,MAT_NEW_NONZERO_LOCATION_ERR);CHKERRQ(ierr);
  */
  
  ierr = MatSetValuesBlocked(A,2,row,3,col,&x[0][0],INSERT_VALUES);CHKERRQ(ierr);

  /* Do another MatSetValues to test the case when only one local block is specified */
  for (i=0; i<3; i++) {
    for (j =0; j<3 ; j++)  y[i][j] = (PetscScalar)(10 + i*eval + j);
  }
  ierr = MatSetValuesBlocked(A,1,row,1,col,&y[0][0],INSERT_VALUES);CHKERRQ(ierr);
  ierr = MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

  
  ierr = PetscOptionsHasName(PETSC_NULL,"-zero_rows",&flg);CHKERRQ(ierr);
  if (flg) {
    col[0] = rstart*bs+0;
    col[1] = rstart*bs+1;
    col[2] = rstart*bs+2;
    ierr = ISCreateGeneral(MPI_COMM_SELF,3,col,&is);CHKERRQ(ierr);
    ierr = MatZeroRows(A,is,&one);CHKERRQ(ierr);
    ISDestroy(is);
  }

  ierr = MatView(A,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

  ierr = MatDestroy(A);CHKERRQ(ierr);
  ierr = PetscFinalize();CHKERRQ(ierr);
  return 0;
}
