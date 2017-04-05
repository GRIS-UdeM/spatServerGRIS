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


#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

//Macro-----------------------
#ifndef USE_JACK
#define USE_JACK 1
#endif
//-----------
#ifndef M_PI
#define M_PI  (3.1415926535897932384626433832795)
#endif
//============================


#include "../JuceLibraryCode/JuceHeader.h"

#include "GrisLookAndFeel.h"

#include "jackClientGRIS.h"
#include "jackServerGRIS.h"
#include "Speaker.h"
#include "SpeakerViewComponent.h"
#include "UiComponent.h"
#include "LevelComponent.h"
#include "OscInput.h"
#include "Input.h"


class spatServerGRISApplication;


using namespace std;

static const unsigned int SizeWidthLevelComp = 36;
static const unsigned int HertzRefreshNormal = 24;
static const unsigned int HertzRefreshLowCpu = 6;


//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class MainContentComponent   :  public Component,
                                public Button::Listener,
                                public TextEditor::Listener,
                                public Slider::Listener,
                                private Timer
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();
    bool exitApp();
    
    vector<Speaker *> getListSpeaker() { return this->listSpeaker; }
    mutex* getLockSpeakers(){ return this->lockSpeakers; }
    
    
    vector<Input *> getListSourceInput(){ return this->listSourceInput; }
    mutex* getLockInputs(){ return this->lockInputs; }
    void updateInputJack(int inInput, Input &inp);
    
    vector<Client> *getListClientjack(){ return &this->jackClient->listClient; }
    void connectionClientJack(String nameCli, bool conn = true) {this->jackClient->connectionClient(nameCli, conn); }
    
    void selectSpeaker(int idS);
    void setNameConfig(String name);
    void setShowShepre(bool value){ this->speakerView->setShowSphere(value); }
    void addSpeaker();
    void savePresetSpeakers(String path);
    void removeSpeaker(int idSpeaker);
    void updateLevelComp();
    void muteInput(int id, bool mute);
    void muteOutput(int id, bool mute);
    
    void soloInput(int id, bool solo);
    void soloOutput(int id, bool solo);
    
    void saveJackSettings(unsigned int rate, unsigned int buff);
    //=======================================================================
    float getLevelsOut(int indexLevel){return (20.0f * log10f(sqrtf(this->jackClient->getLevelsOut(indexLevel))));}
    float getLevelsIn(int indexLevel){return (20.0f * log10f(sqrtf(this->jackClient->getLevelsIn(indexLevel)))); }
    

    void destroyWinSpeakConf() { this->winSpeakConfig = nullptr; }
    void destroyWinJackSetting() { this->winJackSetting = nullptr; }
    //=======================================================================
    void timerCallback() override;
    void paint (Graphics& g) override;
    
    void resized() override;
    void buttonClicked (Button *button) override;
    void sliderValueChanged (Slider *slider) override;
    void textEditorFocusLost (TextEditor &textEditor) override;
    void textEditorReturnKeyPressed (TextEditor &textEditor) override;
    
    
private:

    
    Label*          addLabel(const String &s, const String &stooltip, int x, int y, int w, int h, Component *into);
    TextButton*     addButton(const String &s, const String &stooltip, int x, int y, int w, int h, Component *into);
    ToggleButton*   addToggleButton(const String &s, const String &stooltip, int x, int y, int w, int h, Component *into, bool toggle = false);
    TextEditor*     addTextEditor(const String &s, const String &emptyS, const String &stooltip, int x, int y, int w, int h, Component *into, int wLab = 80);
    Slider*         addSlider(const String &s, const String &stooltip, int x, int y, int w, int h, Component *into);
    
    void openXmlFileSpeaker(String path);
    void openPreset(String path);
    void savePreset(String path);
    
    void updateSkeapersConf();
    
    ApplicationProperties applicationProperties;
    //==============================================================================
    #if USE_JACK
    jackClientGris *jackClient;
    jackServerGRIS *jackServer;
    #endif
    
    
    vector<Speaker *> listSpeaker;
    mutex *lockSpeakers;

    String nameConfig;
    String pathCurrentFileSpeaker;
    String pathCurrentPreset;
    
    OscInput * oscReceiver;
    vector<Input *> listSourceInput;
    mutex *lockInputs;

    //UI Components---------------------------
    TooltipWindow tooltipWindow;
    StretchableLayoutManager verticalLayout;
    ScopedPointer<StretchableLayoutResizerBar> verticalDividerBar;
    GrisLookAndFeel mGrisFeel;
    SpeakerViewComponent *speakerView;
    
    WindowEditSpeaker* winSpeakConfig;
    WindowJackSetting* winJackSetting;
    //3 Main Box---------------------
    Box * boxInputsUI;
    Box * boxOutputsUI;
    Box * boxControlUI;
    
    //Component in Box 3 -----------
    Label *         labelJackStatus;
    Label *         labelJackLoad;
    Label *         labelJackRate;
    Label *         labelJackBuffer;
    Label *         labelJackInfo;
    
    TextButton *    butLoadXMLSpeakers;
    TextButton *    butEditableSpeakers;
    TextButton *    butLoadPreset;
    TextButton *    butSavePreset;
    TextButton *    butDefaultColorIn;
    TextButton *    butJackParam;
    
    ToggleButton *  butShowSpeakerNumber;
    ToggleButton *  butHighPerformance;
    ToggleButton *  butNoiseSound;
    
    Slider *        sliderMasterGainOut;
    
    TextEditor *    tedOSCInPort;
    Label *         labOSCStatus;
    
    TextEditor *    tedAddInputs;
    TextButton *    butAutoConnectJack;
    
    Label *         labelAllClients;
    BoxClient *     boxClientJack;
    
    SplashScreen *  splash;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED

