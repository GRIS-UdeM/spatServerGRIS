/*
 This file is part of spatServerGRIS.
 
 spatServerGRIS is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 spatServerGRIS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with spatServerGRIS.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "MainComponent.h"

MainContentComponent::MainContentComponent(){

    LookAndFeel::setDefaultLookAndFeel(&mGrisFeel);
    
    this->listSpeaker = vector<Speaker *>();
    this->listLevelComp = vector<LevelComponent *>();

    this->lockSpeakers = new mutex();
    
    this->winSpeakConfig = nullptr;
    //SpeakerViewComponent 3D VIEW------------------------------
    this->speakerView= new SpeakerViewComponent(this);
    this->addAndMakeVisible (speakerView);
    
    //BOX Inputs------------------------------------------------
    this->boxInputsUI = new Box(&mGrisFeel, "Inputs~");
    addAndMakeVisible(this->boxInputsUI);
    
    //BOX Outputs-----------------------------------------------
    this->boxOutputsUI = new Box(&mGrisFeel, "Outputs~");
    addAndMakeVisible(this->boxOutputsUI);
    
    //BOX Control-----------------------------------------------
    this->boxControlUI = new Box(&mGrisFeel);
    addAndMakeVisible(this->boxControlUI);

    
    
    //Components in BOX 3 ------------------------------------------------------------------
    this->labelJackStatus = addLabel("Jack Unknown","",0, 0, 150, 28,this->boxControlUI->getContent());
    
    this->butLoadXMLSpeakers = addButton("XML Speakers","Load Xml File Coniguration",4,36,124,24,this->boxControlUI->getContent());
    
    this->butEditableSpeakers = addButton("Edit Speakers","Edit position of spkeakers",4,70,124,24,this->boxControlUI->getContent());
    
    this->butShowSpeakerNumber = addToggleButton("Show numbers", "Show numbers skeapers", 4, 100, 124, 24, this->boxControlUI->getContent());
    
    
    this->tedOSCInIP = addTextEditor("IP OSC In :", "IP Address", "IP Address OSC Input", 140, 36, 120, 24, this->boxControlUI->getContent());
    this->tedOSCInIP->setText("127.0.0.1");
    this->tedOSCInPort = addTextEditor("Port OSC In :", "Port Socket", "Port Socket OSC Input", 140, 70, 120, 24, this->boxControlUI->getContent());
    this->tedOSCInPort->setText("18032");
    
    
    // set up the layout and resizer bars
    this->verticalLayout.setItemLayout (0, -0.2, -0.8, -0.5); // width of the font list must be between 20% and 80%, preferably 50%
    this->verticalLayout.setItemLayout (1, 8, 8, 8);           // the vertical divider drag-bar thing is always 8 pixels wide
    this->verticalLayout.setItemLayout (2, 150, -1.0, -0.65);  // the components on the right must be at least 150 pixels wide, preferably 50% of the total width
    this->verticalDividerBar = new StretchableLayoutResizerBar (&verticalLayout, 1, true);
    this->addAndMakeVisible (verticalDividerBar);

    this->setSize (1360, 760);
    
    

#if USE_JACK
    // #1: this is not working with jack with non-built-in audio devices
    //        setAudioChannels (2, 2);
    
    //Start JACK
    this->jackClient = new jackClientGris();
    
    if(!jackClient->isReady()){
        this->labelJackStatus->setText("JackClient Not Connected", dontSendNotification);
    }else{
         this->labelJackStatus->setText("Jack Ready", dontSendNotification);
    }
#endif
    
    this->oscReceiver = new OscInput();
    this->oscReceiver->startConnection(this->tedOSCInPort->getTextValue().toString().getIntValue());


    openXmlFileSpeaker("/Users/gris/Documents/GRIS/zirkonium/ZirkSpeakers_Dome 16 UdeM.xml");
    
    this->resized();
}


Label* MainContentComponent::addLabel(const String &s, const String &stooltip, int x, int y, int w, int h, Component *into)
{
    Label *lb = new Label();
    lb->setText(s, NotificationType::dontSendNotification);
    lb->setTooltip (stooltip);
    lb->setJustificationType(Justification::left);
    lb->setFont(mGrisFeel.getFont());
    lb->setLookAndFeel(&mGrisFeel);
    lb->setColour(Label::textColourId, mGrisFeel.getFontColour());
    lb->setBounds(x, y, w, h);
    into->addAndMakeVisible(lb);
    return lb;
}

TextButton* MainContentComponent::addButton(const String &s, const String &stooltip, int x, int y, int w, int h, Component *into)
{
    TextButton *tb = new TextButton();
    tb->setTooltip (stooltip);
    tb->setButtonText(s);
    tb->setSize(w, h);
    tb->setTopLeftPosition(x, y);
    tb->addListener(this);
    tb->setColour(ToggleButton::textColourId, mGrisFeel.getFontColour());
    tb->setLookAndFeel(&mGrisFeel);
    into->addAndMakeVisible(tb);
    return tb;
}

ToggleButton* MainContentComponent::addToggleButton(const String &s, const String &stooltip, int x, int y, int w, int h, Component *into, bool toggle)
{
    ToggleButton *tb = new ToggleButton();
    tb->setTooltip (stooltip);
    tb->setButtonText(s);
    tb->setToggleState(toggle, dontSendNotification);
    tb->setSize(w, h);
    tb->setTopLeftPosition(x, y);
    tb->addListener(this);
    tb->setColour(ToggleButton::textColourId, mGrisFeel.getFontColour());
    tb->setLookAndFeel(&mGrisFeel);
    into->addAndMakeVisible(tb);
    return tb;
}

TextEditor* MainContentComponent::addTextEditor(const String &s, const String &emptyS, const String &stooltip, int x, int y, int w, int h, Component *into, int wLab)
{
    if (s.isEmpty()){
        TextEditor *te = new TextEditor();
        te->setTooltip (stooltip);
        te->setTextToShowWhenEmpty(emptyS, mGrisFeel.getOffColour());
        te->setBounds(x, y, w, h);
        te->setColour(ToggleButton::textColourId, mGrisFeel.getFontColour());
        te->setLookAndFeel(&mGrisFeel);
        into->addAndMakeVisible(te);
        return te;
    }else{
        TextEditor *te = new TextEditor();
        te->setTooltip (stooltip);
        te->setTextToShowWhenEmpty(emptyS, mGrisFeel.getOffColour());
        te->setBounds(x+wLab, y, w, h);
        te->setColour(ToggleButton::textColourId, mGrisFeel.getFontColour());
        te->setLookAndFeel(&mGrisFeel);
        into->addAndMakeVisible(te);
        Label *lb =addLabel(s, "", x, y, wLab, h, into);
        lb->setJustificationType(Justification::centredRight);
        return te;
        
    }
}


MainContentComponent::~MainContentComponent() {
    
    if(this->winSpeakConfig != nullptr){
        delete this->winSpeakConfig;
    }
    
    
    this->lockSpeakers->lock();
    for (auto&& it : listSpeaker)
    {
        delete (it);
    }
    listSpeaker.clear();
    this->lockSpeakers->unlock();
    delete this->lockSpeakers;
    
    
    for (auto&& it : listLevelComp)
    {
        delete (it);
    }
    listLevelComp.clear();
    
    delete this->oscReceiver;
    
    delete this->boxInputsUI;
    delete this->boxOutputsUI;
    delete this->boxControlUI;
    delete this->speakerView;

#if USE_JACK
    shutdownAudio();
    delete  this->jackClient;
    #endif
}


/*void MainContentComponent::oscMessageReceived	(const OSCMessage & message)
{
    cout << message.begin()->getString() << endl;
}/*
void MainContentComponent::oscBundleReceived	(const OSCBundle & bundle)
{
  
    cout << bundle.begin() << endl;
}*/


