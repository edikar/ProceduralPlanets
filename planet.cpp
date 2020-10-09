// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader_class.h"
#include "utils.h"
#include "camera.h"
//#include "model.h"
#include "stb_image.h"

#include "PerlinNoise.h"
#include "planet.h"

using namespace std;
using namespace glm;

unsigned int loadTexture(char const * path);


void Planet::toggleNormals(bool state){
    showNormals = state;
}

void Planet::toggleOcean(bool state){
    showOcean = state;
}

void Planet::toggleWireframe(bool state){
    showWireframe = state;   
}

void Planet::setDetalization(int value){
    detalization = value;
    initializePlanet();
}

void Planet::setShapeNoiseLayers(int value){
    shapeNoiseLayers = value;
    initializePlanet();
}

void Planet::setShapeNoiseLacunarity(float value){
    shapeNoiseLacunarity = value;
    initializePlanet();
}

void Planet::setShapeNoisePersistance(float value){
    shapeNoisePersistance = value;
    initializePlanet();
}

void Planet::setShapeNoiseFrequency(float value){
    shapeNoiseFrequency = value;
    initializePlanet();
}

void Planet::setShapeNoiseAmplitude(float value){
    shapeNoiseAmplitude = value;
    initializePlanet();
}

void Planet::setShapeNoiseOffset(float value){
    shapeNoiseOffset = value;
    initializePlanet();
}


void Planet::setDetNoiseLayers(int value){
    detNoiseLayers = value;
    initializePlanet();
}

void Planet::setDetNoiseLacunarity(float value){
    detNoiseLacunarity = value;
    initializePlanet();
}

void Planet::setDetNoisePersistance(float value){
    detNoisePersistance = value;
    initializePlanet();
}

void Planet::setDetNoiseFrequency(float value){
    detNoiseFrequency = value;
    initializePlanet();
}

void Planet::setDetNoiseAmplitude(float value){
    detNoiseAmplitude = value;
    initializePlanet();
}

void Planet::setDetNoiseOffset(float value){
    detNoiseOffset = value;
    initializePlanet();
}

void Planet::setRidgeNoiseLayers(int value){
    ridgeNoiseLayers = value;
    initializePlanet();
}

void Planet::setRidgeNoiseLacunarity(float value){
    ridgeNoiseLacunarity = value;
    initializePlanet();
}

void Planet::setRidgeNoisePersistance(float value){
    ridgeNoisePersistance = value;
    initializePlanet();
}

void Planet::setRidgeNoiseFrequency(float value){
    ridgeNoiseFrequency = value;
    initializePlanet();
}

void Planet::setRidgeNoiseAmplitude(float value){
    ridgeNoiseAmplitude = value;
    initializePlanet();
}

void Planet::setRidgeNoiseVShift(float value){
    ridgeNoiseVerticalShift = value;
    initializePlanet();
}

void Planet::setRidgeNoisePower(int value){
    ridgeNoisePower = value;
    initializePlanet();
}

void Planet::setRidgeNoiseOffset(float value){
    ridgeNoiseOffset = value;
    initializePlanet();
}

void Planet::setColor(int colNum, vec3 color){
    colors[colNum] = color;
    initializePlanet();
}

void Planet::setColorHeight(int colNum, float height){
    colorHeights[colNum] = height;
    initializePlanet();
}

void Planet::setColorBlendingRange(int colNum, float range){
    colorBlendingRanges[colNum] = range;
    initializePlanet();
}

void Planet::setNoiseSeed(int value){
    perlinSeed = value;
    initializePlanet();
}

void Planet::setSteepnessThreshold(float value){
    steepnessThreshold = value;
    initializePlanet();
}

void Planet::setRockBlendingFactor(float value){
    rockBlendingFactor = value;
    initializePlanet();
}

void Planet::setNormalMapScale(float value){
    normalMapScale = value;
    initializePlanet();
}

void Planet::togglePostProc(bool state){
    usePostProc = state;
}

