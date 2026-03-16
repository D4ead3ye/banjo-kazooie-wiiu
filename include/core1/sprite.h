#ifndef BANJO_KAZOOIE_CORE1_SPRITE_H
#define BANJO_KAZOOIE_CORE1_SPRITE_H

u32 sprite_getUnk8(BKSprite *self);
u32 sprite_getUnkA(BKSprite *self);
u32 sprite_getUnk6(BKSprite *self);
u32 sprite_getUnk4(BKSprite *self);
s32 sprite_getFrameCount(BKSprite *self);
BKSpriteFrame *sprite_getFramePtr(BKSprite *self, u32 frame_id);

#endif
