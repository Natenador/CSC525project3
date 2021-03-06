/*==================================================================================================
 PROGRAMMER:			Nathan Kroll, Kroll1483
                        Thomas Kroll, kroll001
 COURSE:				CSC 525/625
 MODIFIED BY:			Nathan Kroll, Kroll1483
                        Thomas Kroll, kroll001
 LAST MODIFIED DATE:	12/5/2016
 DESCRIPTION:           This program is a 3D interactive advertisement for Minecraft.  There is text
                        floating in front of and behind a 3D wall.  The user can move around and
                        interact with the wall by removing blocks.  The goal is to reveal the text
                        behind the wall to see the full message! The user is restricted from moving
                        around the wall to force interaction with the blocks.
                        TO USE:
                            -Right Click and select "Take Control", or press 'm' to take conrol of
                            the camera.
                            -MOVEMENT:
                                -'w' to move forward.
                                -'s' to move backward.
                                -'a' to move left.
                                -'d' to move right.
                                -UP_ARROW to look up.
                                -DOWN_ARROW to look down.
                                -RIGHT_ARROW to look right.
                                -LEFT_ARROW to look left.
                            -LEFT_CLICK or numpad '0' to interact with the block that your
                                aim-dot is hovering over.
                            -ESC or END to surrender control of the camera.

                        PROGRAMMER CONTRIBUTION:
                            Thomas Kroll, kroll001: 50%
                                -Programmed the Camera class.
                                -Wrote functions to build the wall.
                                -Wrote functions/methods to interact with the wall.
                            Nathan Kroll, : 50%
                                -Programmed the Block class
                                -Changed background.
                                -Built Text for advertisement.
								-Created help window
 NOTE:					N/A
 FILES:					project3.cpp, (CSC525Labs.sln)
 IDE/COMPILER:			MicroSoft Visual Studio 2013, Vim/g++
 INSTRUCTION FOR COMPILATION AND EXECUTION:
    WINDOWS/VISUAL STUDIO:
        1.		Double click on myCPPproj.sln	to OPEN the project
        2.		Press Ctrl+F7					to COMPILE
        3.		Press Ctrl+Shift+B				to BUILD (COMPILE+LINK)
        4.		Press Ctrl+F5					to EXECUTE
    LINUX/G++ COMMAND LINE:
        1.      Open a terminal in the project directory
        2.      Execute command to COMPILE: g++ -std=c++11 project2.cpp -o PROJECT -lGL -lGLU -lglut
        3.      Execute command to EXECUTE: ./PROJECT
==================================================================================================*/
#include <cmath>
#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include <GL/glut.h>				// include GLUT library
//***********************************************************************************

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

//***********************************************************************************
using std::cout;
using std::endl;
using std::vector;
using std::string;
// SCREEN GLOBALS //
const int WIDTH = 1800;
const int HEIGHT = 1200;
bool CONTROL = false;
bool JUMPING = false;

void myDisplayCallback();

double toRadians(double degrees) {
	return degrees * 3.14159 / 180;
}

double toDegrees(double radians){
    return radians * 180 / 3.14159;
}

void handleSleep(){
        #ifdef _WIN32
            Sleep(10);
        #else
            usleep(1000);
        #endif
}

class Camera{
    private:
        // ORIGIN OF CAMERA COORD
        double eyeX, eyeY, eyeZ;

        // WHERE CAMERA LOOKS
        double directX, directY, directZ;

        // POINT ABOVE CAMERA
        double orientX, orientY, orientZ;

        // PERSPECTIVE VALUES
        double view_field, ratio, near_view, far_view;

        double movement;

        void move();

    public:
        Camera(double view, double r, double n, double f){
            this->eyeX = -1000;
            this->eyeY = 0;
            this->eyeZ = 50;
            this->directX = this->eyeX+100;
            this->directY = 0;
            this->directZ = this->eyeZ;;
            this->orientX = 0;
            this->orientY = 0;
            this->orientZ = 100;
            this->view_field = view;
            this->ratio = r;
            this->near_view = n;
            this->far_view = f;
            this->movement = 10;
        };

        double lookX(){ return this->directX; };
        double lookY(){ return this->directY; };
        double lookZ(){ return this->directZ; };
        double getEyeX(){ return this->eyeX; };

        void perspective(){
            glMatrixMode(GL_PROJECTION);
            gluPerspective(this->view_field, this->ratio, this->near_view, this->far_view);
        };

