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
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int screen_ancho = 800;
const unsigned int screen_alto = 800;

float rotX = 0.0f;
float rotY = 0.0f;
float rotZ = 0.0f;

GLenum currentMode = GL_TRIANGLES;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_Q) {  // eje X
            rotX += glm::radians(5.0f);
        }
        if (key == GLFW_KEY_W) {  // eje Y
            rotY += glm::radians(5.0f);
        }
        if (key == GLFW_KEY_E) {  // eje Z
            rotZ += glm::radians(5.0f);
        }
        if (key == GLFW_KEY_Y) {  // reset
            rotX = rotY = rotZ = 0.0f;
        }
        if (key == GLFW_KEY_M) {  // cambiar modo de dibujo
            if (currentMode == GL_TRIANGLES)
                currentMode = GL_LINE;
            else if (currentMode == GL_LINE)
                currentMode = GL_POINTS;
            else
                currentMode = GL_TRIANGLES;

            glPolygonMode(GL_FRONT_AND_BACK, currentMode);
        }
    }
}

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
        // Vertices: posición (x,y,z) + color (r,g,b)
        // 24 vertices (4 por cara x 6 caras), para asignar color por cara
        float verticesTemp[] = {
            // Cara inferior (amarillo) y su color
            -r + cx, -r + cy, -r + cz,  1.0f, 1.0f, 0.0f, // 0
             r + cx, -r + cy, -r + cz,  1.0f, 1.0f, 0.0f, // 1
            -r + cx, -r + cy,  r + cz,  1.0f, 1.0f, 0.0f, // 2
             r + cx, -r + cy,  r + cz,  1.0f, 1.0f, 0.0f, // 3

            // Cara superior (blanco)
            -r + cx,  r + cy, -r + cz,  1.0f, 1.0f, 1.0f, // 4
             r + cx,  r + cy, -r + cz,  1.0f, 1.0f, 1.0f, // 5
            -r + cx,  r + cy,  r + cz,  1.0f, 1.0f, 1.0f, // 6
             r + cx,  r + cy,  r + cz,  1.0f, 1.0f, 1.0f, // 7

            // Cara izquierda (azul)
            -r + cx, -r + cy, -r + cz,  0.0f, 0.0f, 1.0f, // 8
            -r + cx,  r + cy, -r + cz,  0.0f, 0.0f, 1.0f, // 9
            -r + cx, -r + cy,  r + cz,  0.0f, 0.0f, 1.0f, //10
            -r + cx,  r + cy,  r + cz,  0.0f, 0.0f, 1.0f, //11

            // Cara derecha (verde)
             r + cx, -r + cy, -r + cz,  0.0f, 1.0f, 0.0f, //12
             r + cx,  r + cy, -r + cz,  0.0f, 1.0f, 0.0f, //13
             r + cx, -r + cy,  r + cz,  0.0f, 1.0f, 0.0f, //14
             r + cx,  r + cy,  r + cz,  0.0f, 1.0f, 0.0f, //15

            // Cara delantera (rojo)
            -r + cx, -r + cy,  r + cz,  1.0f, 0.0f, 0.0f, //16
             r + cx, -r + cy,  r + cz,  1.0f, 0.0f, 0.0f, //17
            -r + cx,  r + cy,  r + cz,  1.0f, 0.0f, 0.0f, //18
             r + cx,  r + cy,  r + cz,  1.0f, 0.0f, 0.0f, //19

            // Cara trasera (naranja)
            -r + cx, -r + cy, -r + cz,  1.0f, 0.5f, 0.0f, //20
             r + cx, -r + cy, -r + cz,  1.0f, 0.5f, 0.0f, //21
            -r + cx,  r + cy, -r + cz,  1.0f, 0.5f, 0.0f, //22
             r + cx,  r + cy, -r + cz,  1.0f, 0.5f, 0.0f  //23
        };

        numVertices = 24;
        vertices = new float[numVertices * 6];
        for (int i = 0; i < numVertices * 6; i++) {
            vertices[i] = verticesTemp[i];
        }

        unsigned int tempIndices[] = {
            0, 1, 2,  2, 1, 3,      // cara inferior
            4, 5, 6,  6, 5, 7,      // cara superior
            8, 9,10, 10, 9,11,      // cara izquierda
           12,13,14, 14,13,15,      // cara derecha
           16,17,18, 18,17,19,      // cara delantera
           20,21,22, 22,21,23       // cara trasera
        };

        numIndices = 36;
        indices = new unsigned int[numIndices];
        for (int i = 0; i < numIndices; i++) {
            indices[i] = tempIndices[i];
        }

        std::string vertexCode = leerArchivoShader("F:\\grafica\\LabCompiler2024_II_CG\\glfw-master\\OwnProjects\\Project_08\\vertex_shader.glsl");
        const char* vertexShaderSource = vertexCode.c_str();
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        std::string fragmentCode = leerArchivoShader("F:\\grafica\\LabCompiler2024_II_CG\\glfw-master\\OwnProjects\\Project_08\\fragment_shader.glsl");
        const char* fragmentShaderSource = fragmentCode.c_str();
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

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

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, numVertices * 6 * sizeof(float), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        // posición
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void draw() {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, rotX, glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, rotY, glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, rotZ, glm::vec3(0.0f, 0.0f, 1.0f));

        glUseProgram(shaderProgram);
		
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
				
		// 1. Dibujo rubik (colores por vértice)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(VAO);
		glDrawElements(currentMode, numIndices, GL_UNSIGNED_INT, 0);

		// 2. Dibujo para las primitivas
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(2.0f);
		glDisableVertexAttribArray(1); // Desactivar atributo de color
		glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f); // Forzar color negro

		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

		glEnableVertexAttribArray(1); // Restaurar atributo de color
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, currentMode);
    }
};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* ventana = glfwCreateWindow(screen_ancho, screen_alto, "Cubo Rubik", NULL, NULL);
    glfwMakeContextCurrent(ventana);
    glfwSetFramebufferSizeCallback(ventana, framebuffer_size_callback);
    glfwSetKeyCallback(ventana, key_callback);

    gladLoadGL(glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);

    std::vector<Cubo*> cubitos;
    float separacion = 0.45f;
    float tamaño = 0.2f;

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                if (x == 0 && y == 0 && z == 0) continue;
                cubitos.push_back(new Cubo(x * separacion, y * separacion, z * separacion, tamaño));
            }
        }
    }

    glPolygonMode(GL_FRONT_AND_BACK, currentMode);
    glPointSize(50.0f);

    while (!glfwWindowShouldClose(ventana)) {
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto cubo : cubitos) {
            cubo->draw();
        }

        glfwSwapBuffers(ventana);
        glfwPollEvents();
    }

    for (auto cubo : cubitos) {
        delete cubo;
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* ventana, int ancho, int alto) {
    glViewport(0, 0, ancho, alto);
}
