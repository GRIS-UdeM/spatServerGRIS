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


#ifndef Input_h
#define Input_h

#include <stdio.h>
#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>

#include "../glm/glm.hpp"

#include "../JuceLibraryCode/JuceHeader.h"

#include "GrisLookAndFeel.h"
#include "ParentLevelComponent.h"


class MainContentComponent;
class LevelComponent;

using namespace std;


class Input :   public ParentLevelComponent
{

public :
    Input(MainContentComponent * parent, GrisLookAndFeel * feel, int id = 0);
    ~Input();
    
    //ParentLevelComponent ===============================
    int getId(){ return this->idChannel ;};
    float getLevel();
    void setMuted(bool mute);
    void setSolo(bool solo);
    void setColor(Colour color, bool updateLevel = false);
    void selectClick(){};
    LevelComponent * getVuMeter(){ return this->vuMeter; }
    
    glm::vec3 getCenter();
    glm::vec3 getColor();
    float getAziMuth(){ return this->azimuth; }
    float getZenith(){ return this->zenith; }
    float getRad(){ return 10.0f; }
    
    void draw();
    void updateValues(float az, float ze, float azS, float zeS, float g);
    
private:
    int idChannel;
    
    float azimuth;
    float zenith;
    float azimSpan;
    float zeniSpan;
    float gain;
    
    float sizeT = 0.5f;
    glm::vec3 center;
    glm::vec3 color;
    
    LevelComponent *vuMeter;
    MainContentComponent * mainParent;
    GrisLookAndFeel * grisFeel;
};
#endif /* Input_h */