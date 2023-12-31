#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-100.0f, 2.0f, -45.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;


// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

//Light Control
float redLight = 1.0f;

//Initial Positions
glm::vec3 PosIniAla(-150.0f, 230.0f, -220.0f);
glm::vec3 PosIniBote(-390.0f, -30.0f, 0.0f);
glm::vec3 PosIniDelfin(150.0f, -40.0f, 100.0f);
glm::vec3 PosIniPelota(-40.0f, -18.0f, 20.0f);
glm::vec3 PosIniMinionPel1(-56.0f, -23.0f, 27.5f);
glm::vec3 PosIniMinionPel2(-9.0f, -24.6f, 20.5f);
glm::vec3 PosIni(-20.0f, 5.0f, 160.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
float posX =PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotBraDer, rotBraIzq;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotBraDer;
	float rotBraIzq;
	float rotInc;
	float rotInc3;
	float rotInc4;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(posX,posY,posZ),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0)
};

glm::vec3 LightP1;

//Animacion de los modelos
float movBoteX = 0.0;
float movBoteZ = 0.0;
float rotBote = 0.0;

bool circuitoBote = false;
bool recorridoBote1 = true;
bool recorridoBote2 = false;
bool recorridoBote3 = false;
bool recorridoBote4 = false;

float movAlaY = 0.0;
float movAlaZ = 0.0;
float rotAlaZ = 0.0;
float rotAlaX = 0.0;

bool circuitoAla = false;
bool recorridoAla1 = true;
bool recorridoAla2 = false;
bool recorridoAla3 = false;
bool recorridoAla4 = false;
bool recorridoAla5 = false;
bool recorridoAla6 = false;
bool recorridoAla7 = false;
bool recorridoAla8 = false;
bool recorridoAla9 = false;

float movDelfinX = 0.0;
float movDelfinY = 0.0;
float movDelfinZ = 0.0;
float rotDelfinY = 0.0;
float rotDelfinX = 0.0;

bool circuitoDelfin = false;
bool recorridoDelfin1 = true;
bool recorridoDelfin2 = false;
bool recorridoDelfin3 = false;
bool recorridoDelfin4 = false;
bool recorridoDelfin5 = false;
bool recorridoDelfin6 = false;
bool recorridoDelfin7 = false;

float movPelotaX = 0.0;
float movPelotaY = 0.0;
float rotPelotaY = 0.0;
float rotPelotaX = 0.0;

bool circuitoPelota = false;
bool recorridoPelota1 = true;
bool recorridoPelota2 = false;
bool recorridoPelota3 = false;
bool recorridoPelota4 = false;

float movMinionPel1Y = 0.0;
float movMinionPel2Y = 0.0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);
	
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	
	KeyFrame[FrameIndex].rotBraIzq = rotBraIzq;
	KeyFrame[FrameIndex].rotBraDer = rotBraDer;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotBraIzq = KeyFrame[0].rotBraIzq;
	rotBraDer = KeyFrame[0].rotBraDer;

}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
	
	KeyFrame[playIndex].rotInc3 = (KeyFrame[playIndex + 1].rotBraIzq - KeyFrame[playIndex].rotBraIzq) / i_max_steps;
	KeyFrame[playIndex].rotInc4 = (KeyFrame[playIndex + 1].rotBraDer - KeyFrame[playIndex].rotBraDer) / i_max_steps;
}

void moveBote()
{
	//Movimiento del coche
	if (circuitoBote)
	{
		if (recorridoBote1)
		{
			movBoteZ += 1.0f;
			if (movBoteZ > 250)
			{
				recorridoBote1 = false;
				recorridoBote2 = true;
				rotBote = 0;
			}
		}
		if (recorridoBote2)
		{
			if (rotBote <= 180)
			{
				rotBote += 1;
				movBoteX += 0.5f;
				if (rotBote <= 90)
				{
					movBoteZ += 0.25f;
				}
				if (rotBote > 90)
				{
					movBoteZ -= 0.25f;
				}
			}
			if (rotBote == 180)
			{
				recorridoBote2 = false;
				recorridoBote3 = true;
			}
		}

		if (recorridoBote3)
		{
			movBoteZ -= 1.0f;
			if (movBoteZ < -100)
			{
				recorridoBote3 = false;
				recorridoBote4 = true;
			}
		}

		if (recorridoBote4)
		{
			if (rotBote <= 360)
			{
				rotBote += 1;
				movBoteX -= 0.5f;
				if (rotBote <= 270)
				{
					movBoteZ -= 0.25f;
				}
				if (rotBote > 270)
				{
					movBoteZ += 0.25f;
				}
			}
			if (rotBote == 360)
			{
				recorridoBote4 = false;
				recorridoBote1 = true;
			}
		}
	}
}

