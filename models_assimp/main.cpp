#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Camera.h" // operacje na macierzach projekcji i widoku
#include "Light.h"
#include "Plain.h"
#include "model.h"

constexpr int WIDTH = 600; // szerokosc okna
constexpr int HEIGHT = 600; // wysokosc okna
//******************************************************************************************
Camera* camera;
Light* light;
Plain* ground;
const int bushesCount = 30;
Plain* bushes[bushesCount];
//******************************************************************************************
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//******************************************************************************************
// wejscie myszy
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
//******************************************************************************************
// ladowanie modeli
const std::string modelName = "models/dragon.obj";

Model* model;
glm::vec3 Modelscale;
//******************************************************************************************

float aspectRatio = static_cast<float>(WIDTH) / HEIGHT;

void errorCallback( int error, const char* description );
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );
void onShutdown();
void initGL();
void renderScene();
void PrepereModel();
void PrepereBushes();
void RenderBushes(glm::mat4 projectionMatrix);

int main(int argc, char* argv[])
{
	atexit( onShutdown );

	srand(time(NULL));

	GLFWwindow* window;

	glfwSetErrorCallback( errorCallback ); // rejestracja funkcji zwrotnej do obslugi bledow

	if( !glfwInit() ) // inicjacja biblioteki GLFW
		exit( EXIT_FAILURE );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); // inicjacja wersji kontekstu
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // incicjacja profilu rdzennego

	window = glfwCreateWindow( WIDTH, HEIGHT, "OGL+GLSL Template", nullptr, nullptr ); // utworzenie okna i zwiazanego z nim kontekstu
	if( !window )
	{
		glfwTerminate(); // konczy dzialanie biblioteki GLFW
		exit( EXIT_FAILURE );
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback( window, keyCallback ); // rejestracja funkcji zwrotnej do oblsugi klawiatury
	glfwSetCursorPosCallback(window, mouse_callback); // rejestracja funkcji zwrotnej do oblsugi myszki

	glfwMakeContextCurrent( window );

	// inicjacja GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if( err != GLEW_OK )
	{
		std::cerr << "Blad: " << glewGetErrorString( err ) << std::endl;
		exit( 1 );
	}

	if( !GLEW_VERSION_3_3 )
	{
		std::cerr << "Brak obslugi OpenGL 3.3\n";
		exit( 2 );
	}

	glfwSwapInterval( 1 ); // v-sync on

	initGL();
	
	// glowna petla programu
	while( !glfwWindowShouldClose( window ) )
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		renderScene();

		glfwSwapBuffers( window ); // zamieniamy bufory
		glfwPollEvents(); // przetwarzanie zdarzen
	}

	glfwDestroyWindow( window ); // niszczy okno i jego kontekst
	glfwTerminate();
	exit( EXIT_SUCCESS );

	return 0;
}

/*------------------------------------------------------------------------------------------
** funkcja zwrotna do obslugi bledow biblioteki GLFW
** error - kod bledu
** description - opis bledu
**------------------------------------------------------------------------------------------*/
void errorCallback( int error, const char* description )
{
	std::cerr << "Error: " << description << std::endl;
}

// funkcja zwrotna do obslugi myszy
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
}

/*------------------------------------------------------------------------------------------
** funkcja zwrotna do obslugi klawiatury
** window - okno, kt�re otrzymalo zdarzenie
** key - klawisz jaki zostal nacisniety lub zwolniony
** scancode - scancode klawisza specyficzny dla systemu
** action - zachowanie klawisza (GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT)
** mods - pole bitowe zawierajace informacje o nacisnietych modyfikatorach (GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, GLFW_MOD_ALT, GLFW_MOD_SUPER)
**------------------------------------------------------------------------------------------*/
void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT) 
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;

		case GLFW_KEY_A:
			camera->ProcessKeyboard(LEFT, deltaTime);
			break;

		case GLFW_KEY_D:
			camera->ProcessKeyboard(RIGHT, deltaTime);
			break;

		case GLFW_KEY_W:
			camera->ProcessKeyboard(FORWARD, deltaTime);
			break;

		case GLFW_KEY_S:
			camera->ProcessKeyboard(BACKWARD, deltaTime);
			break;

		default:
			break;
		}
	}
}

