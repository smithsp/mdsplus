#include <config.h>
#include <treeshr.h>
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

static void setByLevel(PINO_DATABASE *db, NODE *n, int level, int parents, int descendants) {
  NODE *node = n;
  if (node->detail_level <= level) {
    node->invisible = 0;
    if (parents)
      setParents(db, node, 0);
    if (descendants)
      setDescendants(db, node, 0);
  }
  for (node = member_of(n); node; node = brother_of(node)) {
    if (node->detail_level <= level) {
      node->invisible = 0;
      if (parents)
	setParents(db, node, 0);
      if (descendants)
	setDescendants(db, node, 0);
    }
    setByLevel(db, node, level, parents, descendants);
  }
  for (node = child_of(n); node; node = brother_of(node)) {
    if (node->detail_level <= level) {
      node->invisible = 0;
      if (parents)
	setParents(db, node, 0);
      if (descendants)
	setDescendants(db, node, 0);
    }
    setByLevel(db, node, level, parents, descendants);
  }
}

int _TreeSetVisibleByLevel(void *dbid, int level, int parents, int descendants) {
  PINO_DATABASE *db = (PINO_DATABASE *)dbid;
  NID nid = {0,0};
  NODE *node;
  nid_to_node(db, ((NID *)&nid), node);
  setByLevel((PINO_DATABASE *)dbid, node, level, parents, descendants);
  return TreeNORMAL;
}

int TreeSetVisibleByLevel(int level, int parents, int descendants)
{
  return _TreeSetVisibleByLevel(*TreeCtx(), level, parents, descendants);
}

static void setByUsage(PINO_DATABASE *db, NODE *n, int usageMask, int parents, int descendants) {
  NODE *node = n;
  if ((1 << node->usage) & usageMask) {
    node->invisible = 0;
    if (parents)
      setParents(db, node, 0);
    if (descendants)
      setDescendants(db, node, 0);
  }
  for (node = member_of(n); node; node = brother_of(node)) {
    if ((1 << node->usage) & usageMask) {
      node->invisible = 0;
      if (parents)
	setParents(db, node, 0);
      if (descendants)
	setDescendants(db, node, 0);
    }
    setByUsage(db, node, usageMask, parents, descendants);
  }
  for (node = child_of(n); node; node = brother_of(node)) {
    if ((1 << node->usage) & usageMask) {
      node->invisible = 0;
      if (parents)
	setParents(db, node, 0);
      if (descendants)
	setDescendants(db, node, 0);
    }
    setByUsage(db, node, usageMask, parents, descendants);
  }
}

int _TreeSetVisibleByUsage(void *dbid, int usageMask, int parents, int descendants) {
  PINO_DATABASE *db = (PINO_DATABASE *)dbid;
  NID nid = {0,0};
  NODE *node;
  nid_to_node(db, ((NID *)&nid), node);
  setByUsage((PINO_DATABASE *)dbid, node, usageMask, parents, descendants);
  return TreeNORMAL;
}

int TreeSetVisibleByUsage(int usageMask, int parents, int descendants)
{
  return _TreeSetVisibleByUsage(*TreeCtx(), usageMask, parents, descendants);
}



