#ifndef SETUP_MENU_SETUP_MENU_H_
#define SETUP_MENU_SETUP_MENU_H_

#include <M5GFX.h>


class SetupMenu {
public:
    SetupMenu(M5GFX& gfx) : _gfx(gfx)
    {
        init();
    }

private:
    void init();

    M5GFX& _gfx;
};

#endif  // SETUP_MENU_SETUP_MENU_H_