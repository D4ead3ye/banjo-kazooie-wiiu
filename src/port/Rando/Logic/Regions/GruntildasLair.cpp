#include "src/port/ShipInit.hpp"
#include "src/port/Rando/Logic/Logic.h"

using namespace Rando::Logic;

// clang-format off
static RegisterShipInitFunc initFunc([]() {
    Regions[RR_GRUNTILDAS_LAIR_BEFORE_NOTE_DOOR_180] = RandoRegion{ .regionName = "Before Second Note Door", .levelId = LEVEL_6_LAIR,
        .checks = {
		    // CHECK(RC_GL_JIGGY_LOBBY, true),
		},
        .connections = {
            CONNECTION(RR_GRUNTILDAS_LAIR_BEFORE_NOTE_DOOR_260,                 CAN_UNLOCK_NOTE_DOOR(180)),
            CONNECTION(RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_TREASURE_TROVE_COVE,    true),
            CONNECTION(RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_CLANKERS_CAVERN,        CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
            CONNECTION(RR_GRUNTILDAS_LAIR_NEAR_ORANGE_CAULDRON_1,               true),
            CONNECTION(RR_GRUNTILDAS_LAIR_PAST_NOTE_DOOR_50,                    true),
        },
        .events = {
            EVENT(RA_NOTE_DOOR_180,                 CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            EVENT(RA_PUZZLE_BOARD_CLICK_CLOCK_WOOD, CAN_USE_ABILITY(ABILITY_F_DIVE)),
        }
    };

    Regions[RR_GRUNTILDAS_LAIR_BEFORE_NOTE_DOOR_260] = RandoRegion{ .regionName = "Before Third Note Door", .levelId = LEVEL_6_LAIR,
        .checks = {
		    // CHECK(RC_GL_JIGGY_LOBBY, true),
		},
        .connections = {
            CONNECTION(RR_GRUNTILDAS_LAIR_BEFORE_NOTE_DOOR_180,             true),
            CONNECTION(RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_BUBBLEGLOOP_SWAMP,  true),
            CONNECTION(RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_GOBIS_VALLEY,       CAN_UNLOCK_NOTE_DOOR(260) && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
        },
        .events = {
            EVENT(RA_NOTE_DOOR_260, CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
        }
    };

    Regions[RR_GRUNTILDAS_LAIR_LOBBY] = RandoRegion{ .regionName = "Lobby", .levelId = LEVEL_6_LAIR,
        .checks = {
		    CHECK(RC_GL_JIGGY_LOBBY,                        true),
            CHECK(RC_GL_JIGGY_WITCH_SWITCH_MUMBOS_MOUNTAIN, CAN_ACCESS(RA_WITCH_SWITCH_MUMBOS_MOUNTAIN)), // TODO: ADD TRANSFORMATION FROM MMM PR
		},
        .connections = {
            CONNECTION(RR_GRUNTILDAS_LAIR_PAST_NOTE_DOOR_50,    CAN_UNLOCK_NOTE_DOOR(50)),
            CONNECTION(RR_MUMBOS_MOUNTAIN,                      CAN_UNLOCK_WORLD(LEVEL_1_MUMBOS_MOUNTAIN)),
        },
        .events = {
            EVENT(RA_NOTE_DOOR_50,                  CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            EVENT(RA_PUZZLE_BOARD_MUMBOS_MOUNTAIN,  true),
        }
    };

    Regions[RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_BUBBLEGLOOP_SWAMP] = RandoRegion{ .regionName = "Near Bubblegloop Swamp Entrance", .levelId = LEVEL_6_LAIR,
        .checks = {
		    // CHECK(RC_GL_JIGGY_LOBBY, true),
		},
        .connections = {
            // CONNECTION(RR_BUBBLEGLOOP_SWAMP, CAN_UNLOCK_WORLD(LEVEL_4_BUBBLEGLOOP_SWAMP)),
            CONNECTION(RR_GRUNTILDAS_LAIR_BEFORE_NOTE_DOOR_260, true),
        },
        .events = {
            EVENT(RA_PUZZLE_BOARD_FREEZEEZY_PEAK, CAN_USE_ABILITY(ABILITY_E_WADING_BOOTS) || CAN_USE_TRANSFORMATION(TRANSFORM_5_CROC)),
        }
    };

    Regions[RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_CLANKERS_CAVERN] = RandoRegion{ .regionName = "Near Clanker's Cavern Entrance", .levelId = LEVEL_6_LAIR,
        .checks = {
		    // CHECK(RC_GL_JIGGY_LOBBY, true),
		},
        .connections = {
            // CONNECTION(RR_CLANKERS_CAVERN, CAN_UNLOCK_WORLD(LEVEL_3_CLANKERS_CAVERN)),
            CONNECTION(RR_GRUNTILDAS_LAIR_BEFORE_NOTE_DOOR_180, true),
        },
        .events = {
            EVENT(RA_PUZZLE_BOARD_BUBBLEGLOOP_SWAMP,            CAN_ACCESS(RA_TRIGGER_SWITCH_GL_BGS_PUZZLE_BOARD_GRATE) && CAN_USE_ABILITY(ABILITY_F_DIVE)),
            EVENT(RA_TRIGGER_SWITCH_GL_ABOVE_WATER_PIPE,        CAN_ACCESS(RA_TRIGGER_SWITCH_GL_UNDERWATER_PIPES) && CAN_EXTEND_JUMP_DISTANCE),
            EVENT(RA_TRIGGER_SWITCH_GL_BGS_PUZZLE_BOARD_GRATE,  CAN_ACCESS(RA_TRIGGER_SWITCH_GL_ABOVE_WATER_PIPE)),
            EVENT(RA_TRIGGER_SWITCH_GL_UNDERWATER_PIPES,        CAN_USE_ABILITY(ABILITY_2_BEAK_BUSTER)),
        }
    };

    Regions[RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_FREEZEEZY_PEAK] = RandoRegion{ .regionName = "Near Freezeezy Peak Entrance", .levelId = LEVEL_6_LAIR,
        .checks = {
		    CHECK(RC_GL_JIGGY_WITCH_SWITCH_FREEZEEZY_PEAK, CAN_ACCESS(RA_WITCH_SWITCH_FREEZEEZY_PEAK) && CAN_EXTEND_JUMP_DISTANCE && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
		},
        .connections = {
            CONNECTION(RR_FREEZEEZY_PEAK_ENTRANCE,                      CAN_UNLOCK_WORLD(LEVEL_5_FREEZEEZY_PEAK)),
            CONNECTION(RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_GOBIS_VALLEY,   CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
        },
        .events = {
            EVENT(RA_CAULDRON_BLUE_1,   CAN_USE_ABILITY(ABILITY_6_EGGS)),
            EVENT(RA_CAULDRON_PURPLE_2, CAN_USE_ABILITY(ABILITY_6_EGGS)),
            EVENT(RA_NOTE_DOOR_350,     true),
            EVENT(RA_NOTE_DOOR_450,     true),
        }
    };

    Regions[RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_GOBIS_VALLEY] = RandoRegion{ .regionName = "Near Gobi's Valley Entrance", .levelId = LEVEL_6_LAIR,
        .checks = {
		    // CHECK(RC_GL_JIGGY_WITCH_SWITCH_TREASURE_TROVE_COVE, CAN_ACCESS(RA_WITCH_SWITCH_TREASURE_TROVE_COVE) && CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
		},
        .connections = {
            CONNECTION(RR_GOBIS_VALLEY_ENTRANCE,                        CAN_UNLOCK_WORLD(LEVEL_7_GOBIS_VALLEY)),
            CONNECTION(RR_GRUNTILDAS_LAIR_BEFORE_NOTE_DOOR_260,         true),
            CONNECTION(RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_FREEZEEZY_PEAK, true),
        },
        .events = {
            EVENT(RA_TRIGGER_SWITCH_GL_SHOCK_PAD, CAN_USE_ABILITY(ABILITY_2_BEAK_BUSTER)),
        }
    };

    Regions[RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_MAD_MONSTER_MANSION] = RandoRegion{ .regionName = "Near Mad Monster Mansion Entrance", .levelId = LEVEL_6_LAIR,
        .connections = {
            CONNECTION(RR_GRUNTILDAS_LAIR_NEAR_PUZZLE_BOARD_GOBIS_VALLEY,   true),
            CONNECTION(RR_GRUNTILDAS_LAIR_WATER_SWITCH_1_ROOM,              true),
            // CONNECTION(RR_MAD_MONSTER_MANSION, CAN_UNLOCK_WORLD(LEVEL_A_MAD_MONSTER_MANSION)),
        },
        .events = {
            EVENT(RA_PUZZLE_BOARD_GOBIS_VALLEY, true),
        }
    };

    Regions[RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_TREASURE_TROVE_COVE] = RandoRegion{ .regionName = "Near Treasure Trove Cove Entrance", .levelId = LEVEL_6_LAIR,
        .checks = {
		    CHECK(RC_GL_JIGGY_WITCH_SWITCH_TREASURE_TROVE_COVE, CAN_ACCESS(RA_WITCH_SWITCH_TREASURE_TROVE_COVE) && CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
		},
        .connections = {
            CONNECTION(RR_TREASURE_TROVE_COVE,                  CAN_UNLOCK_WORLD(LEVEL_2_TREASURE_TROVE_COVE)),
            CONNECTION(RR_GRUNTILDAS_LAIR_BEFORE_NOTE_DOOR_180, CAN_USE_ABILITY(ABILITY_5_CLIMB)),
        },
    };

    Regions[RR_GRUNTILDAS_LAIR_NEAR_ORANGE_CAULDRON_1] = RandoRegion{ .regionName = "Near Orange Warp Cauldron 1", .levelId = LEVEL_6_LAIR,
        .checks = {
		    // CHECK(RC_GL_JIGGY_LOBBY, true),
		},
        .connections = {
            CONNECTION(RR_GRUNTILDAS_LAIR_BEFORE_NOTE_DOOR_180, true),
        },
        .events = {
            EVENT(RA_CAULDRON_ORANGE_1, true),
        }
    };

    Regions[RR_GRUNTILDAS_LAIR_NEAR_PUZZLE_BOARD_GOBIS_VALLEY] = RandoRegion{ .regionName = "Near Gobi's Valley Puzzle Board", .levelId = LEVEL_6_LAIR,
        .connections = {
            CONNECTION(RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_FREEZEEZY_PEAK,         true),
            CONNECTION(RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_MAD_MONSTER_MANSION,    true),
        },
        .events = {
            EVENT(RA_PUZZLE_BOARD_GOBIS_VALLEY, true),
        }
    };

    Regions[RR_GRUNTILDAS_LAIR_PAST_NOTE_DOOR_50] = RandoRegion{ .regionName = "Past First Note Door", .levelId = LEVEL_6_LAIR,
        .checks = {
		    CHECK(RC_GL_JIGGY_WITCH_SWITCH_CLANKERS_CAVERN, CAN_ACCESS(RA_WITCH_SWITCH_CLANKERS_CAVERN)),
		},
        .connections = {
            CONNECTION(RR_GRUNTILDAS_LAIR_BEFORE_NOTE_DOOR_180, true),
            CONNECTION(RR_GRUNTILDAS_LAIR_LOBBY,                true),
        },
        .events = {
            EVENT(RA_CAULDRON_PURPLE_1,                 true),
            EVENT(RA_PUZZLE_BOARD_CLANKERS_CAVERN,      CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
            EVENT(RA_PUZZLE_BOARD_TREASURE_TROVE_COVE,  true),
        }
    };

    Regions[RR_GRUNTILDAS_LAIR_WATER_SWITCH_1_ROOM] = RandoRegion{ .regionName = "First Water Switch Room", .levelId = LEVEL_6_LAIR,
        .connections = {
            CONNECTION(RR_GRUNTILDAS_LAIR_NEAR_ENTRANCE_MAD_MONSTER_MANSION, true),
        },
        .events = {
            EVENT(RA_TRIGGER_SWITCH_GL_WATER_LEVEL_1, CAN_USE_ABILITY(ABILITY_2_BEAK_BUSTER)),
        }
    };
	
}, {});
// clang-format on