void MainContentComponent::addSpeaker(){
    this->lockSpeakers->lock();
    int idNewSpeaker = listSpeaker.size()+1;
    this->listSpeaker.push_back(new Speaker(this, idNewSpeaker, idNewSpeaker, glm::vec3(0.0f, 0.0f, 0.0f)));
    this->listLevelComp.push_back(new LevelComponent(this, &mGrisFeel,idNewSpeaker));
    this->lockSpeakers->unlock();

    updateLevelComp();
}

void MainContentComponent::removeSpeaker(int idSpeaker){

    this->lockSpeakers->lock();
    int index = 0;
    for (auto&& it : this->listSpeaker)
    {
        if(index == idSpeaker){
            delete (it);
            this->listSpeaker.erase(this->listSpeaker.begin() + idSpeaker);

        }
        index+=1;
    }
    index = 0;
    for (auto&& it : this->listLevelComp)
    {
        if(index == idSpeaker){
            delete (it);
            this->listLevelComp.erase(this->listLevelComp.begin()+idSpeaker);
        }
        index+=1;
    }
    this->lockSpeakers->unlock();
    updateLevelComp();

}

void MainContentComponent::updateLevelComp(){
    int x = 2;
    int indexS = 0;
    for (auto&& it : this->listLevelComp)
    {
        it->setOutputLab(String(this->listSpeaker[indexS]->getOutputPatch()));
        juce::Rectangle<int> level(x, 4, sizeWidthLevelComp, 200);
        it->setBounds(level);
        this->boxOutputsUI->getContent()->addAndMakeVisible(it);
        x+=sizeWidthLevelComp;
        indexS+=1;
    }
    if(this->winSpeakConfig != nullptr){
        this->winSpeakConfig->updateWinContent();
    }
    
    this->boxOutputsUI->repaint();
    this->resized();
}

