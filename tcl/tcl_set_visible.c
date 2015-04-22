#include	<config.h>
#include        "tclsysdef.h"
#include        <ncidef.h>
#include        <usagedef.h>
#include 	<stdlib.h>
#include        <dcl.h>
#ifdef HAVE_ALLOCA_H
#include        <alloca.h>
#endif
#include <dcl.h>
#include <string.h>
#include <strroutines.h>
#include <mdsdcl_messages.h>
#include <tcl_messages.h>
#include <treeshr.h>

    

	/****************************************************************
	 * TclSetVisible:
	 ****************************************************************/
int TclSetVisible(void *ctx, char **error, char **output)
{
  int nid;
  int status = 1;
  int reset  = cli_present(ctx, "RESET") & 1;
  int descendants = cli_present(ctx, "DESCENDANTS") & 1;
  int by_level = cli_present(ctx,"LEVEL") & 1;
  int by_usage = cli_present(ctx,"USAGE") & 1;
  int by_nodename = cli_present(ctx,"NODENAME") & 1;
  int parents = cli_present(ctx, "PARENTS") != MdsdclNEGATED;
  char *errmsg = 0;
  if (reset) {
    status = TreeSetVisibility(0, 0, 0, 1);
    if (!(status & 1)) {
      char *msg = MdsGetMsg(status);
      *error = malloc(strlen(msg) + 100);
      sprintf(*error, "Error: Problem resetting visibility\n"
	    "Error message was: %s\n", msg);
      return status;
    }
  }
  if (by_level) {
    char *levelStr = 0;
    char *endptr;
    long val;
    cli_get_value(ctx, "LEVEL", &levelStr);
    val = strtol(levelStr, &endptr, 0);
    free(levelStr);
    if (endptr[0] != '\0' || (val < 0) || (val > 7)) {
      errmsg = strdup("Specify a level between 0 and 7\n");
      status = TclINVLEVEL;
    }
    if (status & 1)
      status = TreeSetVisibleByLevel((unsigned char)val, parents, descendants);
  } else if (by_usage) {
    char *usageStr = 0;
    int usageMask = 0;
    while (cli_get_value(ctx, "USAGE", &usageStr) & 1) {
      if (usageStr) {
	char usage = tclUsageToNumber(usageStr, error);
	if (usage == -1) {
	  errmsg = strcpy(malloc(strlen(usageStr)+100),"Invalid usage specified: ");
	  strcat(errmsg,usageStr);
	  strcat(errmsg,"\n");
	  status =  TclINVUSAGE;
	  free(usageStr);
	  break;
	}
        else {
	  usageMask = usageMask | (1 << usage);
	  free(usageStr);
	}
      }
    }
    if (status & 1)
      status = TreeSetVisibleByUsage(usageMask, parents, descendants);
  } else if (by_nodename) {
    void *ctx1 = 0;
    char *nodename = 0;
    int nodes_found = 0;
    cli_get_value(ctx, "NODENAME", &nodename);
    while (TreeFindNodeWild(nodename, &nid, &ctx1, -1) & 1) {
      status = TreeSetVisibility(nid, 1, parents, descendants);
      if (!(status & 1))
	break;
      nodes_found = 1;
    }
    TreeFindNodeEnd(&ctx1);
    if (nodename)
      free(nodename);
    if (nodes_found == 0)
      status = TreeNNF;
  } else {
    status = MdsdclERROR;
  }
  if (status & 1)
    TclNodeTouched(0, tree);
  else {
    char *msg = MdsGetMsg(status);
    *error = malloc(strlen(msg)+100+(errmsg ? strlen(errmsg) : 0));
    if (errmsg)
      sprintf(*error,"Error setting nodes visible\n  %s\n  Error message was: %s\n",
	      errmsg,msg);
    else
      sprintf(*error,"Error setting nodes visible\n  Error message was: %s\n",
	      msg);
  }
  return status;
}
