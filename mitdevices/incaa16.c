#include <mdsdescrip.h>
#include <mds_gendevice.h>
#include <mitdevices_msg.h>
#include <mds_stdarg.h>
#include "incaa16_gen.h"
#include <libroutines.h>
#include <strroutines.h>
#include <treeshr.h>
#include <mdsshr.h>
#include <stdlib.h>

typedef struct _CSRegister {
  unsigned clock:4;
  unsigned chans:3;
  unsigned c_off:4;
  unsigned master:1;
  unsigned c_gen:1;
  unsigned state:2;
  unsigned recirc:1;
  unsigned mode:1;
  unsigned:7;
} CSRegister;

static int one = 1;
#define return_on_error(f,retstatus) if (!((status = f) & 1)) return retstatus;
#define pio(f,a, d) {\
 if (!((status = DevCamChk(CamPiow(setup->name,a,f,d,24,0),&one,&one)) & 1)) return status;}
#define fstop(f,a,num,data) {\
 if (!((status = DevCamChk(CamFStopw(setup->name,a,f,num,data,16,0),&one,&one)) & 1)) return status;}
extern int DevCamChk();
extern int DevWait(float);
extern int CamPiow();
extern int CamFStopw();
extern int TdiGetFloat();
extern int DevLong();

extern unsigned short OpcMultiply;
extern unsigned short OpcValue;

int incaa16___arm(struct descriptor *niddsc, InArmStruct * setup)
{
  int status;
  float freq;
  CSRegister csreg;
  if (setup->int_clk_frq) {
    if ((status = TdiGetFloat(setup->int_clk_frq, &freq)) & 1) {
      int i;
      static float freqs[] = { 50, 20, 10, 5, 2, 1, .5, .2 };
      for (i = 0; i < 8; i++)
	if (freq >= freqs[i])
	  break;
      csreg.clock = i + 1;
    } else
      return status;
  } else
    csreg.clock = 0;
  csreg.chans = setup->active_chans_convert;
  csreg.c_off = 0;
  csreg.master = setup->master;
  csreg.c_gen = setup->ext_1mhz;
  csreg.recirc = csreg.mode = 1;
  pio(28, 0, 0);
  {
    DevWait((float)5E-6);
  }
  pio(28, 0, 0);		/* Initialize */
  pio(16, 2, (int *)&csreg);
  pio(16, 1, &setup->ptsc);
  pio(11, 0, 0);
  return status;
}

int incaa16___init(struct descriptor *niddsc, InInitStruct * setup)
{
  int status = incaa16___arm(niddsc, (InArmStruct *) setup);
  pio(25, 0, 0);
  return status;
}

int incaa16___trigger(struct descriptor *niddsc, InTriggerStruct * setup)
{
  int status;
  pio(25, 2, 0);
  return status;
}

