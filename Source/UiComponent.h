//
//  UiComponent.h
//  spatServerGRIS
//
//  Created by GRIS on 2017-03-09.
//
//

#ifndef UiComponent_h
#define UiComponent_h

#include <iostream>

#include "../JuceLibraryCode/JuceHeader.h"
#include "GrisLookAndFeel.h"

class MainContentComponent;
class LevelComponent;

using namespace std;

//======================================= BOX ========================================
class Box : public Component
{
public:
    Box(GrisLookAndFeel *feel, String title="");
    ~Box();
    
    Component * getContent();
    void resized();
    void correctSize(int width, int height);
    void paint(Graphics &g) ;
    
private:
    Component *content;
    Viewport *viewport;
    GrisLookAndFeel *grisFeel;
    Colour bgColour;
    String title;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Box)
};


//======================================= Window Edit Speaker===========================
class WindowEditSpeaker :   public DocumentWindow,
                            public TableListBoxModel,
                            public ToggleButton::Listener
{
public:
    WindowEditSpeaker(const String& name, Colour backgroundColour, int buttonsNeeded, MainContentComponent * parent, GrisLookAndFeel * feel);
    ~WindowEditSpeaker();
    
    void updateWinContent();
    void selectedRow(int value){ this->tableListSpeakers.selectRow(value); this->repaint();}
    
    void initComp();
    void buttonClicked(Button *button);
    void closeButtonPressed();
    
    String getText (const int columnNumber, const int rowNumber) const;
    void setText (const int columnNumber, const int rowNumber, const String& newText);
    int getNumRows() override;
    void paintRowBackground (Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
    void paintCell (Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool /*rowIsSelected*/) override;
    Component* refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                        Component* existingComponentToUpdate) override;
    
private:
    
    MainContentComponent *mainParent;
    GrisLookAndFeel *grisFeel;
    Box * boxListSpeaker;
    
    Label *labColumn;
    ToggleButton *toggleShowSphere;
    TextButton *butAddSpeaker;
    
    TableListBox tableListSpeakers;
    Font font;
    int numRows;
    
    
    class EditableTextCustomComponent  : public Label
    {
    public:
        EditableTextCustomComponent (WindowEditSpeaker& td)  : owner (td)
        {
            setEditable (false, true, false);
            setColour (textColourId, Colours::black);
        }
        void mouseDown (const MouseEvent& event) override
        {
            owner.tableListSpeakers.selectRowsBasedOnModifierKeys (row, event.mods, false);
            Label::mouseDown (event);
        }
        void textWasEdited() override
        {
            owner.setText (columnId, row, getText());
        }
       void setRowAndColumn (const int newRow, const int newColumn)
        {
            row = newRow;
            columnId = newColumn;
            setText (owner.getText(columnId, row), dontSendNotification);
        }
    private:
        WindowEditSpeaker& owner;
        int row, columnId;
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WindowEditSpeaker)
};


#endif /* UiComponent_h */

