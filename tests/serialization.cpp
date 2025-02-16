#include <iostream>

#include "formats/CharacterDef.h"

int main() {
    // Just testing for now...
    // TODO: Find a better way to test this
    CharacterDef characterDef;
    CharacterDef::Read(characterDef, "C:\\Users\\User\\Downloads\\CharacterDef.xml");
    return 0;
}
