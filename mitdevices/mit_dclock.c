#include <mdsdescrip.h>
#include <mds_gendevice.h>
#include <mitdevices_msg.h>
#include <mds_stdarg.h>

#include <treeshr.h>
#include <string.h>
#include "mit_dclock_gen.h"
#include "decoder.h"
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

typedef struct descriptor *Dptr;

extern int mit_dclock___get_setup(Dptr, InGet_setupStruct *);
extern int TdiCompile();
extern int GenDeviceFree();

int mit_dclock__get_setup(Dptr niddsc_ptr, Dptr method, DecoderSetup * setup,
			  EventMask * event_mask, Dptr * output)
{
  int status;
  InGet_setupStruct s;
  status = mit_dclock___get_setup(niddsc_ptr, &s);
  if (status & 1) {
    float max_period;
    float period;
    static float dt1;
    static float dt2;
    DESCRIPTOR_FLOAT(dt1_dsc, &dt1);
    DESCRIPTOR_FLOAT(dt2_dsc, &dt2);
    int clock_source;
    static DESCRIPTOR(output_exp, "MIT_DCLOCK_RANGE(-4.0,$3,[$1,$2],NOT GETNCI($3,'ON'))");
    static EMPTYXD(out);
    static int gate_nid;
    static DESCRIPTOR_NID(gate_dsc, (char *)&gate_nid);
    gate_nid = s.head_nid + MIT_DCLOCK_N_GATE;
    memset(event_mask, 0, sizeof(EventMask));
    if ((s.frequency_1 <= 0) || (s.frequency_2 <= 0)) {
      status = TIMING$_INVCLKFRQ;
      goto error;
    }
    max_period = 1 / min(s.frequency_1, s.frequency_2);
    for (clock_source = EXT_1MHZ, period = 1E-6;
	 period * 65534 < max_period && clock_source <= EXT_100HZ; clock_source++, period *= 10) ;
    if (clock_source > EXT_100HZ) {
      status = TIMING$_INVCLKFRQ;
      goto error;
    }
    setup->output_control = TOGGLE;
    setup->start_high = 0;
    setup->count_up = 0;
    setup->bcd_count = 0;
    setup->repeat_count = 1;
    setup->double_load = 1;
    setup->special_gate = 1;
    setup->clock_source = clock_source;
    setup->falling_edge = 0;
    setup->gating = GATE_NONE;
    setup->load = 1 / s.frequency_1 / period / 2 + .49999;
    setup->hold = 1 / s.frequency_2 / period / 2 + .4999;
    if (setup->load == 0) {
      setup->load++;
      setup->hold--;
    }
    if (setup->hold == 0) {
      setup->load--;
      setup->hold++;
    }
    dt1 = setup->load * period * 2;
    dt2 = setup->hold * period * 2;
    status = TdiCompile(&output_exp, &dt1_dsc, &dt2_dsc, &gate_dsc, &out MDS_END_ARG);
    if (status & 1) {
      static int output_nid;
      static DESCRIPTOR_NID(output_dsc, (char *)&output_nid);
      output_nid = s.head_nid + MIT_DCLOCK_N_OUTPUT;
      status = TreePutRecord(output_nid, (struct descriptor *)&out, 0);
      *output = (status & 1) ? &output_dsc : 0;
    } else
      *output = 0;
    GenDeviceFree(&s);
  }
  return status;
 error:
  GenDeviceFree(&s);
  return status;
}