        void lookAt(){
            glMatrixMode(GL_MODELVIEW);
            gluLookAt(this->eyeX, this->eyeY, this->eyeZ,
                    this->directX, this->directY, this->directZ,
                    this->orientX, this->orientY, this->orientZ);
        };
        void debug(){
            std::cout << "Camera Debug info: " << std::endl;
            std::cout << "Eye coord: (" << this->eyeX << ", " << this->eyeY << ", " << this->eyeZ << ")" << std::endl;
            std::cout << "direct coord: (" << this->directX << ", " << this->directY << ", " << this->directZ << ")" << std::endl;
            std::cout << "orient coord: (" << this->orientX << ", " << this->orientY << ", " << this->orientZ << ")" << std::endl;
        };

        void right();
        void left();
        void forward();
        void backward();
        void mouseMove(int mx, int my);
        void lookUp();
        void lookDown();
        void lookRight();
        void lookLeft();
        void jump();
};


void Camera::move(){
    glLoadIdentity();
    this->lookAt();
    //this->debug();
    myDisplayCallback();
};

void Camera::lookUp(){
    this->directZ += this->movement;
    this->move();
}

void Camera::lookDown(){
    this->directZ -= this->movement;
    this->move();
}

void Camera::lookRight(){
    this->directY -= this->movement;
    this->move();
}

void Camera::lookLeft(){
    this->directY += this->movement;
    this->move();
}

void Camera::right(){
    this->eyeY -= this->movement;
    this->directY -= this->movement;
    this->move();
};

void Camera::left(){
    this->eyeY += this->movement;
    this->directY += this->movement;
    this->move();
};

void Camera::forward(){
    double wall_stop = 0;
    if(this->eyeX+this->movement >= wall_stop)
        return;
    this->eyeX += this->movement;
    this->directX += this->movement;
    this->move();
}

void Camera::backward(){
    this->eyeX -= this->movement;
    this->directX -= this->movement;
    this->move();
}

void Camera::mouseMove(int mx, int my){
    this->directY = -((double)mx - (WIDTH/2));
    this->directZ = (HEIGHT/2) - (double)my;
    this->move();
}

void Camera::jump(){
    JUMPING = true;
    int jump_len = 40;
    for(int i = 0; i < jump_len; i++){
        this->directZ += 2;
        this->eyeZ += 2;
        this->move();
        handleSleep();
    }
    for(int i = jump_len; i > 0; i--){
        this->directZ -= 2;
        this->eyeZ -= 2;
        this->move();
        handleSleep();
    }

    JUMPING = false;
}

void green() {
	glColor3f(0, .7, 0);
}

void brown() {
	glColor3f(.8, .55, .45);
}

// GLOBALS //

Camera camera = Camera(120, 1, 0.1, 2100);
// END GLOBALS //

class Box
{
public:
	Box(int x, int y, int z, int side_len);
	/*void setVertex(int i, Vertex v);
	void setVertex(int i, float x, float y, float z);*/
	int X();
	int Y();
	int Z();
	void setX(int x);
	void setY(int y);
	void setZ(int z);
	void setSideLen(int len);
	int sideLen();
	void draw(bool grass);
	void remove(){ this->render = false; };
	bool exists(){ return this->render; };
	void checkAndRemove();
private:
	int x;
	int y;
	int z;
	int side_len;
	bool render = true;
	//vector<Vertex> vertices;
};

int Box::X() { return this->x; }
int Box::Y() { return this->y; }
int Box::Z() { return this->z; }
int Box::sideLen() { return this->side_len; }
void Box::setX(int x) { this->x = x; }
void Box::setY(int y) { this->y = y; }
void Box::setZ(int z) { this->z = z; }
void Box::setSideLen(int len) { this->side_len = len; }

Box::Box(int x, int y, int z, int side_len) 
{
	setX(x);
	setY(y);
	setZ(z);
	setSideLen(side_len);
}

void Box::checkAndRemove(){
    if(this->y <= camera.lookY() && (this->y+this->side_len) > camera.lookY() && this->z <= camera.lookZ() && (this->z+this->side_len) > camera.lookZ()){
        this->remove();
    }
}

