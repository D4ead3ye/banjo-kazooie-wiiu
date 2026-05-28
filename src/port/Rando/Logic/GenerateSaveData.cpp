#include "Logic.h"
#include "port/ui/Notification.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/ui/cvar_prefixes.h"
#include "port/enhancements/events/hooks/Events.h"

#include "port/save/Types.h"

extern "C" {
void ability_setLearned(s32 move, s32 val);
void ability_setHasUsed(enum ability_e move);

void item_setMaxCount(s32 item);
}

// clang-format off
std::map<ability_e, std::pair<const char*, const char*>> abilityLoadoutMap = {
    { ABILITY_0_BARGE,              { "Beak Barge",     CVAR_RANDOMIZER_SETTING("Loadout.Abilities.BeakBarge") } },
    { ABILITY_1_BEAK_BOMB,          { "Beak Bomb",      CVAR_RANDOMIZER_SETTING("Loadout.Abilities.BeakBomb") } },
    { ABILITY_2_BEAK_BUSTER,        { "Beak Buster",    CVAR_RANDOMIZER_SETTING("Loadout.Abilities.BeakBuster") } },
    { ABILITY_3_CAMERA_CONTROL,     { "Camera Control", CVAR_RANDOMIZER_SETTING("Loadout.Abilities.CameraControl") } },
    { ABILITY_4_CLAW_SWIPE,         { "Claw Swipe",     CVAR_RANDOMIZER_SETTING("Loadout.Abilities.ClawSwipe") } },
    { ABILITY_5_CLIMB,              { "Climb",          CVAR_RANDOMIZER_SETTING("Loadout.Abilities.Climb") } },
    { ABILITY_6_EGGS,               { "Eggs",           CVAR_RANDOMIZER_SETTING("Loadout.Abilities.Eggs") } },
    { ABILITY_7_FEATHERY_FLAP,      { "Feathery Flap",  CVAR_RANDOMIZER_SETTING("Loadout.Abilities.FeatheryFlap") } },
    { ABILITY_8_FLAP_FLIP,          { "Flap Flip",      CVAR_RANDOMIZER_SETTING("Loadout.Abilities.FlapFlip") } },
    { ABILITY_9_FLIGHT,             { "Flight",         CVAR_RANDOMIZER_SETTING("Loadout.Abilities.Flight") } },
    { ABILITY_A_HOLD_A_JUMP_HIGHER, { "Jump Higher",    CVAR_RANDOMIZER_SETTING("Loadout.Abilities.JumpHigher") } },
    { ABILITY_B_RATATAT_RAP,        { "Rat-a-tat Rap",  CVAR_RANDOMIZER_SETTING("Loadout.Abilities.RatatatRap") } },
    { ABILITY_C_ROLL,               { "Roll",           CVAR_RANDOMIZER_SETTING("Loadout.Abilities.Roll") } },
    { ABILITY_D_SHOCK_JUMP,         { "Shock Jump",     CVAR_RANDOMIZER_SETTING("Loadout.Abilities.ShockJump") } },
    { ABILITY_E_WADING_BOOTS,       { "Wading Boots",   CVAR_RANDOMIZER_SETTING("Loadout.Abilities.WadingBoots") } },
    { ABILITY_F_DIVE,               { "Dive",           CVAR_RANDOMIZER_SETTING("Loadout.Abilities.Dive") } },
    { ABILITY_10_TALON_TROT,        { "Talon Trot",     CVAR_RANDOMIZER_SETTING("Loadout.Abilities.TalonTrot") } },
    { ABILITY_11_TURBO_TALON,       { "Turbo Talon",    CVAR_RANDOMIZER_SETTING("Loadout.Abilities.TurboTalon") } },
    { ABILITY_12_WONDERWING,        { "Wonderwing",     CVAR_RANDOMIZER_SETTING("Loadout.Abilities.Wonderwing") } },
};

std::map<item_e, std::pair<const char*, const char*>> itemLoadoutMap = {
    { ITEM_D_EGGS,          { "Blue Eggs",      CVAR_RANDOMIZER_SETTING("Loadout.Items.BlueEggs") } },
    { ITEM_F_RED_FEATHER,   { "Red Feathers",   CVAR_RANDOMIZER_SETTING("Loadout.Items.RedFeathers") } },
    { ITEM_10_GOLD_FEATHER, { "Gold Feathers",  CVAR_RANDOMIZER_SETTING("Loadout.Items.GoldFeathers") } },
};
    // clang-format on

void Rando::Logic::InitializeSaveData(SaveData* saveData) {
    // RandoSaveCheck - Initialize
    for (auto& [randoCheckId, randoStaticCheck] : Rando::StaticData::Checks) {
        RandoSaveCheck randoSaveCheck = {
            .name = randoStaticCheck.name,
            .randoItemId = Rando::StaticData::GetRandoItemByActorId((actor_e)randoStaticCheck.actorId),
            .shuffledCheckId = randoCheckId,
            .randoCollectionId = randoStaticCheck.collectionId,
            .isShuffled = false,
            .obtained = false,
            .skipped = false,
        };

        saveData->shipSaveData.randoSaveData.randoSaveCheck[randoCheckId] = randoSaveCheck;
    }

    for (auto& [randoOptionId, randoStaticOption] : Rando::StaticData::Options) {
        RandoSaveOption randoSaveOption = {
            .name = randoStaticOption.name,
            .optionValue = CVarGetInteger(randoStaticOption.cvar, randoStaticOption.defaultValue),
        };

        saveData->shipSaveData.randoSaveData.randoSaveOption[randoOptionId] = randoSaveOption;
    }
}

void Rando::Logic::GenerateSaveData(SaveData* saveData) {
    for (auto& object : Rando::Logic::shuffledPool) {
        RandoCheckId randoCheckId = Rando::StaticData::GetCheckByName(object.name);

        saveData->shipSaveData.randoSaveData.randoSaveCheck[randoCheckId] = object;
    }
}

void Rando::Logic::GrantStartingLoadout() {
    for (auto& [ability, abilityInfo] : abilityLoadoutMap) {
        if (ability == ABILITY_A_HOLD_A_JUMP_HIGHER || CVarGetInteger(abilityInfo.second, 0)) {
            ability_setLearned(ability, true);
            ability_setHasUsed(ability);
        }
    }
    for (auto& [item, itemInfo] : itemLoadoutMap) {
        if (CVarGetInteger(itemInfo.second, 0)) {
            item_setMaxCount(item);
        }
    }
}