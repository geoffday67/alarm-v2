#ifndef __EVENT__
#define __EVENT__

#define KEY_0       0x00
#define KEY_1       0x01
#define KEY_2       0x02
#define KEY_3       0x03
#define KEY_4       0x04
#define KEY_5       0x05
#define KEY_6       0x06
#define KEY_7       0x07
#define KEY_8       0x08
#define KEY_9       0x09
#define KEY_STAR    0x0A
#define KEY_HASH    0x0C

class Event {
protected:
    Event(int type);

public:
    int type;
};

#endif