void moveAla()
{
	//Movimiento del coche
	if (circuitoAla)
	{
		if (recorridoAla1)
		{
			movAlaZ += 0.5f;
			if (movAlaZ > 50)
			{
				recorridoAla1 = false;
				recorridoAla2 = true;
			}
		}
		if (recorridoAla2)
		{
			rotAlaX += 0.25f;
			rotAlaZ += 1.5f;
			movAlaY += 0.5f;
			movAlaZ += 0.3f;
			if (rotAlaX > 45)
			{
				recorridoAla2 = false;
				recorridoAla3 = true;
			}
		}
		if (recorridoAla3)
		{
			if(rotAlaX > 0)
			{
				rotAlaX -= 0.5f;
			}
			if(rotAlaZ < 360)
			{
				rotAlaZ += 1.5f;
			}
			movAlaZ += 0.5f;
			if (rotAlaX < 0)
			{
				recorridoAla3 = false;
				recorridoAla4 = true;
			}
		}
		if (recorridoAla4)
		{
			if(rotAlaX > -180)
			{
				rotAlaX -= 0.5f;
			}
			movAlaZ += 0.2f;
			movAlaY -= 0.7f;
			if (rotAlaX < -180)
			{
				recorridoAla4 = false;
				recorridoAla5 = true;
			}
		}
		if(recorridoAla5)
		{
			if(rotAlaZ < 540)
			{
				rotAlaZ += 0.5f;
			}
			movAlaZ -= 0.5f;
			movAlaY += 0.5f;
			if (rotAlaZ >= 540)
			{
				recorridoAla5 = false;
				recorridoAla6 = true;
			}
		}
		if (recorridoAla6)
		{
			if(rotAlaX < 0)
			{
				rotAlaX += 0.5f;
			}
			movAlaZ -= 0.5f;
			movAlaY += 0.7f;
			if (rotAlaX > 0)
			{
				recorridoAla6 = false;
				recorridoAla7 = true;
			}
		}
		if(recorridoAla7)
		{
			if(rotAlaZ < 720)
			{
				rotAlaZ += 0.5f;
			}
			movAlaZ += 0.3f;
			if (rotAlaZ >= 720)
			{
				recorridoAla7 = false;
				recorridoAla8 = true;
			}
		}
		if (recorridoAla8)
		{
			if(rotAlaX > -90)
			{
				rotAlaX -= 0.55f;
			}
			if (rotAlaX <= -90)
			{
				recorridoAla8 = false;
				recorridoAla9 = true;
			}
		}
		if (recorridoAla9)
		{
			if (rotAlaX < 0)
			{
				rotAlaX += 0.25f;
			}
			movAlaY -= 1.0f;
			if(movAlaY <= 0)
			{
				recorridoAla9 = false;
				recorridoAla1 = true;
				rotAlaZ = 0.0;
				rotAlaX = 0.0;
			}
		}
	}
}

