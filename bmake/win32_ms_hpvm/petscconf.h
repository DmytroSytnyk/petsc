
#if !defined(INCLUDED_PETSCCONF_H)
#define INCLUDED_PETSCCONF_H

#define PARCH_win32 
#define PETSC_ARCH_NAME "win32_ms_hpvm"
#define PETSC_HAVE_WIN32
#define PETSC_HAVE_LIMITS_H
#define PETSC_HAVE_STDLIB_H 
#define PETSC_HAVE_STRING_H 
#define PETSC_HAVE_SEARCH_H
#define PETSC_HAVE_IO_H
#define PETSC_HAVE_SYS_STAT_H

#define PETSC_STDCALL __stdcall
#define PETSC_HAVE_FORTRAN_MIXED_STR_ARG

#define PETSC_HAVE_FORTRAN_CAPS 

#define PETSC_HAVE_READLINK
#define PETSC_HAVE_MEMMOVE

#define PETSC_HAVE_RAND
#define PETSC_CANNOT_START_DEBUGGER
#define PETSC_HAVE_CLOCK

#define PETSC_HAVE_GET_USER_NAME
#define PETSC_SIZEOF_VOID_P 4
#define PETSC_SIZEOF_INT 4
#define PETSC_SIZEOF_DOUBLE 8
#define PETSC_BITS_PER_BYTE 8
#define PETSC_SIZEOF_FLOAT 4
#define PETSC_SIZEOF_LONG 4
#define PETSC_SIZEOF_LONG_LONG 8

#define PETSC_USE_NT_TIME
#define PETSC_HAVE_NO_GETRUSAGE

#define PETSC_HAVE_F90_H "f90impl/f90_win32.h"
#define PETSC_HAVE_F90_C "src/sys/src/f90/f90_win32.c"

#define PETSC_MISSING_SIGBUS
#define PETSC_MISSING_SIGQUIT
#define PETSC_MISSING_SIGSYS
#define PETSC_MISSING_SIGHUP
#define PETSC_MISSING_SIGTRAP
#define PETSC_MISSING_SIGKILL
#define PETSC_MISSING_SIGUSR1
#define PETSC_MISSING_SIGUSR2
#define PETSC_MISSING_SIGPIPE
#define PETSC_MISSING_SIGALRM
#define PETSC_MISSING_SIGURG
#define PETSC_MISSING_SIGSTOP
#define PETSC_MISSING_SIGTSTP
#define PETSC_MISSING_SIGCONT
#define PETSC_MISSING_SIGCHLD

#define PETSC_HAVE__ACCESS
#define PETSC_HAVE__GETCWD
#define PETSC_HAVE__SLEEP
#define PETSC_HAVE__SLEEP_MILISEC
#define PETSC_HAVE__SNPRINTF

#define PETSC_USE_NARGS
#define PETSC_HAVE_IARG_COUNT_PROGNAME

#endif
