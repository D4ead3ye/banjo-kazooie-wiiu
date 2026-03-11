// [port] Missing function prototypes that cause silent bugs on 64-bit.
// Without these, the compiler assumes int return, which:
//   - Truncates 64-bit pointers (crash on aarch64/x64)
//   - Misreads float returns from integer register (corrupted values)
// Generated from build log C4013/C4047 warning analysis.

#ifndef PORT_PROTOTYPES_H
#define PORT_PROTOTYPES_H

#include <ultra64.h>
#include "structs.h"
#include "enums.h"
#include "model.h"
#include "core2/camera.h"
#include "core2/anim/sprite.h"

// ============================================================
// POINTER-RETURNING FUNCTIONS (crash without prototypes)
// ============================================================

// --- core2/camera/camera_interp.c ---
BKModelBin *func_8030A4B4(s32 arg0);
BKSpriteDisplayData *func_8030A4D4(s32 arg0);
BKSprite *func_8030A55C(s32 arg0);

// --- core2/actor_cubepropsystem.c ---
ActorMarker *func_8032DCAC(void);
BKVertexList *func_80330CFC(Actor *this, s32 arg1);
BKModelBin *func_80330E28(Actor *this);
BKSpriteDisplayData *func_80330E54(ActorMarker *marker, BKSprite **sprite_ptr);
BKSpriteDisplayData *func_80330F30(ActorMarker *marker);
BKSprite *func_80330F50(ActorMarker *marker);
BKSprite *func_8033B6C4(enum asset_e sprite_id, BKSpriteDisplayData **arg1);
NodeProp *codeA5BC0_getPropNodeAtIndex(Cube *cube, s32 prop_index);

// --- core2/actor_cubebounds.c ---
NodeProp *cubeList_findNodePropByActorIdAndPosition_s32(enum actor_e actor_id, s32 position[3]);
NodeProp *func_80305510(s32 arg0);
BKCollisionTri *func_80303800(f32 volume_p1[3], f32 volume_p2[3], f32 arg2[3], u32 arg3);
NodeProp *nodeprop_findByActorIdAndPosition_s16(enum actor_e actor_id, s16 *position);

// --- core2/map/model.c ---
// NOTE: func_802E76B0, func_802E805C, func_802E8E88, func_802E9118,
// func_802E92AC, func_802E9DD8 return BKCollisionTri* but have conflicting
// local externs (bool/s32/void) in decomp source files. Files that need
// the pointer type already have correct local externs.
BKCollisionTri *func_802E76B0(BKCollisionList *collisionList, BKVertexList *vertexList, f32 startPoint[3], f32 endPoint[3], f32 arg4[3], u32 flagFilter);

// --- core2/misc_updates.c ---
BKCollisionTri *func_8029463C(void);
BKModelBin *func_802946A8(void);
BKCollisionTri *func_802946CC(void);

// --- core2/collision/raycast.c ---
BKCollisionTri *func_8031BABC(f32 *arg0, f32 arg1, f32 arg2, u32 arg3, struct86s *arg4);
BKCollisionTri *func_8031BBA0(f32 *this, f32 arg1, f32 arg2, u32 arg3, struct86s *arg4);
BKCollisionTri *func_8031C5EC(struct0 *this);
BKCollisionTri *func_8031C5F4(struct0 *this);
BKModelBin *func_8031C5DC(struct0 *this);

// --- core2/collision/funcs.c ---
BKCollisionTri *func_80320B98(f32 arg0[3], f32 arg1[3], f32 arg2[3], u32 arg3);
// func_80320C94, func_80320DB0 return BKCollisionTri* but
// have conflicting local externs (bool/s32/int) in decomp source files.
void *func_803209EC(void);

// --- core1/collision.c ---
// func_80244D94, func_80244E54, func_8024575C return BKCollisionTri* but
// have conflicting local externs (bool/s32/int/void) in decomp source files.
BKCollisionTri *func_802457C4(f32 arg0[3], f32 arg1[3], f32 arg2, f32 arg3, f32 arg4[3], s32 arg5, u32 arg6);

// --- core2/actor_array.c ---
BKModelBin *func_803257B4(ActorMarker *marker);
Actor *actorArray_findActorFromMarkerId(enum marker_e marker_id);
void *actors_appendToSavestate(void *begin, uintptr_t end);

// --- core2/glspline.c ---
struct56s *func_80342038(s32 indx);
struct56s *func_80341EF0(f32 arg0[3]);
struct56s *func_80341F64(s32 arg0);
struct56s *func_80343F00(s32 indx, f32 arg1[3]);

