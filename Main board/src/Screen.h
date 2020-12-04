#ifndef __SCREEN__
#define __SCREEN__

#include "Output.h"

class Screen {
protected:
    Screen ();
    virtual ~Screen();

    void showWaiting();

public:
    virtual void activate() = 0;
    virtual void deactivate() = 0;
};

#endif