void Planet::setViewport(unsigned int vx, unsigned int vy){
    VIEWPORT_X = vx;
    VIEWPORT_Y = vy;
}

void Planet::setPostProcKernel(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22){
    postProcKernel[0][0] = m00;
    postProcKernel[0][1] = m01;
    postProcKernel[0][2] = m02;

    postProcKernel[1][0] = m10;
    postProcKernel[1][1] = m11;
    postProcKernel[1][2] = m12;

    postProcKernel[2][0] = m20;
    postProcKernel[2][1] = m21;
    postProcKernel[2][2] = m22;
}

int Planet::randomizePlanet(int detalization){
    //update random seed
    perlinSeed = rand()/10000 % 10000;
    printf("perlinSeed = %d\n", perlinSeed);
    //recreate planet
    initializePlanet();

    return perlinSeed;
}

int Planet::getMiddleVertex(int p1, int p2){
    for(int i = 0; i < usedVertices[p1].numNeighbours; i++){
        if(usedVertices[p1].neighbours[i] == p2){
            return usedVertices[p1].middleVertex[i];
        }
    }


    int nextNeighbour1 = usedVertices[p1].numNeighbours;
    int nextNeighbour2 = usedVertices[p2].numNeighbours;

    vec3 v1 = vertices[p1].position;
    vec3 v2 = vertices[p2].position;
    
    vertices[numVertices].position = vertices[numVertices].normal = normalize((v2 - v1) * 0.5f + v1);

    usedVertices[p1].neighbours[nextNeighbour1] = p2;
    usedVertices[p1].middleVertex[nextNeighbour1] = numVertices;
    usedVertices[p1].numNeighbours++;
    usedVertices[p2].neighbours[nextNeighbour2] = p1;
    usedVertices[p2].middleVertex[nextNeighbour2] = numVertices;
    usedVertices[p2].numNeighbours++;
    return numVertices++;
}

void Planet::initializeSphere(){
    numVertices = 6;
    numFaces = 8;

    //generate sphere vertices
    for(int iteration = 0; iteration <= detalization; iteration++){
        memset(usedVertices, 0, NUM_VERTICES * sizeof(USED_VERTICES));
        unsigned int currentNumFaces = numFaces;
        for(int face = 0; face < currentNumFaces; face++){
            /*     p3
                   /\
               n6 /__\ n5
                 /\  /\
                /__\/__\
               p1  n4   p2
            */
            unsigned int p1 = faces[face].v1;
            unsigned int p2 = faces[face].v2;
            unsigned int p3 = faces[face].v3;

            vec3 v1 = vertices[p1].position;
            vec3 v2 = vertices[p2].position;
            vec3 v3 = vertices[p3].position;

            unsigned int n4, n5, n6; // new vertices

            n4 = getMiddleVertex(p1,p2);
            n5 = getMiddleVertex(p2,p3);
            n6 = getMiddleVertex(p3,p1);

            //old big face becomes the middle triangle
            faces[face].v1 = n4;
            faces[face].v2 = n5;
            faces[face].v3 = n6;
            // 3 new faces are added
            faces[numFaces].v1 = p1;
            faces[numFaces].v2 = n4;
            faces[numFaces].v3 = n6;

            faces[numFaces + 1].v1 = n4;
            faces[numFaces + 1].v2 = p2;
            faces[numFaces + 1].v3 = n5;

            faces[numFaces + 2].v1 = n6;
            faces[numFaces + 2].v2 = n5;
            faces[numFaces + 2].v3 = p3;

            numFaces += 3;
        }

        printf("iteration: %d, numFaces = %d (0x%x), numVertices = %d (0x%x)\n", iteration, numFaces, numFaces, numVertices, numVertices);
    }
    printf("numVertices = %d NUM_VERTICES = %d\n", numVertices, NUM_VERTICES);
    printf("max vertices = 0x%x\n", NUM_VERTICES);
    printf("max faces = 0x%x\n", NUM_FACES);
}

