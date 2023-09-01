#include "ResourceManager.h"

// Constructor to initialize renderer
ResourceManager::ResourceManager(Context* context) : context(context) {}

// Destructor to clean up resources
ResourceManager::~ResourceManager() {
    for (auto& pair : textureMap) {
        SDL_DestroyTexture(pair.second);
    }
    /*for (auto& pair : audioMap) {
        Mix_FreeChunk(pair.second);
    }
    for (auto& pair : fontMap) {
        TTF_CloseFont(pair.second);
    }*/
}

// Texture-related methods
bool ResourceManager::LoadTexture(const std::string& id, const std::string& filePath) {
    if (GetTexture(id) == nullptr) {
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
        if (tempSurface == nullptr) {
            return false;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(context->renderer, tempSurface);
        SDL_DestroySurface(tempSurface);
        if (texture != nullptr) {
            textureMap[id] = texture;
            return true;
        }
        return false;
    }
    else {
        return true;
    }
}

SDL_Texture* ResourceManager::GetTexture(const std::string& id) {
    if (textureMap.find(id) != textureMap.end()) {
        return textureMap[id];
    }
    return nullptr;
}

void ResourceManager::UnloadTexture(const std::string& id) {
    if (textureMap.find(id) != textureMap.end()) {
        SDL_DestroyTexture(textureMap[id]);
        textureMap.erase(id);
    }
}