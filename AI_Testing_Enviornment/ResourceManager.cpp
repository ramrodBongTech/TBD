#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager * ResourceManager::m_instance = nullptr;

const int MAX_DELAY = 3;

ResourceManager* ResourceManager::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new ResourceManager;
	}

	return m_instance;
}

void ResourceManager::init(Renderer* renderer)
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		std::cout << "Error initialising SDL audio!!" << std::endl;
	}

	m_renderer = renderer;
}

void ResourceManager::destroy()
{
	delete m_instance;
	m_instance = nullptr;
}

void ResourceManager::update(float dt)
{
	FileMonitor::getInstance().MonitorFiles(dt);
}

SDL_Texture* ResourceManager::getTextureByKey(string key)
{
	auto _texture = m_textures.find(key);

	if (_texture != m_textures.end())
	{
		return _texture->second.first;
	}
	else if (m_textures.find("placeholder") != m_textures.end())
	{
		return m_textures["placeholder"].first;
	}
	else
	{
		return nullptr;
	}
}

Mix_Music* ResourceManager::getMusicByKey(string key)
{
	auto _music = m_music.find(key);

	if (_music != m_music.end())
	{
		return _music->second;
	}
	else if (m_music.find("music_placeholder") != m_music.end())
	{
		return m_music["music_placeholder"];
	}
	else
	{
		return nullptr;
	}
}

Mix_Chunk* ResourceManager::getSoundEffectByKey(string key)
{
	auto _soundEffect = m_soundEffects.find(key);

	if (_soundEffect != m_soundEffects.end())
	{
		return _soundEffect->second;
	}
	else if (m_soundEffects.find("sound_effect_placeholder") != m_soundEffects.end())
	{
		return m_soundEffects["sound_effect_placeholder"];
	}
	else
	{
		return nullptr;
	}
}

pair<SDL_Texture*, vector<Rect>> ResourceManager::getAnimationByKey(string key)
{
	pair<SDL_Texture*, vector<Rect>> _temp;
	_temp.first = getTextureByKey(key);
	_temp.second = getAnimationFrames(key);
	return _temp;
}

void ResourceManager::loadResources(string fileName)
{
	FILE* _file = new FILE();
	fopen_s(&_file, fileName.c_str(), "rb");
	char readBuffer[65536];
	FileReadStream _is(_file, readBuffer, sizeof(readBuffer));
	Document _document;
	_document.ParseStream(_is);
	fclose(_file);

	const Value& _resources = _document["resources"];
	for (Value::ConstMemberIterator _it = _resources.MemberBegin(); _it != _resources.MemberEnd(); ++_it)
	{
		string name = _it->name.GetString();

		if (name == "textures")
		{
			for (const auto& itr: _resources[name.c_str()].GetArray())
			{
				checkJsonObject(itr, "texture");
			}
		}
		else if (name == "music")
		{
			for (const auto& itr : _resources[name.c_str()].GetArray())
			{
				checkJsonObject(itr, "music");
			}
		}
		else if (name == "effects")
		{
			for (const auto& itr : _resources[name.c_str()].GetArray())
			{
				checkJsonObject(itr, "effect");
			}
		}
		else if (name == "animations")
		{
			for (const auto& itr : _resources[name.c_str()].GetArray())
			{
				checkJsonObject(itr, "animation");
			}
		}
	}
}

void ResourceManager::loadResourceQueue()
{
	// Loads each of the resources
	for (auto resource : m_resourceQueue)
	{
		resource->load();
		FileMonitor::getInstance().WatchFile(m_path[resource->getKey().c_str()], std::bind(&Resource::load, resource));
	}
}

Renderer* ResourceManager::getRenderer()
{
	return m_renderer;
}

map<string, pair<SDL_Texture*, tm>>* ResourceManager::getTextures() 
{
	return &m_textures;
}

map<string, Mix_Music*>* ResourceManager::getMusic()
{
	return &m_music;
}

map<string, Mix_Chunk*>* ResourceManager::getSoundEffects()
{
	return &m_soundEffects;
}

void ResourceManager::addResourceToQueue(Resource* resource)
{
	// Get one of each resource using the resource passed
	Texture* _textureResource = dynamic_cast<Texture*>(resource);
	Music* _musicResource = dynamic_cast<Music*>(resource);
	SoundEffect* _soundEffectResource = dynamic_cast<SoundEffect*>(resource);

	// Check each resource and add the path to the map for the resource that exists
	if (_textureResource)
	{
		m_path[_textureResource->getKey()] = _textureResource->m_textureDir;
	}
	else if (_musicResource)
	{
		m_path[_musicResource->getKey()] = _musicResource->m_musicDir;
	}
	else
	{
		m_path[_soundEffectResource->getKey()] = _soundEffectResource->m_soundEffectDir;
	}

	// Add the resource to the queue
	m_resourceQueue.push_back(resource);
}

