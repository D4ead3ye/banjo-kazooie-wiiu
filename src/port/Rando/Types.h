#ifndef RANDO_TYPES_H
#define RANDO_TYPES_H

typedef enum {
    RP_MUSIC_NOTE = 0x164,
} RandoPropId;

typedef enum {
    RCTYPE_UNKNOWN,
    RCTYPE_JIGGY,
    RCTYPE_JINJO,
    RCTYPE_MUMBO_TOKEN,
    RCTYPE_MAX,
} RandoCheckType;

typedef enum {
    RC_UNKNOWN,
    RC_MM_JIGGY_CHIMPY,
    RC_MM_JIGGY_CONGA,
    RC_MM_JIGGY_HILL,
    RC_MM_JIGGY_HUTS,
    RC_MM_JIGGY_JINJO,
    RC_MM_JIGGY_JUJU,
    RC_MM_JIGGY_MUMBOS_SKULL,
    RC_MM_JIGGY_ORANGE_PADS,
    RC_MM_JIGGY_RUINS,
    RC_MM_JIGGY_TICKERS_TOWER,
    RC_MM_JINJO_BLUE,
    RC_MM_JINJO_GREEN,
    RC_MM_JINJO_ORANGE,
    RC_MM_JINJO_PINK,
    RC_MM_JINJO_YELLOW,
    RC_MM_MUMBO_TOKEN_BEHIND_PINK_JINJO,
    RC_MM_MUMBO_TOKEN_BEHIND_RUINS,
    RC_MM_MUMBO_TOKEN_INSIDE_TICKERS_TOWER,
    RC_MM_MUMBO_TOKEN_STUMP_NEAR_CONGA,
    RC_MM_MUMBO_TOKEN_UNDER_RAMP_TO_MUMBOS_SKULL,
    RC_MAX,
} RandoCheckId;

// typedef enum {
//     RE_UNKNOWN,
//     RE_MAX,
// } RandoEntranceId;

typedef enum {
    RITYPE_UNKNOWN,
    RITYPE_JIGGY,
    RITYPE_JINJO,
    RITYPE_MAX,
} RandoItemType;

typedef enum {
    RI_UNKNOWN,
    RI_JIGGY,
    RI_JINJO_BLUE,
    RI_JINJO_GREEN,
    RI_JINJO_ORANGE,
    RI_JINJO_PINK,
    RI_JINJO_YELLOW,
    RI_MAX,
} RandoItemId;

// typedef enum {
//     RR_MAX,
// } RandoRegionId;

// typedef enum {
//     RO_LOGIC,
//     RO_MAX,
// } RandoOptionId;

// typedef enum {
//     RO_GENERIC_OFF,
//     RO_GENERIC_ON,
// } RandoOptionGenericOffOn;

// typedef enum {
//     RO_LOGIC_GLITCHLESS,
//     RO_LOGIC_NO_LOGIC,
// } RandoOptionLogic;

// typedef enum {
//     RANDO_INF_MAX,
// } RandoInf;

#endif // RANDO_TYPES_H
