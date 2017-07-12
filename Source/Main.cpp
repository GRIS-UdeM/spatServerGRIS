/*
 This file is part of spatServerGRIS.
 
 Developers: Nicolas Masson
 
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


#include "../JuceLibraryCode/JuceHeader.h"
#include "GrisLookAndFeel.h"

#define STRING2(x) #x
#define STRING(x) STRING2(x)

#include "MainComponent.h"


//==============================================================================
class spatServerGRISApplication  : public JUCEApplication
{
public:
    //==============================================================================
    spatServerGRISApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        LookAndFeel::setDefaultLookAndFeel(&mGrisFeel);
        // This method is where you should put your application's initialisation code..
        mainWindow = new MainWindow (getApplicationName());
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        if(mainWindow->exitWinApp()){
            quit();
        }
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainContentComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,Colours::lightgrey,DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            String version = STRING(JUCE_APP_VERSION);
            version = "SpatServer GRIS : "+version;
            setName(version);
            
            // Copy JackRouter ini file to preferences folder if not already done.
#ifndef __linux__
            File target = File("~/Library/Preferences/JAS.jpil");
            File source = File(File::getCurrentWorkingDirectory().getFullPathName()+("/spatServerGRIS.app/Contents/Resources/JAS.jpil"));
            if (target.existsAsFile() == false) {
                source.copyFileTo(target);
            }
#endif
            mcc = new MainContentComponent();
            setContentOwned (mcc, true);
            setResizable (true, true);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);
        }
        
        bool exitWinApp(){
            return mcc->exitApp();
        }
        void closeButtonPressed() override
        {
            
            JUCEApplication::getInstance()->systemRequestedQuit();
            
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        MainContentComponent * mcc;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    GrisLookAndFeel mGrisFeel;
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (spatServerGRISApplication)
