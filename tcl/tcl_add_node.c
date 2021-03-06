#include <ncidef.h>
#include <usagedef.h>
#include <string.h>
#include <dcl.h>
#include <stdlib.h>
#include <stdio.h>
#include <mdsshr.h>
#include <treeshr.h>
#include "tcl_p.h"

/**********************************************************************
* TCL_ADD_NODE.C --
*
* TclAddNode:  Add a node
*
* History:
*  02-Feb-1998  TRG  Create.  Ported from original mds code.
*
************************************************************************/

#define DSC(A)        (struct descriptor *)(A)

	/*****************************************************************
	 * TclAddNode:
	 * Add a node
	 *****************************************************************/

char tclUsageToNumber(const char *usage, char **error)
{
  struct usageMap {
    char *name;
    char value;
  } map[] = { {
  "ANY", TreeUSAGE_ANY}, {
  "STRUCTURE", TreeUSAGE_STRUCTURE}, {
  "ACTION", TreeUSAGE_ACTION}, {
  "DEVICE", TreeUSAGE_DEVICE}, {
  "DISPATCH", TreeUSAGE_DISPATCH}, {
  "NUMERIC", TreeUSAGE_NUMERIC}, {
  "SIGNAL", TreeUSAGE_SIGNAL}, {
  "TASK", TreeUSAGE_TASK}, {
  "TEXT", TreeUSAGE_TEXT}, {
  "WINDOW", TreeUSAGE_WINDOW}, {
  "AXIS", TreeUSAGE_AXIS}, {
  "SUBTREE", TreeUSAGE_SUBTREE}, {
  "COMPOUND_DATA", TreeUSAGE_COMPOUND_DATA}};
  int i, mapsize = sizeof(map) / sizeof(struct usageMap);
  for (i = 0; i < mapsize; i++)
    if (strncasecmp(usage, map[i].name, strlen(usage)) == 0)
      return map[i].value;
  *error = malloc(strlen(usage) + 500);
  sprintf(*error, "Error: Invalid usage specified '%s', use one of\n"
	  "ACTION, ANY, AXIS, COMPOUND_DATA, DEVICE, DISPATCH,"
	  "NUMERIC, SIGNAL,\nSTRUCTURE, SUBTREE, TASK, TEXT" " or WINDOW\n", usage);
  return -1;
}

int TclAddNode(void *ctx, char **error, char **output)
{				/* Return: status                 */
  int nid;
  int sts;
  char usage = 0;
  char *nodnam = 0;
  char *modelType = 0;
  char *qualifiers = 0;
  char *usageStr = 0;
  sts = cli_get_value(ctx, "NODENAME", &nodnam);

  if (cli_get_value(ctx, "MODEL", &modelType) & 1) {
    if (cli_get_value(ctx, "QUALIFIERS", &qualifiers) & 1)
      *error = strdup("--> QUALIFIERS option is obsolete (ignored)\n");
    sts = TreeAddConglom(nodnam, modelType, &nid);
  } else {
    cli_get_value(ctx, "USAGE", &usageStr);
    if (usageStr) {
      usage = tclUsageToNumber(usageStr, error);
      if (usage == -1) {
	sts = MdsdclIVVERB;
	goto done;
      }
    }
    sts = TreeAddNode(nodnam, &nid, usage);
    if (sts && (usage == TreeUSAGE_SUBTREE)) {
      sts = TreeSetSubtree(nid);
    }
  }
  if (sts & 1)
    TclNodeTouched(nid, new);
  else {
    char *msg = MdsGetMsg(sts);
    if (*error)
      free(*error);
    *error = malloc(strlen(nodnam) + strlen(msg) + 100);
    sprintf(*error, "Error adding node %s\nError was: %s\n", nodnam, MdsGetMsg(sts));
  }
 done:
  if (nodnam)
    free(nodnam);
  if (modelType)
    free(modelType);
  if (qualifiers)
    free(qualifiers);
  if (usageStr)
    free(usageStr);
  return sts;
}