// --- core2/collision/polydetect.c ---
Struct83s *func_803406B0(void);
Struct83s *func_803406D4(Struct83s *self);

// --- core2/vtxlist.c ---
BKVertexList *vtxList_clone(BKVertexList *vtxList);

// --- core2/spline_bezier.c ---
struct5Bs *func_8034A2C8(void);
struct5Bs *func_8034A348(struct5Bs *this);

// --- core2/mapModel.c ---
struct5Bs *func_803097A0(void);
BKCollisionList *model_getCollisionList(BKModelBin *arg0);

// --- core2/modelRender.c ---
BKModelUnk14List *func_8033A12C(BKModelBin *this);

// --- core2/vtx/colorapply.c ---
Struct70s *func_8034C344(s32 arg0);
Struct70s *func_8034C448(s32 arg0);
Struct70s *func_8034C630(s32 arg0);

// --- core2/particle/samplerate.c ---
Struct5Ds *func_802F47D0(void);
Struct5Ds *func_802F499C(Struct5Ds *this);

// --- core2/font/render.c ---
BKSpriteTextureBlock *func_802E4D5C(s32 arg0, char arg1);

// --- core2/font/print.c ---
BKSpriteTextureBlock *func_802F5494(s32 letterId, s32 *fontType);

// --- core2/anim/anim_spriteframe.c / particle/typeindex.c etc. ---
ParticleEmitter *func_802EDD8C(f32 pos[3], f32 xz_range, f32 arg2);
ParticleEmitter *func_802F1EC8(f32 *position);
ParticleEmitter *func_802F3E98(f32 pos[3], enum asset_e sprite_id);
ParticleEmitter *func_802F4274(f32 arg0[3]);
ParticleEmitter *func_802F0EF0(u8 arg0);

// --- core2/collectible/bundle.c ---
Actor *bundle_spawn_f32(enum bundle_e bundle_id, f32 position[3]);
Actor *bundle_spawn_s32(enum bundle_e bundle_id, s32 position[3]);

// --- core2/actor_array.c (jiggy actors) ---
Actor **actorArray_findJiggyActors(void);

// --- core2/ba/ba_animcache.c ---
#ifndef ANIMATION_H
typedef struct animation_file_s AnimationFile;
#endif
AnimationFile *animBinCache_get(enum asset_e asset_id);

// --- core2/anim/anim_buffer.c ---
BoneTransformList *anim_getTransform(Animation *this, s32 index);
BoneTransformList *animcache_getCurrentTransform(Animation *this);
BoneTransformList *anim_getStartTransform(Animation *this);
BoneTransformList *anim_getTargetTransform(Animation *this);

// --- core1/audio_manager.c ---
void *func_802403B8(void *state);
void *audioManager_getThread_PAL(void);

// --- core2/anim/anim_sequencehandler.c ---
u8 *func_8032479C(void);

// --- core2/anim/anim_defrag.c (animMtxList) ---
// AnimMtxList already typedef'd via anctrl.h (included earlier via prop.h)
// BoneTransformList needs forward declaration (bonetransform.h not in include chain)
#ifndef _BONE_TRANSFORMATION_H_
typedef struct bone_transform_list_s BoneTransformList;
#endif
MtxF *animMtxList_get(AnimMtxList *this, s32 arg1);
AnimMtxList *animMtxList_new(void);
void animMtxList_free(AnimMtxList *this);
s32 animMtxList_len(AnimMtxList *this);
void animMtxList_setBoneless(AnimMtxList **this_ptr, BKAnimationList *anim_list);
void animMtxList_setBoned(AnimMtxList **this_ptr, BKAnimationList *anim_list, BoneTransformList *arg2);
AnimMtxList *animMtxList_defrag(AnimMtxList *this);

// ============================================================
// FLOAT-RETURNING FUNCTIONS (garbage values without prototypes)
// ============================================================

