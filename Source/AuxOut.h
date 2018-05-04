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
class AuxOut    : public Module
{
public:
    AuxOut();
    ~AuxOut();
    
    void paint (juce::Graphics& g) override;
    void setDeviceManager(juce::AudioDeviceManager* manager);
    virtual void process() override;
    
    virtual void configurePins() override;
    
    virtual juce::String getCategory() override {
        return "Input / Output";
    }
    // this is needed to associate the channel with the mixer
    int getChannelIndex();
    void setChannelIndex(int index);
    
private:
    juce::AudioDeviceManager* deviceManager;
    int channelIndex;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuxOut)
};
