/*      Tdi1Pack.C
        F8X transformation to pack an array into a vector under control of a mask.

                PACK(ARRAY, MASK, [VECTOR])

        ARRAY   the source when MASK is true, must be array.
        MASK    logical conformable with ARRAY, missing is false.
        VECTOR  vector with as many elements as trues in MASK or
                with at least as many elements as ARRAY if MASK is scalar true.
                We accept a scalar and short arrays and match types.
        Result type is confomation of ARRAY and VECTOR.
        Result size is VECTOR size if present,
                number of trues in MASK if not scalar true,
                else ARRAY size.
        Result class is a vector.

        Ken Klare, LANL CTR-7   (c)1990
*/

#include <STATICdef.h>
#define _MOVC3(a,b,c) memcpy(c,b,a)
#include "tdirefcat.h"
#include "tdirefstandard.h"
#include <tdishr_messages.h>
#include <stdlib.h>
#include <mdsshr.h>
#include <string.h>



extern int TdiGetArgs();
extern int TdiMasterData();
extern int TdiCvtArgs();
extern int Tdi2Pack();
extern int TdiConvert();

int Tdi1Pack(int opcode, int narg, struct descriptor *list[], struct descriptor_xd *out_ptr)
{
  int status = 1;
  int lena, lenm, numa, numm, numv = 0, bytes, j, cmode = -1;
  char *pi, *pm, *po;
  struct descriptor_xd sig[3], uni[3], dat[3];
  struct TdiCatStruct cats[4];

  status = TdiGetArgs(opcode, narg, list, sig, uni, dat, cats);
  if (status & 1 && dat[0].pointer->class != CLASS_A)
    status = TdiINVCLADSC;
  if (status & 1)
    status = Tdi2Pack(narg, uni, dat, cats, 0);
  if (status & 1)
    status = TdiCvtArgs(narg, dat, cats);
  if (status & 1) {
    struct descriptor_a *parr = (struct descriptor_a *)dat[0].pointer;
    bytes = parr->arsize;
    lena = parr->length;
    if (narg > 2) {
      if (dat[2].pointer->class != CLASS_A)
	numv = -2;
      else if (dat[2].pointer->dtype != DTYPE_MISSING)
	numv = (int)((struct descriptor_a *)dat[2].pointer)->arsize / lena;
    } else
      numv = -1;

	/*******************************************************************************
        * Array MASK: Conform ARRAY to MASK (we use smaller). Result is condensed ARRAY.
        * Scalar MASK: Result is ARRAY for true or null for false.
        *******************************************************************************/
    if (dat[1].pointer->class == CLASS_A) {
      lenm = dat[1].pointer->length;
      numa = bytes / lena;
      numm = (int)((struct descriptor_a *)dat[1].pointer)->arsize / lenm;
      pi = parr->pointer;
#ifdef WORDS_BIGENDIAN
      pm = dat[1].pointer->pointer + lenm - 1;
#else
      pm = dat[1].pointer->pointer;
#endif
      po = pi;

      if (numa < numm)
	numm = numa;
      if (numv >= 0 && numm > numv)
	numm = numv;
      for (; --numm >= 0; pm += lenm, pi += lena)
	if (*pm & 1) {
	  _MOVC3(lena, pi, po);
	  po += lena;
	};
      bytes = po - dat[0].pointer->pointer;
    } else if (!(*dat[1].pointer->pointer & 1))
      bytes = 0;

	/*********************************************
        * Possibly augmented by VECTOR.
        * (-2) If it is a scalar, fill out ARRAY.
        * (++) If it is a vector, copy to VECTOR.
        * (-1) If missing/too short, use partial XD.
        *********************************************/
    parr->aflags.coeff = 0;
    parr->aflags.bounds = 0;
    parr->dimct = 1;
    MdsFree1Dx(out_ptr, NULL);
    if (numv == -2) {
      parr->pointer += bytes;
      parr->arsize -= bytes;
      status = TdiConvert(dat[2].pointer, parr MDS_END_ARG);
      parr->pointer -= bytes;
      parr->arsize += bytes;
      *out_ptr = dat[0];
      dat[0] = EMPTY_XD;
    } else if (bytes < numv * lena) {
      memcpy(dat[2].pointer->pointer, parr->pointer, bytes);
      *out_ptr = dat[2];
      dat[2] = EMPTY_XD;
    } else {
      parr->arsize = bytes;
      *out_ptr = dat[0];
      dat[0] = EMPTY_XD;
    }
  }
  if (status & 1)
    status = TdiMasterData(0, sig, uni, &cmode, out_ptr);
  for (j = narg; --j >= 0;) {
    MdsFree1Dx(&sig[j], NULL);
    MdsFree1Dx(&uni[j], NULL);
    MdsFree1Dx(&dat[j], NULL);
  }
  return status;
}
