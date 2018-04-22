/*
  ==============================================================================

    Sampler.h
    Created: 17 Apr 2018 1:12:15pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
class Sampler  {
 
    
public:
    
    Sampler(float sampleRate, int bufferSize);
    ~Sampler();

    void play();
    void stop();
    float getSampleAt(int channel, long position);
    void loadSample(File file);
    void loadSample(InputStream* input);
    void setStartPosition(long start);
    long getStartPosition();
    void setEndPosition(long end);
    long getEndPosition();
    void setSampleLength(long length);
    long getSampleLength();
    void setLoop(bool loop);
    bool isLoop();
    void setVolume(float volume);
    float getVolume() {
        return volume;
    }
    AudioSampleBuffer* getSampleBuffer();
    bool hasSample();
    void nextSample();
    float getCurrentSample(int channel);
    long getCurrentPosition() {
        return currentSample;
    }
    void reset(){
        currentSample = startPosition;
    }
    
    bool isDone() {
        return !loop && currentSample >= sampleLength - 1;
    }
    
    void setLoaded(bool loaded) {
        this->loaded = loaded;
    }
    
    float getPitch();
    void setPitch(float pitch);
    
private:
    
    AudioFormatManager* manager;
    
    float sampleRate;
    int bufferSize;
    
    float volume = 0.5;
    
    AudioSampleBuffer *sampleBuffer = nullptr;
    
    long currentSample = 0;
    
    long startPosition = 0;
    long endPosition = 0;
    long sampleLength = 0;
    
    bool loop = false;
    bool loaded = false;
    
    float pitch = 1;
    
    float* tempBufferLeft = nullptr;
    float* tempBufferRight = nullptr;
    
    CatmullRomInterpolator* interpolatorLeft;
    CatmullRomInterpolator* interpolatorRight;
};