void Planet::applyNoise(){
    PerlinNoise pn(perlinSeed);

    for(int v = 0; v < numVertices; v++){
        vec3 pos = vertices[v].position;

        float noise = 0;
        float frequency = 0;
        float amplitude = 0;
        float totalNoise = 1;


        /* Apply ridge Noise */
        frequency = ridgeNoiseFrequency;
        amplitude = ridgeNoiseAmplitude;
        noise = 0;
        /*for(int i = 0; i < ridgeNoiseLayers; i++){
            //noise += std::pow(amplitude*(1-(std::abs(2*(pn.noise(pos.x * frequency + ridgeNoiseOffset , pos.y * frequency + ridgeNoiseOffset, pos.z * frequency + ridgeNoiseOffset)-0.5f)))),5);
            noise += amplitude * pow((1- abs(2*((pn.noise(pos.x * frequency + ridgeNoiseOffset , pos.y * frequency + ridgeNoiseOffset, pos.z * frequency + ridgeNoiseOffset)-0.5f)))), ridgeNoiseVerticalShift);
            
        }*/

        for(int i = 0; i < ridgeNoiseLayers; i++){
            float tempNoise = amplitude * (1- abs(2*((pn.noise(pos.x * frequency + ridgeNoiseOffset , pos.y * frequency + ridgeNoiseOffset, pos.z * frequency + ridgeNoiseOffset)-0.5f))));
            if(noise == 0) //in first iteration we can't multiply by 0
                noise += tempNoise;
            else
                noise += noise * tempNoise;

            frequency *= ridgeNoiseLacunarity;
            amplitude *= ridgeNoisePersistance;
        }

        totalNoise += ridgeNoiseVerticalShift * pow(noise, ridgeNoisePower);

        /* Apply Shape Noise */
        frequency = shapeNoiseFrequency;
        amplitude = shapeNoiseAmplitude;
        noise = 0;
        for(int i = 0; i < shapeNoiseLayers; i++){
            //noise += amplitude * ((pn.noise(pos.x * frequency + shapeNoiseOffset , pos.y * frequency +shapeNoiseOffset, pos.z * frequency + shapeNoiseOffset)));
            noise += amplitude * 2*((pn.noise(pos.x * frequency + shapeNoiseOffset , pos.y * frequency +shapeNoiseOffset, pos.z * frequency + shapeNoiseOffset)-0.5f));           
            frequency *= shapeNoiseLacunarity;
            amplitude *= shapeNoisePersistance;
        }
        totalNoise += noise;

        /* Apply Detail Noise */
        
        frequency = detNoiseFrequency;
        amplitude = detNoiseAmplitude;
        noise = 0;
        for(int i = 0; i < detNoiseLayers; i++){
            noise += amplitude * 2*((pn.noise(pos.x * frequency + detNoiseOffset , pos.y * frequency +detNoiseOffset, pos.z * frequency + detNoiseOffset)-0.5f));            
            frequency *= detNoiseLacunarity;
            amplitude *= detNoisePersistance;
        }
        totalNoise += noise;

        noisedVertices[v].position = vertices[v].position * (totalNoise);
        noisedVertices[v].normal = normalize(noisedVertices[v].position);
        
        //make some waves in the ocean
        frequency = 40.0f;
        vertices[v].position = vertices[v].position + vec3(0.005f * pn.noise(pos.x * frequency , pos.y * frequency , pos.z * frequency));
        
        normalsPositions[2 * v].position = noisedVertices[v].position;
        normalsPositions[2 * v + 1].position = noisedVertices[v].position + normalize(noisedVertices[v].position)/30.0f;

    }

    //land normals calculation
    memset(normalsPerVertex, 0, NUM_VERTICES * sizeof(int));
    for(int f = 0; f < numFaces; f++){
        vec3 v1 = noisedVertices[faces[f].v1].position;
        vec3 v2 = noisedVertices[faces[f].v2].position;
        vec3 v3 = noisedVertices[faces[f].v3].position;
        noisedVertices[faces[f].v1].normal += normalize(cross(v2-v1 , v3 - v1));
        normalsPerVertex[faces[f].v1]++;
        noisedVertices[faces[f].v2].normal += normalize(cross(v3-v2 , v1 - v2));
        normalsPerVertex[faces[f].v2]++;
        noisedVertices[faces[f].v3].normal += normalize(cross(v1-v3 , v2 - v3));
        normalsPerVertex[faces[f].v3]++;
    }

    for(int v = 0; v < numVertices; v++){
        noisedVertices[v].normal /= normalsPerVertex[v];
        normalsPositions[2 * v].position = noisedVertices[v].position;
        normalsPositions[2 * v + 1].position = noisedVertices[v].position + noisedVertices[v].normal/30.0f;

        vertices[v].normal /= normalsPerVertex[v];
    }

    //ocean normals calculations
    memset(normalsPerVertex, 0, NUM_VERTICES * sizeof(int));
    for(int f = 0; f < numFaces; f++){
         vec3 v1 = vertices[faces[f].v1].position;
         vec3 v2 = vertices[faces[f].v2].position;
         vec3 v3 = vertices[faces[f].v3].position;
         vertices[faces[f].v1].normal += normalize(cross(v2-v1 , v3 - v1));
         normalsPerVertex[faces[f].v1]++;
         vertices[faces[f].v2].normal += normalize(cross(v3-v2 , v1 - v2));
         normalsPerVertex[faces[f].v2]++;
         vertices[faces[f].v3].normal += normalize(cross(v1-v3 , v2 - v3));
         normalsPerVertex[faces[f].v3]++;       
    }

    for(int v = 0; v < numVertices; v++){
        vertices[v].normal /= normalsPerVertex[v];
    }
}

