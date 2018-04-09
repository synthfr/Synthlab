/*
  ==============================================================================

    Constant
    Created: 4 Apr 2018 3:48:23pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Constant.h"


//==============================================================================
Constant::Constant()
{
   
    setSize(120,30);
    nameLabel->setJustificationType (Justification::left);
    nameLabel->setTopLeftPosition(2,2);
    addPin(Pin::Direction::OUT);
    pins.at(0)->setName("");
    editable = false;
}

Constant::~Constant()
{
 
}

void Constant::paint(juce::Graphics &g) {
    Module::paint(g);
}

float Constant::getValue() {
    return value;
}

void Constant::setValue(float value) {
    this->value = value;
    this->pins.at(0)->setValue(value);
}



