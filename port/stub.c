#include "ResourceHelpers.h"
#include "libultraship/libultra/gbi.h"
#include "libultraship/libultra/interrupt.h"
#include "libultraship/libultra/sptask.h"
#include "libultraship/libultra/thread.h"
#include "resourcebridge.h"
#include <math.h>
#include "bk_string.h"

#include <libultra/convert.h>
#include <libultra/exception.h>
#include <libultra/rcp.h>

#define DEFAULT_FRAMEBUFFER_WIDTH 292
#define DEFAULT_FRAMEBUFFER_HEIGHT 216


u64 osClockRate = OS_CLOCK_RATE;
s32 osViClock = VI_NTSC_CLOCK;
u32 __osShutdown = 0;
u32 __OSGlobalIntMask = OS_IM_ALL;
s32 osCicId = 6103;
u8 D_8000E800;


u16 gFramebuffers[2][DEFAULT_FRAMEBUFFER_WIDTH * DEFAULT_FRAMEBUFFER_HEIGHT];


int ResourceMgr_OTRSigCheck(char* imgData) {
  uintptr_t i = (uintptr_t)(imgData);

  // if (i == 0xD9000000 || i == 0xE7000000 || (i & 1) == 1)
  if ((i & 1) == 1)
    return 0;

  // if ((i & 0xFF000000) != 0xAB000000 && (i & 0xFF000000) != 0xCD000000 && i != 0) {
  if (i != 0) {
    if (imgData[0] == '_' && imgData[1] == '_' && imgData[2] == 'O' && imgData[3] == 'T' && imgData[4] == 'R' &&
        imgData[5] == '_' && imgData[6] == '_')
      return 1;
  }

  return 0;
}

void gSPDisplayList(Gfx* pkt, Gfx* dl) {
  char* imgData = (char*)dl;

  if (ResourceMgr_OTRSigCheck(imgData) == 1) {

    // ResourceMgr_PushCurrentDirectory(imgData);
    // gsSPPushCD(pkt++, imgData);
    dl = ResourceMgr_LoadGfxByName(imgData);
  }

  __gSPDisplayList(pkt, dl);
}

void gSPDisplayListOffset(Gfx* pkt, Gfx* dl, int offset) {
  char* imgData = (char*)dl;

  if (ResourceMgr_OTRSigCheck(imgData) == 1)
    dl = ResourceMgr_LoadGfxByName(imgData);

  __gSPDisplayList(pkt, dl + offset);
}

void gSPVertex(Gfx* pkt, uintptr_t v, int n, int v0) {
  if (ResourceMgr_OTRSigCheck((char*)v) == 1)
    v = (uintptr_t)ResourceMgr_LoadVtxByName((char*)v);

  __gSPVertex(pkt, v, n, v0);
}

void gSPInvalidateTexCache(Gfx* pkt, uintptr_t texAddr) {
  char* imgData = (char*)texAddr;

  if (texAddr != 0 && ResourceMgr_OTRSigCheck(imgData)) {
    // Temporary solution to the mq/nonmq issue, this will be
    // handled better with LUS 1.0
    texAddr = (uintptr_t)ResourceMgr_LoadTexOrDListByName(imgData);
  }

  __gSPInvalidateTexCache(pkt, texAddr);
}

void gSPSegment(void* value, int segNum, uintptr_t target) {
  char* imgData = (char*)target;

  int res = ResourceMgr_OTRSigCheck(imgData);

  // OTRTODO: Disabled for now to fix an issue with HD Textures.
  // With HD textures, we need to pass the path to F3D, not the raw texture data.
  // Otherwise the needed metadata is not available for proper rendering...
  // This should *not* cause any crashes, but some testing may be needed...
  // UPDATE: To maintain compatability it will still do the old behavior if the resource is a display list.
  // That should not affect HD textures.
  if (res) {
    uintptr_t desiredTarget = (uintptr_t)ResourceMgr_LoadIfDListByName(imgData);

    if (desiredTarget != NULL)
      target = desiredTarget;
  }

  __gSPSegment(value, segNum, target);
}

void gSPSegmentLoadRes(void* value, int segNum, uintptr_t target) {
  char* imgData = (char*)target;

  int res = ResourceMgr_OTRSigCheck(imgData);

  if (res) {
    target = (uintptr_t)ResourceMgr_LoadTexOrDListByName(imgData);
  }

  __gSPSegment(value, segNum, target);
}

f32 gu_sqrtf(f32 f) {
  return sqrtf(f);
}
void osCreateThread(OSThread* thread, OSId id, void* entry, void* arg, void* sp, OSPri p) {
}
void osStartThread(OSThread* thread) {
}
void osStopThread(OSThread *t){}
void osDestroyThread(OSThread* thread) {
}
void osSpTaskYield(void) {
}
void osSpTaskLoad(OSTask* task) {
}
void osSpTaskStartGo(OSTask* task) {
}
void osViExtendVStart(u32 arg0) {
}
void osSetThreadPri(OSThread* thread, OSPri p) {
}
s32 osContSetCh(u8 ch) {
  return 0;
}
u32 __osGetSR(void) {
  return 0;
}
void __osSetSR(u32 value) {
}
OSYieldResult osSpTaskYielded(OSTask* task) {
  return 0;
}
// Lighthouse TODO these need to be implemented in LUS
int osStartTimer(void *t){
  return 0;
}
int osStopTimer(void *t){
  return 0;
}

void osDpSetStatus(u32 data){}
OSIntMask osSetIntMask(OSIntMask a) {
  return 0;
}

void __osError(s16 error_code, s16 num_args, ...) {
}

// Lighthouse TODO why is this stubbed and not decompiled?
void func_80253010(void *dest, void *src, s32 size) {

}

s32 osMotorStop(void *pfs){
  return 0;
}
s32 osMotorStart(void *pfs){
  return 0;
}

s32 osAiSetFrequency(u32 frequency) {
  f32 dacRateF = ((f32)osViClock / frequency) + 0.5f;
  u32 dacRate = dacRateF;
  if (dacRate < 132) {
    return -1;
  }
  return osViClock / (s32)dacRate;

}

s32 eeprom_writeBlocks(s32 file, s32 offset, void *buffer, s32 count){return 0;}
s32 eeprom_readBlocks(s32 file, s32 offset, void *buffer, s32 count){ return 0;}

/* BSD memory functions */
void bzero(void *s, size_t n) {
    memset(s, 0, n);
}

void bcopy(const void *src, void *dest, size_t n) {
    memmove(dest, src, n);
}

/* Rename main to SDL_main for SDL compatibility */
#ifdef _WIN32
#define main SDL_main
#endif

int SDL_main (int argc, char *argv[]) {

  return 0;
}