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
#include "SynthEditor.h"
#include "MidiGate.h"
#include "MidiNote.h"
#include "MidiOut.h"
#include "AudioOut.h"
#include "SawtoothModule.h"
#include "NoiseModule.h"
#include "AdderModule.h"
#include "Constant.h"
#include "PrefabFactory.h"
#include "Knob.h"
#include "ADSRModule.h"
#include <stdio.h>
#include <string.h>
//[/Headers]

//[MiscUserDefs] You can add your own user definitions and misc code here...
String SynthEditor::defaultMidiOutputName = "Express 128 Port 7";
String SynthEditor::defaultMidiInputName = "Express 128 Port 7";
//[/MiscUserDefs]

SynthEditor::SynthEditor(){
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]
    
    
    //[UserPreSize]
    //[/UserPreSize]
    
    setSize (1280, 800);
    
    
    //[Constructor] You can add your own custom stuff here..
    
    root = new Module("Root");
    
    setRepaintsOnMouseActivity(true);
    setMouseClickGrabsKeyboardFocus(true);
    setWantsKeyboardFocus(true);
    
    addChildComponent(root);
    
    
    
    //[/Constructor]
}

//==============================================================================
SynthEditor::SynthEditor (double sampleRate, int buffersize)
{
    Logger::writeToLog("Creating SynthEditor with sample rate "+String(sampleRate)+" kHz and buffer size of "+String(buffersize)+" bytes.");
    
    //[Constructor_pre] You can add your own custom stuff here..
    this->bufferSize  = buffersize;
    this->_sampleRate = sampleRate;
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1280, 800);


    //[Constructor] You can add your own custom stuff here..

    root = new Module("Root");

	setRepaintsOnMouseActivity(true);
	setMouseClickGrabsKeyboardFocus(true);
	setWantsKeyboardFocus(true);

    addChildComponent(root);
    
    

    //[/Constructor]
}

SynthEditor::~SynthEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..

    cleanUp();

    
    if (isRoot)
        delete root;
    

    //[/Destructor]
}

//==============================================================================
void SynthEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..

    g.setColour(juce::Colours::white);

	if (isLeftMouseDown) {

		if (state == DRAGGING_CONNECTION) {
			g.drawLine(lineStartX, lineStartY, lineStopX, lineStopY);
		}
        else if (state == DRAGGING_SELECTION) {
            g.drawRect(selectionFrame);
        }
	}

	for (int i = 0; i < root->getConnections()->size(); i++) {
		Connection* c = root->getConnections()->at(i);

        if (c->source != NULL && c->target != NULL) {
            int x1 = c->source->getX() + c->a->x;
            int y1 = c->source->getY() + c->a->y + 5;
            int x2 = c->target->getX() + c->b->x;
            int y2 = c->target->getY() + c->b->y + 5;
            
            if (c->selected) {
                g.setColour(juce::Colours::cyan);
            }
            else {
                g.setColour(juce::Colours::white);
            }
            
            g.drawLine(x1,y1,x2,y2);
        }
        
		/*
		Path p = Path();

		// p.addLineSegment(Line<float>(c->source->getX() + c->a->x, c->source->getY() + c->a->y + 5, c->target->getX() + c->b->x, c->target->getY() + c->b->y + 5),1.0f);


		p.addLineSegment(Line<float>(x1, y1, x2, y2), 1.0f);

		int xm = (x1 + x2) / 2;
		int ym = (y1 + y2) / 2 + 2 ;

		p.cubicTo(Point<float>(x1,y1), Point<float>(xm,ym),Point<float>(x2,y2));


		PathStrokeType(50.0, juce::PathStrokeType::JointStyle::curved,
			juce::PathStrokeType::EndCapStyle::rounded)
			.createStrokedPath(p, p);


		g.strokePath(p,PathStrokeType(1));

		*/




	}


    //[/UserPaint]
}

void SynthEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    setSize(getParentWidth()*1.5, getParentHeight()*1.5);
    //[/UserResized]
}

void SynthEditor::mouseMove (const MouseEvent& e)
{
    //[UserCode_mouseMove] -- Add your code here...

	mouseX = e.getPosition().getX();
	mouseY = e.getPosition().getY();

    //[/UserCode_mouseMove]
}

