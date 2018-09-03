#include<iostream>
#include<cmath>
#include<vector>
#include<gl/glut.h>
using namespace std;

const float RATIO = 1.0;
const int WIDTH = 600;
const int HEIGHT = WIDTH / RATIO;
const double PI = 3.14159265358979323846;
float viewAngleUD = 0;//view angles
float viewAngleLR = 0;
float pos[] = { 1,1,1,1 }, amb1[] = { 1,1,1,1.0 }, amb2[] = { 0.5,0.5,0.5,1 };
float back_amb_diff[4] = { 0.3,0.5,0.7,1.0 };
float theta = 0;//rotate angle of planet
float RADIUS = 1000;//radius of the planet
float viewR = 2100;
float defaultD = 3900;//default z-position of camera
float xo = 0, yo = RADIUS + 10, zo = defaultD;//camera position
float xd = 0, yd = RADIUS + 10, zd = 0;
float speed = 0;//speed of the car
float dspeed = 0;

//base component class
//only has draw method
//every other class is derived from component
class Component {
public:
    virtual void draw() = 0;
};
//the sun
class Sun :Component {
    float c[4] = { 255.0f / 255,200.0f / 255,50.0f / 255,0.5 };
    float spe[4] = { 1,1,1,1.0 };
public:
    void draw()override {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 90);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);
        glutSolidSphere(RADIUS*0.2, 100, 100);
    }
};
class Helicopter :Component {
private:
    float c1[4] = { 100.0f / 255,100.0f / 255,100.0f / 255,1.0 };
    float c2[4] = { 230.0f / 255,116.0f / 255,50.0f / 255,1.0 };
    double propeller[3];//size of propeller
    float angle;//rotate angle
    GLUquadric * quad;
    double body[3];//size
public:
    Helicopter() {
        propeller[0] = 8;
        propeller[1] = 50;
        propeller[2] = 3;
        angle = 10;
        quad = gluNewQuadric();
        body[1] = 30;
        body[0] = 30;
        body[2] = 20;
    }
    ~Helicopter() {
        gluDeleteQuadric(quad);
    }
    float getAngle() {
        return angle;
    }
    void setAngle(float a) {
        angle = a;
    }
    void draw()override {
        glRotated(90, 1, 0, 0);
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c1);
        glPushMatrix();
        glutSolidTorus(3, 1, 20, 30);
        
        glRotated(angle, 0, 0, 1);
        for (int i = 0; i < 4; i++) {
            glPushMatrix();
            glRotated(-90 * i, 0, 0, 1);//rotate the propellers
            glTranslated(0, propeller[1] / 2, propeller[2] / 2);
            glScaled(propeller[0], propeller[1], propeller[2]);
            glutSolidCube(1);
            glPopMatrix();
        }
        
        glPopMatrix();
        
        glPushMatrix();
        gluCylinder(quad, 3, 3, 15, 30, 30);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c2);
        glTranslated(0, 0, body[2]);
        glScaled(body[0], body[1], body[2]);
        glutSolidCube(1);// draw the body
        glPopMatrix();
        glPopMatrix();
    }
};
class Tree :public Component {
private:
    GLUquadric * quad;
    float c1[4] = { 66.0f / 255,39.0f / 255,33.0f / 255,1.0 };
    float c2[4] = { 9.0f / 255,120.0f / 255,30.0f / 255,1.0 };
    float c0[4] = { 9.0f / 255,120.0f / 255,30.0f / 255,1.0 };
    float spe[4] = { 0.35f,0.35f,0.35f,1.0 };
    
public:
    Tree() {
        quad = gluNewQuadric();
    }
    void draw()override {
        
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c1);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
        glRotated(90, 1, 0, 0);
        gluCylinder(quad, 3, 3, 30, 50, 100);
        glPopMatrix();
        
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c2);
        glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, c0);
        glRotated(-90, 1, 0, 0);
        glutSolidCone(30, 15, 20, 30);
        glPopMatrix();
        
        glPushMatrix();
        glTranslated(0, 12, 0);
        glRotated(-90, 1, 0, 0);
        glutSolidCone(20, 15, 20, 30);
        glPopMatrix();
        
        glPushMatrix();
        glTranslated(0, 25, 0);
        glRotated(-90, 1, 0, 0);
        glutSolidCone(10, 10, 20, 30);
        glPopMatrix();
    }
    ~Tree() {
        gluDeleteQuadric(quad);
    }
};

