#ifndef __STATE__
#define __STATE__

#define STATE_IDLE_LIGHT        1
#define STATE_ALARM_ON          2
#define STATE_SNOOZE            3
#define STATE_IDLE_DARK         4

class classState {
public:
    classState();
    void loop();

private:
    int state;
};

extern classState State;

#endif