void SynthEditor::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...

    mouseDownX = e.getPosition().getX();
    mouseDownY = e.getPosition().getY();

	if (e.mods.isLeftButtonDown()) {

        bool hit = false;
		isLeftMouseDown = true;
        
        // deselect all pins
        for (int i = 0; i < root->getModules()->size(); i++) {
            
            Module* m = root->getModules()->at(i);
            
            for (int j = 0; j < m->pins.size(); j++) {
                m->pins.at(j)->setSelected(false);
            }
            checkForPinSelection(e, m);
            
        }
 
        for (int i = 0; i < getSelectedModules().size(); i++) {
            
            if (getSelectedModules().at(i)->getBounds().contains(e.x,e.y)) {

                dragStartX = getSelectedModules().at(i)->getX();
                dragStartY = getSelectedModules().at(i)->getY();
                state = MOVING_SELECTION;
                hit = true;
                break;
            }
            
        }
        
        if (getSelectedModules().size() == 0) {
            
            state = DRAGGING_SELECTION;
            
            for (int i = 0; i < root->getModules()->size(); i++) {
                if (root->getModules()->at(i)->getBounds().contains(e.x,e.y)) {
                    root->getModules()->at(i)->setSelected(true);
                    selectedModules.push_back(root->getModules()->at(i));
                    root->getModules()->at(i)->savePosition();
                    hit = true;
                }
                else {
                    root->getModules()->at(i)->setSelected(false);
                }
            }
            if (getSelectedModules().size() > 0) {
                state = MOVING_SELECTION;
            }
            
        }

        if (hit == false) {
            selectedModules.clear();
            
            for (int i = 0; i < root->getModules()->size(); i++) {
                root->getModules()->at(i)->setSelected(false);
    
            }
        }
        
		for (int i = 0; i < root->getConnections()->size(); i++) {
			Connection* c = root->getConnections()->at(i);
            
            if (c->source != NULL && c->target != NULL)  {
                int x1 = c->source->getX() + c->a->x;
                int y1 = c->source->getY() + c->a->y + 5;
                int x2 = c->target->getX() + c->b->x;
                int y2 = c->target->getY() + c->b->y + 5;
                
                if (PointOnLineSegment(Point<int>(x1, y1), Point<int>(x2, y2), Point<int>(mouseX, mouseY), 5)) {
                    c->selected = true;
                }
                else {
                    c->selected = false;
                }
            }

		}
	}
	else if (e.mods.isRightButtonDown()) {
        showContextMenu(e.getPosition());
    }

    repaint();

    sendChangeMessage();

    //[/UserCode_mouseDown]
}

void SynthEditor::showContextMenu(Point<int> position) {
    PopupMenu* m = new PopupMenu();
    
    Module* module = nullptr;

    if (getSelectedModules().size() == 1){
        module = getSelectedModules().at(0);
    }

    if (module != nullptr) {

        int pinIndex = -1;
        
        for (int i = 0; i < module->getPins().size();i++) {
            if (module->getPins().at(i)->isSelected()) {
                pinIndex = i;
            }
        }
        
        Knob* k = nullptr;
        
        if(pinIndex < 0 ) {
            if (module->isEditable()) {
                m->addItem(1, "Add input");
                m->addItem(2, "Add output");
            }
            else {
                
                if ((k = dynamic_cast<Knob*>(module)) != NULL) {
                    m->addItem(3, "MIDI learn");
                }
                
            }
      
            
            
            const int result = m->show();
            
            if (result == 0)
            {
                // user dismissed the menu without picking anything
            }
            else if (result == 1) {
                module->addPin(Pin::Direction::IN);
                
            }
            else if (result == 2) {
                module->addPin(Pin::Direction::OUT);
            }
            
            else if (result == 3) {
                k->setLearning(true);
            }
        }
        else {
            m->addItem(1, "Add control");
            m->addItem(2, "Add constant");
            
            const int result = m->show();
            
            if (result == 0)
            {
                // user dismissed the menu without picking anything
            }
            else if (result == 1) {
                Knob* k = dynamic_cast<Knob*>(PrefabFactory::getInstance()->getPrefab(69, _sampleRate, bufferSize));
                addChangeListener(k);
                k->setValue(1);
                k->setName(module->getPins().at(pinIndex)->getName());
                Point<int> pos = module->getPinPosition(pinIndex);
                k->setTopLeftPosition(pos.getX() - 150, mouseY - k->getHeight() / 2);
                k->getPins().at(0)->connections.push_back(module->pins.at(pinIndex));
                addAndMakeVisible(k);
                Connection* con = new Connection();
                con->a = k->getPins().at(0);
                con->b = module->pins.at(pinIndex);
                con->source = k;
                con->target = module;
                root->getConnections()->push_back(con);
                root->getModules()->push_back(k);
                getSelectedModules().clear();
                k->setSelected(true);
                getSelectedModules().push_back(k);
                repaint();
            }
            else if (result == 2) {
                Constant* c = dynamic_cast<Constant*>(PrefabFactory::getInstance()->getPrefab(67, _sampleRate, bufferSize));
                addChangeListener(c);
                c->setValue(1);
                c->setName(module->getPins().at(pinIndex)->getName());
                Point<int> pos = module->getPinPosition(pinIndex);
                c->setTopLeftPosition(pos.getX() - 150, mouseY - c->getHeight() / 2);
                c->getPins().at(0)->connections.push_back(module->pins.at(pinIndex));
                addAndMakeVisible(c);
                Connection* con = new Connection();
                con->a = c->getPins().at(0);
                con->b = module->pins.at(pinIndex);
                con->source = c;
                con->target = module;
                root->getConnections()->push_back(con);
                root->getModules()->push_back(c);
                repaint();
                
            }
        }
        
    }
    else {
        
        m->addItem(1, "Add module");
        m->addItem(2, "Save");
        m->addItem(3, "Load");
        m->addItem(4, "New");
        
        PopupMenu* prefabMenu = new PopupMenu();
        
        StringArray* categories = PrefabFactory::getInstance()->getCategories();
        
        for (int i = 0; i < categories->size();i++) {
            PopupMenu category = PopupMenu();
            
            std::map<int,PrefabFactory::Prefab> prefabs = PrefabFactory::getInstance()->getPrefabNames();
            
            for (std::map<int,PrefabFactory::Prefab>::iterator it  = prefabs.begin();it != prefabs.end();++it) {
                if ((*it).second.getCategory() == categories->getReference(i)) {
                    category.addItem((*it).first,(*it).second.getName(), true);
                }
                
            }
            
            prefabMenu->addSubMenu(categories->getReference(i), category, true);
        }
        
        m->addSubMenu("Prefabs",*prefabMenu);
        
        const int result = m->show();
        
        if (result == 0)
        {
            // user dismissed the menu without picking anything
        }
        else if (result == 1)
        {
            Module* m3 = new Module("Macro");
            
            m3->setTopLeftPosition(position.getX(), position.getY());
            m3->setIndex(Time::currentTimeMillis());
            
            addAndMakeVisible(m3);
            root->getModules()->push_back(m3);
            
        }
        else if (result == 2)
        {
            saveFile();
        }
        else if (result == 3) {
            newFile();
            openFile();
        }
        else if (result == 4) {
            newFile();
        }
        else {
            Module* m = PrefabFactory::getInstance()->getPrefab(result, _sampleRate, bufferSize);
            addChangeListener(m);
            m->setTopLeftPosition(position);
            
            
            addAndMakeVisible(m);
            root->getModules()->push_back(m);
            
            m->setSelected(true);
            getSelectedModules().push_back(m);
            repaint();
            
            AudioOut* out;
            if ((out = dynamic_cast<AudioOut*>(m)) != NULL) {
                outputChannels.push_back(out);
                if (!running) {
                    running = true;
                }
            }
        }
        
        delete prefabMenu;
    }
    
    delete m;

}