void MainContentComponent::openXmlFileSpeaker(String path)
{
    this->lockSpeakers->lock();
    for (auto&& it : listSpeaker)
    {
        delete (it);
    }
    listSpeaker.clear();
    this->lockSpeakers->unlock();
    
    for (auto&& it : listLevelComp)
    {
        delete (it);
    }
    listLevelComp.clear();

    XmlDocument myDocument (File (path.toStdString()));
    ScopedPointer<XmlElement> mainElement (myDocument.getDocumentElement());
    if (mainElement == nullptr)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon,"Error XML !",
                                          "Your file is corrupted !\n"+myDocument.getLastParseError(),String(),0);
    }
    else
    {
        if(mainElement->hasTagName("SpeakerSetup")){
            nameConfig =  mainElement->getStringAttribute("Name");
            cout << nameConfig << newLine;
            this->speakerView->setNameConfig(nameConfig);
            
            forEachXmlChildElement (*mainElement, ring)
            {
                if (ring->hasTagName ("Ring"))
                {
                    forEachXmlChildElement (*ring, spk)
                    {
                        if (spk->hasTagName ("Speaker"))
                        {
                            
                            this->listSpeaker.push_back(new Speaker(this, spk->getIntAttribute("LayoutIndex"),
                                                              spk->getIntAttribute("OutputPatch"),
                                                              glm::vec3(spk->getDoubleAttribute("PositionX")*10.0f,
                                                                        spk->getDoubleAttribute("PositionZ")*10.0f,
                                                                        spk->getDoubleAttribute("PositionY")*10.0f)));
                 
                            this->listLevelComp.push_back(new LevelComponent(this, &mGrisFeel,spk->getIntAttribute("OutputPatch")));
                        }
                    }
                    
                }
            }
            
        }else{
            AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon,"Error XML !",
                                              "SpeakerSetup not found !",String(),0);
        }
        
    }
    

    updateLevelComp();
}


void MainContentComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
    
}

void MainContentComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {
    bufferToFill.clearActiveBufferRegion();
    
    //sample code taken from juce 4.3.0 audio app example
    //        for (int chan = 0; chan < bufferToFill.buffer->getNumChannels(); ++chan) {
    //            phase = originalPhase;
    //            float* const channelData = bufferToFill.buffer->getWritePointer (chan, bufferToFill.startSample);
    //            for (int i = 0; i < bufferToFill.numSamples ; ++i) {
    //                channelData[i] = amplitude * std::sin (phase);
    //
    //                // increment the phase step for the next sample
    //                phase = std::fmod (phase + phaseDelta, float_Pi * 2.0f);
    //            }
    //        }
}

void MainContentComponent::releaseResources() {
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    
    // For more details, see the help for AudioProcessor::releaseResources()
}

void MainContentComponent::paint (Graphics& g) {
    g.fillAll (mGrisFeel.getWinBackgroundColour());
}


void MainContentComponent::buttonClicked (Button *button)
{
    if(button == this->butLoadXMLSpeakers){
        
        FileChooser fc ("Choose a file to open...",File::getCurrentWorkingDirectory(),"*.xml",true);
        if (fc.browseForFileToOpen())
        {
            String chosen = fc.getResults().getReference(0).getFullPathName();
            int r = AlertWindow::showOkCancelBox (AlertWindow::AlertIconType::WarningIcon,"Open XML !",
                                          "You want to load : "+chosen+"\nEverything not saved will be lost !",
                                                  String(),String(),0);
            //Click OK -> Open xml
            if(r==1){
                this->openXmlFileSpeaker(chosen);
            }
        }
    }else if(button == this->butEditableSpeakers){
    
        if(this->winSpeakConfig == nullptr){
            this->winSpeakConfig = new WindowEditSpeaker("Speakers config", this->mGrisFeel.getWinBackgroundColour(),DocumentWindow::allButtons, this, &this->mGrisFeel);
            
            Rectangle<int> result (this->getScreenX()+ this->speakerView->getWidth()+22,this->getScreenY(),600,480);
            this->winSpeakConfig->setBounds (result);
            this->winSpeakConfig->setResizable (true, true);
            this->winSpeakConfig->setUsingNativeTitleBar (true);
            this->winSpeakConfig->setVisible (true);
            this->winSpeakConfig->setAlwaysOnTop(true);
            
            this->winSpeakConfig->initComp();
            this->winSpeakConfig->repaint();
        }
    }else if(button == butShowSpeakerNumber){
        this->speakerView->setShowNumber(this->butShowSpeakerNumber->getToggleState());
        
    }
}

void MainContentComponent::resized() {
    
    Rectangle<int> r (getLocalBounds().reduced (2));
    
    // lay out the list box and vertical divider..
    Component* vcomps[] = { this->speakerView, this->verticalDividerBar, nullptr };
    
    // lay out side-by-side and resize the components' heights as well as widths
    this->verticalLayout.layOutComponents (vcomps, 3, r.getX(), r.getY(), r.getWidth(), r.getHeight(), false, true);
    

    this->boxInputsUI->setBounds(this->speakerView->getWidth()+6, 2, getWidth()-(this->speakerView->getWidth()+10),240);
    this->boxInputsUI->correctSize(this->boxInputsUI->getWidth()-8, 210);//Not important

    this->boxOutputsUI->setBounds(this->speakerView->getWidth()+6, 244, getWidth()-(this->speakerView->getWidth()+10),240);
    this->boxOutputsUI->correctSize((this->listLevelComp.size()*(sizeWidthLevelComp))+4, 210);
    
    this->boxControlUI->setBounds(this->speakerView->getWidth()+6, 488, getWidth()-(this->speakerView->getWidth()+10), getHeight()-490);
    this->boxControlUI->correctSize(this->boxControlUI->getWidth()-8, 450);
    

}