STATUS init()
{

    srand(time(0));

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return ERROR;
    }

    return OK;
}


void Planet::initializePlanet()
{
    STATUS status = OK;

    NUM_FACES = (1<<(5+detalization*2)); //calculate expected number of faces
    NUM_VERTICES = ((1<<(4+detalization*2)) + 2); //calculate expected number of vertices

    //alloate all memories
    faces = new FACE[NUM_FACES];// (FACE*)malloc(NUM_FACES * sizeof(FACE));
    memset(faces, 0, NUM_FACES * sizeof(FACE));
    faces[0] = { 0, 4, 2 };    //0
    faces[1] = { 4, 1, 2 };    //1
    faces[2] = { 0, 3, 4 };    //2
    faces[3] = { 4, 3, 1 };  //3
    faces[4] = { 0, 2, 5 };   //4
    faces[5] = { 1, 5, 2 };    //5
    faces[6] = { 0, 5, 3 };   //6
    faces[7] = { 3, 5, 1 };    //7

    vertices = new VERTEX[NUM_VERTICES]; //(VERTEX*)malloc(NUM_VERTICES * sizeof(VERTEX));
    memset(vertices, 0, NUM_VERTICES * sizeof(VERTEX));
    vertices[0] = { vec3(-1.0f, 0.0f, 0.0f),     vec3(-1.0f,  0.0f, 0.0f) };  //0
    vertices[1] = { vec3(1.0f, 0.0f, 0.0f),      vec3(1.0f,  0.0f, 0.0f) };  //1
    vertices[2] = { vec3(0.0f, 1.0f, 0.0f),      vec3(0.0f,  1.0f, 0.0f) };   //2
    vertices[3] = { vec3(0.0f, -1.0f, 0.0f),     vec3(0.0f,  -1.0f, 0.0f) };  //3
    vertices[4] = { vec3(0.0f, 0.0f, 1.0f),      vec3(0.0f,  0.0f, 1.0f) };   //4
    vertices[5] = { vec3(0.0f, 0.0f, -1.0f),     vec3(0.0f,  0.0f, -1.0f) };  //5

    normalsPerVertex = new int[NUM_VERTICES]; //(int*)malloc(NUM_VERTICES * sizeof(int));
    memset(normalsPerVertex, 0, NUM_VERTICES * sizeof(int));

    usedVertices = new USED_VERTICES[NUM_VERTICES];
    memset(usedVertices, 0, NUM_VERTICES * sizeof(USED_VERTICES));

    noisedVertices = new NOISED_VERTEX[NUM_VERTICES];
    memset(noisedVertices, 0, NUM_VERTICES * sizeof(NOISED_VERTEX));

    normalsPositions = new NORMAL_POSITION[ 2 * NUM_VERTICES];
    memset(normalsPositions, 0, 2 * NUM_VERTICES * sizeof(NORMAL_POSITION));

    
    initializeSphere();
    applyNoise();



    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * sizeof(NOISED_VERTEX), noisedVertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_FACES * sizeof(FACE), faces, GL_STATIC_DRAW);
    //vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindVertexArray(oceanVAO);
    glBindBuffer(GL_ARRAY_BUFFER, oceanVBO);
    glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * sizeof(VERTEX), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oceanEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_FACES * sizeof(FACE), faces, GL_STATIC_DRAW);
    
    //vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    glBindVertexArray(normalsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * NUM_VERTICES * sizeof(NORMAL_POSITION), normalsPositions, GL_STATIC_DRAW);
    //vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //POST PROCESSING PLANE
    static float plane[] = {
        // positions          // texture Coords
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
    };


    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), &plane, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    //BUFFER FOR POST PROCESSING
    //generate FBO
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);   
    // generate texture
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);

    printf("VIEWPORT_X = %d VIEWPORT_Y = %d\n",VIEWPORT_X, VIEWPORT_Y );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIEWPORT_X, VIEWPORT_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    //create render buffer object for depth and stencil buffers
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, VIEWPORT_X, VIEWPORT_Y);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //check if frame buffer was successfully created
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 


    //free malloced arrays
    delete[] faces;
    delete[] vertices;
    delete[] normalsPerVertex;
    delete[] usedVertices;
    delete[] noisedVertices;
    delete[] normalsPositions;

    return;
}