void Box::draw(bool grass)
{
	glBegin(GL_POLYGON);
	if (grass) {
		//top z-x parallel plane
		green();
		glVertex3i(x, y + side_len, z + side_len);
		brown();
		glVertex3i(x, y + side_len, z);
		glVertex3i(x + side_len, y + side_len, z);

		green();
		glVertex3i(x + side_len, y + side_len, z + side_len);


		brown();
		//far z-y parallel plane
		glVertex3i(x, y, z);
		glVertex3i(x, y + side_len, z);
		green();
		glVertex3i(x, y + side_len, z + side_len);
		glVertex3i(x, y, z + side_len);
		//far x-y parallel plane
		brown();
		glVertex3i(x, y, z);
		glVertex3i(x, y + side_len, z);
		glVertex3i(x + side_len, y + side_len, z);
		glVertex3i(x + side_len, y, z);
		//near z-y parallel plane

		green();
		glVertex3i(x + side_len, y, z + side_len);
		brown();
		glVertex3i(x + side_len, y + side_len, z);
		glVertex3i(x + side_len, y, z);
		green();
		glVertex3i(x + side_len, y + side_len, z + side_len);
		//near x-y paralled plane
		glVertex3i(x + side_len, y, z + side_len);
		glVertex3i(x, y, z + side_len);
		glVertex3i(x, y + side_len, z + side_len);
		glVertex3i(x + side_len, y + side_len, z + side_len);
		//far z-x parallel plane
		brown();
		glVertex3i(x, y, z);
		glVertex3i(x + side_len, y, z);
		green();
		glVertex3i(x, y, z + side_len);
		glVertex3i(x + side_len, y, z + side_len);
	}
	else
	{
		//top z-x parallel plane
		brown();
		glVertex3i(x, y + side_len, z + side_len);
		glVertex3i(x, y + side_len, z);
		glVertex3i(x + side_len, y + side_len, z);
		glVertex3i(x + side_len, y + side_len, z + side_len);
		//far z-y parallel plane
		glVertex3i(x, y, z);
		glVertex3i(x, y + side_len, z);
		glVertex3i(x, y + side_len, z + side_len);
		glVertex3i(x, y, z + side_len);
		//far x-y parallel plane
		glVertex3i(x, y, z);
		glVertex3i(x, y + side_len, z);
		glVertex3i(x + side_len, y + side_len, z);
		glVertex3i(x + side_len, y, z);
		//near z-y parallel plane
		glVertex3i(x + side_len, y, z + side_len);
		glVertex3i(x + side_len, y + side_len, z);
		glVertex3i(x + side_len, y, z);
		glVertex3i(x + side_len, y + side_len, z + side_len);
		//near x-y paralled plane
		glVertex3i(x + side_len, y, z + side_len);
		glVertex3i(x, y, z + side_len);
		glVertex3i(x, y + side_len, z + side_len);
		glVertex3i(x + side_len, y + side_len, z + side_len);
		//far z-x parallel plane
		glVertex3i(x, y, z);
		glVertex3i(x + side_len, y, z);
		glVertex3i(x, y, z + side_len);
		glVertex3i(x + side_len, y, z + side_len);
	}

	glEnd();
}

vector<vector<Box>> boxes;

void drawChar(int aChar, bool smallText = false) {
	if (smallText) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, aChar);
	}
	else {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, aChar);
	}
}


void draw3dChar(int aChar) {
	glutStrokeCharacter(GLUT_STROKE_ROMAN, aChar);
}

void initBoxes(){
    int width = 35;
    int height = 9;
    int box_len = 50;
    int start_y = -(width*box_len/2);
    int y = start_y;
    int z = 0;
    for(int i=0; i < height; i++){
        vector<Box> temp;
        for(int j=0; j < width; j++){
            temp.push_back(Box(0, y, z, box_len));
            y += box_len;
        }
        boxes.push_back(temp);
        z += box_len;
        y = start_y;
        temp.clear();
    }
}

void drawMasterWall(){
    for(int i = 0; i < boxes.size(); i++){
        for(int j = 0; j < boxes.at(i).size(); j++){
            if(boxes.at(i).at(j).exists())
				if(i < boxes.size() - 1)
					boxes.at(i).at(j).draw(false);
				else
					boxes.at(i).at(j).draw(true);
        }
    }
}

void drawAimDot(){
    double x = camera.lookX();
    double y = camera.lookY();
    double z = camera.lookZ();
    double len = 2;
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex3d(x, y+len, z-len);
    glVertex3d(x, y-len, z-len);
    glVertex3d(x, y-len, z+len);
    glVertex3d(x, y+len, z+len);
    glPointSize(1);
    glEnd();
}

void draw3dString(std::string message, int x_depth, int y_depth, int z_depth) {
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glRotatef(-90, 0, 1, 0);
	glRotatef(-90, 0, 0, 1);
	glTranslated(y_depth, 0, 0);
	glTranslated(0, z_depth, x_depth);
	for (int i = 0; i < message.size(); i++) {
		draw3dChar(message[i]);
	}
	glPopMatrix();
}

void myDisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// draw the background

    drawMasterWall();
    drawAimDot();
	glColor3f(1, 1, 1);
	std::string message = "Play Minecraft?";
	draw3dString(message, 500, -500, 100);
	glColor3f(.3, .3, .3);
	message = "Awww Yeeaahh! Buy It Now!";
	draw3dString(message, -300, -750, 50);

	glFlush(); // flush out the buffer contents
}

