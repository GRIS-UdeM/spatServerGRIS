/*
 This file is part of SpatGRIS2.

 Developers: Samuel Béland, Olivier Bélanger, Nicolas Masson

 SpatGRIS2 is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 SpatGRIS2 is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with SpatGRIS2.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "JackClientGRIS.h"
#include "macros.h"

DISABLE_WARNINGS
#include <JuceHeader.h>
ENABLE_WARNINGS

#include "Box.h"

class EditableTextCustomComponent;
class MainContentComponent;
class GrisLookAndFeel;

//==============================================================================
class EditSpeakersWindow final
    : public juce::DocumentWindow
    , public juce::TableListBoxModel
    , public juce::ToggleButton::Listener
    , public juce::TextEditor::Listener
    , public juce::Slider::Listener
{
    MainContentComponent & mMainContentComponent;
    GrisLookAndFeel & mLookAndFeel;

    Box mListSpeakerBox;

    juce::TextButton mAddSpeakerButton;
    juce::TextButton mCompSpeakersButton;

    juce::Label mNumOfSpeakersLabel;
    juce::TextEditor mNumOfSpeakersTextEditor;
    juce::Label mZenithLabel;
    juce::TextEditor mZenithTextEditor;
    juce::Label mRadiusLabel;
    juce::TextEditor mRadiusTextEditor;
    juce::Label mOffsetAngleLabel;
    juce::TextEditor mOffsetAngleTextEditor;
    juce::TextButton mAddRingButton;

    juce::ToggleButton mPinkNoiseToggleButton;
    juce::Slider mPinkNoiseGainSlider;

    juce::TableListBox mSpeakersTableListBox;
    juce::Font mFont;

    int mNumRows{};
    bool mInitialized{ false };
    //==============================================================================
    friend EditableTextCustomComponent; // TODO: temporary solution whiling refactoring is going on...
public:
    //==============================================================================
    EditSpeakersWindow(juce::String const & name,
                       GrisLookAndFeel & lookAndFeel,
                       MainContentComponent & mainContentComponent,
                       juce::String const & configName);
    //==============================================================================
    EditSpeakersWindow() = delete;
    ~EditSpeakersWindow() override = default;

    EditSpeakersWindow(EditSpeakersWindow const &) = delete;
    EditSpeakersWindow(EditSpeakersWindow &&) = delete;

    EditSpeakersWindow & operator=(EditSpeakersWindow const &) = delete;
    EditSpeakersWindow & operator=(EditSpeakersWindow &&) = delete;
    //==============================================================================
    void initComp();
    void selectedRow(int value);
    void updateWinContent();

private:
    //==============================================================================
    ModeSpatEnum getModeSelected() const;
    bool getDirectOutForSpeakerRow(int row) const;
    juce::String getText(int columnNumber, int rowNumber) const;
    void setText(int columnNumber, int rowNumber, juce::String const & newText, bool altDown = false);
    //==============================================================================
    // VIRTUALS
    int getNumRows() override { return this->mNumRows; }
    void buttonClicked(juce::Button * button) override;
    void textEditorTextChanged(juce::TextEditor & editor) override;
    void textEditorReturnKeyPressed(juce::TextEditor & textEditor) override;
    void closeButtonPressed() override;
    void sliderValueChanged(juce::Slider * slider) override;
    void sortOrderChanged(int newSortColumnId, bool isForwards) override;
    void resized() override;
    void paintRowBackground(juce::Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    Component * refreshComponentForCell(int rowNumber,
                                        int columnId,
                                        bool isRowSelected,
                                        Component * existingComponentToUpdate) override;
    //==============================================================================
    JUCE_LEAK_DETECTOR(EditSpeakersWindow)
};
