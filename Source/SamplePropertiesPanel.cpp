/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "SamplePropertiesPanel.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SamplePropertiesPanel::SamplePropertiesPanel ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (loadSampleButton = new TextButton ("loadSampleButton"));
    loadSampleButton->setButtonText (TRANS("Load sample"));
    loadSampleButton->addListener (this);

    loadSampleButton->setBounds (8, 8, 150, 24);

    addAndMakeVisible (sampleStartSlider = new Slider ("sampleStartSlider"));
    sampleStartSlider->setRange (0, 10, 1);
    sampleStartSlider->setSliderStyle (Slider::LinearHorizontal);
    sampleStartSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sampleStartSlider->addListener (this);

    sampleStartSlider->setBounds (8, 72, 280, 24);

    addAndMakeVisible (startLabel = new Label ("startLabel",
                                               TRANS("Sample start\n")));
    startLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    startLabel->setJustificationType (Justification::centredLeft);
    startLabel->setEditable (false, false, false);
    startLabel->setColour (TextEditor::textColourId, Colours::black);
    startLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    startLabel->setBounds (8, 40, 150, 24);

    addAndMakeVisible (endLabel = new Label ("endLabel",
                                             TRANS("Sample end\n")));
    endLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    endLabel->setJustificationType (Justification::centredLeft);
    endLabel->setEditable (false, false, false);
    endLabel->setColour (TextEditor::textColourId, Colours::black);
    endLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    endLabel->setBounds (496, 40, 150, 24);

    addAndMakeVisible (sampleEndSlider = new Slider ("sampleEndSlider"));
    sampleEndSlider->setRange (0, 10, 1);
    sampleEndSlider->setSliderStyle (Slider::LinearHorizontal);
    sampleEndSlider->setTextBoxStyle (Slider::TextBoxRight, false, 80, 20);
    sampleEndSlider->addListener (this);

    sampleEndSlider->setBounds (296, 72, 280, 24);

    addAndMakeVisible (loopToggleButton = new ToggleButton ("loopToggleButton"));
    loopToggleButton->setButtonText (TRANS("loopSample"));
    loopToggleButton->addListener (this);

    loopToggleButton->setBounds (168, 128, 104, 24);

    addAndMakeVisible (pitchSlider = new Slider ("pitchSlider"));
    pitchSlider->setRange (0.25, 2, 0.025);
    pitchSlider->setSliderStyle (Slider::LinearHorizontal);
    pitchSlider->setTextBoxStyle (Slider::TextBoxRight, false, 80, 20);
    pitchSlider->addListener (this);

    pitchSlider->setBounds (296, 128, 280, 24);

    addAndMakeVisible (pitchLabel = new Label ("pitchLabel",
                                               TRANS("Pitch\n")));
    pitchLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    pitchLabel->setJustificationType (Justification::centredLeft);
    pitchLabel->setEditable (false, false, false);
    pitchLabel->setColour (TextEditor::textColourId, Colours::black);
    pitchLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    pitchLabel->setBounds (496, 104, 56, 24);

    addAndMakeVisible (recordButton = new TextButton ("recordButton"));
    recordButton->setButtonText (TRANS("Record sample"));
    recordButton->addListener (this);

    recordButton->setBounds (176, 8, 150, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SamplePropertiesPanel::~SamplePropertiesPanel()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    loadSampleButton = nullptr;
    sampleStartSlider = nullptr;
    startLabel = nullptr;
    endLabel = nullptr;
    sampleEndSlider = nullptr;
    loopToggleButton = nullptr;
    pitchSlider = nullptr;
    pitchLabel = nullptr;
    recordButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SamplePropertiesPanel::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SamplePropertiesPanel::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SamplePropertiesPanel::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == loadSampleButton)
    {
        //[UserButtonCode_loadSampleButton] -- add your button handler code here..
        openSample();
        //[/UserButtonCode_loadSampleButton]
    }
    else if (buttonThatWasClicked == loopToggleButton)
    {
        //[UserButtonCode_loopToggleButton] -- add your button handler code here..
        module->setLoop(loopToggleButton->getToggleState());
        //[/UserButtonCode_loopToggleButton]
    }
    else if (buttonThatWasClicked == recordButton)
    {
        //[UserButtonCode_recordButton] -- add your button handler code here..
        if (!recording) {
            startRecording();
        }
        else {
            stopRecording();
        }
        //[/UserButtonCode_recordButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SamplePropertiesPanel::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sampleStartSlider)
    {
        //[UserSliderCode_sampleStartSlider] -- add your slider handling code here..
        module->getCurrentSampler()->setStartPosition(sliderThatWasMoved->getValue());
        thumb->setStartPosition(sliderThatWasMoved->getValue());
        //[/UserSliderCode_sampleStartSlider]
    }
    else if (sliderThatWasMoved == sampleEndSlider)
    {
        //[UserSliderCode_sampleEndSlider] -- add your slider handling code here..
        module->getCurrentSampler()->setEndPosition(sliderThatWasMoved->getValue());
        thumb->setEndPosition(sliderThatWasMoved->getValue());
        //[/UserSliderCode_sampleEndSlider]
    }
    else if (sliderThatWasMoved == pitchSlider)
    {
        //[UserSliderCode_pitchSlider] -- add your slider handling code here..
        module->getCurrentSampler()->setPitch(pitchSlider->getValue());
        //[/UserSliderCode_pitchSlider]
    }

    //[UsersliderValueChanged_Post]

    thumb->repaint();
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void SamplePropertiesPanel::setModule(SamplerModule *module) {
    this->module = module;

}

void SamplePropertiesPanel::setThumbnail(AudioThumbnailComponent* thumbnail) {
    this->thumb = thumbnail;
}

void SamplePropertiesPanel::openSample() {

    FileChooser chooser("Select file to open", File::nonexistent, "*");

    if (chooser.browseForFileToOpen()) {

#if JUCE_IOS
        URL url = chooser.getURLResult();
        InputStream* is = url.createInputStream(false);

        module->loadSample(is);

        delete is;

        ScopedPointer<XmlElement> xml = XmlDocument(data).getDocumentElement();
#else
        File file = chooser.getResult();
        FileInputStream* is = new FileInputStream(file);
        module->setSamplePath(file.getFullPathName(),module->getCurrentSamplerIndex());
        module->loadSample(is, module->getCurrentSamplerIndex());
        if (module->getBuffer() != nullptr) {
            thumb->setSampleBuffer(module->getBuffer());
        }

        // delete is;

#endif

    }
}

void SamplePropertiesPanel::updateValues() {
    std::function<void(void)> changeLambda =
    [=]() {
        loopToggleButton->setToggleState(module->isLoop(), juce::NotificationType::dontSendNotification);

        long start = module->getCurrentSampler()->getStartPosition();
        long end = module->getCurrentSampler()->getEndPosition();

        thumb->setStartPosition(start);
        thumb->setEndPosition(end);

        sampleStartSlider->setRange(0, module->getCurrentSampler()->getEndPosition() - 1, juce::NotificationType::dontSendNotification);
        sampleEndSlider->setRange(module->getCurrentSampler()->getStartPosition() + 1,module->getCurrentSampler()->getSampleLength(), juce::NotificationType::dontSendNotification);
        sampleStartSlider->setValue(start);
        sampleEndSlider->setValue(end);
        pitchSlider->setValue(module->getCurrentSampler()->getPitch());
    };
    juce::MessageManager::callAsync(changeLambda);

}

void SamplePropertiesPanel::startRecording() {

    if (!recording) {
        recording = true;
        recordButton->setButtonText("Stop recording");
        module->startRecording();
    }
    
}


void SamplePropertiesPanel::stopRecording() {
    if (recording) {
        recording = false;
        recordButton->setButtonText("Start recording");
        module->stopRecording();
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SamplePropertiesPanel" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTBUTTON name="loadSampleButton" id="a16960d6fb35b217" memberName="loadSampleButton"
              virtualName="" explicitFocusOrder="0" pos="8 8 150 24" buttonText="Load sample"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="sampleStartSlider" id="44fd26b4ee3614d2" memberName="sampleStartSlider"
          virtualName="" explicitFocusOrder="0" pos="8 72 280 24" min="0.00000000000000000000"
          max="10.00000000000000000000" int="1.00000000000000000000" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <LABEL name="startLabel" id="71e7a2809e388c0" memberName="startLabel"
         virtualName="" explicitFocusOrder="0" pos="8 40 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Sample start&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="33"/>
  <LABEL name="endLabel" id="9c8285b56816945c" memberName="endLabel" virtualName=""
         explicitFocusOrder="0" pos="496 40 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Sample end&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="sampleEndSlider" id="d602ebbdcb49beb6" memberName="sampleEndSlider"
          virtualName="" explicitFocusOrder="0" pos="296 72 280 24" min="0.00000000000000000000"
          max="10.00000000000000000000" int="1.00000000000000000000" style="LinearHorizontal"
          textBoxPos="TextBoxRight" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <TOGGLEBUTTON name="loopToggleButton" id="c99e9f970df31557" memberName="loopToggleButton"
                virtualName="" explicitFocusOrder="0" pos="168 128 104 24" buttonText="loopSample"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="pitchSlider" id="3bdd1a5a126708ef" memberName="pitchSlider"
          virtualName="" explicitFocusOrder="0" pos="296 128 280 24" min="0.25000000000000000000"
          max="2.00000000000000000000" int="0.02500000000000000139" style="LinearHorizontal"
          textBoxPos="TextBoxRight" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <LABEL name="pitchLabel" id="2ddf56590cbe152d" memberName="pitchLabel"
         virtualName="" explicitFocusOrder="0" pos="496 104 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Pitch&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="recordButton" id="c1019ef444e98335" memberName="recordButton"
              virtualName="" explicitFocusOrder="0" pos="176 8 150 24" buttonText="Record sample"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
