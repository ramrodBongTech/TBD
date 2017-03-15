#include "stdafx.h"
#include "Resource.h"

void Texture::load()
{
	// Load the new texture and throw an exception if we cannot
	Renderer* renderer = ResourceManager::getInstance()->getRenderer();
	map<string, pair<SDL_Texture*, tm>>* textures = ResourceManager::getInstance()->getTextures();

	SDL_Texture* _temp = IMG_LoadTexture(renderer->getRenderer(), m_textureDir.c_str());

	// If the file does not exist we throw an exception
	if (!doesFileExists(m_textureDir.c_str()))
		throw(LoadException("Could not load sound effect " + m_key + " from " + m_textureDir));

	if (_temp == 0)
		throw(LoadException("Could not load texture " + m_key + " from " + m_textureDir + "\n" + IMG_GetError() + "\n"));

	// Destroy the old texture and set the new one and the new time data
	if (textures->find(m_key) != textures->end())
		SDL_DestroyTexture((*textures)[m_key].first);
	(*textures)[m_key] = std::make_pair(_temp, ResourceManager::getInstance()->getTimeInfo(m_textureDir.c_str()));

}

void Music::load()
{
	map<string, Mix_Music*>* music = ResourceManager::getInstance()->getMusic();

	Mix_Music* _temp = nullptr;

	// If the file does not exist we throw an exception
	if (!doesFileExists(m_musicDir.c_str()))
		throw(LoadException("Could not load sound effect " + m_key + " from " + m_musicDir));

	// Load the music file and throw an exception if it does not load
	_temp = Mix_LoadMUS(m_musicDir.c_str());
	if (_temp == 0)
		throw(LoadException("Could not load music " + m_key + " from " + m_musicDir + "\n" + Mix_GetError() + "\n"));

	// Add the music file to the map using the key passed
	if (music->find(m_key) != music->end())
		Mix_FreeMusic((*music)[m_key]);
	(*music)[m_key] = _temp;
}

void SoundEffect::load()
{
	map<string, Mix_Chunk*>* effects = ResourceManager::getInstance()->getSoundEffects();

	Mix_Chunk* _temp = nullptr;

	// If the file does not exist we throw an exception
	if (!doesFileExists(m_soundEffectDir.c_str()))
		throw(LoadException("Could not load sound effect " + m_key + " from " + m_soundEffectDir));

	// Load the sound effect and throw an exception if it does not load
	_temp = Mix_LoadWAV(m_soundEffectDir.c_str());
	if (_temp == 0)
		throw(LoadException("Could not load sound effect " + m_key + " from " + m_soundEffectDir + "\n" + Mix_GetError() + "\n"));

	// Add the sound effect to the map using the key passed
	if (effects->find(m_key) != effects->end())
		Mix_FreeChunk((*effects)[m_key]);
	(*effects)[m_key] = _temp;
}
