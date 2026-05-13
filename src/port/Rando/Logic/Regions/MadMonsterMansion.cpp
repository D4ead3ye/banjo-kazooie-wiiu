#include "src/port/ShipInit.hpp"
#include "src/port/Rando/Logic/Logic.h"

using namespace Rando::Logic;

// clang-format off
static RegisterShipInitFunc initFunc([]() {
    Regions[RR_MAD_MONSTER_MANSION_ENTRANCE] = RandoRegion{ .regionName = "Mad Monster Mansion", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_JIGGY_JINJO_JIGGY,                           	true),
            CHECK(RC_MMM_JINJO_BLUE,                                  	CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
            CHECK(RC_MMM_JINJO_GREEN,                                 	CAN_USE_ABILITY(ABILITY_5_CLIMB) && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
            CHECK(RC_MMM_JINJO_ORANGE,                                	true),
            CHECK(RC_MMM_MUMBO_TOKEN_HIDDEN_NEAR_FOUNTAIN,            	true),
            CHECK(RC_MMM_MUMBO_TOKEN_IN_THE_MAZE,                     	true),
            CHECK(RC_MMM_MUMBO_TOKEN_INSIDE_WHIPCRACK_IN_FOUNTAIN,    	CAN_USE_ABILITY(ABILITY_6_EGGS)),
            CHECK(RC_MMM_MUMBO_TOKEN_NEAR_FRONT_TUMBLARS_SHED,        	true),
            CHECK(RC_MMM_MUMBO_TOKEN_NEXT_TO_MAZE_HEDGE_RAMP,         	CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_MUMBO_TOKEN_TOP_OF_TUMBLARS_SHED,            	CAN_USE_ABILITY(ABILITY_10_TALON_TROT) && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
            CHECK(RC_MMM_NOTE_AROUND_THE_WELL_EAST_1,	              	CAN_USE_ABILITY(ABILITY_7_FEATHERY_FLAP)),
            CHECK(RC_MMM_NOTE_AROUND_THE_WELL_EAST_2,                 	CAN_USE_ABILITY(ABILITY_7_FEATHERY_FLAP)),
            CHECK(RC_MMM_NOTE_AROUND_THE_WELL_WEST_1,                 	CAN_USE_ABILITY(ABILITY_7_FEATHERY_FLAP)),
            CHECK(RC_MMM_NOTE_AROUND_THE_WELL_WEST_2,                 	CAN_USE_ABILITY(ABILITY_7_FEATHERY_FLAP)),
            CHECK(RC_MMM_NOTE_GRASS_BEHIND_THE_MANSION_1,             	true),
            CHECK(RC_MMM_NOTE_GRASS_BEHIND_THE_MANSION_2,             	true),
            CHECK(RC_MMM_NOTE_GRASS_BEHIND_THE_MANSION_3,             	true),
            CHECK(RC_MMM_NOTE_MAIN_ENTRANCE_1,                        	true),
            CHECK(RC_MMM_NOTE_MAIN_ENTRANCE_2,                        	true),
            CHECK(RC_MMM_NOTE_MAIN_ENTRANCE_3,                        	true),
            CHECK(RC_MMM_NOTE_MAIN_ENTRANCE_4,                        	true),
            CHECK(RC_MMM_NOTE_MANSION_EXTERIOR_LOWER_NORTHEAST_CORNER,	CAN_USE_ABILITY(ABILITY_5_CLIMB) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_NOTE_MANSION_EXTERIOR_LOWER_NORTHWEST_CORNER,	CAN_USE_ABILITY(ABILITY_5_CLIMB) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_NOTE_MANSION_EXTERIOR_LOWER_SOUTHEAST_CORNER,	CAN_USE_ABILITY(ABILITY_5_CLIMB) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_NOTE_MANSION_EXTERIOR_LOWER_SOUTHWEST_CORNER,	CAN_USE_ABILITY(ABILITY_5_CLIMB) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_NOTE_MANSION_EXTERIOR_UPPER_NORTHEAST_CORNER,	CAN_USE_ABILITY(ABILITY_5_CLIMB)),
            CHECK(RC_MMM_NOTE_MANSION_EXTERIOR_UPPER_NORTHWEST_CORNER,	CAN_USE_ABILITY(ABILITY_5_CLIMB)),
            CHECK(RC_MMM_NOTE_MANSION_EXTERIOR_UPPER_SOUTHEAST_CORNER,	CAN_USE_ABILITY(ABILITY_5_CLIMB)),
            CHECK(RC_MMM_NOTE_MANSION_EXTERIOR_UPPER_SOUTHWEST_CORNER,	CAN_USE_ABILITY(ABILITY_5_CLIMB)),
            CHECK(RC_MMM_NOTE_MAZE_1,                                 	true),
            CHECK(RC_MMM_NOTE_MAZE_2,                                 	true),
            CHECK(RC_MMM_NOTE_MAZE_3,                                 	true),
            CHECK(RC_MMM_NOTE_MAZE_4,                                 	true),
            CHECK(RC_MMM_NOTE_MAZE_5,                                 	true),
            CHECK(RC_MMM_NOTE_MAZE_6,                                 	true),
            CHECK(RC_MMM_NOTE_SHED_ROOF_1,                            	CAN_USE_ABILITY(ABILITY_10_TALON_TROT) && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
            CHECK(RC_MMM_NOTE_SHED_ROOF_2,                            	CAN_USE_ABILITY(ABILITY_10_TALON_TROT) && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
            CHECK(RC_MMM_NOTE_SHED_ROOF_3,                            	CAN_USE_ABILITY(ABILITY_10_TALON_TROT) && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
            CHECK(RC_MMM_NOTE_SHED_ROOF_4,                            	CAN_USE_ABILITY(ABILITY_10_TALON_TROT) && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
            CHECK(RC_MMM_NOTE_STAGNANT_FOUNTAIN_1,                    	true),
            CHECK(RC_MMM_NOTE_STAGNANT_FOUNTAIN_2,                    	true),
            CHECK(RC_MMM_NOTE_STAGNANT_FOUNTAIN_3,                    	true),
            CHECK(RC_MMM_NOTE_STAGNANT_FOUNTAIN_4,                    	true),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_BARREL, CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CONNECTION(RR_MAD_MONSTER_MANSION_BATHROOM, CAN_BREAK_OBJECT(RA_BREAK_OBJECT_WINDOWS) && (CAN_USE_ABILITY(ABILITY_5_CLIMB) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN))),
            CONNECTION(RR_MAD_MONSTER_MANSION_BEDROOM, CAN_BREAK_OBJECT(RA_BREAK_OBJECT_WINDOWS) && CAN_USE_ABILITY(ABILITY_5_CLIMB) && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
            CONNECTION(RR_MAD_MONSTER_MANSION_CELLAR, CAN_BREAK_OBJECT(RA_BREAK_OBJECT_WOODEN_DOOR)),
            CONNECTION(RR_MAD_MONSTER_MANSION_CHURCH_EXTERIOR, CAN_BREAK_OBJECT(RA_BREAK_OBJECT_IRON_GATE)),
            CONNECTION(RR_MAD_MONSTER_MANSION_DINING_ROOM, CAN_BREAK_OBJECT(RA_BREAK_OBJECT_WOODEN_DOOR) || (CAN_USE_ABILITY(ABILITY_5_CLIMB) && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP))),
            CONNECTION(RR_MAD_MONSTER_MANSION_MUMBOS_SKULL, CAN_BREAK_OBJECT(RA_BREAK_OBJECT_IRON_GATE) && CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
            CONNECTION(RR_MAD_MONSTER_MANSION_TUMBLARS_SHED, CAN_BREAK_OBJECT(RA_BREAK_OBJECT_WOODEN_DOOR)),
            CONNECTION(RR_MAD_MONSTER_MANSION_WELL, true),
            CONNECTION(RR_MAD_MONSTER_MANSION_WINDOW_BOTTOM_NORTH, CAN_BREAK_OBJECT(RA_BREAK_OBJECT_WINDOWS)),
            CONNECTION(RR_MAD_MONSTER_MANSION_WINDOW_BOTTOM_SOUTH, CAN_BREAK_OBJECT(RA_BREAK_OBJECT_WINDOWS)),
            CONNECTION(RR_MAD_MONSTER_MANSION_WINDOW_MIDDLE_WEST, CAN_BREAK_OBJECT(RA_BREAK_OBJECT_WINDOWS) && (CAN_USE_ABILITY(ABILITY_5_CLIMB) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN))),
            CONNECTION(RR_MAD_MONSTER_MANSION_WINDOW_TOP_ATTIC, CAN_BREAK_OBJECT(RA_BREAK_OBJECT_WINDOWS) && CAN_USE_ABILITY(ABILITY_5_CLIMB) && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_BARREL] = RandoRegion{ .regionName = "Mad Monster Mansion Barrel", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_JIGGY_BARREL_INTERIOR,                       	true),
            CHECK(RC_MMM_NOTE_BARREL_INTERIOR_1,                      	true),
            CHECK(RC_MMM_NOTE_BARREL_INTERIOR_2,                      	true),
            CHECK(RC_MMM_NOTE_BARREL_INTERIOR_3,                      	true),
            CHECK(RC_MMM_NOTE_BARREL_INTERIOR_4,                      	true),
            CHECK(RC_MMM_NOTE_BARREL_INTERIOR_5,                      	true),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, true),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_BATHROOM] = RandoRegion{ .regionName = "Mad Monster Mansion Bathroom", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_MUMBO_TOKEN_IN_LOGGOS_ROOM,                  	true),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, true),
            CONNECTION(RR_MAD_MONSTER_MANSION_LOGGO_INTERIOR, CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_BEDROOM] = RandoRegion{ .regionName = "Mad Monster Mansion Bedroom", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_JINJO_YELLOW,                                	CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
            CHECK(RC_MMM_MUMBO_TOKEN_INSIDE_BEDROOM,                  	true),
            CHECK(RC_MMM_NOTE_BEDROOM_1,                              	true),
            CHECK(RC_MMM_NOTE_BEDROOM_2,                              	true),
            CHECK(RC_MMM_NOTE_BEDROOM_3,                              	true),
            CHECK(RC_MMM_NOTE_BEDROOM_4,                              	true),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, true),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_CELLAR] = RandoRegion{ .regionName = "Mad Monster Mansion Cellar", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_JIGGY_CELLAR,                                	CAN_BREAK_OBJECT(RA_BREAK_OBJECT_CELLAR_CASK)),
            CHECK(RC_MMM_JINJO_PINK,                                  	CAN_BREAK_OBJECT(RA_BREAK_OBJECT_CELLAR_CASK)),
            CHECK(RC_MMM_MUMBO_TOKEN_INSIDE_BARREL_IN_CELLAR,         	CAN_BREAK_OBJECT(RA_BREAK_OBJECT_CELLAR_CASK)),
            CHECK(RC_MMM_NOTE_BASEMENT_1,                             	CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
            CHECK(RC_MMM_NOTE_BASEMENT_2,                             	CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
            CHECK(RC_MMM_NOTE_BASEMENT_3,                             	CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
            CHECK(RC_MMM_NOTE_BASEMENT_4,                             	CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, true),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_CHURCH_CLOCK_TOWER] = RandoRegion{ .regionName = "Mad Monster Mansion Clock Tower", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_JIGGY_CLOCKTOWER_TOP,                        	CAN_USE_ABILITY(ABILITY_5_CLIMB) && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
            CHECK(RC_MMM_NOTE_CLOCKTOWER_ROOF_1,                      	true),
            CHECK(RC_MMM_NOTE_CLOCKTOWER_ROOF_2,                      	true),
            CHECK(RC_MMM_NOTE_CLOCKTOWER_ROOF_3,                      	true),
            CHECK(RC_MMM_NOTE_CLOCKTOWER_ROOF_4,                      	true),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_CHURCH_EXTERIOR, true),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_CHURCH_EXTERIOR] = RandoRegion{ .regionName = "Mad Monster Mansion Church Exterior", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_JIGGY_CEMETARY_POTS,                         	CAN_USE_ABILITY(ABILITY_6_EGGS)),
            CHECK(RC_MMM_MUMBO_TOKEN_BEHIND_CHURCH,                   	true),
            CHECK(RC_MMM_MUMBO_TOKEN_ALCOVE_CLOCK_TOWER,              	CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            CHECK(RC_MMM_NOTE_CHURCH_ABOVE_THE_ENTRANCE,              	CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            CHECK(RC_MMM_NOTE_CHURCH_ROOFTOP_1,                       	CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            CHECK(RC_MMM_NOTE_CHURCH_ROOFTOP_2,                       	CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            CHECK(RC_MMM_NOTE_CHURCH_ROOFTOP_3,                       	CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            CHECK(RC_MMM_NOTE_CHURCH_ROOFTOP_4,                       	CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            CHECK(RC_MMM_NOTE_CHURCH_ROOFTOP_5,                       	CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            CHECK(RC_MMM_NOTE_CHURCH_ROOFTOP_6,                       	CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            CHECK(RC_MMM_NOTE_CHURCH_ROOFTOP_7,                       	CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            CHECK(RC_MMM_NOTE_CHURCH_ROOFTOP_8,                       	CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            CHECK(RC_MMM_NOTE_CHURCH_ROOFTOP_9,                       	CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, CAN_BREAK_OBJECT(RA_BREAK_OBJECT_IRON_GATE)),
            CONNECTION(RR_MAD_MONSTER_MANSION_CHURCH_INTERIOR, CAN_USE_ABILITY(ABILITY_2_BEAK_BUSTER) && CAN_USE_ABILITY(ABILITY_10_TALON_TROT) && CAN_USE_ABILITY(ABILITY_11_TURBO_TALON)),
            CONNECTION(RR_MAD_MONSTER_MANSION_CHURCH_CLOCK_TOWER, CAN_USE_ABILITY(ABILITY_10_TALON_TROT)),
            CONNECTION(RR_MAD_MONSTER_MANSION_CHURCH_WINDOW, true),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_CHURCH_INTERIOR] = RandoRegion{ .regionName = "Mad Monster Mansion Church Interior", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_MUMBO_TOKEN_TOP_OF_STOOL_INSIDE_CHURCH,      	CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
            CHECK(RC_MMM_NOTE_ORGAN_PEDALS_1,                         	true),
            CHECK(RC_MMM_NOTE_ORGAN_PEDALS_2,                         	true),
            CHECK(RC_MMM_NOTE_CHURCH_PEW_NORTHEAST,                   	CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
            CHECK(RC_MMM_NOTE_CHURCH_PEW_NORTHWEST,                   	CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
            CHECK(RC_MMM_NOTE_CHURCH_PEW_SOUTHEAST,                   	CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
            CHECK(RC_MMM_NOTE_CHURCH_PEW_SOUTHWEST,                   	CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP)),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_CHURCH_EXTERIOR, true),
            CONNECTION(RR_MAD_MONSTER_MANSION_CHURCH_INTERIOR_UPPER_ORGAN, CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP) && CAN_USE_ABILITY(ABILITY_10_TALON_TROT) && CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP))
        },
    };
        Regions[RR_MAD_MONSTER_MANSION_CHURCH_INTERIOR_UPPER_ORGAN] = RandoRegion{ .regionName = "Mad Monster Mansion Organ", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_JIGGY_MOTZAND,                               	CAN_USE_ABILITY(ABILITY_2_BEAK_BUSTER)),
            CHECK(RC_MMM_NOTE_ORGAN_PIPES_1,                          	true),
            CHECK(RC_MMM_NOTE_ORGAN_PIPES_2,                          	true),
            CHECK(RC_MMM_NOTE_ORGAN_PIPES_3,                          	true),
            CHECK(RC_MMM_NOTE_ORGAN_PIPES_4,                          	true),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_CHURCH_INTERIOR, true),
            CONNECTION(RR_MAD_MONSTER_MANSION_CHURCH_RAFTERS, CAN_USE_ABILITY(ABILITY_9_FLIGHT)),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_CHURCH_RAFTERS] = RandoRegion{ .regionName = "Mad Monster Mansion Church Rafters", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_EMPTY_HONEYCOMB_CHURCH_RAFTERS,              	true),
            CHECK(RC_MMM_MUMBO_TOKEN_TOP_OF_RAFTER_INSIDE_CHURCH,     	true),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_CHURCH_INTERIOR, true),
            CONNECTION(RR_MAD_MONSTER_MANSION_CHURCH_INTERIOR_UPPER_ORGAN, true),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_CHURCH_WINDOW] = RandoRegion{ .regionName = "Mad Monster Mansion Church Window", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_CHURCH_EXTERIOR, true),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_DINING_ROOM] = RandoRegion{ .regionName = "Mad Monster Mansion Dining Room", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_JIGGY_MANSION_TABLE,                         	CAN_USE_ABILITY(ABILITY_D_SHOCK_JUMP)),
            CHECK(RC_MMM_MUMBO_TOKEN_IN_CHIMNEY_INSIDE_DINING_ROOM,   	CAN_USE_ABILITY(ABILITY_12_WONDERWING)),
            CHECK(RC_MMM_NOTE_CHAIR_WEST_SIDE_1,                      	true),
            CHECK(RC_MMM_NOTE_CHAIR_WEST_SIDE_2,                      	true),
            CHECK(RC_MMM_NOTE_CHAIR_WEST_SIDE_3,                      	true),
            CHECK(RC_MMM_NOTE_CHAIR_NORTH_SIDE_1,                     	true),
            CHECK(RC_MMM_NOTE_CHAIR_NORTH_SIDE_2,                     	true),
            CHECK(RC_MMM_NOTE_CHAIR_EAST_SIDE_1,                      	true),
            CHECK(RC_MMM_NOTE_CHAIR_EAST_SIDE_2,                      	true),
            CHECK(RC_MMM_NOTE_CHAIR_EAST_SIDE_3,                      	true),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, true),
        },
    };
        Regions[RR_MAD_MONSTER_MANSION_LOGGO_INTERIOR] = RandoRegion{ .regionName = "Mad Monster Mansion Inside Loggo", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_JIGGY_INSIDE_LOGGO,                          	true),
            CHECK(RC_MMM_MUMBO_TOKEN_INSIDE_LOGGO,                    	true),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_BATHROOM, true),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_MUMBOS_SKULL] = RandoRegion{ .regionName = "Mad Monster Mansion Mumbos Skull", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_NOTE_MUMBOS_SKULL_INTERIOR_1,                	true),
            CHECK(RC_MMM_NOTE_MUMBOS_SKULL_INTERIOR_2,                	true),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, CAN_USE_ABILITY(ABILITY_8_FLAP_FLIP))
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_TUMBLARS_SHED] = RandoRegion{ .regionName = "Mad Monster Mansion Tumblar's Shed", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_JIGGY_TUMBLARS_PUZZLE,                       	true),
            CHECK(RC_MMM_NOTE_SHED_INTERIOR_1,                        	true),
            CHECK(RC_MMM_NOTE_SHED_INTERIOR_2,                        	true),
            CHECK(RC_MMM_NOTE_SHED_INTERIOR_3,                        	true),
            CHECK(RC_MMM_NOTE_SHED_INTERIOR_4,                        	true),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, true),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_WELL] = RandoRegion{ .regionName = "Mad Monster Mansion Well", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_JIGGY_INSIDE_THE_WELLS_BUCKET,               	CAN_USE_ABILITY(ABILITY_F_DIVE) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_MUMBO_TOKEN_INSIDE_WELL,                     	CAN_USE_ABILITY(ABILITY_F_DIVE) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_NOTE_WELL_INTERIOR_1,                        	CAN_USE_ABILITY(ABILITY_F_DIVE) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_NOTE_WELL_INTERIOR_2,                        	CAN_USE_ABILITY(ABILITY_F_DIVE) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_NOTE_WELL_INTERIOR_3,                        	CAN_USE_ABILITY(ABILITY_F_DIVE) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_NOTE_WELL_INTERIOR_4,                        	CAN_USE_ABILITY(ABILITY_F_DIVE) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_NOTE_WELL_INTERIOR_5,                        	CAN_USE_ABILITY(ABILITY_F_DIVE) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_NOTE_WELL_INTERIOR_6,                        	CAN_USE_ABILITY(ABILITY_F_DIVE) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
            CHECK(RC_MMM_NOTE_WELL_INTERIOR_7,                        	CAN_USE_ABILITY(ABILITY_F_DIVE) || CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, CAN_USE_ABILITY(ABILITY_5_CLIMB) || GET_CURRENT_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_WINDOW_BOTTOM_NORTH] = RandoRegion{ .regionName = "Mad Monster Mansion Bottom North Window", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, true),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_WINDOW_BOTTOM_SOUTH] = RandoRegion{ .regionName = "Mad Monster Mansion Bottom South Window", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, true),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_WINDOW_MIDDLE_WEST] = RandoRegion{ .regionName = "Mad Monster Mansion Middle West Window", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_EMPTY_HONEYCOMB_BENEATH_THE_FLOORBOARDS,     	CAN_USE_TRANSFORMATION(TRANSFORM_3_PUMPKIN)),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, true),
        },
    };
    Regions[RR_MAD_MONSTER_MANSION_WINDOW_TOP_ATTIC] = RandoRegion{ .regionName = "Mad Monster Mansion Attic", .levelId = LEVEL_A_MAD_MONSTER_MANSION,
        .checks = {
            CHECK(RC_MMM_NOTE_FRONT_WINDOW_ATTIC_1,                   	true),
            CHECK(RC_MMM_NOTE_FRONT_WINDOW_ATTIC_2,                   	true),
            CHECK(RC_MMM_NOTE_FRONT_WINDOW_ATTIC_3,                   	true),
            CHECK(RC_MMM_NOTE_FRONT_WINDOW_ATTIC_4,                   	true),
            CHECK(RC_MMM_NOTE_FRONT_WINDOW_ATTIC_5,                   	true),
            CHECK(RC_MMM_NOTE_FRONT_WINDOW_ATTIC_6,                   	true),
            CHECK(RC_MMM_NOTE_FRONT_WINDOW_ATTIC_7,                   	true),
            CHECK(RC_MMM_NOTE_FRONT_WINDOW_ATTIC_8,                   	true),
            CHECK(RC_MMM_NOTE_FRONT_WINDOW_ATTIC_9,                   	true),
        },
        .connections = {
            CONNECTION(RR_MAD_MONSTER_MANSION_ENTRANCE, true),
        },
    };

}, {});
// clang-format on