void SynthEditor::mouseDrag (const MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...

    mouseX = e.getPosition().getX();
    mouseY = e.getPosition().getY();
    
    dragDistanceX = e.getDistanceFromDragStartX();
    dragDistanceY = e.getDistanceFromDragStartY();
    
    if (state != DRAGGING_SELECTION) {
        for (int i = 0; i < root->getModules()->size(); i++) {
        
            Module* m = root->getModules()->at(i);
        
            if (m->isSelected()) {
                for (int j = 0; j < m->pins.size(); j++) {
                    if (m->pins.at(j)->isSelected()) {
                        lineStartX = m->getX() + m->pins.at(j)->x + 5;
                        lineStartY = m->getY() + m->pins.at(j)->y + 5;
                        startPin = m->pins.at(j);
                        state = DRAGGING_CONNECTION;
                        break;
                    }
                    
                }
            }
        }
    }
    
	//lineStartX = e.getMouseDownPosition().x;
	//lineStartY = e.getMouseDownPosition().y;

	lineStopX = lineStartX + e.getDistanceFromDragStartX();
	lineStopY = lineStartY + e.getDistanceFromDragStartY();

    if (state == MOVING_SELECTION) {
        for (int i = 0; i < root->getModules()->size(); i++) {
            
            Module* m = root->getModules()->at(i);
            
            if (m->isSelected()) {
                
                if (m->getSelectedPin() == nullptr || m->getSelectedPin() == NULL ) {
                    
                   
                    m->setTopLeftPosition(m->getSavedPosition().getX() + e.getOffsetFromDragStart().getX(), m->getSavedPosition().getY()+ e.getOffsetFromDragStart().getY());
                    

                    repaint();
                }
            }
            checkForPinSelection(e, m);
        }
    }
    else {
        for (int i = 0; i < root->getModules()->size(); i++) {
            Module* m = root->getModules()->at(i);
            if (!m->isSelected()) {
                checkForPinSelection(e,m);
            }
        }
    }
    
    if (state == DRAGGING_SELECTION) {

        int x = mouseDownX;
        int y = mouseDownY;
        
        if (mouseX < x) {
            x = mouseX;
        }
        if (mouseY < y) {
            y = mouseY;
        }
        selectionFrame.setBounds(x,y,abs(dragDistanceX),abs(dragDistanceY));
        
        for (int i = 0; i < root->getModules()->size(); i++) {
            Module* m = root->getModules()->at(i);
            
            if (selectionFrame.contains(m->getBounds())) {
                if (!m->isSelected()) {
                    m->setSelected(true);
                    m->savePosition();
                    
                    selectedModules.push_back(m);
                }

            }
        }
    }

    
	repaint();

    //[/UserCode_mouseDrag]
}

void SynthEditor::mouseUp (const MouseEvent& e)
{
    //[UserCode_mouseUp] -- Add your code here...
	if (e.mods.isLeftButtonDown()) {
		isLeftMouseDown = false;
	}

    if (state == DRAGGING_CONNECTION) {
        for (int i = 0; i < root->getModules()->size(); i++) {

            Module* m = root->getModules()->at(i);
            
            m->savePosition();

            if (m->isSelected()) {
                addConnection(e, m);
            }
        }
    }
	lineStopX = 0;
	lineStopY = 0;
    
    // startPin = nullptr;
    
    state = NONE;

    //[/UserCode_mouseUp]
}

