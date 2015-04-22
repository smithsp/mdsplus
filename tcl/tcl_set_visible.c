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

static int setVisible(int nid, int parents, int descendants, char **error) {
  int visible = 1;
  NCI_ITM setnci[] = { {sizeof(visible), NciVISIBLE, (unsigned char *)&visible, 0}, {0, NciEND_OF_LIST, 0, 0} };
  int status = TreeSetVisibility(nid, visible, parents, descendants);
  if (!(status & 1)) {
    char *msg = MdsGetMsg(status);
    char *nodename = TreeGetPath(nid);
    *error = malloc(strlen(nodename) + strlen(msg) + 100);
    sprintf(*error, "Error: Problem setting visibility for node %s\n"
	    "Error message was: %s\n", nodename, msg);
    if (nodename)
      free(nodename);
  }
  return status;
}
    

	/****************************************************************
	 * TclSetVisible:
    <parameter name="p1" prompt="What" required="True" type="SET_TYPE"/>
    <parameter name="p2" label="NODENAME" prompt="Node" required="True"/>
    <qualifier name="RESET"/>
    <qualifier name="USAGE" required="True" list="True"/>
    <qualifier name="DESCENDANTS"/>
    <qualifier name="PARENTS"/>
    <qualifier name="LEVEL" required="True"/>
	 ****************************************************************/
int TclSetVisible(void *ctx, char **error, char **output)
{
  int nid;
  int status = 1;
  int reset  = cli_present(ctx, "RESET") & 1;
  int descendants = cli_present(ctx, "DESCENDANTS") & 1;
  int parents = cli_present(ctx, "PARENTS") != MdsdclNEGATED;
  void *ctx1 = 0;
  char *nodename = 0;
  char *levelStr = 0;
  int usageMask = -1;
  unsigned char level;
  cli_get_value(ctx, "LEVEL", &levelStr);
  if (levelStr) {
    char *endptr;
    long val = strtol(levelStr, &endptr, 0);
    free(levelStr);
    if (endptr[0] != '\0' || (val < 0) || (val > 7)) {
      sprintf(*error,"Specify a level between 0 and 7\n");
      return TclINVLEVEL;
    }
    level = (unsigned char)val;
  }
  if (cli_present(ctx, "USAGE") & 1) {
    char *usageStr = 0;
    usageMask = 0;
    while (cli_get_value(ctx, "USAGE", &usageStr) & 1) {
      if (usageStr) {
	char usage = tclUsageToNumber(usageStr, error);
	if (usage == -1) {
	  free(usageStr);
	  return 0;
	} else {
	  usageMask = usageMask | (1 << usage);
	  free(usageStr);
	}
      }
    }
  }
  if (reset) {
    void *ctx1 = 0;
    NCI_ITM getnci[] = { {0, NciFULLPATH, 0, 0}, {0, NciEND_OF_LIST, 0, 0} };
    TreeGetNci(0,getnci);
    nodename=strcpy(malloc(strlen((char *)getnci[0].pointer)+10),getnci[0].pointer);
    free(getnci[0].pointer);
    strcat(nodename,"***");
    while ((status = TreeFindNodeWild(nodename, &nid, &ctx1, -1)) & 1) {
      static int zero=0;
      NCI_ITM setnci[] = { {sizeof(int), NciVISIBLE, &zero, 0}, {0, NciEND_OF_LIST, 0, 0} };
      status = TreeSetNci(nid, setnci);
      if (!(status & 1)) {
	char *msg = MdsGetMsg(status);
	*error = malloc(strlen(nodename) + strlen(msg) + 100);
	sprintf(*error, "Error: Problem setting visibility for node %s\n"
		"Error message was: %s\n", nodename, msg);
	goto error;
      }
    }
    free(nodename);
    TreeFindNodeEnd(&ctx1);
  }
  cli_get_value(ctx, "NODENAME", &nodename);
  while ((status = TreeFindNodeWild(nodename, &nid, &ctx1, usageMask)) & 1) {
    int visible = 1;
    if (levelStr) {
      unsigned char nodelevel;
      NCI_ITM getnci[] = { {sizeof(level), NciDETAIL_LEVEL, (unsigned char *)&nodelevel, 0}, {0, NciEND_OF_LIST, 0, 0} };
      status = TreeGetNci(nid, getnci);
      if (!(status & 1)) {
	char *msg = MdsGetMsg(status);
	free(nodename);
	nodename = TreeGetPath(nid);
	*error = malloc(strlen(nodename) + strlen(msg) + 100);
	sprintf(*error, "Error: Problem getting detail level value for node %s\n"
		"Error message was: %s\n", nodename, msg);
	goto error;
      }
      visible = nodelevel <= level;
    }
    if (visible) {
      status = setVisible(nid, parents, descendants, error);
      if (!(status & 1))
	goto error;
    }
  }
  TreeGetDefaultNid(&nid);
  TclNodeTouched(nid, tree);

 error:
  TreeFindNodeEnd(&ctx1);
  if (status == TreeNMN)
    status = 1;
  if (nodename)
    free(nodename);
  return status;
}
