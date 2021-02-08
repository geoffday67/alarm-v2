#ifndef __TUNE__
#define __TUNE__

#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"

class classTune: public Screen, EventReceiver {
public:
    virtual void activate();
    virtual void deactivate();
    virtual void onEvent(Event* pevent);

private:
    void handleKeyEvent(KeyEvent *pevent);
};

extern classTune Tune;

#endif