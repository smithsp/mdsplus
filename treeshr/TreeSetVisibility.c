#include <config.h>
#include "treeshrp.h"
#include <treeshr_messages.h>

extern void **TreeCtx();

static void setParents(PINO_DATABASE *db, NODE *n, int invisible) {
  NODE *node;
  for (node = parent_of(n); node; node = parent_of(node))
    node->invisible = invisible;
}

static void setDescendants(PINO_DATABASE *db, NODE *n, int invisible) {
  NODE *node;
  for (node = member_of(n); node; node = brother_of(node)) {
    node->invisible = invisible;
    setDescendants(db, node, invisible);
  }
  for (node = child_of(n); node; node = brother_of(node)) {
    node->invisible = invisible;
    setDescendants(db, node, invisible);
  }
}

int _TreeSetVisibility(void *dbid, int nid_in, int visible, int parents, int descendants) {
  int status = TreeNORMAL;
  PINO_DATABASE *db= (PINO_DATABASE *)dbid;
  NODE *node;
  nid_to_node(db, ((NID *)&nid_in), node);
  if (node) {
    node->invisible = visible == 0;
    if (parents)
      setParents(db, node, visible == 0);
    if (descendants)
      setDescendants(db, node, visible == 0);
  }
  else
    status = TreeNNF;
  return status;
}

int TreeSetVisibility(int nid_in, int visible, int parents, int descendants)
{
  return _TreeSetVisibility(*TreeCtx(), nid_in, visible, parents, descendants);
}
