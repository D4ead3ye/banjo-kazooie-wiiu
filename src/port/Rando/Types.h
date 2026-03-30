#ifndef RANDO_TYPES_H
#define RANDO_TYPES_H

typedef enum {
    RP_MUSIC_NOTE = 0x164,
} RandoPropId;

typedef enum {
    RCTYPE_UNKNOWN,
    RCTYPE_JINJO,
    RCTYPE_MAX,
} RandoCheckType;

typedef enum {
    RC_UNKNOWN,
    RC_MM_JINJO_BLUE,
    RC_MM_JINJO_GREEN,
    RC_MM_JINJO_ORANGE,
    RC_MM_JINJO_PINK,
    RC_MM_JINJO_YELLOW,
    RC_MAX,
} RandoCheckId;

typedef enum {
    RE_UNKNOWN,
    RE_MAX,
} RandoEntranceId;

// typedef enum {
//     RITYPE_UNKNOWN,
//     RITYPE_MAX,
// } RandoItemType;

// typedef enum {
//     RI_UNKNOWN,
//     RI_MAX,
// } RandoItemId;

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
