#ifdef __VMS
#define MdsCOMPRESSIBLE MDS$_COMPRESSIBLE
#define MdsGet1Dx MDS$GET1_DX
#define MdsGet1DxS MDS$GET1_DX_S
#define MdsGet1DxA Mds$GET1_DX_A
#define MdsFree1Dx MDS$FREE1_DX
#define MdsXpand MDS$XPAND
#define MdsCopyDxXd MDS$COPY_DXXD
#define MdsPk MDS$PK
#define MdsUnpk MDS$UNPK
#define MdsCmprs MDS$CMPRS

#define MdsGetCurrentShotid MDS$GET_CURRENT_SHOTID
#pragma EXPORT extern_model save
#pragma EXPORT extern_model globalvalue
EXPORT extern MdsCOMPRESSIBLE;
#pragma EXPORT extern_model restore
#else
#define MdsCOMPRESSIBLE 3
#endif
#ifndef HAVE_VXWORKS_H
#include <config.h>
#else
#define EXPORT
#endif
#include <mdsdescrip.h>
#include <mdstypes.h>
EXPORT extern int MdsCmprs(int *nitems, struct descriptor_a *items, struct descriptor_a *pack, int *bit);
EXPORT extern int MdsCompress(struct descriptor *cimage, struct descriptor *centry, struct descriptor *in, struct descriptor_xd *out);
EXPORT extern int MdsDecompress(struct descriptor_r *rec_ptr, struct descriptor_xd *out_ptr);
EXPORT extern int MdsCopyDxXd(struct descriptor *in, struct descriptor_xd *out);
EXPORT extern int MdsCopyDxXdZ(struct descriptor *in, struct descriptor_xd *out, void **zone, int (*fixup_nid) (), void *fixup_nid_arg,
                        int (*fixup_path) (), void *fixup_path_arg);
EXPORT extern char *MdsDescrToCstring(struct descriptor *);
EXPORT extern void MdsFree(void *);
EXPORT extern int MdsGet1Dx(unsigned int *len, unsigned char *dtype, struct descriptor_xd *dsc, void **zone);
EXPORT extern int MdsGet1DxA(struct descriptor_a * in, unsigned short *len, unsigned char *dtype, struct descriptor_xd *out);
EXPORT extern int MdsGet1DxS(unsigned short *len, unsigned char *dtype, struct descriptor_xd *out);
EXPORT extern int MdsGetCurrentShotId(char *experiment);
EXPORT extern char *MdsGetMsg(int sts);
EXPORT extern int MdsFree1Dx(struct descriptor_xd *dsc, void **zone);
EXPORT extern int MdsMsg(int sts, char fmt[], ...);
EXPORT extern void MdsPk(char *nbits, int *nitems, int pack[], int items[], int *bit);
EXPORT extern int MDSQueueEvent(char *event, int *eventid);
EXPORT extern int MDSGetEventQueue(int eventid, int timeout, int *data_len, char  **data);
EXPORT extern int  MdsSetCurrentShotId(char *experiment,int shot);
EXPORT extern int MdsSerializeDscIn(char const * in, struct descriptor_xd *out);
EXPORT extern int MdsSerializeDscOutZ(struct descriptor *in,struct descriptor_xd *out,
    int (*fixupNid)(), void *fixupNidArg, int (*fixupPath)(),
    void *fixupPathArg, int compress, int *compressible,
    unsigned int *length, unsigned int *reclen,  unsigned char *dtype,
    unsigned char *class, int  altbuflen, void *altbuf, int *data_in_altbuf);
