#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "rapidjson\document.h"
#include "rapidjson\filereadstream.h"
#include "rapidjson\reader.h"

#include "../dependancies/MetaSystem/MetaSystem.h"
#include "Renderer.h"
#include "Resource.h"

class Resource;

using namespace std;
using namespace rapidjson;

class ResourceManager
{
public:
	static ResourceManager*					getInstance();											// Creates and returns an instance of resource manager

	void									init(Renderer* renderer);								// Initialises the resource manager
	void									destroy();												// Deletes the variable m_instance
	void									update(float dt);										// Checks for file changes

	SDL_Texture*							getTextureByKey(string key);							// Returns the texture mapped to the string argument
	Mix_Music*								getMusicByKey(string key);								// Returns the music file mapped to the string argument
	Mix_Chunk*								getSoundEffectByKey(string key);						// Returns the sound effect mapped to the string argument
	pair<SDL_Texture*, vector<Rect>>		getAnimationByKey(string key);							// Returns the texture and animation frames mapped to the string argument

	void									loadResources(string fileName);							// Adds all resources to the queue from a JSON file
	void									loadResourceQueue();									// Loads all of the resources in the queue 

	Renderer*								getRenderer();
	map<string, pair<SDL_Texture*, tm>>*	getTextures();
	map<string, Mix_Music*>*				getMusic();
	map<string, Mix_Chunk*>*				getSoundEffects();
	tm										getTimeInfo(const char* path);							// Returns the time information for the file, passing the path to the file as an argument

private:
	static ResourceManager*					m_instance;												// The instance of the singleton

	map<string, pair<SDL_Texture*, tm>>		m_textures;												// The map to the textures
	map<string, Mix_Music*>					m_music;												// The map to the music files
	map<string, Mix_Chunk*>					m_soundEffects;											// The map to the sound effect

	map<string, pair<vector<Rect>, tm>>		m_animations;											// The map to the animation frames

	map<string, string>						m_path;													// The map to the paths of all resources

	vector<Resource*>						m_resourceQueue;										// The resource queue we use to load all the resources

	float									m_resourcesLoaded;										// The number of resources loaded
	float									m_fileCheckDelay;										// The delay for checking the files
	string									m_source;												// The source file that we use to load all resources
	tm										m_sourceTimeInfo;										// The last time the source file was changed

	Renderer*								m_renderer;												// The renderer

	void									addResourceToQueue(Resource* resource);					// Adds a resource to the queue
	void									checkJsonObject(const Value& object, string type);		// Checks the JSON object for the values for each resource
	void									reloadFromJSON(string key, string filePath);								// Reloads the animation frames from a JSON file
	vector<Rect>							getAnimationFrames(string key);							// Returns the animation frames, passing the key for the map as an argument
	void									loadAnimations(string key, string filePath);

	ResourceManager();																				// Standard constructor - does nothing
	~ResourceManager();																				// Standard destructor - destroys all resources
};
