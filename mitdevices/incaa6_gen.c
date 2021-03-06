#include <mitdevices_msg.h>
#include <mds_gendevice.h>
#include "incaa6_gen.h"
int incaa6__add(struct descriptor *name_d_ptr, struct descriptor *dummy_d_ptr, int *nid_ptr)
{
  static DESCRIPTOR(library_d, "MIT$DEVICES");
  static DESCRIPTOR(model_d, "INCAA6");
  static DESCRIPTOR_CONGLOM(conglom_d, &library_d, &model_d, 0, 0);
  int usage = TreeUSAGE_DEVICE;
  int curr_nid, old_nid, head_nid, status;
  long int flags = NciM_WRITE_ONCE;
  NCI_ITM flag_itm[] = { {2, NciSET_FLAGS, 0, 0}, {0, 0, 0, 0} };
  char *name_ptr = strncpy(malloc(name_d_ptr->length + 1), name_d_ptr->pointer, name_d_ptr->length);
  flag_itm[0].pointer = (unsigned char *)&flags;
  name_ptr[name_d_ptr->length] = 0;
  status = TreeStartConglomerate(INCAA6_K_CONG_NODES);
  if (!(status & 1))
    return status;
  status = TreeAddNode(name_ptr, &head_nid, usage);
  if (!(status & 1))
    return status;
  *nid_ptr = head_nid;
  status = TreeSetNci(head_nid, flag_itm);
  status = TreePutRecord(head_nid, (struct descriptor *)&conglom_d, 0);
  if (!(status & 1))
    return status;
  status = TreeGetDefaultNid(&old_nid);
  if (!(status & 1))
    return status;
  status = TreeSetDefaultNid(head_nid);
  if (!(status & 1))
    return status;
 ADD_NODE(:NAME, TreeUSAGE_TEXT)
 ADD_NODE(:COMMENT, TreeUSAGE_TEXT)
#define expr " 5.0       "
 ADD_NODE_EXPR(:INT_CLK_FRQ, TreeUSAGE_NUMERIC)
#undef expr
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(:CLOCK_IN, TreeUSAGE_AXIS)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(:CLOCK_OUT, TreeUSAGE_AXIS)
      flags |= NciM_WRITE_ONCE;
  flags |= NciM_NO_WRITE_MODEL;
  status = TreeSetNci(curr_nid, flag_itm);
#define expr " 0.	"
 ADD_NODE_EXPR(:TRIG_IN, TreeUSAGE_NUMERIC)
#undef expr
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
#define expr " TRIG_IN   "
 ADD_NODE_EXPR(:TRIG_OUT, TreeUSAGE_NUMERIC)
#undef expr
      flags |= NciM_WRITE_ONCE;
  flags |= NciM_NO_WRITE_MODEL;
  flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE_INTEGER(:MASTER, 1, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE_INTEGER(:EXT_1MHZ, 1, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE_INTEGER(:PTSC, 1, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE_INTEGER(:ACTIVE_CHANS, 1, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(:INPUT_1, TreeUSAGE_SIGNAL)
      flags |= NciM_WRITE_ONCE;
  flags |= NciM_COMPRESS_ON_PUT;
  flags |= NciM_NO_WRITE_MODEL;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(INPUT_1:STARTIDX, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(INPUT_1:ENDIDX, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(:INPUT_2, TreeUSAGE_SIGNAL)
      flags |= NciM_WRITE_ONCE;
  flags |= NciM_COMPRESS_ON_PUT;
  flags |= NciM_NO_WRITE_MODEL;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(INPUT_2:STARTIDX, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(INPUT_2:ENDIDX, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(:INPUT_3, TreeUSAGE_SIGNAL)
      flags |= NciM_WRITE_ONCE;
  flags |= NciM_COMPRESS_ON_PUT;
  flags |= NciM_NO_WRITE_MODEL;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(INPUT_3:STARTIDX, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(INPUT_3:ENDIDX, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(:INPUT_4, TreeUSAGE_SIGNAL)
      flags |= NciM_WRITE_ONCE;
  flags |= NciM_COMPRESS_ON_PUT;
  flags |= NciM_NO_WRITE_MODEL;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(INPUT_4:STARTIDX, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(INPUT_4:ENDIDX, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(:INPUT_5, TreeUSAGE_SIGNAL)
      flags |= NciM_WRITE_ONCE;
  flags |= NciM_COMPRESS_ON_PUT;
  flags |= NciM_NO_WRITE_MODEL;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(INPUT_5:STARTIDX, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(INPUT_5:ENDIDX, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(:INPUT_6, TreeUSAGE_SIGNAL)
      flags |= NciM_WRITE_ONCE;
  flags |= NciM_COMPRESS_ON_PUT;
  flags |= NciM_NO_WRITE_MODEL;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(INPUT_6:STARTIDX, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(INPUT_6:ENDIDX, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE_ACTION(:INIT_ACTION, INIT, INIT, 50, 0, 0, CAMAC_SERVER, 0)
 ADD_NODE_ACTION(:STORE_ACTION, STORE, STORE, 50, 0, 0, CAMAC_SERVER, 0)
      status = TreeEndConglomerate();
  if (!(status & 1))
    return status;
  return (TreeSetDefaultNid(old_nid));
}

int incaa6__part_name(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr,
		      struct descriptor *out_d)
{
  int element = 0, status;
  NCI_ITM nci_list[] = { {4, NciCONGLOMERATE_ELT, 0, 0}, {0, 0, 0, 0} };
  nci_list[0].pointer = (unsigned char *)&element;
  status = TreeGetNci(*(int *)nid_d_ptr->pointer, nci_list);
  if (!(status & 1))
    return status;
  switch (element) {
  case (INCAA6_N_HEAD + 1):
    StrFree1Dx(out_d);
    break;
  case (INCAA6_N_NAME + 1):
 COPY_PART_NAME(:NAME) break;
  case (INCAA6_N_COMMENT + 1):
 COPY_PART_NAME(:COMMENT) break;
  case (INCAA6_N_INT_CLK_FRQ + 1):
 COPY_PART_NAME(:INT_CLK_FRQ) break;
  case (INCAA6_N_CLOCK_IN + 1):
 COPY_PART_NAME(:CLOCK_IN) break;
  case (INCAA6_N_CLOCK_OUT + 1):
 COPY_PART_NAME(:CLOCK_OUT) break;
  case (INCAA6_N_TRIG_IN + 1):
 COPY_PART_NAME(:TRIG_IN) break;
  case (INCAA6_N_TRIG_OUT + 1):
 COPY_PART_NAME(:TRIG_OUT) break;
  case (INCAA6_N_MASTER + 1):
 COPY_PART_NAME(:MASTER) break;
  case (INCAA6_N_EXT_1MHZ + 1):
 COPY_PART_NAME(:EXT_1MHZ) break;
  case (INCAA6_N_PTSC + 1):
 COPY_PART_NAME(:PTSC) break;
  case (INCAA6_N_ACTIVE_CHANS + 1):
 COPY_PART_NAME(:ACTIVE_CHANS) break;
  case (INCAA6_N_INPUT_1 + 1):
 COPY_PART_NAME(:INPUT_1) break;
  case (INCAA6_N_INPUT_1_STARTIDX + 1):
 COPY_PART_NAME(INPUT_1:STARTIDX) break;
  case (INCAA6_N_INPUT_1_ENDIDX + 1):
 COPY_PART_NAME(INPUT_1:ENDIDX) break;
  case (INCAA6_N_INPUT_2 + 1):
 COPY_PART_NAME(:INPUT_2) break;
  case (INCAA6_N_INPUT_2_STARTIDX + 1):
 COPY_PART_NAME(INPUT_2:STARTIDX) break;
  case (INCAA6_N_INPUT_2_ENDIDX + 1):
 COPY_PART_NAME(INPUT_2:ENDIDX) break;
  case (INCAA6_N_INPUT_3 + 1):
 COPY_PART_NAME(:INPUT_3) break;
  case (INCAA6_N_INPUT_3_STARTIDX + 1):
 COPY_PART_NAME(INPUT_3:STARTIDX) break;
  case (INCAA6_N_INPUT_3_ENDIDX + 1):
 COPY_PART_NAME(INPUT_3:ENDIDX) break;
  case (INCAA6_N_INPUT_4 + 1):
 COPY_PART_NAME(:INPUT_4) break;
  case (INCAA6_N_INPUT_4_STARTIDX + 1):
 COPY_PART_NAME(INPUT_4:STARTIDX) break;
  case (INCAA6_N_INPUT_4_ENDIDX + 1):
 COPY_PART_NAME(INPUT_4:ENDIDX) break;
  case (INCAA6_N_INPUT_5 + 1):
 COPY_PART_NAME(:INPUT_5) break;
  case (INCAA6_N_INPUT_5_STARTIDX + 1):
 COPY_PART_NAME(INPUT_5:STARTIDX) break;
  case (INCAA6_N_INPUT_5_ENDIDX + 1):
 COPY_PART_NAME(INPUT_5:ENDIDX) break;
  case (INCAA6_N_INPUT_6 + 1):
 COPY_PART_NAME(:INPUT_6) break;
  case (INCAA6_N_INPUT_6_STARTIDX + 1):
 COPY_PART_NAME(INPUT_6:STARTIDX) break;
  case (INCAA6_N_INPUT_6_ENDIDX + 1):
 COPY_PART_NAME(INPUT_6:ENDIDX) break;
  case (INCAA6_N_INIT_ACTION + 1):
 COPY_PART_NAME(:INIT_ACTION) break;
  case (INCAA6_N_STORE_ACTION + 1):
 COPY_PART_NAME(:STORE_ACTION) break;
  default:
    status = TreeILLEGAL_ITEM;
  }
  return status;
}

extern int incaa6___init();
#define free_xd_array { int i; for(i=0; i<2;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}
#define error(nid,code,code1) {free_xd_array return GenDeviceSignal(nid,code,code1);}

int incaa6__init(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
  declare_variables(InInitStruct)
      static struct {
    short code;
    int value;
  } active_chans_t[] = {
  0, 0, 1, 1, 2, 2, 0, 0};
  static struct {
    short code;
    int value;
  } master_t[] = {
  0, 0, 1, 1, 0, 0};
  static struct {
    short code;
    int value;
  } ext_1mhz_t[] = {
  0, 0, 1, 1, 0, 0};
  struct descriptor_xd work_xd[2];
  int xd_count = 0;
  memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 2);
  initialize_variables(InInitStruct)

      read_string_error(INCAA6_N_NAME, name, DEV$_BAD_NAME);
  read_descriptor(INCAA6_N_INT_CLK_FRQ, int_clk_frq);
  read_integer_error(INCAA6_N_ACTIVE_CHANS, active_chans, INCAA$_BAD_ACTIVE_CHANS);
  check_integer_set(active_chans, active_chans_t, 3, INCAA$_BAD_ACTIVE_CHANS);
  read_integer_error(INCAA6_N_MASTER, master, INCAA$_BAD_MASTER);
  check_integer_set(master, master_t, 2, INCAA$_BAD_MASTER);
  read_integer_error(INCAA6_N_EXT_1MHZ, ext_1mhz, INCAA$_BAD_EXT_1MHZ);
  check_integer_set(ext_1mhz, ext_1mhz_t, 2, INCAA$_BAD_EXT_1MHZ);
  read_integer_error(INCAA6_N_PTSC, ptsc, INCAA$_BAD_PTSC);
  status = incaa6___init(nid_d_ptr, &in_struct);
  free_xd_array return status;
}

#undef free_xd_array

extern int incaa6___arm();
#define free_xd_array { int i; for(i=0; i<2;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}

int incaa6__arm(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
  declare_variables(InArmStruct)
      static struct {
    short code;
    int value;
  } active_chans_t[] = {
  0, 0, 1, 1, 2, 2, 0, 0};
  static struct {
    short code;
    int value;
  } master_t[] = {
  0, 0, 1, 1, 0, 0};
  static struct {
    short code;
    int value;
  } ext_1mhz_t[] = {
  0, 0, 1, 1, 0, 0};
  struct descriptor_xd work_xd[2];
  int xd_count = 0;
  memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 2);
  initialize_variables(InArmStruct)

      read_string_error(INCAA6_N_NAME, name, DEV$_BAD_NAME);
  read_descriptor(INCAA6_N_INT_CLK_FRQ, int_clk_frq);
  read_integer_error(INCAA6_N_ACTIVE_CHANS, active_chans, INCAA$_BAD_ACTIVE_CHANS);
  check_integer_set(active_chans, active_chans_t, 3, INCAA$_BAD_ACTIVE_CHANS);
  read_integer_error(INCAA6_N_MASTER, master, INCAA$_BAD_MASTER);
  check_integer_set(master, master_t, 2, INCAA$_BAD_MASTER);
  read_integer_error(INCAA6_N_EXT_1MHZ, ext_1mhz, INCAA$_BAD_EXT_1MHZ);
  check_integer_set(ext_1mhz, ext_1mhz_t, 2, INCAA$_BAD_EXT_1MHZ);
  read_integer_error(INCAA6_N_PTSC, ptsc, INCAA$_BAD_PTSC);
  status = incaa6___arm(nid_d_ptr, &in_struct);
  free_xd_array return status;
}

#undef free_xd_array

extern int incaa6___trigger();
#define free_xd_array { int i; for(i=0; i<1;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}

int incaa6__trigger(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
  declare_variables(InTriggerStruct)
  struct descriptor_xd work_xd[1];
  int xd_count = 0;
  memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 1);
  initialize_variables(InTriggerStruct)

      read_string_error(INCAA6_N_NAME, name, DEV$_BAD_NAME);
  status = incaa6___trigger(nid_d_ptr, &in_struct);
  free_xd_array return status;
}

#undef free_xd_array

extern int incaa6___store();
#define free_xd_array { int i; for(i=0; i<1;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}

int incaa6__store(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
  declare_variables(InStoreStruct)
  struct descriptor_xd work_xd[1];
  int xd_count = 0;
  memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 1);
  initialize_variables(InStoreStruct)

      read_string_error(INCAA6_N_NAME, name, DEV$_BAD_NAME);
  read_integer_error(INCAA6_N_PTSC, ptsc, INCAA$_BAD_PTSC);
  status = incaa6___store(nid_d_ptr, &in_struct);
  free_xd_array return status;
}

#undef free_xd_array