void SynthEditor::mouseDoubleClick (const MouseEvent& e)
{
    //[UserCode_mouseDoubleClick] -- Add your code here...

    if (isAltDown) {
        for (int i = 0; i < root->getModules()->size(); i++) {

            Module* m = root->getModules()->at(i);

            if (m->isSelected()) {
                m->setEditing(true);
                break;
            }
        }
    }
    else {
        for (int i = 0; i < root->getModules()->size(); i++) {

            Module* m = root->getModules()->at(i);
            
            if (m->isSelected() && m->isEditable()) {
                SynthEditor* editor = new SynthEditor(_sampleRate, bufferSize);
                editor->setModule(m);
                tab->addTab(m->getName(), juce::Colours::grey,editor, true);
            }
        }

    }
    
    if (getSelectedModules().size() == 0) {
        showContextMenu(e.getPosition());
    }

    //[/UserCode_mouseDoubleClick]
}

bool SynthEditor::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...

    if (key.getKeyCode() == KeyPress::deleteKey || key.getKeyCode() == KeyPress::backspaceKey) {
        removeSelectedItem();
    }
    if(key.getKeyCode() == 65 && isCtrlDown) {
        for (int i = 0; i < root->getModules()->size();i++) {
            root->getModules()->at(i)->setSelected(true);
            selectedModules.push_back(root->getModules()->at(i));
        }
    }
    
    repaint();
    

    return true;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyPressed]
}

bool SynthEditor::keyStateChanged (bool isKeyDown)
{
    //[UserCode_keyStateChanged] -- Add your code here...
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyStateChanged]
}

void SynthEditor::modifierKeysChanged (const ModifierKeys& modifiers)
{
    //[UserCode_modifierKeysChanged] -- Add your code here...
    isAltDown = modifiers.isAltDown();
    isCtrlDown = modifiers.isCtrlDown() ||  modifiers.isCommandDown();
    //[/UserCode_modifierKeysChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void SynthEditor::cleanUp() {
    for(std::vector<Module*>::iterator it = root->getModules()->begin();it != root->getModules()->end();it++) {
        removeModule((*it));
    }
    for(std::vector<Module*>::iterator it = root->getModules()->begin();it != root->getModules()->end();) {
        delete *it;
        it = root->getModules()->erase(it);
    }
    for(std::vector<Connection*>::iterator it = root->getConnections()->begin();it != root->getConnections()->end();) {
        delete *it;
        it = root->getConnections()->erase(it);
    }
    
    PrefabFactory::getInstance()->reset();
    
    delete root;
    root = nullptr;
    //deleteSelected(true);
    selectedModules.clear();
    outputChannels.clear();
    
}

void SynthEditor::removeSelectedItem() {
    std::vector<Connection*>* cons = root->getConnections();
    cons->erase(std::remove_if(cons->begin(), cons->end(), [](Connection* c){
        if (c->selected){
            delete c;
            return true;
        }
        return false;
    }),cons->end());
    
    for(std::vector<Module*>::iterator it = root->getModules()->begin();it != root->getModules()->end();it++) {
        if ((*it)->isSelected())
            removeModule((*it));
    }
    for(std::vector<Module*>::iterator it = root->getModules()->begin();it != root->getModules()->end();) {
        if ((*it)->isSelected()) {
            delete *it;
            it = root->getModules()->erase(it);
        }
        else {
            ++it;
        }
    }
    
}

void SynthEditor::newFile() {
    cleanUp();
    root = new Module("Root");
    repaint();
}