void moveDelfin()
{
	//Movimiento del coche
	if (circuitoDelfin)
	{
		if (recorridoDelfin1)
		{
			movDelfinZ -= 1.0f;
			if (movDelfinZ < -80)//180
			{
				recorridoDelfin1 = false;
				recorridoDelfin2 = true;
				rotDelfinY = 0;
			}
		}
		if (recorridoDelfin2)
		{
			if(rotDelfinX < 180)
			{
				rotDelfinX += 2.0f;
			}
			movDelfinZ -= 0.25f;
			movDelfinY += 0.75f;
			if (rotDelfinX >= 180)
			{
				recorridoDelfin2 = false;
				recorridoDelfin3 = true;
			}
		}
		if (recorridoDelfin3)
		{
			if(rotDelfinX < 360)
			{
				rotDelfinX += 2.0f;
			}
			movDelfinZ -= 0.25f;
			movDelfinY -= 0.75f;
			if (rotDelfinX >= 360)
			{
				recorridoDelfin3 = false;
				recorridoDelfin4 = true;
				rotDelfinX = 0;
			}
		}
		if (recorridoDelfin4)
		{
			movDelfinZ -= 1.0f;
			if (movDelfinZ < -250)
			{
				recorridoDelfin4 = false;
				recorridoDelfin5 = true;
			}
		}
		if (recorridoDelfin5)
		{
			if (rotDelfinY <= 180)
			{
				rotDelfinY += 1;
				movDelfinX += 0.5f;
				if (rotDelfinY <= 90)
				{
					movDelfinZ -= 0.25f;
				}
				if (rotDelfinY > 90)
				{
					movDelfinZ += 0.25f;
				}
			}
			if (rotDelfinY == 180)
			{
				recorridoDelfin5 = false;
				recorridoDelfin6 = true;
			}
		}
		if (recorridoDelfin6)
		{
			movDelfinZ += 1.0f;
			if (movDelfinZ > 180)
			{
				recorridoDelfin6 = false;
				recorridoDelfin7 = true;
			}
		}

		if (recorridoDelfin7)
		{
			if (rotDelfinY <= 360)
			{
				rotDelfinY += 1;
				movDelfinX -= 0.5f;
				if (rotDelfinY <= 270)
				{
					movDelfinZ += 0.5f;
				}
				if (rotDelfinY > 270)
				{
					movDelfinZ -= 0.5f;
				}
			}
			if (rotDelfinY == 360)
			{
				recorridoDelfin7 = false;
				recorridoDelfin1 = true;
			}
		}
	}
}

