#ifndef MENU_H_
#define MENU_H_

// An abstract class that represents a single menu being rendered to the screen.
// Has some basic life-cycle stuff to perform an initial render and redraw and
// receive input/output events.
class Menu {
public:
    virtual void initialDraw();
};

#endif  // MENU_H_
