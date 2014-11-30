// Contains config class declaration
#ifndef CURVE_T_MESSAGE
#define CURVE_T_MESSAGE
// Needed Headers
#include "..\curve.h"
//#include <iostream>
//#include <vector>
//
class T_Message{
public:
    enum class Type : int {Server,Client,Logic} type;
    // Data
    union{
        bool b;
        int i;
        float f;
        sf::String s;
    };
}
#endif T_MESSAGE