class Planet :public Component {
    float front_amb_diff[4] = { 135.0f / 255,206.0f / 255,98.0f / 255,1.0 };
    float spe[4] = { 0.10f,0.10f,0.10f,1.0 };
    Tree t;
    Helicopter h1;
    float angle;
    float helicopterSpeed = 0;
public:
    Planet() {
        angle = 0;
    }
    float getAngle() {
        return angle;
    }
    void setAngle(float a) {
        angle = a;
        h1.setAngle(a);
    }
    float getSpeed() {
        return helicopterSpeed;
    }
    void setSpeed(float s) {
        helicopterSpeed = s;
    }
    void draw() override {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_amb_diff);
        glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, back_amb_diff);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
        
        glPushMatrix();
        glutSolidSphere(RADIUS, 800, 1000);
        glPopMatrix();
        //draw trees
        for (int j = 6; j < 67; j += 15) {
            int i = 0;
            while (i < 360) {
                glPushMatrix();
                glRotated(j, cos(i*PI / 180), sin(i*PI / 180), 0);
                glRotated(i, 0, 0, 1);
                glTranslated(0, RADIUS + 30, 0);
                t.draw();
                glPopMatrix();
                glPushMatrix();
                
                glRotated(-j, cos(i*PI / 180), sin(i*PI / 180), 0);
                glRotated(i, 0, 0, 1);
                glTranslated(0, RADIUS + 30, 0);
                t.draw();
                
                glPopMatrix();
                i += j;
            }
        }
        //Helicopter 1
        glPushMatrix();
        glRotated(helicopterSpeed, 1, 0, 0);
        glTranslated(0, RADIUS + 150, 0);
        h1.draw();
        glPopMatrix();
        //Helicopter 2
        glPushMatrix();
        glRotated(helicopterSpeed * 2, 0, 1, 0);
        glRotated(-90, 0, 0, 1);
        glTranslated(0, RADIUS + 250, 0);
        h1.draw();
        glPopMatrix();
        //Helicopter 3
        glPushMatrix();
        glRotated(helicopterSpeed * 3, 0, 0, 1);
        glRotated(-90, 0, 1, 0);
        glTranslated(0, RADIUS + 350, 0);
        h1.draw();
        glPopMatrix();
    }
};
class Pipe :public Component {
    GLUquadric * q;
public:
    void draw()override {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c1);
        q = gluNewQuadric();
        glPushMatrix();
        glRotated(90, 1, 0, 0);
        gluCylinder(q, 1, 1, 12, 50, 100);
        glPopMatrix();
        
    }
    ~Pipe() {
        gluDeleteQuadric(q);
    }
private:
    float c1[4] = { 120.0f / 255,120.0f / 255,120.0f / 255,1.0 };
    float spe[4] = { 0.10f,0.10f,0.10f,1.0 };
};
class Wheel :public Component {
public:
    
    Wheel() {
        _angle = 0;
    }
    void draw()override {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c1);
        glutSolidTorus(1, 2, 50, 100);
        for (int i = 0; i < 360; i += 40) {
            glPushMatrix();
            glRotated(_angle, 0, 0, 1);
            glRotated(i, 0, 0, 1);
            Pipe p;
            p.draw();
            glPopMatrix();
        }
        glutSolidTorus(1, 12, 50, 100);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c2);
        glutSolidTorus(2, 14, 50, 100);
    }
    void running(float a) {
        _angle = a;
    }