void checkAndRemoveBlocks(){
    for(int i = 0; i < boxes.size(); i++){
        for(int j = 0; j < boxes.at(i).size(); j++){
            if(boxes.at(i).at(j).exists())
                boxes.at(i).at(j).checkAndRemove();
        }
    }
    myDisplayCallback();
}

void handleKeys(unsigned char key, int cur_x, int cur_y){
    if(key == 27)
        CONTROL = false;
    if(key == 'm')
        CONTROL = true;
    if(!CONTROL || JUMPING)
        return;

    switch(key){
        case 'd':
            camera.right();
            break;
        case 'a':
            camera.left();
            break;
        case 'w':
            camera.forward();
            break;
        case 's':
            camera.backward();
            break;
        case ' ':
            camera.jump();
            break;
        case '0':
            checkAndRemoveBlocks();
            break;
    }
}

// MENU GLOBALS
const int ACTIVATE = 1;
const int EXIT = 2;
const int HELP = 3;
int help_window_id;


void drawText(std::string text) {
	for (int i = 0; i < text.length(); i++) {
		drawChar(text[i]);
	}
}

void helpWindowCallback() {
	int min_x = -400;
	int y_diff = 25;
	int y = 375;
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(5);
	glColor3f(.5, .5, 0);
	std::vector<std::string> help_messages;
	help_messages.push_back("How to use:");
	help_messages.push_back("Use menu by right clicking, or press 'm' to start moving around.");
	help_messages.push_back("How to move:");
	help_messages.push_back("-'w' to move forward.");
	help_messages.push_back("- 's' to move backward.");
	help_messages.push_back("- 'a' to move left.");
	help_messages.push_back("- 'd' to move right.");
	help_messages.push_back("- UP_ARROW to look up.");
	help_messages.push_back("- DOWN_ARROW to look down.");
	help_messages.push_back("- RIGHT_ARROW to look right.");
	help_messages.push_back("- LEFT_ARROW to look left.");
	help_messages.push_back("- Numpad '0' to interact with the block that your aim - dot is hovering over.");
	help_messages.push_back("- ESC or END to surrender control of the camera.");
	for (int i = 0; i < help_messages.size() - 1; i++) {
		glRasterPos2i(min_x, y);
		drawText(help_messages.at(i));
		y -= y_diff;
	}
	glFlush();
}

void helpMenu(int entryId) {
	switch (entryId) {
	case 0:
		glutIconifyWindow();
		break;
	}
}

void displayHelpWindow() {
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	help_window_id = glutCreateWindow("Help Menu");
	glutCreateMenu(helpMenu);
	glutAddMenuEntry("Hide", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glClearColor(1, 1, 1, 0);
	gluOrtho2D(-400, 400, -400, 400);
	glutDisplayFunc(helpWindowCallback);
}

void mainMenu(int id){
    switch(id){
        case ACTIVATE:
            CONTROL = true;
            break;
		case HELP:
			glutSetWindow(help_window_id);
			glutShowWindow();
			break;
        case EXIT:
            exit(0);
    }
}

void handleSpecial(int key, int mx, int my){
    switch(key){
        case GLUT_KEY_END:
            CONTROL = false;
            break;
        case GLUT_KEY_UP:
            camera.lookUp();
            break;
        case GLUT_KEY_DOWN:
            camera.lookDown();
            break;
        case GLUT_KEY_RIGHT:
            camera.lookRight();
            break;
        case GLUT_KEY_LEFT:
            camera.lookLeft();
            break;
    }
}



//***********************************************************************************
void myInit()
{glClearColor(0, .75, 1, 0);			// specify a background color: white 
camera.perspective();
glEnable(GL_DEPTH_TEST);
camera.lookAt();
}

//***********************************************************************************

//***********************************************************************************
int  main()
{
    //====================================================================//
    // These lines are only here so I (Thomas) can work on this at home
    // since the only c++ compiler available to me is g++
    // (Visual Studio and Linux don't play well together)
    int argc = 1;
    char *argv[1] = {(char*)"Something"};
    glutInit(&argc, argv);
    //====================================================================//
    glutInitDisplayMode(GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);				// specify a window size
    glutInitWindowPosition(0, 0);			// specify a window position
    glutCreateWindow("3D Stuff");	// create a titled window

    myInit();

    glutCreateMenu(mainMenu);
    glutAddMenuEntry("Take Control", ACTIVATE);
	glutAddMenuEntry("Help Window", HELP);
    glutAddMenuEntry("Exit", EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutKeyboardFunc(handleKeys);
    glutSpecialFunc(handleSpecial);

    initBoxes();
    glutDisplayFunc(myDisplayCallback);	// register a callback
	displayHelpWindow();
    glutMainLoop();							// get into an infinite loop
}
