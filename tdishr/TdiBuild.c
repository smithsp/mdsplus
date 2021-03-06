/*      Tdi1Build.C
        Generic builds and such.

        Ken Klare, LANL P-4     (c)1989,1990,1991
*/
#include <stdlib.h>
#include <mdsdescrip.h>
#include "tdirefcat.h"
#include "tdireffunction.h"
#include "tdirefstandard.h"
#include <tdishr_messages.h>
#include <mdsshr.h>
#include <STATICdef.h>



extern int TdiGetLong();
extern int TdiEvaluate();
extern int TdiGetArgs();
extern int TdiCvtArgs();
extern int TdiMasterData();

/*--------------------------------------------------------------
        Return without evaluation. Passes paths, nids, and functions.
                any = AS_IS(any)
*/
int Tdi1AsIs(int opcode, int narg, struct descriptor *list[], struct descriptor_xd *out_ptr)
{
  int status = 1;

  status = MdsCopyDxXd(list[0], out_ptr);
  return status;
}

/*--------------------------------------------------------------
        Make a class-R record of pointers.
        Standard generic call:
                result = BUILD_xxxx(arg1,...)

        The first argument is evaluated for
        BUILD_CONDITION (byte modifier)
        BUILD_DEPENDENCY (byte op_code)
        BUILD_DISPATCH (byte type)
        BUILD_FUNCTION (word opcode)
        BUILD_CALL (byte dtype)
        and the remaining arguments are shifted by one.
        Each argument is NOT evaluated to get "real" data or to do further builds:
        BUILD_ACTION(BUILD_DISPATCH(...),BUILD_TASK(...))

        Limitations:
                254 arguments in some uses plus 1 for output.
                No type checking at build time.
                No arrays of descriptors.
*/
int Tdi1Build(int opcode, int narg, struct descriptor *list[], struct descriptor_xd *out_ptr)
{
  int status = 1;
  struct descriptor *ptr;
  struct TdiFunctionStruct *fun_ptr = (struct TdiFunctionStruct *)&TdiRefFunction[opcode];
  DESCRIPTOR_FUNCTION(build, 0, 255);
  int j, k, modif;
  unsigned short modif_s;
  unsigned char modif_c;

  build.length = 0;
  build.dtype = fun_ptr->o1;
  build.ndesc = (unsigned char)narg;
  if (fun_ptr->o2 != fun_ptr->o1) {
    status = TdiGetLong(list[0], &modif);
    switch (fun_ptr->o2) {
    case DTYPE_BU:
      build.length = 1;
      modif_c = (unsigned char)modif;
      build.pointer = &modif_c;
      break;
    case DTYPE_WU:
      build.length = 2;
      modif_s = (unsigned short)modif;
      build.pointer = (unsigned char *)&modif_s;
      break;
    default:
      status = TdiINVDTYDSC;
      break;
    }
    --build.ndesc;
    j = 1;
  } else
    j = 0;
  for (k = 0; j < narg; ++j, ++k) {
    ptr = list[j];
    while (ptr && ptr->dtype == DTYPE_DSC)
      ptr = (struct descriptor *)ptr->pointer;
    build.arguments[k] = ptr;
  }
  if (status & 1)
    status = MdsCopyDxXd((struct descriptor *)&build, out_ptr);
  return status;
}

/*--------------------------------------------------------------
        Make text into data type path.
                BUILD_PATH(string)
                BUILD_EVENT(string)
        WARNING: BUILD_PATH and BUILD_EVENT should be called MAKE_xxx.
*/
int Tdi1BuildPath(int opcode, int narg, struct descriptor *list[], struct descriptor_xd *out_ptr)
{
  int status = 1;
  struct descriptor_xd sig[1], uni[1], dat[1];
  struct TdiCatStruct cats[2];
  int cmode = 0;

  status = TdiGetArgs(opcode, narg, list, sig, uni, dat, cats);
  cats[1].out_dtype = cats[1].in_dtype;
  cats[1].out_cat = cats[1].in_cat;
  if (status & 1)
    status = TdiCvtArgs(narg, dat, cats);
	/*********************************
        Change from text to path or event.
        *********************************/
  if (status & 1) {
    dat[0].pointer->dtype = TdiRefFunction[opcode].o1;
    status = MdsCopyDxXd((struct descriptor *)&dat[0], out_ptr);
  }
  if (status & 1)
    status = TdiMasterData(narg, sig, uni, &cmode, out_ptr);
  if (sig[0].pointer)
    MdsFree1Dx(&sig[0], NULL);
  if (uni[0].pointer)
    MdsFree1Dx(&uni[0], NULL);
  if (dat[0].pointer)
    MdsFree1Dx(&dat[0], NULL);
  return status;
}

/*--------------------------------------------------------------
        Evaluate arguments before building into the given data type.
        Use in returning calculations using local variables from a FUN.
        Some arguments may be "unevaluated" by using an AS_IS(arg).
        This can be used to pass back expressions evaluated in an outer FUN.
        So BUILD_xxx(x,y,...) is the same as MAKE_xxx(AS_IS(x),AS_IS(y),...).
*/
int Tdi1Make(int opcode, int narg, struct descriptor *list[], struct descriptor_xd *out_ptr)
{
  int status = 1;
  struct descriptor *ptr;
  int j, k, modif;
  struct TdiFunctionStruct *fun_ptr = (struct TdiFunctionStruct *)&TdiRefFunction[opcode];
  DESCRIPTOR_FUNCTION(build, 0, 255);
  struct descriptor_xd xd[255];
  unsigned char modif_c;
  unsigned short modif_s;

  build.length = 0;
  build.dtype = fun_ptr->o1;
  build.ndesc = (unsigned char)narg;
  if (fun_ptr->o2 != fun_ptr->o1) {
    status = TdiGetLong(list[0], &modif);
    switch (fun_ptr->o2) {
    case DTYPE_BU:
      build.length = 1;
      modif_c = (unsigned char)modif;
      build.pointer = &modif_c;
      break;
    case DTYPE_WU:
      build.length = 2;
      modif_s = (unsigned short)modif;
      build.pointer = (unsigned char *)&modif_s;
      break;
    default:
      status = TdiINVDTYDSC;
      break;
    }
    --build.ndesc;
    j = 1;
  } else
    j = 0;
  for (k = 0; j < narg && status & 1; ++j, ++k) {
    xd[k] = EMPTY_XD;
    if (list[j])
      status = TdiEvaluate(list[j], &xd[k] MDS_END_ARG);
    ptr = (struct descriptor *)&xd[k];
    while (ptr && ptr->dtype == DTYPE_DSC)
      ptr = (struct descriptor *)ptr->pointer;
    if (ptr && ptr->dtype == DTYPE_MISSING)
      ptr = 0;
    build.arguments[k] = ptr;
  }
  if (status & 1)
    status = MdsCopyDxXd((struct descriptor *)&build, out_ptr);
  for (; --k >= 0;)
    MdsFree1Dx(&xd[k], NULL);
  return status;
}
