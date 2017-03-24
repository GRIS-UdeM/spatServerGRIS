//
//  LevelComponent.h
//  spatServerGRIS
//
//  Created by GRIS on 2017-03-13.
//
//

#ifndef LevelComponent_h
#define LevelComponent_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "GrisLookAndFeel.h"
#include "UiComponent.h"
#include "ParentLevelComponent.h"

static const float MinLevelComp  = -60.f;
static const float MaxLevelComp  = 1.f;
static const float MaxMinLevComp = MaxLevelComp - MinLevelComp;
static const int   WidthRect     = 2;

//======================================= LevelBox ===================================
class LevelBox : public Component
{
public:
    LevelBox(LevelComponent* parent, GrisLookAndFeel *feel);
    ~LevelBox();
    
    void setBounds(const Rectangle<int> &newBounds);
    void paint (Graphics& g);
    
private:
    LevelComponent *mainParent;
    GrisLookAndFeel *grisFeel;
    ColourGradient colorGrad;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelBox)
};

//====================================================================================
class LevelComponent : public Component, public ToggleButton::Listener
{
public:
    LevelComponent(ParentLevelComponent * parent, GrisLookAndFeel *feel);
    ~LevelComponent();
    
    void setOutputLab(String value) { this->labId->setText(value, dontSendNotification); }
    float getLevel();
    void update();
    bool isMuted();
    void setSelected(bool value);
    void buttonClicked(Button *button);
    void setBounds(const Rectangle<int> &newBounds);

private:
    ParentLevelComponent* mainParent;
    LevelBox * levelBox;
    Label * labId;
    ToggleButton * muteToggleBut;
    ToggleButton * soloToggleBut;
    GrisLookAndFeel * grisFeel;
    float level = -100.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelComponent)
};
#endif /* LevelComponent_h */
