/*      Tdi1SetRange
        Set the upper and lower bounds of an array.
                SET_RANGE(range0,... array)

        Each range specifier must be:
                * : * or missing
                lower : *
                lower : upper
                upper or * : upper
        If the lower or upper bound is missing or * it is computed,
        If both are missing or the argument is omitted, no change is made.
        The result will have that many subscripts. Fortran-8x requires match.
        Missing values are supplied from the source array and require that dimension.
        Subscript triads are not allowed.

        Ken Klare, LANL CTR-7   (c)1989,1990
        NEED way to specify character string subscript range
*/

#define _MOVC3(a,b,c) memcpy(c,b,a)

#include <STATICdef.h>
#include "tdirefcat.h"
#include "tdirefstandard.h"
#include <tdishr_messages.h>
#include <stdlib.h>
#include <mdsshr.h>
#include <string.h>



extern int TdiGetArgs();
extern int TdiGetData();
extern int TdiGetLong();
extern int TdiConvert();
extern int TdiMasterData();

int Tdi1SetRange(int opcode, int narg, struct descriptor *list[], struct descriptor_xd *out_ptr)
{
  int status = 1;
  STATIC_CONSTANT DESCRIPTOR_A(arr0, 1, DTYPE_BU, 0, 1);
  struct descriptor_xd sig[1], uni[1], dat[1], tmp = EMPTY_XD;
  struct descriptor_range *prange;
  struct TdiCatStruct cats[2];
  array_bounds *pa = 0, arr;
  int bounds = 0, coeff = 0, defhi, deflo, dimct, hi, j, lo, ndim = 0, cmode = 0;

	/******************
        Get the data basis.
        ******************/
  status = TdiGetArgs(opcode, 1, &list[narg - 1], sig, uni, dat, cats);

  if (status & 1) {
    _MOVC3(sizeof(arr0), (char *)&arr0, (char *)&arr);
    pa = (array_bounds *) dat[0].pointer;
    if (pa == 0)
      status = TdiNULL_PTR;
		/*******************
                Accept scalars also.
                *******************/
    else
      switch (pa->class) {
      case CLASS_S:
      case CLASS_D:
	bounds = coeff = ndim = 0;
	break;
      case CLASS_A:
	if ((coeff = pa->aflags.coeff) == 1) {
	  ndim = pa->dimct;
	  bounds = pa->aflags.bounds;
	} else {
	  bounds = 0;
	  ndim = 1;
	}
	break;
      default:
	status = TdiINVCLADSC;
	break;
      }
  }
  arr.dimct = (unsigned char)(dimct = narg - 1);
  arr.a0 = 0;

	/******************************
        For each input, check defaults.
        ******************************/
  for (j = 0; j < dimct; ++j) {

		/*********************
                Omitted, use original.
                *********************/
    defhi = deflo = 0;
    if (list[j] == 0)
      defhi = deflo = 1;

		/*****************
                Get data or range.
                *****************/
    else {
      unsigned char dtype = (unsigned char)DTYPE_RANGE;
      if (status & 1)
	status = TdiGetData(&dtype, list[j], &tmp);
      if (!(status & 1))
	break;
      prange = (struct descriptor_range *)tmp.pointer;

			/**********************************
                        Simple limit is number of elements.
                        **********************************/
      if (prange->dtype != DTYPE_RANGE) {
	lo = 0;
	status = TdiGetLong(&tmp, &hi);
	--hi;
      } else if (!(prange->ndesc == 2 || (prange->ndesc == 3 && prange->deltaval == 0)))
	status = TdiINVDTYDSC;
      else {
	if (prange->begin)
	  status = TdiGetLong(prange->begin, &lo);
	else
	  deflo = 1;
	if (prange->ending && status & 1)
	  status = TdiGetLong(prange->ending, &hi);
	else
	  defhi = 1;
      }
    }

		/***************************
                Fill in defaults, if we can.
                ***************************/
    if (status & 1 && (defhi || deflo)) {
      if (j >= ndim)
	status = TdiINVDTYDSC;
      else if (defhi) {
	if (deflo) {
	  if (bounds)
	    lo = pa->m[dimct + 2 * j];
	  else
	    lo = 0;
	}
	if (coeff)
	  hi = pa->m[j] - 1 + lo;
	else
	  hi = (int)pa->arsize / (int)pa->length - 1 + lo;
      } else {
	if (coeff)
	  lo = hi + 1 - pa->m[j];
	else
	  lo = hi + 1 - (int)pa->arsize / (int)pa->length;
      }
    }

    if (lo != 0)
      arr.aflags.bounds = 1;
    arr.a0 -= lo * (int)arr.arsize;
    arr.m[dimct + 2 * j] = lo;
    arr.m[dimct + 2 * j + 1] = hi;
    arr.m[j] = hi - lo + 1;
    arr.arsize *= arr.m[j];
  }
  MdsFree1Dx(&tmp, NULL);

  arr.aflags.coeff = (unsigned char)(dimct > 1 || arr.aflags.bounds);
  if (status & 1)
    status = MdsGet1DxA((struct descriptor_a *)&arr, &pa->length, &pa->dtype, out_ptr);

	/***********************
        Copy/expand data to new.
        ***********************/
  if (status & 1)
    status = TdiConvert(pa, out_ptr->pointer MDS_END_ARG);

	/******************************
        Embed result in signal, if any.
        Then free input.
        ******************************/
  if (status & 1)
    status = TdiMasterData(1, sig, uni, &cmode, out_ptr);
  if (sig[0].pointer)
    MdsFree1Dx(&sig[0], NULL);
  if (uni[0].pointer)
    MdsFree1Dx(&uni[0], NULL);
  if (dat[0].pointer)
    MdsFree1Dx(&dat[0], NULL);
  return status;
}
