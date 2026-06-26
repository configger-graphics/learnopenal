// source: https://www.openal.org/documentation/openal-1.1-specification.pdf
// source https://www.openal.org/documentation/OpenAL_Programmers_Guide.pdf
// This is where I get my information, I adapt and transform it into new information for the user to understand.

#include <iostream>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <AL/al.h>
#include <AL/alc.h>
#define STB_VORBIS_IMPLEMENTATION
#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

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
	
	alSourcei(SBO, AL_BUFFER, (ALuint)ABO);
	// some looping code I'll add in later for listeners positon and source position
  return 0;
}