void Planet::setupDrawParameters(){

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glEnable(GL_DEPTH_TEST);

    //filled or wireframe triangles
    if(showWireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

}

void Planet::drawPlanet(){
    // activate shader
    planetShader->use();
    planetShader->setVec3("objectColor", glm::vec3(0.4f, 0.4f, 0.4f));
    planetShader->setVec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
    planetShader->setVec3("lightPos", lightPos);
    planetShader->setVec3("viewPos", camera->Position);   

    // set the model, view and projection matrix uniforms
    planetShader->setMat4("model", model);
    planetShader->setMat4("view", view);
    planetShader->setMat4("projection", projection);
    planetShader->setMat4("normalMat", normalMat);

    planetShader->setVec3("color0", colors[0]);
    planetShader->setVec3("color1", colors[1]);
    planetShader->setVec3("color2", colors[2]);
    planetShader->setVec3("color3", colors[3]);
    planetShader->setVec3("color4", colors[4]);

    planetShader->setFloat("colorHeight0", colorHeights[0]);
    planetShader->setFloat("colorHeight1", colorHeights[1]);
    planetShader->setFloat("colorHeight2", colorHeights[2]);
    planetShader->setFloat("colorHeight3", colorHeights[3]);
    planetShader->setFloat("colorHeight4", colorHeights[4]);

    planetShader->setFloat("colorBlendingRange1", colorBlendingRanges[1]);
    planetShader->setFloat("colorBlendingRange2", colorBlendingRanges[2]);
    planetShader->setFloat("colorBlendingRange3", colorBlendingRanges[3]);

    planetShader->setFloat("steepnessThreshold", steepnessThreshold);
    planetShader->setFloat("rockBlendingFactor", rockBlendingFactor);
    planetShader->setFloat("normalMapScale", normalMapScale);

    glBindVertexArray(sphereVAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, normalMap);  

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glDrawElements(GL_TRIANGLES, NUM_FACES * sizeof(FACE), GL_UNSIGNED_INT, 0);

}

void Planet::drawOcean(){
    // activate shader
    oceanShader->use();
    oceanShader->setVec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
    oceanShader->setVec3("lightPos", lightPos);
    oceanShader->setVec3("viewPos", camera->Position);   

    // set the model, view and projection matrix uniforms
    oceanShader->setMat4("model", model);
    oceanShader->setMat4("view", view);
    oceanShader->setMat4("projection", projection);
    oceanShader->setMat4("normalMat", normalMat);

    oceanShader->setVec3("objectColor", glm::vec3(0.0f, 0.0f, 0.5f));
    oceanShader->setFloat("time", (float)glfwGetTime()/100);

    glBindVertexArray(oceanVAO);
    glDrawElements(GL_TRIANGLES, NUM_FACES * sizeof(FACE), GL_UNSIGNED_INT, 0);

}

void Planet::drawNormals(){
    // activate shader
    normalsShader->use();
    // set the model, view and projection matrix uniforms
    normalsShader->setMat4("model", model);
    normalsShader->setMat4("view", view);
    normalsShader->setMat4("projection", projection);
    normalsShader->setMat4("normalMat", normalMat);

    glBindVertexArray(normalsVAO);
    glDrawArrays(GL_LINES, 0, 2*numVertices);
}

void Planet::planetDraw(){
    
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    setupDrawParameters();
    
    if(usePostProc)
        //draw to texture then postprocess
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    else
        //draw to screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.7f, 0.7f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //glm::mat4 lightPosTransform = glm::rotate(glm::mat4(1.0f),0.01f, glm::vec3(0.0f, -1.0f, 0.0f));
    //lightPos = lightPosTransform * glm::vec4(lightPos,1.0f);

    //create model, view and projection matrices
    model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime()/10, glm::vec3(0.0f, 1.0f, 0.0f));
    view = camera->GetViewMatrix();
    projection = glm::perspective(glm::radians(camera->Zoom), (float)VIEWPORT_X / (float)VIEWPORT_Y, 0.1f, 100.0f);
    normalMat = glm::transpose(glm::inverse(model));

    drawPlanet();

    if(showOcean)
        drawOcean();
    if(showNormals)
        drawNormals();

    
    if(usePostProc){
        //NOW POST-PROCESS the image
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // plane VAO
        postProcessingShader->use();    
        postProcessingShader->setMat3("postProcKernel", postProcKernel);
    
        glClearColor(0.1f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6); 
    }
}

