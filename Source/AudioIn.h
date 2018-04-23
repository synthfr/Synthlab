/*
 ==============================================================================
 
 MidiGate.h
 Created: 4 Apr 2018 3:48:23pm
 Author:  Matthias Pueski
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Module.h"

//==============================================================================
/*
 */
class AudioIn    : public Module
{
public:
    AudioIn();
    ~AudioIn();
    
    void paint (Graphics& g) override;
    void setDeviceManager(AudioDeviceManager* manager);
    virtual void process() override;
    
    virtual void configurePins() override;
    
    virtual String getCategory() override {
        return "Input / Output";
    }
    
private:
    AudioDeviceManager* deviceManager;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioIn)
};
