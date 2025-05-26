#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// HECHO POR CHRISTOPHER OPE ROSADO

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int screen_ancho = 800;
const unsigned int screen_alto = 800;

std::string leerArchivoShader(const char* ruta) {
    std::ifstream archivo(ruta);
    std::stringstream buffer;
    buffer << archivo.rdbuf();
    return buffer.str();
}



class Figura {
public:
    float* vertices = nullptr;
    unsigned int* indices = nullptr;
	
	
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	unsigned int VAO, VBO, EBO;
	
	
    int numVertices = 0;
    int numIndices = 0;

    virtual ~Figura() {
        delete[] vertices;
        delete[] indices;
    }
};

class Cubo : public Figura {
public:
    Cubo(float cx, float cy, float cz, float r) {
        vertices = new float[8 * 3];
        int v = 0;
        for (int dx = -1; dx <= 1; dx += 2) {
            for (int dy = -1; dy <= 1; dy += 2) {
                for (int dz = -1; dz <= 1; dz += 2) {
                    vertices[v++] = cx + dx * r;
                    vertices[v++] = cy + dy * r;
                    vertices[v++] = cz + dz * r;
                }
            }
        }
        numVertices = 8;

        unsigned int tempIndices[] = {
            0,1,2, 2,1,3, // cara inferior
            4,6,5, 5,6,7, // cara superior
            0,2,4, 4,2,6, // cara izquierda
            1,5,3, 3,5,7, // cara derecha
            0,4,1, 1,4,5, // cara delantera
            2,3,6, 6,3,7  // cara trasera
        };
        numIndices = 36;
        indices = new unsigned int[numIndices];
        for (int i = 0; i < numIndices; i++) {
            indices[i] = tempIndices[i];
        }
		
		
		std::string vertexCode = leerArchivoShader("vertex_shader.glsl");
		const char* vertexShaderSource = vertexCode.c_str();

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		
		
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		
		
		

		std::string fragmentCode = leerArchivoShader("fragment_shader.glsl");
		const char* fragmentShaderSource = fragmentCode.c_str();

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		
		//unsigned int shaderProgram;
		
		
		
		
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//unsigned int VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
			
    }
	void draw(){
		
		
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::rotate(transform,timeValue, glm::vec3(0.5f, 1.0f, 0.0f));

		glUseProgram(shaderProgram);
		
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	}
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* ventana = glfwCreateWindow(screen_ancho, screen_alto, "Cubo 3D", NULL, NULL);
    glfwMakeContextCurrent(ventana);
    glfwSetFramebufferSizeCallback(ventana, framebuffer_size_callback);

    gladLoadGL(glfwGetProcAddress);

	Cubo * cubo_comp;
	cubo_comp = new Cubo(0.0f,0.0f,0.0f,0.2f);




    while (!glfwWindowShouldClose(ventana))
    {
        glClearColor(0.2f,0.3f,0.4f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        
        cubo_comp->draw();
        
        glfwSwapBuffers(ventana);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* ventana, int ancho, int alto)
{
    glViewport(0, 0, ancho, alto);
}
