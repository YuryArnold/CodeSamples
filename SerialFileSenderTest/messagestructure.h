#ifndef MESSAGESTRUCTURE_H
#define MESSAGESTRUCTURE_H
#include <QMetaType>

struct message{
    message(){
        x= y = z = w = 0.0;
    }
    float x;
    float y;
    float z;
    float w;
};
Q_DECLARE_METATYPE(message)

#endif // MESSAGESTRUCTURE_H
