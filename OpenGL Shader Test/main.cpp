//
//  main.cpp
//  OpenGL Shader Test
//
//  Created by Owen Morgan on 19/01/2018.
//  Copyright © 2018 Owen Morgan. All rights reserved.
//

#include <iostream>
#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Globals{
    int xWindowSize, yWindowSize;
    float xMouse, yMouse;
};

void errorCallback(int error, const char* description){
    std::cout << "GLFW Error Number: " << error << ". Description: " << description <<".\n";
}

void cursorPositionCallback(GLFWwindow* window, double x, double y){
    using namespace Globals;
    xMouse = (float) x;
    yMouse = (float) y;
}

void windowResizeCallback(GLFWwindow* window, int xSize, int ySize){
    Globals::xWindowSize = xSize;
    Globals::yWindowSize = ySize;
}

const char* glErrorString(GLenum err) {
    switch(err) {
        case GL_INVALID_ENUM: return "Invalid Enum.\n";
        case GL_INVALID_VALUE: return "Invalid Value.\n";
        case GL_INVALID_OPERATION: return "Invalid Operation.\n";
        case GL_STACK_OVERFLOW: return "Stack Overflow.\n";
        case GL_STACK_UNDERFLOW: return "Stack Underflow.\n";
        case GL_OUT_OF_MEMORY: return "Out of Memory.\n";
        case GL_TABLE_TOO_LARGE: return "Table too Large.\n";
        default: return "Unknown Error.\n";
    }
}

bool openglInit(GLFWwindow** window){
    if(!glfwInit()){
        std::cerr << "GLFW did not init correctly.\n";
        return false;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwSetErrorCallback(errorCallback);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    *window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);
    if(!*window){
        std::cerr << "GLFW could not create window.\n";
        return false;
    }
    
    glfwSetWindowSizeCallback(*window, windowResizeCallback);
    glfwSetCursorPosCallback(*window, cursorPositionCallback);
    glfwMakeContextCurrent(*window);
    glfwGetWindowSize(*window, &Globals::xWindowSize, &Globals::yWindowSize);
    double x,y;
    glfwGetCursorPos(*window, &x, &y);
    
    Globals::xMouse = (float) x;
    Globals::yMouse = (float) y;
    glewExperimental = true;
    if(glewInit() != GLEW_OK){
        std::cerr << "GLEW did not init correctly.\n";
        return false;
    }
    glfwSwapInterval(1);
    return true;
}

struct Vec4{
    float x, y, z, w;
    inline float& operator[](size_t i){
        return *((float*) this + i);
    }
    inline const float& operator[](size_t i) const{
        return *((float*) this + i);
    }
};

struct Vec3{
    float x, y, z;
    inline float& operator[](size_t i){
        return *((float*) this + i);
    }
    inline const float& operator[](size_t i) const{
        return *((float*) this + i);
    }
};

struct Vec2{
    float x, y;
    inline float& operator[](size_t i){
        return *((float*) this + i);
    }
    inline const float& operator[](size_t i) const{
        return *((float*) this + i);
    }
    inline Vec2 operator*(float f) const{
        return {this->x * f, this->y * f};
    }

    inline Vec2 operator+(Vec2 a) const{
        return {a.x + this->x, a.y + this->y};
    }
    inline Vec2 operator-(Vec2 a) const{
        return {this->x - a.x, this->y - a.y};
    }
};

