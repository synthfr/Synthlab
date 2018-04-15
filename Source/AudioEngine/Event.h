//
//  Event.hpp
//  Synthlab - App
//
//  Created by Matthias Pueski on 05.04.18.
//

#ifndef Event_hpp
#define Event_hpp

#include "../JuceLibraryCode/JuceHeader.h"

class Event {
  
public:
    
    enum Type {
        GATE,
        NOTE,
        CONTROLLER
    };
    
    Event();
    Event(Event* e);
    Event(String name, Type type);
    ~Event();
    
  String getName();
    Type getType();
    int getValue();
    void setValue(int value);
    int getNote();
    void setNote(int note);
    int getNumber();
    void setNumber(int num);
private:
    
    Type type;
    String name;
    int number = 1;
    int value = 0;
    int note = 0;
};


#endif /* Event_hpp */
