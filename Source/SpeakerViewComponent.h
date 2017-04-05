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

#ifndef SPEAKERVIEWCOMPONENT_H_INCLUDED
#define SPEAKERVIEWCOMPONENT_H_INCLUDED

#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>

#include "../JuceLibraryCode/JuceHeader.h"

#include "../glm/glm.hpp"

#include "Ray.h"


class MainContentComponent;
class Speaker;

using namespace std;

static const int NbrGridLines = 16;


class SpeakerViewComponent : public OpenGLAppComponent {
public:
    //==============================================================================
    SpeakerViewComponent(MainContentComponent *parent = nullptr);
    
    ~SpeakerViewComponent();
    
    void initialise() override;
    
    void shutdown() override;
    
    void setShowSphere(bool value){ this->showShpere = value; }
    void setShowNumber(bool value){ this->showNumber = value; }
    void setHighPerfor(bool value){ this->highPerf = value; }
    
    void setNameConfig(String name){ this->nameConfig = name; this->repaint(); }
    void render() override;
    
    void paint (Graphics& g) override;
    void resized() override;
    
    void mouseDown(const MouseEvent& e)override;
    void mouseDrag (const MouseEvent& e)override;
    
    void mouseWheelMove(const MouseEvent& e,const MouseWheelDetails& wheel)override;
    
    
private:
    
    float raycast(Speaker *speaker);
    bool speakerNearCam(glm::vec3 speak1, glm::vec3 speak2, glm::vec3 cam);
    
    void clickRay();
    void drawBackground();
    void drawOriginGrid();
    void drawText( string val, glm::vec3 position,float scale = 0.005f, bool camLock = true);
    void drawCube(float x, float y, float z);
    
    bool showShpere = false;
    bool showNumber = false;
    bool highPerf   = false;
    bool clickLeft  = false;
    
    float camAngleX= 30.0f;
    float camAngleY= 25.0f;
    float distance = 15.0f;
    
    float deltaClickX;
    float deltaClickY;
    
    double rayClickX;
    double rayClickY;
    
    Ray ray;
    MainContentComponent *mainParent;
    glm::vec3 camPos;
    glm::vec4 perspectivCam;

    String nameConfig = "...";
    
    GLdouble xS, yS, zS = 0;
    GLdouble xE, yE, zE = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpeakerViewComponent)
};

#endif  // SPEAKERVIEWCOMPONENT_H_INCLUDED