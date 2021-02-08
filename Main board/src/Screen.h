#ifndef __SCREEN__
#define __SCREEN__

#include "Output.h"

class Screen {
protected:
    Screen ();
    virtual ~Screen();

    bool active;
    void showWaiting();
    virtual void onActivate();
    virtual void onDeactivate();

public:
    virtual void activate();
    virtual void deactivate();
};

#endif