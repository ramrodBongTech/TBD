#pragma once
#include <string>
#include <functional>
using namespace std;
#include "sdl\SDL_image.h"
#include "sdl\SDL_mixer.h"
#include <map>

#include "ResourceManager.h";

// Custom exception struct
// Causes a crash upon bad load
struct LoadException : public std::exception
{
	LoadException(string ss) { printf(ss.c_str()); }
	~LoadException() throw () {}
};

// Inline function to check if a string is within the file
inline bool doesFileExists(const string& name)
{
	ifstream _ifs(name.c_str());
	if (_ifs.good())
	{
		_ifs.close();
		return true;
	}
	else
	{
		_ifs.close();
		return false;
	}
}

struct Resource
{
public:
	Resource(string key) : m_key(key){}

	virtual string getKey(){ return m_key; }
	virtual void load() = 0;

protected:
	string m_key;

};

struct Texture : Resource
{
public:
	Texture(string key, string textureDir) : Resource(key), m_textureDir(textureDir) {}

	string m_textureDir;

	void load();
	
};

struct Music : Resource
{
	Music(string key, string musicDir) : Resource(key), m_musicDir(musicDir) {}

	string m_musicDir;

	void load();
};

struct SoundEffect : Resource
{
	SoundEffect(string key, string soundEffectDir) : Resource(key), m_soundEffectDir(soundEffectDir) {}

	string m_soundEffectDir;

	void load();
};
