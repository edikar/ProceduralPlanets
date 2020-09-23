#ifndef PLANET_H
#define PLANET_H
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "camera.h"
#include "shader_class.h"
#include <glm/glm.hpp>

using glm::vec3;
using glm::mat4;

typedef struct {
    vec3 position;
    vec3 normal;
} VERTEX;

typedef struct {
    int numNeighbours;
    int neighbours[6];
    int middleVertex[6];
} USED_VERTICES;

typedef struct {
    vec3 position;
    vec3 normal;
} NOISED_VERTEX;

typedef struct {
    vec3 position;
} NORMAL_POSITION;

typedef struct {
    unsigned int v1;
    unsigned int v2;
    unsigned int v3;
} FACE;


void processInput(int,int);



class Planet {
public:
	Planet();
	~Planet();
	
	void planetDraw();
	void toggleOcean(bool);
	void toggleNormals(bool);
	void toggleWireframe(bool);
	int randomizePlanet(int);
	void processInput(int,int);
	void setDetalization(int);
	void setNoiseSeed(int);
	void setHeightColor(int, vec3);
	void setSteepnessThreshold(float);

	//shape noise setters
	void setShapeNoiseLayers(int);
	void setShapeNoiseLacunarity(float);
	void setShapeNoisePersistance(float);
	void setShapeNoiseFrequency(float);
	void setShapeNoiseAmplitude(float);
	void setShapeNoiseOffset(float);

	//ridge noise setters
	void setRidgeNoiseLayers(int);
	void setRidgeNoiseLacunarity(float);
	void setRidgeNoisePersistance(float);
	void setRidgeNoiseFrequency(float);
	void setRidgeNoiseAmplitude(float);
	void setRidgeNoiseVShift(float);
	void setRidgeNoisePower(int);
	void setRidgeNoiseOffset(float);

	//detail noise setters
	void setDetNoiseLayers(int);
	void setDetNoiseLacunarity(float);
	void setDetNoisePersistance(float);
	void setDetNoiseFrequency(float);
	void setDetNoiseAmplitude(float);
	void setDetNoiseOffset(float);

private:

	//matrices
	mat4 model;
	mat4 view;
	mat4 projection;
	mat4 normalMat;
	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	// camera
	Camera *camera;
	//shaders	
	Shader *planetShader;
	Shader *oceanShader;;
	Shader *normalsShader;
	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;
	//light position
	vec3 lightPos;
	//GL objects
	unsigned int sphereVBO, sphereVAO, sphereEBO;
	unsigned int oceanVBO, oceanVAO, oceanEBO;
	unsigned int normalsVBO, normalsVAO;

	unsigned int normalMap;

	//controls
	bool showNormals = 0;
	bool showOcean = 1;
	bool showWireframe = 0;
	//planet generation parameters
	int  detalization;
	int perlinSeed;

	// shape noise
	int   shapeNoiseLayers;
	float shapeNoiseAmplitude;
	float shapeNoiseFrequency;
	float shapeNoiseLacunarity;
	float shapeNoisePersistance;
	float shapeNoiseOffset;

	// ridge noise
	int   ridgeNoiseLayers;
	float ridgeNoiseAmplitude;
	float ridgeNoiseFrequency;
	float ridgeNoiseLacunarity;
	float ridgeNoisePersistance;
	float ridgeNoiseVerticalShift;
	int ridgeNoisePower;
	float ridgeNoiseOffset;

	// detalization noise
	int detNoiseLayers;
	float detNoiseAmplitude;
	float detNoiseFrequency;
	float detNoiseLacunarity;
	float detNoisePersistance;
	float detNoiseOffset;

	//height colors
	vec3 heightColors[5];
	float steepnessThreshold;



	//These are temporary pointers for planet generations - probalby should not be here...
	VERTEX *vertices;
	int *normalsPerVertex; //this is needed to count number of faces each vertex involved in to average the normal
	USED_VERTICES *usedVertices;   //this array stores the vertices which were already split in current iteration - to prevent creating same vertex twice
	NOISED_VERTEX *noisedVertices;
	NORMAL_POSITION *normalsPositions;
	FACE *faces;

	int numVertices = 6;
	int numFaces = 8;

	int NUM_FACES;
	int NUM_VERTICES;


	void initializePlanet();

	//these are helper functions
	void applyNoise();
	void initializeSphere();
	int getMiddleVertex(int, int);
	void drawPlanet();
	void drawOcean();
	void drawNormals();
	void setupDrawParameters();
};

#endif /* PLANET_H */