void showFrameRate(){

	static double previousTime;
	static int frameCount;
	double currentTime = glfwGetTime();
	double deltaMs = (currentTime - previousTime) * 1000;
	frameCount++;
	if ( currentTime - previousTime >= 1.0 )
	{
		// Display the frame count here any way you want.
		std::stringstream ss;
		ss << "FPS: " << frameCount;
        printf("FPS = %d\n", frameCount);
		//glfwSetWindowTitle(window, ss.str().c_str());

		frameCount = 0;
		previousTime = currentTime;
	}
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Planet::processInput(int key, int action)
{
    if (key == GLFW_KEY_W && action/* == GLFW_PRESS*/)
        camera->ProcessKeyboard(FORWARD, deltaTime);

    if (key == GLFW_KEY_S && action)
        camera->ProcessKeyboard(BACKWARD, deltaTime);

    if (key == GLFW_KEY_A && action)
        camera->ProcessKeyboard(LEFT, deltaTime);
    
    if (key == GLFW_KEY_D && action)
        camera->ProcessKeyboard(RIGHT, deltaTime);



    int cameraSpeed = 3.0f;
    if (key == GLFW_KEY_LEFT && action)
        camera->ProcessMouseMovement(-cameraSpeed, 0.0f);
    if (key == GLFW_KEY_RIGHT && action)
        camera->ProcessMouseMovement(cameraSpeed, 0.0f);
    if (key == GLFW_KEY_UP && action)
        camera->ProcessMouseMovement(0.0f, cameraSpeed);
    if (key == GLFW_KEY_DOWN && action)
        camera->ProcessMouseMovement(0.0f, -cameraSpeed);
    
}

Planet::Planet() {
    //init project
    STATUS status = init();    
    if(status != OK)
    {
        cout << "init() failed at " << __FILE__  << ":" << __LINE__ << __FUNCTION__ << endl;
        return;
    }

    planetShader = new Shader("shaders/planetShader2.vs", "shaders/planetShader2.fs"/*, "shaders/planetShader2.gs"*/);
    oceanShader =  new Shader("shaders/oceanShader.vs", "shaders/oceanShader.fs"/*, "shaders/planetShader2.gs"*/);
    normalsShader = new Shader("shaders/normalsShader.vs", "shaders/normalsShader.fs"/*, "shaders/planetShader2.gs"*/);
    postProcessingShader = new Shader("shaders/postProcessingShader.vs", "shaders/postProcessingShader.fs"/*, "shaders/planetShader2.gs"*/);

    //normalMap = loadTexture("images/planetNormalMap.png");
    normalMap = loadTexture("images/planetNormalMap_s.jpg");
    //normalMap = loadTexture("../LearnOpenGL/LearnOpenGL/images/brickwall_normal.jpg");
    //normalMap = loadTexture("images/nums.png");

    camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));
    lightPos = vec3(0.0f, 0.0f, 8.0f);

    perlinSeed = 1;
    printf("perlinSeed = %d\n", perlinSeed);

    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &oceanVBO);
    glGenBuffers(1, &oceanEBO);
    glGenVertexArrays(1, &oceanVAO);
    glGenBuffers(1, &normalsVBO);
    glGenVertexArrays(1, &normalsVAO);
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenFramebuffers(1, &framebuffer);
    glGenTextures(1, &texColorBuffer);
    glGenRenderbuffers(1, &rbo);

    detalization = 5;

    shapeNoiseLayers = 0;
    shapeNoiseLacunarity = 1.0f;
    shapeNoisePersistance = 1.0f;
    shapeNoiseFrequency = 0.7f;
    shapeNoiseAmplitude = 0.5f;
    shapeNoiseOffset = 0.0f;

    ridgeNoiseLayers = 0;
    ridgeNoiseLacunarity = 2.0f;
    ridgeNoisePersistance = 0.05f;
    ridgeNoiseFrequency = 1.0f;
    ridgeNoiseAmplitude = 0.7f;
    ridgeNoiseVerticalShift = 2.0f;
    ridgeNoisePower = 10;
    ridgeNoiseOffset = 0.0f;

    detNoiseLayers = 5;
    detNoiseLacunarity = 2.0f;
    detNoisePersistance = 0.5f;
    detNoiseFrequency = 1.0f;
    detNoiseAmplitude = 0.3f;
    detNoiseOffset = 0.0f;

    colors[0] = vec3(0, 0, 0.1f);
    colors[1] = vec3(0.6, 0.6, 0.1f);
    colors[2] = vec3(0.1, 0.5, 0.1f);
    colors[3] = vec3(1.0, 1.0, 1.0f);
    colors[4] = vec3(0.5, 0.2, 0.0f);

    colorHeights[0] = 0.0;
    colorHeights[1] = 0.9;
    colorHeights[2] = 1.05;
    colorHeights[3] = 1.2;
    colorHeights[4] = 0.0; // not used

    colorBlendingRanges[0] = 0.0; // not used
    colorBlendingRanges[1] = 0.03;
    colorBlendingRanges[2] = 0.03;
    colorBlendingRanges[3] = 0.03;
    colorBlendingRanges[4] = 0.0; // not used

    steepnessThreshold = 0.9f;
    rockBlendingFactor = 0.04f;
    normalMapScale = 1.0f;

    postProcKernel = glm::mat3(0.0);
    postProcKernel[1][1] = 1.0;

    initializePlanet();
}




// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}