/*------------------------------------------------------------------------------------------
** funkcja wykonywana przed zamknieciem programu
**------------------------------------------------------------------------------------------*/
void onShutdown()
{
	delete camera;
	delete light;
	delete ground;
	for (int i = 0; i < bushesCount; i++)
	{
		delete bushes[i];
	}
}

/*------------------------------------------------------------------------------------------
** funkcja inicjujaca ustawienia OpenGL
**------------------------------------------------------------------------------------------*/
void initGL()
{
	std::cout << "GLEW = " << glewGetString( GLEW_VERSION ) << std::endl;
	std::cout << "GL_VENDOR = " << glGetString( GL_VENDOR ) << std::endl;
	std::cout << "GL_RENDERER = " << glGetString( GL_RENDERER ) << std::endl;
	std::cout << "GL_VERSION = " << glGetString( GL_VERSION ) << std::endl;
	std::cout << "GLSL = " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f); // kolor uzywany do czyszczenia bufora koloru

	glEnable(GL_DEPTH_TEST);

	camera = new Camera();
	light = new Light();
	ground = new Plain();

	light->lightAmbient = glm::vec3(1.0f, 1.0f, 1.0f);

	ground->SetScale(glm::vec3(30.0f, 30.0f, 30.0f));
	ground->SetRotation(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	PrepereModel();
	PrepereBushes();
}

/*------------------------------------------------------------------------------------------
** funkcja rysujaca scene
**------------------------------------------------------------------------------------------*/
void renderScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // czyszczenie bufora koloru

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	
	ground->Draw(projectionMatrix, camera->GetViewMatrix());
	
	RenderBushes(projectionMatrix);
	model->draw(projectionMatrix, camera->GetViewMatrix(), *light);
}

void PrepereModel()
{
	model = new Model(modelName);
	model->SetShader("shaders/model.vert", "shaders/model.frag");

	model->materialAmbient = glm::vec3(0.3f, 0.3f, 0.3f);
	model->materialDiffuse = glm::vec3(0.6f, 0.6f, 0.6f);

	glm::vec3 extent = glm::abs(model->getBBmax() - model->getBBmin());
	float maxExtent = glm::max(glm::max(extent.x, extent.y), extent.z);
	Modelscale = glm::vec3(7.0 / maxExtent);

	model->SetTranslation(-model->getCentroid());
	model->SetTranslation(glm::vec3(0.0f, 2.3f, 5.0f));
	model->SetRotation(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model->SetScale(Modelscale);

	model->SetTexture(L"textures/diffuse.png");
}

void PrepereBushes()
{
	for (int i = 0; i < bushesCount; i++)
	{

		bushes[i] = new Plain("shaders/bush.vert", "shaders/bush.frag");

		int texNr = rand() % 2;
		if (texNr == 1)
			bushes[i]->SetTexture(L"textures/bush1.png");
		else
			bushes[i]->SetTexture(L"textures/bush2.png");

		float x; // wspolrzedna x
		float z; //wpolrzedna z
		do
		{
			x = (rand() % 50) - 25;
			z = (rand() % 50) - 25;
		} while ((x <= 1.0f && x >= -1.0f) && (z >= -4.0f && z <= -2.0f)); //krzak nie pojawi sie na pozycji startowej gracza
		bushes[i]->SetTranslation(glm::vec3(x, 1.0f, z));
	}
}

void RenderBushes(glm::mat4 projectionMatrix)
{
	for (int i = 0; i < bushesCount; i++)
	{
		bushes[i]->Bilbording(camera->GetCameraPos());
		bushes[i]->Draw(projectionMatrix, camera->GetViewMatrix());
	}
}