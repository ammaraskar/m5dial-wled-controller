#ifndef SETUP_MENU_SETUP_MENU_H_
#define SETUP_MENU_SETUP_MENU_H_

#include <M5GFX.h>
#include "../menu.hpp"


class SetupMenu : public Menu {
public:
    explicit SetupMenu(M5GFX& gfx) : _gfx(gfx)
    {
    }

    virtual void initialDraw();

private:
    M5GFX& _gfx;
};

#endif  // SETUP_MENU_SETUP_MENU_H_