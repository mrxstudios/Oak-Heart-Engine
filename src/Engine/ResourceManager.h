#pragma once

#include <map>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Engine/Context.h"

//#include <SDL_mixer.h> // for audio

class Context;

class ResourceManager {
private:
    Context* context;
    std::map<std::string, SDL_Texture*> textureMap;
    //std::map<std::string, Mix_Chunk*> audioMap; // Assuming you're using SDL_Mixer
    //std::map<std::string, TTF_Font*> fontMap; // Assuming you're using SDL_TTF
    // ... other resource types like shaders

public:
    //ResourceManager() {};
    ResourceManager(Context* context);
    
    ~ResourceManager();

    // Texture-related methods
    bool LoadTexture(const std::string& id, const std::string& filePath);
    SDL_Texture* GetTexture(const std::string& id);
    void UnloadTexture(const std::string& id);
};
    // Audio-related methods
    //bool LoadAudio(const std::string& id, const std::string& filePath);
    //Mix_Chunk* GetAudio(const std::string& id);
    //void UnloadAudio(const std::string& id);

    // Font-related methods
    //bool LoadFont(const std::string& id, const std::string& filePath, int size);
    //TTF_Font* GetFont(const std::string& id);
    //void UnloadFont(const std::string& id);