void SynthEditor::saveStructure(std::vector<Module *>* modules, std::vector<Connection*>* connections, ValueTree* v) {
    ValueTree mods = ValueTree("Modules");

    for (std::vector<Module*>::iterator it = modules->begin(); it != modules->end(); ++it) {
        
        ValueTree file = ValueTree("Module");
        
        file.setProperty("name",(*it)->getName(), nullptr);
        file.setProperty("index",String((*it)->getIndex()), nullptr);
        file.setProperty("x",(*it)->getPosition().getX(),nullptr);
        file.setProperty("y",(*it)->getPosition().getY(),nullptr);
        file.setProperty("isPrefab",(*it)->isPrefab(),nullptr);
        file.setProperty("prefabId", (*it)->getId(), nullptr);
        
        Knob* k;
        
        if ((k = dynamic_cast<Knob*>((*it))) != NULL) {
            file.setProperty("minvalue",k->getMinimum(), nullptr);
            file.setProperty("maxvalue",k->getMaximum(), nullptr);
            file.setProperty("stepsize", k->getStepsize(), nullptr);
            file.setProperty("value", k->getValue(), nullptr);
            file.setProperty("isController",k->isMidiController(), nullptr);
            file.setProperty("controllerNum", k->getController(), nullptr);
        }
        
        ADSRModule* adsr;
        
        if ((adsr = dynamic_cast<ADSRModule*>((*it))) != NULL) {
            file.setProperty("attack",adsr->getAttack(), nullptr);
            file.setProperty("decay",adsr->getDecay(), nullptr);
            file.setProperty("sustain", adsr->getSustain(), nullptr);
            file.setProperty("release", adsr->getRelease(), nullptr);
        }
        
        Constant* c = nullptr;
        if ((c = dynamic_cast<Constant*>((*it))) != NULL) {
            file.setProperty("value", c->getValue(), nullptr);
        }
        
        ValueTree pins = ValueTree("Pins");

        for (std::vector<Pin*>::iterator it2 =  (*it)->pins.begin(); it2 != (*it)->pins.end(); ++it2) {
            ValueTree pin = ValueTree("Pin");
            pin.setProperty("type", (*it2)->type, nullptr);
            pin.setProperty("direction", (*it2)->direction, nullptr);
            pin.setProperty("index", String(((*it2)->index)), nullptr);
            pin.setProperty("x", (*it2)->x, nullptr);
            pin.setProperty("y", (*it2)->y, nullptr);
            pins.addChild(pin,-1,nil);
        }

        mods.addChild(file,-1, nullptr);
        file.addChild(pins,-1, nullptr);

        saveStructure((*it)->getModules(), (*it)->getConnections(), &file);
    }

    ValueTree cons = ValueTree("Connections");

    for (std::vector<Connection*>::iterator it = connections->begin(); it != connections->end(); ++it) {
        ValueTree con = ValueTree("Connection");
        con.setProperty("source", String((*it)->source->getIndex()), nullptr);
        con.setProperty("target", String((*it)->target->getIndex()), nullptr);
        con.setProperty("a", String((*it)->a->index), nullptr);
        con.setProperty("b", String((*it)->b->index), nullptr);
        cons.addChild(con,-1, nullptr);
    }

    v->addChild(mods, -1, nullptr);
    v->addChild(cons, -1, nullptr);


}

void SynthEditor::saveFile() {
    
#if JUCE_IOS
    File path = File::getSpecialLocation(File::userApplicationDataDirectory);
    File outputDir = File(path.getFullPathName()+"/structure.xml");
    
    FileChooser chooser("Select target file...",outputDir, "*");
    
    if (chooser.browseForFileToSave(true)) {
        
        ValueTree* v = new ValueTree("Synth");
        
        saveStructure(root->getModules(),root->getConnections(), v);
        
        URL file = chooser.getURLResult();
        
        OutputStream* os = file.createOutputStream();
        
        XmlElement* xml = v->createXml();
        xml->writeToStream(*os, "");
        
        delete os;
        delete xml;
        delete v;
    }
#else
    FileChooser chooser("Select target file...", File::nonexistent, "*");
    
    if (chooser.browseForFileToSave(true)) {
        
        ValueTree* v = new ValueTree("Synth");
        
        saveStructure(root->getModules(),root->getConnections(), v);
        
        File file = chooser.getResult();
        
        XmlElement* xml = v->createXml();
        xml->writeToFile(file, "");
        delete xml;
        delete v;
        
    }
#endif
    

}

void SynthEditor::openFile() {
    FileChooser chooser("Select file to open", File::nonexistent, "*");

    if (chooser.browseForFileToOpen()) {

        cleanUp();
        root = new Module("Root");
#if JUCE_IOS
        URL url = chooser.getURLResult();
        InputStream* is = url.createInputStream(false);
        String data = is->readEntireStreamAsString();
        delete is;
        
        ScopedPointer<XmlElement> xml = XmlDocument(data).getDocumentElement();
#else
        File file = chooser.getResult();
        ScopedPointer<XmlElement> xml = XmlDocument(file).getDocumentElement();
#endif
   
        ValueTree v = ValueTree::fromXml(*xml.get());

        running = false;
        loadStructure(root->getModules(),root->getConnections(), &v);

        for (std::vector<Module*>::iterator it = root->getModules()->begin(); it != root->getModules()->end(); ++it) {
            addAndMakeVisible((*it));
        }

        xml = nullptr;
        running = true;

    }
}

