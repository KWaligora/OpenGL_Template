#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <IL/il.h>
#include <iostream>

#include "Camera.h" // operacje na macierzach projekcji i widoku
#include "Floor.h"

constexpr int WIDTH = 600; // szerokosc okna
constexpr int HEIGHT = 600; // wysokosc okna
//******************************************************************************************
Camera* camera; // operacje na macierzach projekcji i widoku
Floor* plane;
//******************************************************************************************
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//******************************************************************************************

float aspectRatio = static_cast<float>(WIDTH) / HEIGHT;

void errorCallback( int error, const char* description );
void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );
void onShutdown();
void initGL();
void renderScene();

int main(int argc, char* argv[])
{
	atexit( onShutdown );

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

	glfwSetKeyCallback( window, keyCallback ); // rejestracja funkcji zwrotnej do oblsugi klawiatury

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

/*------------------------------------------------------------------------------------------
** funkcja zwrotna do obslugi klawiatury
** window - okno, które otrzymalo zdarzenie
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
			camera->GoLeft(deltaTime);
			break;

		case GLFW_KEY_D:
			camera->GoRight(deltaTime);
			break;

		case GLFW_KEY_W:
			camera->GoForward(deltaTime);
			break;

		case GLFW_KEY_S:
			camera->GoBack(deltaTime);
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
	delete plane;
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

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ); // kolor uzywany do czyszczenia bufora koloru

	glEnable(GL_DEPTH_TEST);

	camera = new Camera();
	plane = new Floor();
}

/*------------------------------------------------------------------------------------------
** funkcja rysujaca scene
**------------------------------------------------------------------------------------------*/
void renderScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // czyszczenie bufora koloru

	camera->SetViewMatrix();
	camera->SetProjectionMatrix();

	plane->Render(camera);
}
