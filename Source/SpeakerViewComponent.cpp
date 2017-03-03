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


#include "SpeakerViewComponent.h"

//==========================================================================================
// ACTUAL SPEAKER VIEW CLASS DEFS
//==========================================================================================
SpeakerViewComponent::SpeakerViewComponent() {
    setSize(400, 400);

    this->listSpeaker = std::vector<Speaker3D>();
    
    
    this->listSpeaker.push_back(Speaker3D(glm::vec3(5,0.5,0)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(5,3,0)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(4,6,0)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(3,8,0)));
    
    
    this->listSpeaker.push_back(Speaker3D(glm::vec3(-5,0.5,0)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(-5,3,0)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(-4,6,0)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(-3,8,0)));
    
    this->listSpeaker.push_back(Speaker3D(glm::vec3(0,0.5,5)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(0,3,5)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(0,6,4)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(0,8,3)));

    this->listSpeaker.push_back(Speaker3D(glm::vec3(0,0.5,-5)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(0,3,-5)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(0,6,-4)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(0,8,-3)));
    
    
    
    this->listSpeaker.push_back(Speaker3D(glm::vec3(5,0.5,5)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(5,3,5)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(4,6,4)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(3,8,3)));
    
    this->listSpeaker.push_back(Speaker3D(glm::vec3(5,0.5,-5)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(5,3,-5)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(4,6,-4)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(3,8,-3)));
    
    
    
    this->listSpeaker.push_back(Speaker3D(glm::vec3(-5,0.5,5)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(-5,3,5)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(-4,6,4)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(-3,8,3)));
    
    this->listSpeaker.push_back(Speaker3D(glm::vec3(-5,0.5,-5)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(-5,3,-5)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(-4,6,-4)));
    this->listSpeaker.push_back(Speaker3D(glm::vec3(-3,8,-3)));
    
    
    
}

SpeakerViewComponent::~SpeakerViewComponent() {
    shutdownOpenGL();
}

void SpeakerViewComponent::initialise() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, 16.0/9.0, 1, 40);
    

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(4, 6, 5, 0, 0, 0, 0, 1, 0);
}

void SpeakerViewComponent::shutdown() {
    
}



void SpeakerViewComponent::render() {
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    drawBackground();
    
    gluPerspective(90.0, 16.0/9.0, 0.1f, 60.0f);
    glMatrixMode(GL_MODELVIEW);

    float camX = distance * sinf(camAngleX*(M_PI/180.f)) * cosf((camAngleY)*(M_PI/180.f));
    float camY = distance * sinf((camAngleY)*(M_PI/180.f));
    float camZ = distance * cosf((camAngleX)*(M_PI/180.f)) * cosf((camAngleY)*(M_PI/180.f));
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, 0, 0, 0, 0,1,0);

    
    drawOriginGrid();
    
    for(int i = 0; i < this->listSpeaker.size(); ++i) {
        this->listSpeaker[i].draw();
    }
    
    glFlush();
}

void SpeakerViewComponent::paint (Graphics& g) {
//    // You can add your component specific drawing code here!
//    // This will draw over the top of the openGL background.
//    
    g.setColour(Colours::white);
    g.setFont (16);
    g.drawText ("3D View", 25, 20, 300, 30, Justification::left);
//    g.drawLine (20, 20, 170, 20);
//    g.drawLine (20, 50, 170, 50);
}

void SpeakerViewComponent::resized() {
    //draggableOrientation.setViewport (getLocalBounds());
}


