#include <mitdevices_msg.h>
#include <mds_gendevice.h>
#include "joerger_cg_gen.h"
int joerger_cg__add(struct descriptor *name_d_ptr, struct descriptor *dummy_d_ptr, int *nid_ptr)
{
  static DESCRIPTOR(library_d, "MIT$DEVICES");
  static DESCRIPTOR(model_d, "JOERGER_CG");
  static DESCRIPTOR_CONGLOM(conglom_d, &library_d, &model_d, 0, 0);
  int usage = TreeUSAGE_DEVICE;
  int curr_nid, old_nid, head_nid, status;
  long int flags = NciM_WRITE_ONCE;
  NCI_ITM flag_itm[] = { {2, NciSET_FLAGS, 0, 0}, {0, 0, 0, 0} };
  char *name_ptr = strncpy(malloc(name_d_ptr->length + 1), name_d_ptr->pointer, name_d_ptr->length);
  flag_itm[0].pointer = (unsigned char *)&flags;
  name_ptr[name_d_ptr->length] = 0;
  status = TreeStartConglomerate(JOERGER_CG_K_CONG_NODES);
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
#define expr " *:*:1. "
 ADD_NODE_EXPR(:CHANNEL_1, TreeUSAGE_AXIS)
#undef expr
 ADD_NODE(:CHANNEL_1.INVERTED, 0)
#define expr " *:*:1. "
 ADD_NODE_EXPR(:CHANNEL_2, TreeUSAGE_AXIS)
#undef expr
 ADD_NODE(:CHANNEL_2.INVERTED, 0)
#define expr " *:*:1. "
 ADD_NODE_EXPR(:CHANNEL_3, TreeUSAGE_AXIS)
#undef expr
 ADD_NODE(:CHANNEL_3.INVERTED, 0)
#define expr " *:*:1. "
 ADD_NODE_EXPR(:CHANNEL_4, TreeUSAGE_AXIS)
#undef expr
 ADD_NODE(:CHANNEL_4.INVERTED, 0)
 ADD_NODE_ACTION(:INIT_ACTION, INIT, INIT, 10, 0, 0, CAMAC_SERVER, 0)
      status = TreeEndConglomerate();
  if (!(status & 1))
    return status;
  return (TreeSetDefaultNid(old_nid));
}

int joerger_cg__part_name(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr,
			  struct descriptor *out_d)
{
  int element = 0, status;
  NCI_ITM nci_list[] = { {4, NciCONGLOMERATE_ELT, 0, 0}, {0, 0, 0, 0} };
  nci_list[0].pointer = (unsigned char *)&element;
  status = TreeGetNci(*(int *)nid_d_ptr->pointer, nci_list);
  if (!(status & 1))
    return status;
  switch (element) {
  case (JOERGER_CG_N_HEAD + 1):
    StrFree1Dx(out_d);
    break;
  case (JOERGER_CG_N_NAME + 1):
 COPY_PART_NAME(:NAME) break;
  case (JOERGER_CG_N_COMMENT + 1):
 COPY_PART_NAME(:COMMENT) break;
  case (JOERGER_CG_N_CHANNEL_1 + 1):
 COPY_PART_NAME(:CHANNEL_1) break;
  case (JOERGER_CG_N_CHANNEL_1_INVERTED + 1):
 COPY_PART_NAME(:CHANNEL_1.INVERTED) break;
  case (JOERGER_CG_N_CHANNEL_2 + 1):
 COPY_PART_NAME(:CHANNEL_2) break;
  case (JOERGER_CG_N_CHANNEL_2_INVERTED + 1):
 COPY_PART_NAME(:CHANNEL_2.INVERTED) break;
  case (JOERGER_CG_N_CHANNEL_3 + 1):
 COPY_PART_NAME(:CHANNEL_3) break;
  case (JOERGER_CG_N_CHANNEL_3_INVERTED + 1):
 COPY_PART_NAME(:CHANNEL_3.INVERTED) break;
  case (JOERGER_CG_N_CHANNEL_4 + 1):
 COPY_PART_NAME(:CHANNEL_4) break;
  case (JOERGER_CG_N_CHANNEL_4_INVERTED + 1):
 COPY_PART_NAME(:CHANNEL_4.INVERTED) break;
  case (JOERGER_CG_N_INIT_ACTION + 1):
 COPY_PART_NAME(:INIT_ACTION) break;
  default:
    status = TreeILLEGAL_ITEM;
  }
  return status;
}

extern int joerger_cg___init();
#define free_xd_array { int i; for(i=0; i<5;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}
#define error(nid,code,code1) {free_xd_array return GenDeviceSignal(nid,code,code1);}

int joerger_cg__init(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
  declare_variables(InInitStruct)
  struct descriptor_xd work_xd[5];
  int xd_count = 0;
  memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 5);
  initialize_variables(InInitStruct)

      read_string_error(JOERGER_CG_N_NAME, name, DEV$_BAD_NAME);
  read_descriptor(JOERGER_CG_N_CHANNEL_1, channel_1);
  read_descriptor(JOERGER_CG_N_CHANNEL_2, channel_2);
  read_descriptor(JOERGER_CG_N_CHANNEL_3, channel_3);
  read_descriptor(JOERGER_CG_N_CHANNEL_4, channel_4);
  status = joerger_cg___init(nid_d_ptr, &in_struct);
  free_xd_array return status;
}

#undef free_xd_array

extern int joerger_cg___stop();
#define free_xd_array { int i; for(i=0; i<1;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}

int joerger_cg__stop(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
  declare_variables(InStopStruct)
  struct descriptor_xd work_xd[1];
  int xd_count = 0;
  memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 1);
  initialize_variables(InStopStruct)

      read_string_error(JOERGER_CG_N_NAME, name, DEV$_BAD_NAME);
  status = joerger_cg___stop(nid_d_ptr, &in_struct);
  free_xd_array return status;
}

#undef free_xd_array