int incaa16___store(struct descriptor_s *niddsc_ptr, InStoreStruct * setup)
{

#define min(a,b) ((a) <= (b)) ? (a) : (b)
#define max(a,b) ((a) >= (b)) ? (a) : (b)

  static struct descriptor_d name = { 0, DTYPE_T, CLASS_D, 0 };
  static DESCRIPTOR_A_BOUNDS(raw, sizeof(short), DTYPE_W, 0, 1, 0);
  static DESCRIPTOR(counts_str, "counts");
  static DESCRIPTOR_WITH_UNITS(counts, &raw, &counts_str);
  static DESCRIPTOR_LONG(start_d, &raw.bounds[0].l);
  static DESCRIPTOR_LONG(end_d, &raw.bounds[0].u);
  static int trig_in_nid;
  static DESCRIPTOR_NID(trigger_d, &trig_in_nid);
  static float frequency;
  static DESCRIPTOR_FLOAT(frequency_d, &frequency);
  static DESCRIPTOR_RANGE(int_clock_d, 0, 0, &frequency_d);
  static int clock_in_nid;
  static DESCRIPTOR_NID(ext_clock_d, &clock_in_nid);
  static float coefficient = 20.0 / 4096;
  static DESCRIPTOR_FLOAT(coef_d, &coefficient);
  static int key;
  static DESCRIPTOR_LONG(key_d, &key);
  static DESCRIPTOR_FUNCTION_1(value, (unsigned char *)&OpcValue, 0);
  static DESCRIPTOR_FUNCTION_2(mult_exp, (unsigned char *)&OpcMultiply, &coef_d, &value);
  static DESCRIPTOR(volts_str, "volts");
  static DESCRIPTOR_WITH_UNITS(volts, &mult_exp, &volts_str);
  static DESCRIPTOR_WINDOW(window, &start_d, &end_d, &trigger_d);
  static int clock_out_nid;
  static DESCRIPTOR_NID(clock_out_d, &clock_out_nid);
  static DESCRIPTOR_DIMENSION(dimension, &window, &clock_out_d);
  static DESCRIPTOR(time_str, "seconds");
  static DESCRIPTOR_WITH_UNITS(time, &dimension, &time_str);
  static DESCRIPTOR_SIGNAL_1(signal, &volts, &counts, &time);
  short *data_ptr;
  int status;
  int chan;
  int active_chans;
  static int chans_select[] = { 1, 2, 4, 8, 16 };
  int samples_to_read;
  int samps_per_chan;
  int i;
  int polarity;
  CSRegister csreg = { 4, 4, 0, 1, 1, 0, 0, 1 };
  int min_idx;
  int max_idx;
  int pts = 0;
  unsigned int start_addr = 0;
  unsigned int first_start_addr;
  int tries;
  int memsize;
  trig_in_nid = setup->head_nid + INCAA16_N_TRIG_IN;
  clock_in_nid = setup->head_nid + INCAA16_N_CLOCK_IN;
  clock_out_nid = setup->head_nid + INCAA16_N_CLOCK_OUT;
  value.ndesc = 0;

  pio(0, 2, (int *)&csreg);
  memsize = csreg.mode ? 256 * 1024 : 64 * 1024;
  if (csreg.state != 0)
    return DEV$_NOT_TRIGGERED;
  if (csreg.clock == 0) {
    return_on_error(TreePutRecord(clock_out_nid, (struct descriptor *)&ext_clock_d, 0), status);
  } else {
    static float freqs[] = { 0, 2E-5, 5E-5, 1E-4, 2E-4, 5E-4, 1E-3, 2E-3, 5E-3, 1E-2 };
    frequency = freqs[min(csreg.clock, 9)];
    return_on_error(TreePutRecord(clock_out_nid, (struct descriptor *)&int_clock_d, 0), status);
  }
  active_chans = chans_select[min(csreg.chans, 4)];
  samps_per_chan = memsize / active_chans;
  pio(0, 0, &first_start_addr);
  start_addr = first_start_addr % memsize;
  max_idx = setup->ptsc - 1;
  min_idx = setup->ptsc - samps_per_chan;
  if (raw.pointer)
    free(raw.pointer);
  raw.pointer = malloc(samps_per_chan * 2);
  for (chan = 0; ((chan < active_chans) && (status & 1)); chan++) {
    int input_nid =
	setup->head_nid + INCAA16_N_INPUT_01 + chan * (INCAA16_N_INPUT_02 - INCAA16_N_INPUT_01);
    int startidx_nid = input_nid + 1;
    int endidx_nid = input_nid + 2;
    if (TreeIsOn(input_nid) & 1) {
      status = DevLong(&startidx_nid, &raw.bounds[0].l);
      if (status & 1)
	raw.bounds[0].l = min(max_idx, max(min_idx, raw.bounds[0].l));
      else
	raw.bounds[0].l = min_idx;

      status = DevLong(&endidx_nid, &raw.bounds[0].u);
      if (status & 1)
	raw.bounds[0].u = min(max_idx, max(min_idx, raw.bounds[0].u));
      else
	raw.bounds[0].u = max_idx;

      raw.m[0] = raw.bounds[0].u - raw.bounds[0].l + 1;
      if (raw.m[0] > 0) {
	int addr;
	int samps;
	status = 1;
	for (samples_to_read = raw.m[0], data_ptr = (short *)raw.pointer,
	     addr =
	     (unsigned int)(start_addr + chan +
			    (raw.bounds[0].l - min_idx) * active_chans) % memsize;
	     (samples_to_read > 0) && (status & 1);
	     samples_to_read -= samps, data_ptr += samps, addr += (samps * active_chans)) {
	  pio(16, 0, &addr);
	  samps = min(samples_to_read, 32767);
	  fstop(2, 0, samps, data_ptr);
	}
	if (status & 1) {
	  raw.a0 = raw.pointer - raw.bounds[0].l;
	  raw.arsize = raw.m[0] * 2;
	  status = TreePutRecord(input_nid, (struct descriptor *)&signal, 0);
	}
      }
    }
  }
  pio(16, 0, &first_start_addr);
  free(raw.pointer);
  raw.pointer = 0;
  return status;
}