f32 alCents2Ratio(s32 cents);
f32 babuzz_80290890(f32 arg0);
f32 babuzz_80290920(f32 arg0, f32 arg1, f32 arg2);
f32 baeyes_getEyePosition(s32 id);
f32 baanim_getTimer(void);
f32 batimer_get(s32 id);
f32 bafalldamage_get_distance_fallen(void);
f32 bastick_calculateZonePosition(f32 arg0, f32 arg1, f32 arg2);
f32 bastick_getX(void);
f32 bastick_getAngle(void);
f32 baModel_getYaw(void);
f32 baphysics_get_gravity(void);
f32 baphysics_get_target_horizontal_velocity(void);
f32 baphysics_get_target_vertical_velocity(void);
f32 baphysics_get_target_yaw(void);
f32 baphysics_get_vertical_velocity(void);
f32 baphysics_get_horizontal_velocity(void);
f32 baphysics_get_horizontal_velocity_percentage(void);
f32 climbGetRadius(void);
f32 func_8029B3B0(f32 arg0);
f32 func_8029B56C(f32 arg0, f32 arg1, f32 arg2, f32 arg3);
f32 func_8029B9C0(void);
f32 func_8029B9FC(void);
f32 func_8029BA44(void);
f32 func_8029CED0(void);
f32 func_802BB34C(s32 arg0);
f32 func_802BB938(f32 arg0[3], f32 arg1[3]);
f32 func_802BBD48(void);
f32 func_802BBEA4(f32 arg0[3], f32 arg1[3], f32 arg2, s32 arg3, s32 arg4);
f32 func_802A2858(void);
f32 func_802A88B0(void);
f32 func_802A8900(void);
f32 func_802A8934(void);
f32 func_802A8984(void);
f32 func_802A716C(void);
f32 func_802B051C(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
f32 __bscroc_getMaxVelocity(void);
f32 func_802D7038(Actor *this);
f32 player_getYaw(void);

// ============================================================
// POINTER-RETURNING FUNCTIONS (additional, from void/int section)
// ============================================================

// These return pointers but are commonly listed with utility functions.
// Only functions that return pointers or have no conflicting local externs.
NodeProp *cube_findNodePropByActorId(Cube *cube, enum actor_e actor_id);
AnimTexture *model_getAnimTextureList(BKModelBin *);

// --- core2/nc/cameranodelist.c ---
CameraNodeType1 *ncCameraNodeList_getCameraNodeType1(int camera_node_index);
CameraNodeType2 *ncCameraNodeList_getCameraNodeType2(int camera_node_index);
CameraNodeType3 *ncCameraNodeList_getCameraNodeType3(int camera_node_index);
CameraNodeType4 *ncCameraNodeList_getCameraNodeType4(int camera_node_index);

// ============================================================
// [port] MISC POINTER-RETURNING + OS + GBI PROTOTYPES
// ============================================================

s16 *func_8030C704(void);

// ============================================================
// COMMONLY MISSING PROTOTYPES (C4013 cleanup)
// These mostly return int/void/bool so implicit int was harmless,
// but explicit prototypes prevent accidental misuse.
// ============================================================

// --- core2/cutscene/flag.c ---
enum map_e map_get(void);
s32 exit_get(void);
s32 func_80334904(void);

// --- core2/fx/score_jiggy.c ---
u32 jiggyscore_isCollected(enum jiggy_e jiggy_id);
int jiggyscore_isSpawned(enum jiggy_e jiggy_id);

// --- core2/fx/score_honeycomb.c ---
bool honeycombscore_get(enum honeycomb_e indx);

// --- core2/map/specificflags.c ---
s32 mapSpecificFlags_get(s32 i);

// --- core2/level/specificflags.c ---
s32 levelSpecificFlags_get(s32 i);

// --- core2/terrain_material.c (item system) ---
s32 item_empty(enum item_e item);
void item_set(s32 item, s32 val);

// --- core2/game_complete.c ---
bool fileProgressFlag_get(enum file_progress_e index);
s32 fileProgressFlag_getN(enum file_progress_e offset, s32 numBits);
s32 volatileFlag_get(enum volatile_flags_e index);

// --- core2/dialog/code_CF3E0.c ---
void volatileFlag_setAndTriggerDialog_0(enum volatile_flags_e arg0);

// --- core1/init.c ---
s32 globalTimer_getTime(void);

// --- core2/actor_array.c ---
bool func_80329530(Actor *this, s32 dist);
s32 func_803297C8(Actor *arg0, f32 arg1[3]);
s32 func_80329054(Actor *arg0, s32 arg1); // decomp has s32 arg0 but callers pass Actor*
void subaddie_set_ideal_yaw(Actor *this, int arg1);
bool subaddie_playerIsWithinSphere(Actor *this, s32 dist);
Actor *actor_spawnWithYaw_s32(enum actor_e id, s32 (*pos)[3], s32 rot);

// --- core2/actor_cubebounds.c ---
bool nodeProp_findPositionFromActorId(enum actor_e actor_id, f32 *arg1);

// --- core2/ba/ba_lookdir.c ---
enum bsgroup_e player_movementGroup(void);
enum hitbox_e player_getActiveHitbox(ActorMarker *marker);
bool player_isDead(void);
bool func_8028F20C(void);
s32 func_8028F66C(enum bs_interrupt_e arg0);
void func_8028F8F8(s32 arg0, bool arg1);
void func_8028F918(s32 arg0);
void func_8028FA14(enum map_e map_id, s32 exit_id);

// --- core2/frame/bufferreadback.c ---
s32 getGameMode(void);

// --- core2/sfx/source.c ---
void func_8030DD90(u8 indx, s32 arg1);
int func_8030E3FC(u8 indx);

// --- core2/camera/camera_motor1.c ---
void func_802BAFE4(s32 arg0);
bool func_802BB270(void);

// --- core2/gc/dialog.c ---
void func_803114D0(void);
int func_803114B0(void);

// --- core2/cutscene/lair.c ---
void func_8031CD20(NodeProp *arg0, s32 arg1, s32 arg2);

// --- core2/collision/polydetect.c ---
void func_80340690(Struct83s *self);

// --- core2/collision/funcs.c ---
void func_80320B24(void *arg0, void *arg1, void *arg2);

// --- core2/collision/climbsurface.c ---
s32 func_8029453C(void);

// --- core2/spline_bezier.c ---
void func_8034A2A8(struct5Bs *this);

// --- core2/vtxlist.c ---
void vtxList_free(BKVertexList *vtxList);

// --- core2/ba/ba_model.c / assetcache ---
void assetcache_release(void *);

// --- core2/glspline.c ---
void func_80343DEC(Actor *this);

// --- core2/model/render.c ---
s32 func_8033A0F0(s32 arg0);

// --- core2/spawn_queue.c ---
void __spawnQueue_add_0(void (*arg0)(void));
void __spawnQueue_add_2(void (*arg0)(void), uintptr_t arg1, uintptr_t arg2);

// --- core2/sprite/displaydata.c ---
void func_8033E73C(ActorMarker *arg0, s32 arg1, void *arg2); // arg2 is FuncUnk40 but callers pass varied callback types
int func_8033E3F0(enum common_particle_e particle_id, int arg1);

// --- core2/particle/particle.c ---
void particleEmitter_setModel(ParticleEmitter *this, enum asset_e model_id);
void particleEmitter_setSfx(ParticleEmitter *this, enum sfx_e sfx_id, s32 arg2);
void particleEmitter_setAlpha(ParticleEmitter *this, s32 arg1);
void particleEmitter_setVelocityAccelerationAndPositionRanges(ParticleEmitter *this, ParticleSettingsVelocityAccelerationPosition *settings);
void func_802EFC28(ParticleEmitter *this, ParticleSettingsScaleAndLifetimeDrawModeEmitCount *settings);

// --- core2/quiz/game.c ---
void func_802D68F0(s32 seconds);
void func_802D4A9C(Actor *this, s32 arg1);
void func_802D4AC0(Actor *this, s32 arg1, enum file_progress_e arg2);

// --- core2/projectile_blueegg.c ---
void func_80353580(ActorMarker *marker);

// --- core1/audio_musicplayer.c ---
void func_8025A58C(u32 arg0, u32 arg1);
void func_8025AABC(enum comusic_e track_id);
void func_8025AEA0(enum comusic_e track_id, s32 arg1);

// --- core1/stopnswop.c ---
bool sns_get_item_state(s32 item, s32 set);

// --- FP/ma/slalom.c ---
bool maSlalom_isActive(void);

// --- FP/ch/boggy2.c ---
bool func_8038A1A0(ActorMarker *marker);

// func_80389F5C omitted: decomp defines as (void) but callers pass Actor*

// --- CC/model_renderstate.c ---
void CC_func_80387D4C(void);

// OS function prototypes
extern uintptr_t osVirtualToPhysical(void *addr);
extern void *osPhysicalToVirtual(uintptr_t addr);

// GBI function prototypes (implementations in src/port/stub.c)
void gSPSegment(void* value, int segNum, uintptr_t target);
void gSPSegmentLoadRes(void* value, int segNum, uintptr_t target);
void gSPDisplayList(Gfx* pkt, Gfx* dl);
void gSPDisplayListOffset(Gfx* pkt, Gfx* dl, int offset);
void gSPVertex(Gfx* pkt, uintptr_t v, int n, int v0);
void gSPInvalidateTexCache(Gfx* pkt, uintptr_t texAddr);
int ResourceMgr_OTRSigCheck(char* imgData);

#endif // PORT_PROTOTYPES_H