void SynthEditor::loadStructure(std::vector<Module *>* modules, std::vector<Connection*>* connections,juce::ValueTree *v) {
    ValueTree mods = v->getChildWithName("Modules");

    for (int i = 0; i < mods.getNumChildren();i++) {
        ValueTree mod = mods.getChild(i);

        Module* m = nullptr;
        
        if (mod.getProperty("isPrefab").toString().getIntValue() == 1) {
            m = PrefabFactory::getInstance()->getPrefab(mod.getProperty("prefabId").toString().getIntValue(), _sampleRate, bufferSize);
            addChangeListener(m);
        }
        else {
            m = new Module(mod.getProperty("name"));
            addChangeListener(m);
            ValueTree pins = mod.getChildWithName("Pins");
            
            for (int j = 0; j < pins.getNumChildren();j++) {
                ValueTree pin = pins.getChild(j);
                Pin* p = new Pin(Pin::Type::AUDIO);
                
                int type = pin.getProperty("type").toString().getIntValue();
                int direction = pin.getProperty("direction").toString().getIntValue();
                long index = pin.getProperty("index").toString().getLargeIntValue();
                int x = pin.getProperty("x").toString().getIntValue();
                int y = pin.getProperty("y").toString().getIntValue();
                
                Pin::Direction dir = static_cast<Pin::Direction>(direction);
                p->direction = dir;
                Pin::Type t = static_cast<Pin::Type>(type);
                p->type = t;
                p->index = index;
                p->x = x;
                p->y = y;
                m->addPin(p);
                m->setIndex(mod.getProperty("index").toString().getLargeIntValue());
            }
        }

        
        m->setTopLeftPosition(mod.getProperty("x").toString().getIntValue(), mod.getProperty("y").toString().getIntValue());

        modules->push_back(m);
        
        AudioOut* out;
        
        if ((out = dynamic_cast<AudioOut*>(m)) != NULL) {
            outputChannels.push_back(out);
            addChangeListener(out);
        }
        
        Knob* k;
        
        if ((k = dynamic_cast<Knob*>(m)) != NULL) {
            addChangeListener(k);
            k->setMaximum(mod.getProperty("maxvalue").toString().getFloatValue());
            k->setMinimum(mod.getProperty("minvalue").toString().getFloatValue());
            k->setStepSize(mod.getProperty("stepsize").toString().getFloatValue());
            k->setValue(mod.getProperty("value").toString().getFloatValue());
            k->setIsMidicontroller(mod.getProperty("isController").toString().getIntValue() > 0);
            k->setController(mod.getProperty("controllerNum").toString().getIntValue());
        }
        
        ADSRModule* adsr;
        
        if ((adsr = dynamic_cast<ADSRModule*>(m)) != NULL) {
            addChangeListener(adsr);
            adsr->setAttack(mod.getProperty("attack").toString().getFloatValue());
            adsr->setDecay(mod.getProperty("decay").toString().getFloatValue());
            adsr->setSustain(mod.getProperty("sustain").toString().getFloatValue());
            adsr->setRelease(mod.getProperty("release").toString().getFloatValue());
        }
        
        Constant* c = nullptr;
        if ((c = dynamic_cast<Constant*>((m))) != NULL) {
            addChangeListener(c);
            c->setValue(mod.getProperty("value").toString().getFloatValue());
        }
        
        // addAndMakeVisible(m);

        loadStructure(m->getModules(),m->getConnections(),&mod);
    }

    ValueTree cons = v->getChildWithName("Connections");

    for (int i = 0; i < cons.getNumChildren();i++) {
        ValueTree con = cons.getChild(i);

        Connection* c = new Connection();
        long sourceIndex = con.getProperty("source").toString().getLargeIntValue();
        long targetIndex = con.getProperty("target").toString().getLargeIntValue();
        long aIndex = con.getProperty("a").toString().getLargeIntValue();
        long bIndex = con.getProperty("b").toString().getLargeIntValue();
        
        Module* source = nullptr;
        Module* target = nullptr;
        Pin* a = nullptr;
        Pin* b = nullptr;
        
        for (int j = 0; j < modules->size(); j++) {
            
            Module* m = modules->at(j);
            
            if (m->getIndex() == sourceIndex) {
                source = m;
            }
            if (m->getIndex() == targetIndex) {
                target = m;
            }
            
            for (int k = 0; k < m->getPins().size();k++) {
                if (m->getPins().at(k)->index == aIndex) {
                    a = m->getPins().at(k);
                }
                if (m->getPins().at(k)->index == bIndex) {
                    b = m->getPins().at(k);
                }
            }
            
            if (source != nullptr && target != nullptr && a != nullptr && b != nullptr) {
                
                
                // EVENTS: in listens at out
                // OTHER: in gets its values from out
                if ((a->type != Pin::Type::EVENT && a->direction == Pin::Direction::IN && b->direction == Pin::Direction::OUT)
                    || (a->type == Pin::Type::EVENT && a->direction == Pin::Direction::OUT && b->direction == Pin::Direction::IN)) {
                    
                    if (!a->hasConnection(b)) {
                        
                        a->connections.push_back(b);
                        Logger::writeToLog("Connecting pin "+ String(a->index)+ " to pin "  +String(b->index));
                    }
                    
                    // xConnection* c = new Connection(source, a, target, b);
                    // root->getConnections()->push_back(c);
                    
                    c->a = a;
                    c->b = b;
                    c->source = source;
                    c->target = target;
                    if (!connectionExists(*connections, c)) {
                        Logger::writeToLog("Adding connection from module "+ source->getName() + ", index "+ String(source->getIndex()) + " to module "  +target->getName()+ ", index : " + String(target->getIndex()));
                        connections->push_back(c);
                    }
                    
                }
                else if ((a->type != Pin::Type::EVENT && a->direction == Pin::Direction::OUT && b->direction == Pin::Direction::IN)
                         || (a->type == Pin::Type::EVENT && a->direction == Pin::Direction::IN && b->direction == Pin::Direction::OUT)) {
                    if (!b->hasConnection(a)) {
                        b->connections.push_back(a);
                        Logger::writeToLog("Conencting pin "+ String(b->index)+ " to pin "  +String(a->index));
                    }
                    
                    // Connection* c = new Connection(target, b, source, a);
                    // root->getConnections()->push_back(c);
                    c->a = b;
                    c->b = a;
                    c->source = target;
                    c->target = source;
                    if (!connectionExists(*connections, c)) {
                        Logger::writeToLog("Adding connection from module "+ source->getName() + ", index "+ String(source->getIndex()) + " to module "  +target->getName()+ ", index : " + String(target->getIndex()));
                        connections->push_back(c);
                    }
                }
                
            }
            
        }

        repaint();
        
    }

}

