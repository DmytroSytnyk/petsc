/* $Id: petscts.h,v 1.46 2001/04/10 19:37:08 bsmith Exp bsmith $ */
/*
   User interface for the timestepping package. This is package
   is for use in solving time-dependent PDEs.
*/
#if !defined(__PETSCTS_H)
#define __PETSCTS_H
#include "petscsnes.h"

/*S
     TS - Abstract PETSc object that manages all time-steppers (ODE integrators)

   Level: beginner

  Concepts: ODE solvers

.seealso:  TSCreate(), TSSetType(), TSType, SNES, SLES, KSP, PC
S*/
typedef struct _p_TS* TS;

#define TS_COOKIE PETSC_COOKIE+18

/*E
    TSType - String with the name of a PETSc TS method or the creation function
       with an optional dynamic library name, for example
       http://www.mcs.anl.gov/petsc/lib.a:mytscreate()

   Level: beginner

.seealso: TSSetType(), TS
E*/
#define TS_EULER           "euler"
#define TS_BEULER          "beuler"
#define TS_PSEUDO          "pseudo"
#define TS_CRANK_NICHOLSON "crank-nicholson"
#define TS_PVODE           "pvode"
typedef char *TSType;

/*E
    TSProblemType - Determines the type of problem this TS object is to be used to solve

   Level: beginner

.seealso: TSCreate()
E*/
typedef enum {TS_LINEAR,TS_NONLINEAR} TSProblemType;

EXTERN int TSCreate(MPI_Comm,TSProblemType,TS*);
EXTERN int TSSetType(TS,TSType);
EXTERN int TSGetProblemType(TS,TSProblemType*);
EXTERN int TSDestroy(TS);

EXTERN int TSSetMonitor(TS,int(*)(TS,int,PetscReal,Vec,void*),void *,int (*)(void*));
EXTERN int TSClearMonitor(TS);
EXTERN int TSGetType(TS,TSType*);

EXTERN int TSSetOptionsPrefix(TS,char *);
EXTERN int TSAppendOptionsPrefix(TS,char *);
EXTERN int TSGetOptionsPrefix(TS,char **);
EXTERN int TSSetFromOptions(TS);
EXTERN int TSSetUp(TS);

EXTERN int TSSetSolution(TS,Vec);
EXTERN int TSGetSolution(TS,Vec*);

EXTERN int TSSetDuration(TS,int,PetscReal);

EXTERN int TSDefaultMonitor(TS,int,PetscReal,Vec,void*);
EXTERN int TSVecViewMonitor(TS,int,PetscReal,Vec,void*);
EXTERN int TSStep(TS,int *,PetscReal*);

EXTERN int TSSetInitialTimeStep(TS,PetscReal,PetscReal);
EXTERN int TSGetTimeStep(TS,PetscReal*);
EXTERN int TSGetTime(TS,PetscReal*);
EXTERN int TSGetTimeStepNumber(TS,int*);
EXTERN int TSSetTimeStep(TS,PetscReal);

EXTERN int TSSetRHSFunction(TS,int (*)(TS,PetscReal,Vec,Vec,void*),void*);
EXTERN int TSSetRHSMatrix(TS,Mat,Mat,int (*)(TS,PetscReal,Mat*,Mat*,MatStructure*,void*),void*);
EXTERN int TSSetRHSJacobian(TS,Mat,Mat,int(*)(TS,PetscReal,Vec,Mat*,Mat*,MatStructure*,void*),void*);
EXTERN int TSSetRHSBoundaryConditions(TS,int (*)(TS,PetscReal,Vec,void*),void*);

EXTERN int TSDefaultComputeJacobianColor(TS,PetscReal,Vec,Mat*,Mat*,MatStructure*,void*);
EXTERN int TSDefaultComputeJacobian(TS,PetscReal,Vec,Mat*,Mat*,MatStructure*,void*);

EXTERN int TSGetRHSMatrix(TS,Mat*,Mat*,void**);
EXTERN int TSGetRHSJacobian(TS,Mat*,Mat*,void**);

EXTERN int TSPseudoSetTimeStep(TS,int(*)(TS,PetscReal*,void*),void*);
EXTERN int TSPseudoDefaultTimeStep(TS,PetscReal*,void*);
EXTERN int TSPseudoComputeTimeStep(TS,PetscReal *);

EXTERN int TSPseudoSetVerifyTimeStep(TS,int(*)(TS,Vec,void*,PetscReal*,int*),void*);
EXTERN int TSPseudoDefaultVerifyTimeStep(TS,Vec,void*,PetscReal*,int*);
EXTERN int TSPseudoVerifyTimeStep(TS,Vec,PetscReal*,int*);
EXTERN int TSPseudoSetTimeStepIncrement(TS,PetscReal);
EXTERN int TSPseudoIncrementDtFromInitialDt(TS);

EXTERN int TSComputeRHSFunction(TS,PetscReal,Vec,Vec);
EXTERN int TSComputeRHSBoundaryConditions(TS,PetscReal,Vec);
EXTERN int TSComputeRHSJacobian(TS,PetscReal,Vec,Mat*,Mat*,MatStructure*);

extern PetscFList      TSList;
EXTERN int        TSRegisterAll(char*);
EXTERN int        TSRegisterDestroy(void);
extern PetscTruth TSRegisterAllCalled;

EXTERN int TSRegister(char*,char*,char*,int(*)(TS));
#if defined(PETSC_USE_DYNAMIC_LIBRARIES)
#define TSRegisterDynamic(a,b,c,d) TSRegister(a,b,c,0)
#else
#define TSRegisterDynamic(a,b,c,d) TSRegister(a,b,c,d)
#endif

EXTERN int TSGetSNES(TS,SNES*);
EXTERN int TSGetSLES(TS,SLES*);

EXTERN int TSView(TS,PetscViewer);

EXTERN int TSSetApplicationContext(TS,void *);
EXTERN int TSGetApplicationContext(TS,void **);

EXTERN int TSLGMonitorCreate(char *,char *,int,int,int,int,PetscDrawLG *);
EXTERN int TSLGMonitor(TS,int,PetscReal,Vec,void *);
EXTERN int TSLGMonitorDestroy(PetscDrawLG);

/*
       PETSc interface to PVode
*/
#define PVODE_UNMODIFIED_GS PVODE_CLASSICAL_GS
typedef enum { PVODE_ADAMS,PVODE_BDF } TSPVodeType;
typedef enum { PVODE_MODIFIED_GS = 0,PVODE_CLASSICAL_GS = 1 } TSPVodeGramSchmidtType;
EXTERN int TSPVodeSetType(TS,TSPVodeType);
EXTERN int TSPVodeGetPC(TS,PC*);
EXTERN int TSPVodeSetTolerance(TS,PetscReal,PetscReal);
EXTERN int TSPVodeGetIterations(TS,int *,int *);
EXTERN int TSPVodeSetGramSchmidtType(TS,TSPVodeGramSchmidtType);
EXTERN int TSPVodeSetGMRESRestart(TS,int);
EXTERN int TSPVodeSetLinearTolerance(TS,PetscReal);
EXTERN int TSPVodeSetExactFinalTime(TS,PetscTruth);

#endif