int main(int argc, const char * argv[]) {
    GLFWwindow* window = nullptr;
    
    if(!openglInit(&window)){
        return EXIT_FAILURE;
    }

    size_t numPoints = 1e3;
    Vec3* colours;
    Vec2* positions;
    float* radius;
    
    colours = new Vec3[numPoints];
    positions = new Vec2[numPoints];
    radius = new float[numPoints];
    
    std::default_random_engine gen;
    std::uniform_real_distribution<float> rand1(0.0, 1.0);
    std::uniform_real_distribution<float> rand2(-1.0, 1.0);
    
    for(size_t i = 0; i < numPoints; ++i){
        colours[i] = {rand1(gen), rand1(gen), rand1(gen)};
        radius[i] = rand1(gen) * 0.2f;
        positions[i] = {rand2(gen), rand2(gen)};
    }
//    Vec3 colours[] = {
//        {1,0,0},
//        {0,1,0},
//        {0,0,1}
//    };
//    Vec2 positions[] = {
//        {-0.4, 0},
//        {0.4, 0},
//        {0, 0.4}
//    };
//    float radius[] = {
//        0.2f, 0.2f, 0.1f
//    };
//
    Vec2 pos[6] = {
        {-1.0f, -1.0f},
        {1.0f, -1.0f},
        {-1.0f,  1.0f},
        {1.0f, -1.0f},
        {1.0f, 1.0f},
        {-1.0f,  1.0f}
    };
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    GLuint vertexbuffer[4];
    glGenBuffers(4, vertexbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0); //quad position
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * numPoints, positions, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0); //position
    glVertexAttribDivisor(1, 1);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numPoints, radius, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (void*) 0); //radius
    glVertexAttribDivisor(2, 1);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * numPoints, colours, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0); //colour
    glVertexAttribDivisor(3, 1);
    
    const char* vertexShader =
    #include "vertex.glsl"
    ;
    const char* fragmentShader =
    #include "frag.glsl"
    ;
    
    auto glVertShaderID = glCreateShader(GL_VERTEX_SHADER);
    auto glFragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(glVertShaderID, 1, &vertexShader, nullptr);
    glCompileShader(glVertShaderID);
    GLint result;
    int errorLogSize = 0;
    glGetShaderiv(glVertShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(glVertShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
    
    if(!result){
        std::cout<<"Vertex did not compile\n";
        char* log = (char*) malloc(errorLogSize + 1);
        glGetShaderInfoLog(glVertShaderID, errorLogSize, nullptr, log);
        std::cout<<log<<"\n";
        free(log);
    }
    
    glShaderSource(glFragShaderID, 1, &fragmentShader, nullptr);
    glCompileShader(glFragShaderID);
    glGetShaderiv(glFragShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(glFragShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
    
    if(!result){
        std::cout<<"Frag shader did not compile\n";
        char* log = (char*) malloc(errorLogSize + 1);
        glGetShaderInfoLog(glFragShaderID, errorLogSize, nullptr, log);
        std::cout<<log<<"\n";
        free(log);
    }
    
    GLuint programID = glCreateProgram();
    glAttachShader(programID, glVertShaderID);
    glAttachShader(programID, glFragShaderID);
    glLinkProgram(programID);
    
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &errorLogSize);
    if(!result){
        std::cout<<"Program did not link.\n";
        char* log = (char*) malloc(errorLogSize + 1);
        glGetProgramInfoLog(programID, errorLogSize, nullptr, log);
        std::cout<<log<<"\n";
        free(log);
    }
    
    GLuint resolutionID = glGetUniformLocation(programID, "resolution");

    glUseProgram(programID);
    auto err = glGetError();
    if(err){
        std::cout<<glErrorString(err);
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[2]);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[3]);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
    
//    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    size_t fpsCounter = 0;
    double fps = 0.0f;
    float deltaTime = 0.016f;
    float speed = 0.1f;
    do{
        ++fpsCounter;
        double startTime = glfwGetTime();
        
        glUniform2f(resolutionID, Globals::xWindowSize, Globals::yWindowSize);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
        speed += -0.0981f * deltaTime * 0.5f;
        for(size_t i = 0; i < numPoints; ++i){
            positions[i].y += speed * deltaTime;
            positions[i].y = positions[i].y < -0.5f ? -0.5f : positions[i].y;
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vec2) * numPoints, positions);

        
        
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (GLint) numPoints);
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        double endTime = glfwGetTime();
        fps += (endTime - startTime);
        deltaTime = (float)(endTime - startTime);
        
    } while(!glfwWindowShouldClose(window));
    std::cout<<"FPS: "<<(double)fpsCounter / fps<<"\n";
    glfwTerminate();
}