void ResourceManager::checkJsonObject(const Value& object, string type)
{
	// Get the key and path for the resource
	string _key = object["key"].GetString();
	string _path = object["path"].GetString();

	m_path[_key] = _path;

	// Add resource to the queue based on the type of resource
	if (type == "texture")
	{
		addResourceToQueue(new Texture(_key, _path));
	}
	else if (type == "music")
	{
		addResourceToQueue(new Music(_key, _path));
	}
	else if (type == "effect")
	{
		addResourceToQueue(new SoundEffect(_key, _path));
	}
	else
	{
		addResourceToQueue(new Texture(_key, _path));
		string _meta = object["metaPath"].GetString();
		loadAnimations(_key, _meta);
	}
}

void ResourceManager::reloadFromJSON(string key, string filePath)
{
	FILE* _file = new FILE();
	fopen_s(&_file, m_path[filePath].c_str(), "rb");
	char readBuffer[65536];
	FileReadStream _is(_file, readBuffer, sizeof(readBuffer));
	Document _document;
	_document.ParseStream(_is);
	fclose(_file);

	vector<Rect>  _animationList;
	for (const auto& frame : _document["frames"].GetArray())
	{
		Rect _tempRect = Rect();
		_tempRect.pos.x = frame["frame"]["x"].GetDouble();
		_tempRect.pos.y = frame["frame"]["y"].GetDouble();
		_tempRect.size.w = frame["frame"]["w"].GetDouble();
		_tempRect.size.h = frame["frame"]["h"].GetDouble();

		_animationList.push_back(_tempRect);
	}
	m_animations[key].first = _animationList;
	m_animations[key].second = getTimeInfo(m_path[filePath].c_str());
}

tm ResourceManager::getTimeInfo(const char* path)
{
	struct stat _result;
	if (stat(path, &_result) == 0)					// Gets the data for the file
	{
		tm _timeInfo = tm();
		localtime_s(&_timeInfo, &_result.st_mtime);	// Turns the file data into time data
		return _timeInfo;
	}
}

vector<Rect> ResourceManager::getAnimationFrames(string key)
{
	if (m_animations.find(key) != m_animations.end())
	{
		return m_animations[key].first;
	}
	else
	{
		return m_animations["placeholder"].first;
	}
}

void ResourceManager::loadAnimations(string key, string filePath)
{
	FILE* _file = new FILE();
	fopen_s(&_file, filePath.c_str(), "rb");
	char readBuffer[65536];
	FileReadStream _is(_file, readBuffer, sizeof(readBuffer));
	Document _document;
	_document.ParseStream(_is);
	fclose(_file);

	string keyName = key + "_json";
	m_path[keyName] = filePath;

	vector<Rect>  _animationList;
	for (const auto& frame: _document["frames"].GetArray())
	{
		Rect _tempRect = Rect();
		_tempRect.pos.x = frame["frame"]["x"].GetDouble();
		_tempRect.pos.y = frame["frame"]["y"].GetDouble();
		_tempRect.size.w = frame["frame"]["w"].GetDouble();
		_tempRect.size.h = frame["frame"]["h"].GetDouble();

		_animationList.push_back(_tempRect);
	}
	m_animations[key].first = _animationList;
	m_animations[key].second = getTimeInfo(filePath.c_str());

	FileMonitor::getInstance().WatchFile(filePath, std::bind(&ResourceManager::reloadFromJSON, ResourceManager::getInstance(), key, (key + "_json")));
}

ResourceManager::ResourceManager(){}

ResourceManager::~ResourceManager()
{
	// Destroy the resources
	for (vector<Resource*>::iterator _it = m_resourceQueue.begin(); _it != m_resourceQueue.end(); ++_it)
	{
		Resource* _r = *_it;
		delete _r;
		(*_it) = nullptr;
	}

	// Destroy the textures
	for (map<string, pair<SDL_Texture*, tm>>::iterator _it = m_textures.begin(); _it != m_textures.end(); ++_it)
	{
		SDL_Texture* _t = (*_it).second.first;
		SDL_DestroyTexture(_t);
		(*_it).second.first = NULL;
	}

	// Destroy the music files
	for (map<string, Mix_Music*>::iterator _it = m_music.begin(); _it != m_music.end(); ++_it)
	{
		Mix_Music* _m = (*_it).second;
		Mix_FreeMusic(_m);
		(*_it).second = NULL;
	}

	// Destroy the sound effects
	for (map<string, Mix_Chunk*>::iterator _it = m_soundEffects.begin(); _it != m_soundEffects.end(); ++_it)
	{
		Mix_Chunk* _s = (*_it).second;
		Mix_FreeChunk(_s);
		(*_it).second = NULL;
	}

	m_renderer = nullptr;

	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
}