bool SynthEditor::connectionExists(std::vector<Connection*> connections,Connection *c) {
    
    for (int i = 0; i < connections.size();i++) {
        
        Connection* d = connections.at(i);
        if (d->a->index == c->a->index && d->b->index == c->b->index && d->source->getIndex() == c->source->getIndex() && d->target->getIndex() == c->target->getIndex()) {
            return true;
        }

    }
    
    return false;
}

void SynthEditor::setTab(juce::TabbedComponent *t) {
    this->tab = t;
}

void SynthEditor::removeModule(Module* module) {
    vector<long> pinsToBeRemoved;
    
    // find the indices of the pins being involved in the disconnect

    for (int j = 0; j < root->getModules()->size(); j++) {
        if (root->getModules()->at(j)->getIndex() != module->getIndex()) {
            
            for (int k = 0; k < root->getModules()->at(j)->getPins().size(); k++) {
                
                // for every connection of each pin
                for (int l = 0; l < root->getModules()->at(j)->getPins().at(k)->connections.size(); l++) {
                    
                    // for each pin of the module being removed
                    for (int n = 0; n < module->getPins().size();n++) {
                        // if the index matches remove pin from vector
                        
                        if (root->getModules()->at(j)->getPins().at(k)->connections.at(l)->index == module->getPins().at(n)->index) {
                            pinsToBeRemoved.push_back(root->getModules()->at(j)->getPins().at(k)->connections.at(l)->index);
                            
                        }
                    }
                    
                }
            }
        }
    }
    
    // remove according pins
    
    for (int i = 0; i < pinsToBeRemoved.size();i++) {
        for (int j = 0; j < root->getModules()->size(); j++) {
            
            for (int k = 0; k < root->getModules()->at(j)->getPins().size();k++) {
                
                Pin* p = root->getModules()->at(j)->getPins().at(k);
                for (std::vector<Pin*>::iterator it = p->connections.begin();it != p->connections.end();) {
                    if ((*it)->index == pinsToBeRemoved.at(i)) {
                        it = p->connections.erase(it);
                    }
                    else {
                        ++it;
                    }
                }
            }
        }
    }
    
    // remove dangling connections from other modules
    
    
    std::vector<Connection*>* cons = root->getConnections();
   
    for(int i = 0;i < root->getModules()->size();i++) {
        for (std::vector<Connection*>::iterator it = cons->begin(); it != cons->end(); ) {
            if ((*it)->source->getIndex() == module->getIndex() ||
                (*it)->target->getIndex() == module->getIndex() ) {
                it = cons->erase(it);
            }
            else {
                ++it;
            }
        }
    }
    
    
}

void SynthEditor::deleteSelected(bool deleteAll) {
    
    if(!deleteAll) {
        for (int i = 0; i < getSelectedModules().size();i++) {
            removeChangeListener(getSelectedModules().at(i));
            removeModule(getSelectedModules().at(i));
        }
    }
    
    // handle connections at root level
    if (deleteAll) {
        std::vector<Connection*>* cons = root->getConnections();
        cons->erase(std::remove_if(cons->begin(), cons->end(), [](Connection* c){return true;}),cons->end());

    }
    else {
        std::vector<Connection*>* cons = root->getConnections();
        cons->erase(std::remove_if(cons->begin(), cons->end(), [](Connection* c){
            if (c->selected){
                delete c;
                return true;
            }
            return false;
        }),cons->end());
    }
    std::vector<Module*>* mods = root->getModules();
    
    if (deleteAll) {
        
        mods->erase(std::remove_if(mods->begin(), mods->end(), [](Module* m){
            delete m; return true;
        }),mods->end());
    }
    else {
        
        mods->erase(std::remove_if(mods->begin(), mods->end(), [](Module* m){
            bool selected =  m->isSelected();
            if (selected) {
                delete m;
                m = nullptr;
            }
            return selected;
        }),mods->end());
    }

    
    sendChangeMessage();
 
}

void SynthEditor::checkForPinSelection(const MouseEvent& e, Module* m) {

	for (int j = 0; j < m->pins.size(); j++) {

		if (m->isMouseOverPin(j, e.getPosition())) {
			m->pins.at(j)->setSelected(true);
            break;
		}


	}


	m->repaint();

}