void movePelota()
{
	if(circuitoPelota)
	{
		if (recorridoPelota1)
		{
			movMinionPel1Y += 0.3f;
			if(movPelotaY < 40)
			{
				rotPelotaY += 0.5f;
			}
			movPelotaY += 1.0f;
			movPelotaX += 0.25f;
			rotPelotaX += 2.0f;
			if (movPelotaY == 40)
			{
				recorridoPelota1 = false;
				recorridoPelota2 = true;
			}
		}
		if (recorridoPelota2)
		{
			movMinionPel1Y -= 0.33f;
			if(movPelotaY > 0)
			{
				rotPelotaY += 0.5f;
			}
			movPelotaY -= 1.0f;
			movPelotaX += 0.25f;
			rotPelotaX += 2.0f;
			if (movPelotaY == 4)
			{
				recorridoPelota2 = false;
				recorridoPelota3 = true;
			}
		}
		if (recorridoPelota3)
		{
			movMinionPel2Y += 0.3f;
			if(movPelotaY < 40)
			{
				rotPelotaY -= 0.5f;
			}
			movPelotaY += 1.0f;
			movPelotaX -= 0.25f;
			rotPelotaX -= 2.0f;
			if (movPelotaY == 40)
			{
				recorridoPelota3 = false;
				recorridoPelota4 = true;
			}
		}
		if (recorridoPelota4)
		{
			movMinionPel2Y -= 0.27f;
			if(movPelotaY > 0)
			{
				rotPelotaY -= 0.5f;
			}
			movPelotaY -= 1.0f;
			movPelotaX -= 0.25f;
			rotPelotaX -= 2.0f;
			if (movPelotaY == 0)
			{
				recorridoPelota4 = false;
				recorridoPelota1 = true;
			}
		}
	}
}

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto CG", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	Model Ala((char*)"Models/Minion_volando/minion.obj");
	Model Playa((char*)"Models/Playa/playa.obj");
	Model Bote((char*)"Models/Minion_bote/minion.obj");
	Model Delfin((char*)"Models/Dolphin/dolphin.obj");
	Model Pelota((char*)"Models/Pelota/ball.obj");
	Model MinionPel1((char*)"Models/Minion_pelota/minion1.obj");
	Model MinionPel2((char*)"Models/Minion_pelota/minion2.obj");
	Model Torso((char*)"Models/Minion_separado/minion_cuerpo.obj");
	Model BrazoDer((char*)"Models/Minion_separado/minion_manoDer.obj");
	Model BrazoIzq((char*)"Models/Minion_separado/minion_manoIzq.obj");
	// Build and compile our shader program

	//Inicializaci�n de KeyFrames
	
	for(int i=0; i<MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotBraIzq = 0;
		KeyFrame[i].rotBraDer = 0;
		KeyFrame[i].rotInc = 0;
		KeyFrame[i].rotInc3 = 0;
		KeyFrame[i].rotInc4 = 0;
	}



	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");
	
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();


		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), redLight, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//glBindVertexArray(VAO);

		glBindVertexArray(VAO);
		glm::mat4 model(1);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp
		glm::mat4 tmp2 = glm::mat4(1.0f); //Temp

		//Carga de modelo
		//Playa
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-95.0f, -2.0f, -45.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Playa.Draw(lightingShader);

		//Bote
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, PosIniBote + glm::vec3(movBoteX, 0,movBoteZ));
		model = glm::rotate(model, glm::radians(rotBote), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Bote.Draw(lightingShader);

		//Ala Delta
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, PosIniAla + glm::vec3(0, movAlaY, movAlaZ));
		model = glm::rotate(model, glm::radians(rotAlaZ), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rotAlaX), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ala.Draw(lightingShader);

		//Delfin
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, PosIniDelfin + glm::vec3(movDelfinX, movDelfinY, movDelfinZ));
		model = glm::rotate(model, glm::radians(rotDelfinY), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotDelfinX), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Delfin.Draw(lightingShader);

		//Pelota
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, PosIniPelota + glm::vec3(movPelotaX, movPelotaY, 0));
		model = glm::rotate(model, glm::radians(rotPelotaY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPelotaX), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pelota.Draw(lightingShader);

		//Minion 1 Pelota
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, PosIniMinionPel1 + glm::vec3(0, movMinionPel1Y, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MinionPel1.Draw(lightingShader);

		//Minion 2 Pelota
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, PosIniMinionPel2 + glm::vec3(0, movMinionPel2Y, 0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MinionPel2.Draw(lightingShader);

		//Personaje
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0, 1, 0));
		tmp = model = glm::translate(model,glm::vec3(posX,posY,posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Torso.Draw(lightingShader);

		//Brazo derecho
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotBraDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-4.75f, 0.0f, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoDer.Draw(lightingShader);

		//Brazo Izquierdo
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotBraIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(3.75f, 0.0f, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoIzq.Draw(lightingShader);


		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default




		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}


void animacion()
{

		//Movimiento del personaje

		if (play)
		{
			if (i_curr_steps >= i_max_steps) //end of animation between frames?
			{
				playIndex++;
				if (playIndex>FrameIndex - 2)	//end of total animation?
				{
					printf("termina anim\n");
					playIndex = 0;
					play = false;
				}
				else //Next frame interpolations
				{
					i_curr_steps = 0; //Reset counter
									  //Interpolation
					interpolation();
				}
			}
			else
			{
				//Draw animation
				posX += KeyFrame[playIndex].incX;
				posY += KeyFrame[playIndex].incY;
				posZ += KeyFrame[playIndex].incZ;

				rotBraIzq += KeyFrame[playIndex].rotInc3;
				rotBraDer += KeyFrame[playIndex].rotInc4;

				i_curr_steps++;
			}

		}
	
	moveBote();
	moveAla();
	moveDelfin();
	movePelota();
}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex<MAX_FRAMES)
		{
			saveFrame();
		}

	}


	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}

}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{

		rot += 1;

	}

	if (keys[GLFW_KEY_6])
	{
		if (rotBraIzq<5.0f)
		{
			rotBraIzq += 1.0f;
			rotBraDer += 1.0f;
		}
			
			
	}

	if (keys[GLFW_KEY_7])
	{
		if (rotBraIzq>-5)
		{
			rotBraIzq -= 1.0f;
			rotBraDer -= 1.0f;
		}
	}

	

	//Mov Personaje
	if (keys[GLFW_KEY_H])
	{
		posZ += 1;
	}

	if (keys[GLFW_KEY_Y])
	{
		posZ -= 1;
	}

	if (keys[GLFW_KEY_G])
	{
		posX -= 1;
	}

	if (keys[GLFW_KEY_J])
	{
		posX += 1;
	}




	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}


	if (camera.GetPosition().y <= -31){
		redLight = 0.0f;
	}
	else{
		redLight = 1.0f;
	}

	if (keys[GLFW_KEY_I])
	{
		circuitoBote = true;
		circuitoAla = true;
		circuitoDelfin = true;
		circuitoPelota = true;
	}

	if (keys[GLFW_KEY_O])
	{
		circuitoBote = false;
		circuitoAla = false;
		circuitoDelfin = false;
		circuitoPelota = false;
	}

}

