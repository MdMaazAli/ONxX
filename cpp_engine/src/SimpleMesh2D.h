#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
using namespace glm;

class SimpleMesh2D{
    private:
        std::string type;
    
    public:
        unsigned int VAO;
        int vertexCount;
        SimpleMesh2D(string type){
            if(type=="square"){
                float vertices[] = {
                    // vertices     // color        // texCoord         // normals
        
                    // FRONT FACE (Normal: 0, 0, 1)
                    -0.5f, -0.5f,  0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
                    0.5f, -0.5f,  0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
                    0.5f,  0.5f,  0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
                    0.5f,  0.5f,  0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
                    -0.5f,  0.5f,  0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
                    -0.5f, -0.5f,  0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
        
                };
                vertexCount=6;
                setupBuffers(vertices,sizeof(vertices));
            }
        }

        void setupGridBuffers(float* vertices,size_t size){
            glGenVertexArrays(1,&VAO);
            glBindVertexArray(VAO);
    
            unsigned int VBO;
            glGenBuffers(1,&VBO);
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            glBufferData(GL_ARRAY_BUFFER,size,vertices,GL_STATIC_DRAW);
    
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)(0));
            glEnableVertexAttribArray(0);
        }

        void setupBuffers(float* vertices,size_t size){
            glGenVertexArrays(1,&VAO);
            glBindVertexArray(VAO);

            unsigned int VBO;
            glGenBuffers(1,&VBO);
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            glBufferData(GL_ARRAY_BUFFER,size,vertices,GL_STATIC_DRAW);

            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,11*sizeof(float),(void*)(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,11*sizeof(float),(void*)(3*sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,11*sizeof(float),(void*)(6*sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,11*sizeof(float),(void*)(8*sizeof(float)));
            glEnableVertexAttribArray(3);
        }

        void draw(){
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES,0,vertexCount);
        }
};