void SynthEditor::addConnection(const MouseEvent& e, Module* source) {
    
    Pin* a = nullptr;
    Pin* b = nullptr;
    Module* target = nullptr;
    
    // find the selected input of the source module
    for (int j = 0; j < source->pins.size(); j++) {
        
        if (source->pins.at(j)->isSelected()) {
            a = source->pins.at(j);
            break;
        }
        
    }
    
    // now find the target module
    for (int i = 0; i < root->getModules()->size(); i++) {
        
        Module* m = root->getModules()->at(i);
        
        if (m->getIndex() == source->getIndex()) {
            continue;
        }
        
        // find the selected input of the target  module
        for (int j = 0; j < m->pins.size(); j++) {
            
            if (m->pins.at(j)->isSelected()) {
                b = m->pins.at(j);
                target = m;
                break;
            }
            
        }
        
    }
    
    if (source != nullptr && target != nullptr && a != nullptr && b != nullptr) {
        
        if (a->type == b->type) {
            
            // for event based pins the direction is from source to target
            if (a->type == Pin::Type::EVENT ) {
                if (a->direction == Pin::Direction::IN && b->direction == Pin::Direction::OUT) {
                    
                    b->connections.push_back(a);
                    Connection* c = new Connection(source, a, target, b);
                    root->getConnections()->push_back(c);
                    
                }
                else if (a->direction == Pin::Direction::OUT && b->direction == Pin::Direction::IN) {
                    a->connections.push_back(b);
                    Connection* c = new Connection(source, a, target, b);
                    root->getConnections()->push_back(c);
                }
            }
            // in all other cases its the opposite direction
            else {
                if (a->direction == Pin::Direction::IN && b->direction == Pin::Direction::OUT) {
                    a->connections.push_back(b);
                    Connection* c = new Connection(source, a, target, b);
                    root->getConnections()->push_back(c);
                }
                else if (a->direction == Pin::Direction::OUT && b->direction == Pin::Direction::IN) {
                    b->connections.push_back(a);
                    
                    Connection* c = new Connection(source, a, target, b);
                    root->getConnections()->push_back(c);
                }
            }
            
            
        }
        
        repaint();
    }
    
}


bool SynthEditor::PointOnLineSegment(Point<int> pt1, Point<int> pt2, Point<int> pt, double epsilon = 0.001)
{
	if (pt.x - std::max(pt1.x, pt2.x) > epsilon ||
		std::min(pt1.x, pt2.x) - pt.x > epsilon ||
		pt.y - std::max(pt1.y, pt2.y) > epsilon ||
		std::min(pt1.y, pt2.y) - pt.y > epsilon)
		return false;

	if (abs(pt2.x - pt1.x) < epsilon)
		return abs(pt1.x - pt.x) < epsilon ||abs(pt2.x - pt.x) < epsilon;
	if (abs(pt2.y - pt1.y) < epsilon)
		return abs(pt1.y - pt.y) < epsilon || abs(pt2.y - pt.y) < epsilon;

	double x = pt1.x + (pt.y - pt1.y) * (pt2.x - pt1.x) / (pt2.y - pt1.y);
	double y = pt1.y + (pt.x - pt1.x) * (pt2.y - pt1.y) / (pt2.x - pt1.x);

	return abs(pt.x - x) < epsilon ||abs(pt.y - y) < epsilon;
}



void SynthEditor::setModule(Module *m) {
    this->root = m;
    isRoot = false;

    for (std::vector<Module*>::iterator it = root->getModules()->begin(); it != root->getModules()->end(); ++it) {
        addAndMakeVisible((*it));
    }

    repaint();
}

Module* SynthEditor::getModule() {
    return root;
}


Module* SynthEditor::getSelectedModule() {
    if (root != NULL && root != nullptr) {
        for (std::vector<Module*>::iterator it = root->getModules()->begin(); it != root->getModules()->end(); ++it) {
            if ((*it)->isSelected()) {
                return (*it);
            }
        }
    }
    return nullptr;
}

std::vector<Module*> SynthEditor::getSelectedModules() {
    return selectedModules;
}


bool SynthEditor::channelIsValid(int channel) {
    if (outputChannels.at(0)->getPins().at(channel)->connections.size() == 1 &&
        outputChannels.at(0)->getPins().at(channel)->connections.at(0) != nullptr &&
        outputChannels.at(0)->getPins().at(channel)->connections.at(0)->getAudioBuffer() != nullptr) {
        return true;
    }
    return false;
}

void SynthEditor::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    this->_sampleRate = sampleRate;
    this->bufferSize = samplesPerBlockExpected;
    
    Logger::writeToLog("SynthEditor using sample rate "+String(sampleRate)+" kHz and buffer size of "+String(samplesPerBlockExpected)+" bytes.");
    
    sendChangeMessage();
   
}

void SynthEditor::setSamplerate(double rate) {
    this->_sampleRate = rate;
}

void SynthEditor::setBufferSize(int buffersize) {
    this->bufferSize = buffersize;
}

std::vector<AudioOut*> SynthEditor::getOutputChannels() {
    return outputChannels;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SynthEditor" componentName=""
                 parentClasses="public Component, public ChangeBroadcaster" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="1280" initialHeight="800">
  <METHODS>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseUp (const MouseEvent&amp; e)"/>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
    <METHOD name="modifierKeysChanged (const ModifierKeys&amp; modifiers)"/>
    <METHOD name="mouseMove (const MouseEvent&amp; e)"/>
    <METHOD name="keyStateChanged (bool isKeyDown)"/>
    <METHOD name="mouseDoubleClick (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