EXPORT extern int MdsSerializeDscOut(struct descriptor *in,struct descriptor_xd *out);
EXPORT extern void MdsUnpk(char *nbits, int *nitems, int pack[], int items[], int *bit);
EXPORT extern int MdsXpand(int *nitems, struct descriptor_a *pack, struct descriptor_a *items, int *bit);
EXPORT extern int MDSEvent(char const * eventName, int num_bytes, char *data);
EXPORT extern int MDSEventAst(char const * eventName, void (*astadr)(void *,int,char *), void *astprm, int *eventid);
EXPORT extern int MDSEventCan(int eventid);
EXPORT extern int MDSUdpEvent(char const * eventName, int bufLen, char const * buf);	
EXPORT extern int MDSUdpEventAst(char const * eventName, void (*astadr)(void *,int,char *), void *astprm, int *eventid);
EXPORT extern int MDSUdpEventCan(int id);
EXPORT extern int MDSWfevent(char *evname, int buflen, char *data, int *datlen);
EXPORT extern int MDSWfeventTimed(char *evname, int buflen, char *data, int *datlen, int timeout);
EXPORT extern char  *MdsDtypeString( int  id );
EXPORT extern char  *MdsClassString( int  id );
EXPORT extern int   MDSprintf( char *fmt , ... );
EXPORT extern char  *MdsUsageString( int  id );
EXPORT extern char *TranslateLogical(char *name);
EXPORT extern void TranslateLogicalFree(char *value);
EXPORT extern int TranslateLogicalXd(struct descriptor *in, struct descriptor_xd *out);
EXPORT extern char *MdsRelease();
EXPORT extern void MdsFloatToTime(double floatTime, uint64_t *outTime);
EXPORT extern void MdsFloatToDelta(double floatTime, uint64_t *outTime);
EXPORT extern void MdsTimeToFloat(uint64_t inTime, float *outFloat);
EXPORT extern void MdsTimeToDouble(uint64_t inTime, double *outFloat);
EXPORT extern int MdsPutEnv(char *cmd);

#ifdef HAVE_WINDOWS_H
#ifndef HAVE_PTHREAD_COND_INIT
typedef int pthread_key_t;
typedef void *pthread_t;
#ifndef HAVE_PTHREAD_ONCE
EXPORT extern void pthread_once(int *one_time,void (*key_alloc));
#endif
#ifndef HAVE_PTHREAD_GETSPECIFIC
EXPORT extern void *pthread_getspecific(pthread_key_t buffer_key);
#endif
#ifndef HAVE_PTHREAD_SETSPECIFIC
EXPORT extern void pthread_setspecific(pthread_key_t  buffer_key, void *p);
#endif
#ifndef HAVE_PTHREAD_KEY_CREATE
EXPORT extern void pthread_key_create(pthread_key_t *buffer_key,void *d2);
#endif
#ifndef HAVE_PTHREAD_MUTEX_LOCK
EXPORT extern int pthread_mutex_lock(void **mutex);
#endif
#ifndef HAVE_PTHREAD_MUTEX_UNLOCK
EXPORT extern int pthread_mutex_unlock(void **mutex);
#endif
#ifndef HAVE_PTHREAD_MUTEX_INIT
EXPORT extern int pthread_mutex_init(void **mutex, void *);
#endif
#ifndef HAVE_PTHREAD_CLEANUP_POP
EXPORT extern void pthread_cleanup_pop();
#endif
#ifndef HAVE_PTHREAD_CLEANUP_PUSH
EXPORT extern void pthread_cleanup_push();
#endif
#ifndef HAVE_PTHREAD_COND_INIT
EXPORT extern int pthread_cond_init(void **cond,void *def);
#endif
#ifndef HAVE_PTHREAD_COND_WAIT
EXPORT extern int pthread_cond_wait(void **cond, void **mutex);
#endif
#ifndef HAVE_PTHREAD_CREATE
EXPORT extern int pthread_create(pthread_t *thread, void *dummy, void *(*rtn)(void *), void *rtn_param);
#endif
#ifndef HAVE_PTHREAD_LOCK_GLOBAL_NP
EXPORT extern void pthread_lock_global_np();
#endif
#ifndef HAVE_PTHREAD_UNLOCK_GLOBAL_NP
EXPORT extern void pthread_unlock_global_np();
#endif
#ifndef HAVE_PTHREAD_COND_SIGNAL
EXPORT extern int pthread_cond_signal(void *cond);
#endif
#ifndef HAVE_PTHREAD_COND_TIMEDWAIT
EXPORT extern int pthread_cond_timedwait(void **cond, void **mutex, int msec);
#endif
#ifndef HAVE_PTHREAD_COND_DESTROY
EXPORT extern int pthread_cond_destroy(void **cond);
#endif
#ifndef HAVE_PTHREAD_MUTEX_DESTROY
EXPORT extern int pthread_mutex_destroy(void **mutex);
#endif
#ifndef HAVE_PTHREAD_CANCEL
EXPORT extern void pthread_cancel(void *thread);
#endif
#endif
#endif
