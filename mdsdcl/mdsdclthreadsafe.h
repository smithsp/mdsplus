#include <config.h>
#ifndef __MINGW64__
#include <mdstypes.h>
#endif
#include <mdsdescrip.h>
#include "mdsdcl.h"

#if HAVE_PTHREAD_H
#include <pthread.h>
#endif

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#ifndef HAVE_PTHREAD_H
#define pthread_mutex_t HANDLE
#define pthread_once_t int
#define PTHREAD_ONCE_INIT 0
#endif
#endif

typedef struct _thread_static {
  struct _mdsdcl_ctrl  ctrl;
} MdsdclThreadStatic;

extern MdsdclThreadStatic *MdsdclGetThreadStatic();