private:
    float _angle;
    float c1[4] = { 211.0f / 255,211.0f / 255,211.0f / 255,1.0 };
    float c2[4] = { 3.0f / 255,3.0f / 255,3.0f / 255,1.0 };
    float spe[4] = { 0.10f,0.10f,0.10f,1.0 };
};
class BodyPart :public Component {
public:
    BodyPart(float w, float h, float t) {
        width = w;
        height = h;
        thick = t;
    }
    void draw()override {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 70);
        glPushMatrix();
        glScaled(width, height, thick);
        glutSolidCube(1);
        glPopMatrix();
    }
private:
    float width, height, thick;
    float c[4] = { 139.0f / 255,170.0f / 255,230.0f / 255,1.0 };
    float spe[4] = { 0.20f,0.20f,0.20f,1.0 };
};
class Car :public Component {
    Wheel wheel;
    Pipe p;
    float angle = 0;
public:
    void setAngle(float a) {
        angle = a;
    }
    float getAngle() {
        return angle;
    }
    void draw()override {
        glPushMatrix();
        glTranslated(0, 0, -50);
        glRotated(90, 0, 0, 1);
        glScaled(1, 6.5, 1);
        p.draw();
        glPopMatrix();
        
        glPushMatrix();
        glTranslated(0, 0, 50);
        glRotated(90, 0, 0, 1);
        glScaled(1, 6.5, 1);
        p.draw();
        glPopMatrix();
        
        wheel.running(angle);
        
        glPushMatrix();
        glTranslated(0, 0, -50);
        glRotated(90, 0, 1, 0);
        wheel.draw();
        glPopMatrix();
        
        
        glPushMatrix();
        glTranslated(80, 0, -50);
        glRotated(90, 0, 1, 0);
        wheel.draw();
        glPopMatrix();
        
        glPushMatrix();
        glTranslated(0, 0, 50);
        glRotated(90, 0, 1, 0);
        wheel.draw();
        glPopMatrix();
        
        glPushMatrix();
        glTranslated(80, 0, 50);
        glRotated(90, 0, 1, 0);
        wheel.draw();
        glPopMatrix();
        
        glPushMatrix();
        glTranslated(40, 3, 0);
        glRotated(-90, 0, 0, 1);
        glRotated(-90, 0, 1, 0);
        BodyPart p1(125, 65, 10);
        p1.draw();
        glPopMatrix();
        
        glPushMatrix();
        glTranslated(40, 20, 0);
        glRotated(-90, 0, 0, 1);
        glRotated(-90, 0, 1, 0);
        p1.draw();
        glPopMatrix();
        
        glPushMatrix();
        glTranslated(11, 10, 0);
        glRotated(-90, 0, 1, 0);
        BodyPart p2(125, 30, 8);
        p2.draw();
        glPopMatrix();
        
        glPushMatrix();
        glTranslated(70, 10, 0);
        glRotated(-90, 0, 1, 0);
        p2.draw();
        glPopMatrix();
        
        glPushMatrix();
        glTranslated(43, 12, -58);
        glRotated(-90, 1, 0, 0);
        BodyPart p3(60, 10, 25);
        p3.draw();
        
        glPopMatrix();
        glPushMatrix();
        glTranslated(43, 12, 58);
        glRotated(-90, 1, 0, 0);
        p3.draw();
        glPopMatrix();
        
        glPushMatrix();
        glTranslated(40, 30, 0);
        glRotated(-90, 0, 0, 1);
        glRotated(-90, 0, 1, 0);
        BodyPart p4(75, 65, 20);
        p4.draw();
        glPopMatrix();
        
    }
};
//use cubes to draw 4 letters: 'Z','C','A','I'
class Name :public Component {
    float front_amb_diff[4] = { 0.7f,0.5f,0.1f,1.0f };
    float spec[4] = { 0.25f,0.25f,0.25f };
    float angle = 0;
    float dt[52];
    float distance[52];
    int direct[52];
public:
    Name() {
        for (int i = 0; i < 52; i++) {
            dt[i] = rand() % 3;
            distance[i] = 0;
            direct[i] = rand() == 0 ? -1 : 1;
        }
    }
    void setAngle(float a) {
        angle = a;
    }
    
