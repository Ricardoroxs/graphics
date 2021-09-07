#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
bool mask[WINDOW_HEIGHT][WINDOW_WIDTH];
GLFWwindow *window;


// Color structure. Can be used to define the brush and background color.
struct color { float r, g, b; };
float mySize = 4;
bool isPressing = false;
float myBrushColor = 7;
float myBackgroundColor = 0;
bool sprayMode = false;

// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))

// Set a particular pixel of the frameBuffer to the provided color
void SetFrameBufferPixel(int x, int y, struct color lc)
{
	// Origin is upper left corner. 
	// Changes the origin from the lower-left corner to the upper-left corner
	y = WINDOW_HEIGHT - 1 - y;

	x = CLAMP(x, 0, WINDOW_WIDTH - 1);
	y = CLAMP(y, 0, WINDOW_HEIGHT - 1);

	frameBuffer[y][x][0] = lc.r;
	frameBuffer[y][x][1] = lc.g;
	frameBuffer[y][x][2] = lc.b;

}

void ClearFrameBuffer()
{
	memset(frameBuffer, 0.0f, sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
}

// Display frameBuffer on screen
void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

color colorPicker(int id)
{
		struct color paintColor;
		if (id == 0) { paintColor.r = 0; paintColor.b = 0; paintColor.g = 0; }
		else if (id == 1) { paintColor.r = 0; paintColor.b = 0; paintColor.g = 1; }
		else if (id == 2) { paintColor.r = 0; paintColor.b = 1; paintColor.g = 0; }
		else if (id == 3) { paintColor.r = 0; paintColor.b = 1; paintColor.g = 1; }
		else if (id == 4) { paintColor.r = 1; paintColor.b = 0; paintColor.g = 0; }
		else if (id == 5) { paintColor.r = 1; paintColor.b = 0; paintColor.g = 1; }
		else if (id == 6) { paintColor.r = 1; paintColor.b = 1; paintColor.g = 0; }
		else if (id == 7) { paintColor.r = 1; paintColor.b = 1; paintColor.g = 1; }
		return paintColor;
}

void paintSquare(float xPos, float yPos, int size)
{
		for (int x = (xPos - size); x < (xPos + size); x++)
		{
				for (int y = (yPos - size); y < (yPos + size); y++)
				{
						mask[y][x] = true;
						if(!sprayMode)
								SetFrameBufferPixel(x, y, colorPicker(myBrushColor));
						else
						{
								int randNum = rand() % 10 + 1; //Generates random number between 1 and 10
								if(randNum >= 7)
										SetFrameBufferPixel(x, y, colorPicker(myBrushColor));
						}
				}
		}
}

// Mouse position callback function
void CursorPositionCallback(GLFWwindow* lWindow, double xpos, double ypos)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		std::cout << "Mouse position is: x - " << xpos << ", y - " << ypos << std::endl;
		if (isPressing)
				paintSquare(xpos, ypos, mySize);
	}
}

void changeBackgroundColor()
{
		for (int x = 0; x < WINDOW_WIDTH; x++)
		{
				for (int y = 0; y < WINDOW_HEIGHT; y++)
				{
						if (mask[y][x] == false)
								SetFrameBufferPixel(x, y, colorPicker(myBackgroundColor));
				}
		}
}

// Mouse callback function
void MouseCallback(GLFWwindow* lWindow, int button, int action, int mods)
{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
				std::cout << "Mouse left button is pressed." << std::endl;
				isPressing = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
				isPressing = false;
				for (int x = 0; x < WINDOW_WIDTH; x++)
				{
						for (int y = 0; y < WINDOW_HEIGHT; y++)
						{
								mask[y][x] = false;
						}
				}
				changeBackgroundColor();
		}
		else
				isPressing = false;
}

// You can use "switch" or "if" to compare key to a specific character.
// for example,
// if (key == '0')
//     DO SOMETHING

// Keyboard callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	std::cout << "Key " << (char)key << " is pressed." << std::endl;
	if ((char)key == '+' && mySize < 128)
	{
			mySize *= 2;
			std::cout << "Size increased to " << mySize << std::endl;
	}
	else if ((char)key == '-' && mySize > 1)
	{
			mySize /= 2;
			std::cout << "Size decreased to " << mySize << std::endl;
	}
	else if ((char)key == '0') myBrushColor = 0;
	else if ((char)key == '1') myBrushColor = 1;
	else if ((char)key == '2') myBrushColor = 2;
	else if ((char)key == '3') myBrushColor = 3;
	else if ((char)key == '4') myBrushColor = 4;
	else if ((char)key == '5') myBrushColor = 5;
	else if ((char)key == '6') myBrushColor = 6;
	else if ((char)key == '7') myBrushColor = 7;
	else if ((char)key == ')') { myBackgroundColor = 0;  changeBackgroundColor(); }
	else if ((char)key == '!') { myBackgroundColor = 1; changeBackgroundColor(); }
	else if ((char)key == '@') { myBackgroundColor = 2; changeBackgroundColor(); }
	else if ((char)key == '#') { myBackgroundColor = 3; changeBackgroundColor(); }
	else if ((char)key == '$') { myBackgroundColor = 4; changeBackgroundColor(); }
	else if ((char)key == '%') { myBackgroundColor = 5; changeBackgroundColor(); }
	else if ((char)key == '^') { myBackgroundColor = 6; changeBackgroundColor(); }
	else if ((char)key == '&') { myBackgroundColor = 7; changeBackgroundColor(); }
	else if ((char)key == 's')
	{
			if (sprayMode) 
			{ 
					sprayMode = false; 
					std::cout << "Spray Mode off" << std::endl;
			}
			else { sprayMode= true; std::cout << "Spray Mode on" << std::endl; }
	}
}

void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment 1 - <Richard Liu>", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetCharCallback(window, CharacterCallback);
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();
}


int main()
{	
	Init();
	while (glfwWindowShouldClose(window) == 0)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
