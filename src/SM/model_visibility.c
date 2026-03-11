#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* .code */
void func_80388CF0(void){
    if(getGameMode() != GAME_MODE_7_ATTRACT_DEMO){
        func_8034DEB4(&func_8034C528(0x1F2)->type_6D, 0.0f); // [port] Struct70s union → Struct6Ds member
        func_8034DEB4(&func_8034C528(0x1F3)->type_6D, -5000.0f); // [port] Struct70s union → Struct6Ds member
    }
}

void func_80388D48(void){
    // Lighthouse TODO is this an anti-piracy check?
    #if 0
    if(*(u32*)PHYS_TO_K1(0x200) - PHYS_TO_K1(0xC290000)){
        func_80388CF0();
    }
#endif
}
