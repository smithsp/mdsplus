#include <mitdevices_msg.h>
#include <mds_gendevice.h>
#include "joerger_tr612_gen.h"
int joerger_tr612__add(struct descriptor *name_d_ptr, struct descriptor *dummy_d_ptr, int *nid_ptr)
{
  static DESCRIPTOR(library_d, "MIT$DEVICES");
  static DESCRIPTOR(model_d, "JOERGER_TR612");
  static DESCRIPTOR_CONGLOM(conglom_d, &library_d, &model_d, 0, 0);
  int usage = TreeUSAGE_DEVICE;
  int curr_nid, old_nid, head_nid, status;
  long int flags = NciM_WRITE_ONCE;
  NCI_ITM flag_itm[] = { {2, NciSET_FLAGS, 0, 0}, {0, 0, 0, 0} };
  char *name_ptr = strncpy(malloc(name_d_ptr->length + 1), name_d_ptr->pointer, name_d_ptr->length);
  flag_itm[0].pointer = (unsigned char *)&flags;
  name_ptr[name_d_ptr->length] = 0;
  status = TreeStartConglomerate(JOERGER_TR612_K_CONG_NODES);
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
 ADD_NODE_INTEGER(:ACTMEM, 7, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE_INTEGER(:PRETRIG, 0, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE_INTEGER(:MODE, 0, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE_INTEGER(:FREQUENCY, 2, TreeUSAGE_NUMERIC)
      flags |= NciM_NO_WRITE_SHOT;
  status = TreeSetNci(curr_nid, flag_itm);
 ADD_NODE(:EXT_CLOCK, TreeUSAGE_AXIS)
#define expr " 0.						"
 ADD_NODE_EXPR(:TRIGGER, TreeUSAGE_NUMERIC)
#undef expr
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

int joerger_tr612__part_name(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr,
			     struct descriptor *out_d)
{
  int element = 0, status;
  NCI_ITM nci_list[] = { {4, NciCONGLOMERATE_ELT, 0, 0}, {0, 0, 0, 0} };
  nci_list[0].pointer = (unsigned char *)&element;
  status = TreeGetNci(*(int *)nid_d_ptr->pointer, nci_list);
  if (!(status & 1))
    return status;
  switch (element) {
  case (JOERGER_TR612_N_HEAD + 1):
    StrFree1Dx(out_d);
    break;
  case (JOERGER_TR612_N_NAME + 1):
 COPY_PART_NAME(:NAME) break;
  case (JOERGER_TR612_N_COMMENT + 1):
 COPY_PART_NAME(:COMMENT) break;
  case (JOERGER_TR612_N_ACTMEM + 1):
 COPY_PART_NAME(:ACTMEM) break;
  case (JOERGER_TR612_N_PRETRIG + 1):
 COPY_PART_NAME(:PRETRIG) break;
  case (JOERGER_TR612_N_MODE + 1):
 COPY_PART_NAME(:MODE) break;
  case (JOERGER_TR612_N_FREQUENCY + 1):
 COPY_PART_NAME(:FREQUENCY) break;
  case (JOERGER_TR612_N_EXT_CLOCK + 1):
 COPY_PART_NAME(:EXT_CLOCK) break;
  case (JOERGER_TR612_N_TRIGGER + 1):
 COPY_PART_NAME(:TRIGGER) break;
  case (JOERGER_TR612_N_INPUT_1 + 1):
 COPY_PART_NAME(:INPUT_1) break;
  case (JOERGER_TR612_N_INPUT_1_STARTIDX + 1):
 COPY_PART_NAME(INPUT_1:STARTIDX) break;
  case (JOERGER_TR612_N_INPUT_1_ENDIDX + 1):
 COPY_PART_NAME(INPUT_1:ENDIDX) break;
  case (JOERGER_TR612_N_INPUT_2 + 1):
 COPY_PART_NAME(:INPUT_2) break;
  case (JOERGER_TR612_N_INPUT_2_STARTIDX + 1):
 COPY_PART_NAME(INPUT_2:STARTIDX) break;
  case (JOERGER_TR612_N_INPUT_2_ENDIDX + 1):
 COPY_PART_NAME(INPUT_2:ENDIDX) break;
  case (JOERGER_TR612_N_INPUT_3 + 1):
 COPY_PART_NAME(:INPUT_3) break;
  case (JOERGER_TR612_N_INPUT_3_STARTIDX + 1):
 COPY_PART_NAME(INPUT_3:STARTIDX) break;
  case (JOERGER_TR612_N_INPUT_3_ENDIDX + 1):
 COPY_PART_NAME(INPUT_3:ENDIDX) break;
  case (JOERGER_TR612_N_INPUT_4 + 1):
 COPY_PART_NAME(:INPUT_4) break;
  case (JOERGER_TR612_N_INPUT_4_STARTIDX + 1):
 COPY_PART_NAME(INPUT_4:STARTIDX) break;
  case (JOERGER_TR612_N_INPUT_4_ENDIDX + 1):
 COPY_PART_NAME(INPUT_4:ENDIDX) break;
  case (JOERGER_TR612_N_INPUT_5 + 1):
 COPY_PART_NAME(:INPUT_5) break;
  case (JOERGER_TR612_N_INPUT_5_STARTIDX + 1):
 COPY_PART_NAME(INPUT_5:STARTIDX) break;
  case (JOERGER_TR612_N_INPUT_5_ENDIDX + 1):
 COPY_PART_NAME(INPUT_5:ENDIDX) break;
  case (JOERGER_TR612_N_INPUT_6 + 1):
 COPY_PART_NAME(:INPUT_6) break;
  case (JOERGER_TR612_N_INPUT_6_STARTIDX + 1):
 COPY_PART_NAME(INPUT_6:STARTIDX) break;
  case (JOERGER_TR612_N_INPUT_6_ENDIDX + 1):
 COPY_PART_NAME(INPUT_6:ENDIDX) break;
  case (JOERGER_TR612_N_INIT_ACTION + 1):
 COPY_PART_NAME(:INIT_ACTION) break;
  case (JOERGER_TR612_N_STORE_ACTION + 1):
 COPY_PART_NAME(:STORE_ACTION) break;
  default:
    status = TreeILLEGAL_ITEM;
  }
  return status;
}

extern int joerger_tr612___init();
#define free_xd_array { int i; for(i=0; i<1;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}
#define error(nid,code,code1) {free_xd_array return GenDeviceSignal(nid,code,code1);}

int joerger_tr612__init(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
  declare_variables(InInitStruct)
  struct descriptor_xd work_xd[1];
  int xd_count = 0;
  memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 1);
  initialize_variables(InInitStruct)

      read_string_error(JOERGER_TR612_N_NAME, name, DEV$_BAD_NAME);
  read_integer_error(JOERGER_TR612_N_ACTMEM, actmem, J_TR612$_BAD_ACTMEM);
  check_range(actmem, 0, 7, J_TR612$_BAD_ACTMEM);
  read_integer_error(JOERGER_TR612_N_PRETRIG, pretrig, J_TR612$_BAD_PRETRIG);
  check_range(pretrig, 0, 7, J_TR612$_BAD_PRETRIG);
  read_integer_error(JOERGER_TR612_N_MODE, mode, J_TR612$_BAD_MODE);
  check_range(mode, 0, 1, J_TR612$_BAD_MODE);
  read_integer_error(JOERGER_TR612_N_FREQUENCY, frequency, J_TR612$_BAD_FREQUENCY);
  check_range(frequency, 0, 4, J_TR612$_BAD_FREQUENCY);
  status = joerger_tr612___init(nid_d_ptr, &in_struct);
  free_xd_array return status;
}

#undef free_xd_array

extern int joerger_tr612___trigger();
#define free_xd_array { int i; for(i=0; i<1;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}

int joerger_tr612__trigger(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
  declare_variables(InTriggerStruct)
  struct descriptor_xd work_xd[1];
  int xd_count = 0;
  memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 1);
  initialize_variables(InTriggerStruct)

      read_string_error(JOERGER_TR612_N_NAME, name, DEV$_BAD_NAME);
  status = joerger_tr612___trigger(nid_d_ptr, &in_struct);
  free_xd_array return status;
}

#undef free_xd_array

extern int joerger_tr612___stop();
#define free_xd_array { int i; for(i=0; i<1;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}

int joerger_tr612__stop(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
  declare_variables(InStopStruct)
  struct descriptor_xd work_xd[1];
  int xd_count = 0;
  memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 1);
  initialize_variables(InStopStruct)

      read_string_error(JOERGER_TR612_N_NAME, name, DEV$_BAD_NAME);
  status = joerger_tr612___stop(nid_d_ptr, &in_struct);
  free_xd_array return status;
}

#undef free_xd_array

extern int joerger_tr612___store();
#define free_xd_array { int i; for(i=0; i<1;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}

int joerger_tr612__store(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
  declare_variables(InStoreStruct)
  struct descriptor_xd work_xd[1];
  int xd_count = 0;
  memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 1);
  initialize_variables(InStoreStruct)

      read_string_error(JOERGER_TR612_N_NAME, name, DEV$_BAD_NAME);
  status = joerger_tr612___store(nid_d_ptr, &in_struct);
  free_xd_array return status;
}

#undef free_xd_array
