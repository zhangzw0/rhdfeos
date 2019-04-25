/* do not remove */

#include <Rdefines.h>
#include <R_ext/Error.h>

#include <eos.h>

SEXP Reos_demo(void)
{
    int a = 1;
    int b = 1;
    int val = fun_add(a, b);  
    val = val - 100;   
    SEXP Rval;

    PROTECT(Rval = allocVector(INTSXP, 1));
    INTEGER(Rval)[0] = val;
    SEXP names = PROTECT(allocVector(STRSXP,1));
    SET_STRING_ELT(names, 0, mkChar("fun_add"));
    SET_NAMES(Rval, names);
    UNPROTECT(1);
    UNPROTECT(1);
    return Rval;
}

#include <R_ext/Rdynload.h>

R_CallMethodDef callMethods[] = {
  {"Reos_demo", (DL_FUNC) &Reos_demo, 0},
  {NULL, NULL, 0}
};

void R_init_rhdfeos(DllInfo *info)
{
  R_registerRoutines(info, NULL, callMethods, NULL, NULL);
}