void SpeakerViewComponent::mouseDown (const MouseEvent& e) {

    for(int i = 0; i < this->listSpeaker.size(); ++i) {
        this->listSpeaker[i].unSelectSpeaker();
    }
    
    double matModelView[16], matProjection[16];
    int viewport[4];
    
    glGetDoublev( GL_MODELVIEW_MATRIX, matModelView );
    glGetDoublev( GL_PROJECTION_MATRIX, matProjection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    double winX = (double)e.getPosition().x;
    double winY = viewport[3] - (double)e.getPosition().y;
	
    GLdouble xS, yS, zS;
    GLdouble xE, yE, zE;

    gluUnProject(winX, winY, 0.0, matModelView, matProjection,viewport, &xS, &yS,&zS);
    gluUnProject(winX, winY, 1.0, matModelView, matProjection, viewport, &xE, &yE, &zE);
    
    r = Ray(glm::vec3(xS, yS, zS),glm::vec3(xE, yE, zE));
    
    for(int i = 0; i < this->listSpeaker.size(); ++i) {
        if (ToolsGL::Raycast(r, this->listSpeaker[i]) != -1 ) {
            this->listSpeaker[i].selectSpeaker();
        }
    }
}

void SpeakerViewComponent::mouseDrag (const MouseEvent& e) {
    if(e.mods.isRightButtonDown()){
        camAngleX = e.getPosition().x;
        camAngleY = e.getPosition().y;
    }
}

void SpeakerViewComponent::mouseWheelMove(const MouseEvent& e,const MouseWheelDetails& wheel){
    distance -= (wheel.deltaY*1.5f);
}

void SpeakerViewComponent::drawBackground()
{
    glMatrixMode(GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0,1,0,1,-1,1);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //draw 2D image
    glBegin(GL_QUADS);
    glColor3f(0.48,0.55,0.63);
    glVertex2f(1.0,1.0);
    glVertex2f(-1.0,1.0);
    
    glColor3f(0.0,0.0,0.0);
    glVertex2f(0.0, 0.0);
    glVertex2f(1.0, 0.0);
    glEnd();
    
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void SpeakerViewComponent::drawOriginGrid()
{
    
    
    //Draw circle------------------------------
    glLineWidth(1.5f);
    
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    for(int i =0; i <= 180; i++){
        double angle = (2 * M_PI * i / 180);
        glVertex3f(cos(angle)*1.0f, 0.0f, sin(angle)*1.0f);
    }
    glEnd();
    
    glBegin(GL_LINE_LOOP);
    for(int i =0; i <= 180; i++){
        double angle = (2 * M_PI * i / 180);
        glVertex3f(cos(angle)*5.0f, 0.0f, sin(angle)*5.0f);
    }
    glEnd();
    
    glBegin(GL_LINE_LOOP);
    for(int i =0; i <= 180; i++){
        double angle = (2 * M_PI * i / 180);
        glVertex3f(cos(angle)*10.0f, 0.0f, sin(angle)*10.0f);
    }
    glEnd();
    
    
    //3D RGB line
    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(0.4, 0, 0); glVertex3f(-10, 0, 0); glVertex3f(0, 0, 0);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(10, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 10, 0);
    glColor3f(0, 0, 0.4); glVertex3f(0, 0, -10); glVertex3f(0, 0, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 10);
    glEnd();

    
    glLineWidth(1);
    
    //Grid-----------------------------
    glColor3f(0.49, 0.49, 0.49);
    for(int x = -12; x < 13; x+=2){
        if(x != 0){
            glBegin(GL_LINE_LOOP);
            glVertex3f(x,0,-12);
            glVertex3f(x,0,12);
            glEnd();
        }
    };
    
    for(int z = -12; z < 13; z+=2){
        if(z != 0){
            glBegin(GL_LINE_LOOP);
            glVertex3f(-12,0,z);
            glVertex3f(12,0,z);
            glEnd();
        }
    };
}


void SpeakerViewComponent::drawCube(float x, float y, float z)

{
    const float sizex = 0.5f;
    const float sizey = 0.5f;
    const float sizez = 0.5f;
    
    glTranslatef(x, y, z);
    
    glBegin(GL_QUADS);
    
    glColor3f(0.85, 0.86, 0.87);
    
    // FRONT
    glVertex3f(-sizex, -sizey, sizez);
    glVertex3f(sizex, -sizey, sizez);
    glVertex3f(sizex, sizey, sizez);
    glVertex3f(-sizex, sizey, sizez);
    
    // BACK
    glVertex3f(-sizex, -sizey, -sizez);
    glVertex3f(-sizex, sizey, -sizez);
    glVertex3f(sizex, sizey, -sizez);
    glVertex3f(sizex, -sizey, -sizez);
    
    // LEFT
    glVertex3f(-sizex, -sizey, sizez);
    glVertex3f(-sizex, sizey, sizez);
    glVertex3f(-sizex, sizey, -sizez);
    glVertex3f(-sizex, -sizey, -sizez);
    
    // RIGHT
    glVertex3f(sizex, -sizey, -sizez);
    glVertex3f(sizex, sizey, -sizez);
    glVertex3f(sizex, sizey, sizez);
    glVertex3f(sizex, -sizey, sizez);
    
    // TOP
    glVertex3f(-sizex, sizey, sizez);
    glVertex3f(sizex, sizey, sizez);
    glVertex3f(sizex, sizey, -sizez);
    glVertex3f(-sizex, sizey, -sizez);
    
    // BOTTOM
    glVertex3f(-sizex, -sizey, sizez);
    glVertex3f(-sizex, -sizey, -sizez);
    glVertex3f(sizex, -sizey, -sizez);
    glVertex3f(sizex, -sizey, sizez);
    
    glEnd();
    
    glTranslatef(-x, -y, -z);
}