    void draw()override {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_amb_diff);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 75);
        glPushMatrix();
        glTranslated(-120, 0, 0);
        glPushMatrix();
        glRotated(angle, 1, 1, 1);
        int index = 0;
        //'Z'
        for (int i = 0; i < 6; i++) {
            glPushMatrix();
            if (angle <= 180) {
                distance[index] += dt[index] * direct[index];
            }
            else {
                distance[index] -= dt[index] * direct[index];
            }
            glTranslated(0, -distance[index], distance[index]);
            index++;
            glRotated(angle, 0.5, 0, -1);
            glTranslated(10 * i, 0, 0);
            glutSolidCube(5);
            glPopMatrix();
        }
        
        for (int i = 0; i < 6; i++) {
            glPushMatrix();
            if (angle <= 180) {
                distance[index] += dt[index] * direct[index];
            }
            else {
                distance[index] -= dt[index] * direct[index];
            }
            glTranslated(0, 0, distance[index]);
            glRotated(-angle, 0, 1, -1);
            glTranslated(10 * i, 50, 0);
            
            index++;
            glutSolidCube(5);
            glPopMatrix();
        }
        for (int i = 0; i < 6; i++) {
            glPushMatrix();
            if (angle <= 180) {
                distance[index] += dt[index] * direct[index];
            }
            else {
                distance[index] -= dt[index] * direct[index];
            }
            glTranslated(distance[index], 0, distance[index]);
            index++;
            glRotated(angle, 0, 0, 1);
            glTranslated(10 * i, 10 * i, 0);
            
            glutSolidCube(5);
            glPopMatrix();
        }
        //'C'
        glTranslated(100, 0, 0);
        for (int i = 1; i < 5; i++) {
            glPushMatrix();
            if (angle <= 180) {
                distance[index] += dt[index] * direct[index];
            }
            else {
                distance[index] -= dt[index] * direct[index];
            }
            glTranslated(distance[index], 0, distance[index]);
            index++;
            glRotated(angle, 0, 1, -1);
            glTranslated(0, 10 * i, 0);
            
            glutSolidCube(5);
            glPopMatrix();
        }
        for (int i = 0; i < 5; i++) {
            glPushMatrix();
            if (angle <= 180) {
                distance[index] += dt[index] * direct[index];
            }
            else {
                distance[index] -= dt[index] * direct[index];
            }
            glTranslated(0, 0, distance[index]);
            index++;
            glRotated(angle, 0, -1, -1);
            glTranslated(10 * i, 0, 0);
            
            glutSolidCube(5);
            glPopMatrix();
        }
        glTranslated(0, 50, 0);
        for (int i = 0; i < 5; i++) {
            glPushMatrix();
            if (angle <= 180) {
                distance[index] += dt[index] * direct[index];
            }
            else {
                distance[index] -= dt[index] * direct[index];
            }
            glTranslated(0, distance[index], distance[index]);
            index++;
            glRotated(angle, 0, 1, -1);
            glTranslated(10 * i, 0, 0);
            
            glutSolidCube(5);
            glPopMatrix();
        }
        //'A'
        glTranslated(60, -50, 0);
        for (int i = 0; i < 6; i++) {
            glPushMatrix();
            if (angle <= 180) {
                distance[index] += dt[index] * direct[index];
            }
            else {
                distance[index] -= dt[index] * direct[index];
            }
            glTranslated(distance[index], 0, distance[index]);
            index++;
            glRotated(angle, 1, -1, 1);
            glTranslated(5 * i, 10 * i, 0);
            
            glutSolidCube(5);
            glPopMatrix();
        }
        glTranslated(30, 0, 0);
        for (int i = 0; i < 5; i++) {
            glPushMatrix();
            if (angle <= 180) {
                distance[index] += dt[index] * direct[index];
            }
            else {
                distance[index] -= dt[index] * direct[index];
            }
            glTranslated(distance[index], 0, distance[index]);
            index++;
            glRotated(angle, 2, 0.7, 1);
            glTranslated(5 * i, 50 - 10 * (i + 1), 0);
            
            glutSolidCube(5);
            glPopMatrix();
        }
        glTranslated(-10, 0, 0);
        for (int i = 0; i < 3; i++) {
            glPushMatrix();
            if (angle <= 180) {
                distance[index] += dt[index] * direct[index];
            }
            else {
                distance[index] -= dt[index] * direct[index];
            }
            glTranslated(0, distance[index], distance[index]);
            index++;
            glTranslated(5 * i, 20, 0);
            
            glutSolidCube(5);
            glPopMatrix();
        }
        //'I'
        glTranslated(50, 0, 0);
        
        for (int i = 0; i < 6; i++) {
            glPushMatrix();
            if (angle <= 180) {
                distance[index] += dt[index] * direct[index];
            }
            else {
                distance[index] -= dt[index];
            }
            glTranslated(distance[index], 0, distance[index]);
            index++;
            glTranslated(0, 10 * i, 0);
            
            glutSolidCube(5);
            glPopMatrix();
        }
        glPopMatrix();
        
        glPopMatrix();
    }
};
bool running = false;
bool fromHellicopter = false;
Car car;
Planet p1;
Sun s;
float sunAngle = 0;
float dt = 1;
Helicopter h;
float propeller = 0;
int direct[3] = { 0,1,0 };
double  nameAngle = 0;
float dna = 0;
int duration = 100;
Name n;
void display() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    
    //set the camera
    gluLookAt(xo, yo, zo, xd, yd, zd, direct[0], direct[1], direct[2]);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb2);
    glPushMatrix();
    //set the view angle
    if (fromHellicopter)
        glRotated(viewAngleLR, 0, 0, 1);
    else
        glRotated(-viewAngleLR, 0, 1, 0);
    glRotated(viewAngleUD, 1, 0, 0);
    glPushMatrix();
    glPushMatrix();
    //draw the planet
    glRotated(theta, 0, 0, 1);
    p1.draw();
    glPopMatrix();
    
    //draw the car
    glPushMatrix();
    glRotated(-speed, 0, 0, 1);
    glTranslated(0, RADIUS + 12, 50);
    glRotated(90, 0, 1, 0);
    car.draw();
    glPopMatrix();
    
    //draw the helicopter
    glPushMatrix();
    h.setAngle(propeller*1.5);
    glRotated(-speed, 0, 0, 1);
    glTranslated(0, RADIUS + 500, 50);
    h.draw();
    glPopMatrix();
    
    //draw the sun which is a light source
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb1);
    glRotated(-sunAngle, 1, 1, 0);
    glTranslated(0, 0, RADIUS *5.5);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    s.draw();
    glPopMatrix();
    
    //draw my name.
    glPopMatrix();
    glRotated(nameAngle, 0, 1, 0);
    glTranslated(0, 0, RADIUS *1.8);
    glScaled(5, 5, 5);
    n.setAngle(nameAngle);
    n.draw();
    glPopMatrix();
    
    
    glPopMatrix();
    glutSwapBuffers();
}
void moveDisplay(int value)
{
    theta = (theta < 360) ? theta + dt : dt;
    car.setAngle(car.getAngle() < 360 ? car.getAngle() + 10 : 10);
    speed = (speed < 360) ? speed + dspeed : dspeed;
    p1.setAngle(p1.getAngle() < 360 ? p1.getAngle() + 30 : 30);
    propeller = p1.getAngle();
    p1.setSpeed(p1.getSpeed() < 360 ? p1.getSpeed() + 1 : 1);
    sunAngle = sunAngle < 360 ? sunAngle + 2 : 2;
    
    if (nameAngle < 5 || nameAngle>355) {
        dna = 0.2;
    }
    else if (nameAngle < 10 || nameAngle>350) {
        dna = 0.5;
    }
    else if (nameAngle < 50 || nameAngle>310) {
        dna = 1;
    }
    else {
        dna = 5;
    }
    nameAngle = nameAngle < 360 ? nameAngle + dna : dna;
    glutPostRedisplay();
    glutTimerFunc(duration, moveDisplay, 1);
}
//reset all
void reset() {
    xo = 0;
    yo = yd = RADIUS + 10;
    zo = defaultD;
    xd = zd = 0;
    viewAngleUD = 0;
    viewAngleLR = 0;
    speed = 0;
    dspeed = 0;
    direct[1] = 1;
    direct[0] = 0;
    fromHellicopter = false;
    dt = 1;
}
void idle() {
    if (!running) {
        running = true;
        moveDisplay(1);
    }
}
void key(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
        case 'W':
            if (fromHellicopter) {
                if (yo < RADIUS + 800) break;
                yo -= 50;
                yd -= 50;
                viewR = yo;
            }
            else {
                if (zo <= 1000) break;
                zo -= 50;
                zd -= 50;
                if (zo > 2500) {
                    yo += 11;
                    yd += 11;
                }
                else {
                    yo += 0.001;
                    yd += 0.001;
                }
                if (yo > RADIUS + 10) {
                    yo = RADIUS + 10;
                    yd = yo;
                }
            }
            break;
        case 's':
        case 'S':
            if (fromHellicopter) {
                if (yo > RADIUS * 3) break;
                yo += 50;
                yd += 50;
                viewR = yo;
            }
            else {
                if (zo > 5000) break;
                zo += 50;
                zd += 50;
                if (zo < 2500) {
                    yo -= 0.001;
                    yd -= 0.001;
                }
                else {
                    yo -= 11;
                    yd -= 11;
                }
            }
            break;
        case '=':
            
            dspeed = dspeed < 100 ? dspeed += 0.2 : 100;
            dt = dt < 2 ? dt + 0.5 : 2;
            break;
        case '-':
            dspeed = dspeed > -0.5 ? dspeed - 0.2 : -0.5;
            dt = dt > 0 ? dt - 0.5 : 0;
            break;
        case 'v':
        case 'V':
            if (fromHellicopter) {
                fromHellicopter = false;
                xo = xd = 0;
                yo = yd = RADIUS + 10;
                zo = defaultD;
                zd = 0;
                viewAngleLR = 0;
                viewAngleUD = 0;
                direct[1] = 1;
                direct[0] = 0;
            }
            else {
                fromHellicopter = true;
                yo = viewR;
                yd = 0;
                zo = zd = 0;
                direct[1] = 0;
                direct[0] = -1;
                viewAngleLR = 0;
                viewAngleUD = 0;
            }
            break;
        case 'r':
        case 'R':
            reset();
            break;
    };
}
void specialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (fromHellicopter)
                viewAngleLR -= 2;
            else
                viewAngleUD += 2;
            break;
        case GLUT_KEY_DOWN:
            if (!fromHellicopter)
                viewAngleUD -= 2;
            else
                viewAngleLR += 2;
            break;
        case GLUT_KEY_LEFT:
            if (fromHellicopter)
                viewAngleUD -= 2;
            else
                viewAngleLR -= 2;
            break;
        case GLUT_KEY_RIGHT:
            if (fromHellicopter)
                viewAngleUD += 2;
            else
                viewAngleLR += 2;
            break;
    }
}
int main(int argc, char* argv[]) {
    //instructions
    cout << "\'+\': increase speed. " << endl;
    cout << "\'-\': decrease speed. " << endl;
    cout << "\'V\': change view." << endl;
    cout << "\'r\': reset." << endl;
    cout << "\'W\': move forward." << endl;
    cout << "\'S\': move backward." << endl;
    cout << "UP, DOWN, LEFT, RIGHT : rotate" << endl;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Sample");
    glClearColor(135 / 255.0, 205 / 255.0, 255 / 255.0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(60, 1.0, 10, 20000);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glEnable(GL_DEPTH_TEST);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0003);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(specialKey);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
