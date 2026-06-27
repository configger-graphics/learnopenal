// source: https://www.openal.org/documentation/openal-1.1-specification.pdf
// source https://www.openal.org/documentation/OpenAL_Programmers_Guide.pdf
// This is where I get my information, I adapt and transform it into new information for the user to understand.

#include <iostream>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// the newly added OpenGL Mathematic abstractionsi
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <AL/al.h>
#include <AL/alc.h>
#define STB_VORBIS_IMPLEMENTATION
#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"
#include <AbstractedStuff.hpp>

int main() {
  	if (window == NULL) {
		std::cout << "Failed to create window" << "\n";
		glfwTerminate();
		return -1;
  }
  glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
            std::cout << "Failed to initialize glad" << "\n";
            return -1;
  }

 	glViewport(0, 0, 800, 600);
	
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // where your OpenAL Code starts
  
	ALCdevice* AudioDevice = alcOpenDevice(NULL); // perferred device
	
	if (AudioDevice) {
		ALCcontext* Context = alcCreateContext(AudioDevice,NULL);
		if (Context != NULL) {
			alcMakeContextCurrent(Context);
		}
	}
	unsigned int ABO;
	alGenBuffers(1, &ABO);
	if ((error = alGetError()) != AL_NO_ERROR) {
    	switch (error) {
      		case AL_INVALID_NAME: std::cout << "Invalid name\n"; break;
      		case AL_INVALID_ENUM: std::cout << "Invalid enum\n"; break;
      		case AL_INVALID_VALUE: std::cout << "Invalid value\n"; break;
      		case AL_INVALID_OPERATION: std::cout << "Invalid operation!\n"; break;
      		case AL_OUT_OF_MEMORY: std::cout << "Out of memory!\n"; break;
    }
    alDeleteBuffers(1, &ABO);
    return 1;
  }
	unsigned int SBO;
	alGenSources(1, &SBO);

	int NrChannels;
	int sampleRate;
	short* output;

	int numSamples = stb_vorbis_decode_filename((char*)"", &NrChannels, &sampleRate, &output);
	if (numSamples < 0) {
		std::cout << "failed to decode OGG file\n";
		alDeleteBuffers(1, &ABO);
		alDeleteSources(1, &SBO);
		return 1;
	}
	ALenum format;
	if (NrChannels == 1) {
		format = AL_FORMAT_MONO16;
	} else if (NrChannels == 2) {
		format = AL_FORMAT_STEREO16;
	} else {
		std::cout << "too many, not supported.\n";
		alDeleteBuffers(1, &ABO);
		alDeleteSources(1, &SBO);
		return 1;
	}
	
	alSourcei(SBO, AL_BUFFER, (ALint)ABO); // why convert it to an ALint? that is because this function (alSourcei) accepts negatives for other AL_stuff.
	glm::mat4 SBOLoc = glm::mat4(1.0f);
	SBOLoc = glm::translate(SBOLoc, glm::vec3(0.0f, 0.0f, 0.0f));
	alSourcefv(SBO, AL_POSITION, SBOLoc);
	alSource3f(SBO, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSource3f(SBO, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
	OrientationArray = {camFront.x, camFront.y, camFront.z, camUp.x, camUp.y, camUp.z};
	LOAL::someBindingAbstraction(); // assuming you know what this does. If you don't, it just binds a cube, that's all.
	float OrientationArray;
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
		LOAL::processInput(window);
		OrientationArray = {camFront.x, camFront.y, camFront.z, camUp.x, camUp.y, camUp.z};
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(LOAL::fov), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(LOAL::camPos, LOAL::camPos + LOAL::camFront, LOAL::camUp);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		LOAL::uniformAbstraction(model, view, projection);
		alListenerfv(AL_POSITION, camPos);
		alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		alListenerfv(AL_ORIENTATION, OrientationArray);
		LOAL::drawingAbstraction();
	} 
	Context=alcGetCurrentContext();
	Device=alcGetContextsDevice(Context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device); 
	return 0;
}
