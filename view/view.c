#include <stdio.h>
#include "view.h"

int main() {
    Interface Intf = *createInterface(MENU_ST);
    RunProcess(&Intf);
}