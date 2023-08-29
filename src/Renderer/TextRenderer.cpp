#include "TextRenderer.h"

TextRenderer::TextRenderer(SDL_Renderer* renderer, ResourceManager* resourceManager)
    : renderer(renderer), resourceManager(resourceManager) {
};

void TextRenderer::LoadFontTexture(const std::string& id, const std::string& filePath, int gridSize) {
    resourceManager->LoadTexture(id, filePath);
    fontTexture = resourceManager->GetTexture(id);
    this->gridSize = gridSize;
};

void TextRenderer::Render(
    const char* text, 
    const Point& location, 
    const SDL_Color color,
    int fontSize, 
    TextAlignment alignment
) {
    this->fontSize = fontSize;
    this->alignment = alignment;

    int length = strlen(text);

    for (int i = 0; i < length; i++) {
        int asciiValue = static_cast<int>(text[i]);

        int x = asciiValue % gridSize;
        int y = asciiValue / gridSize;

        SDL_FRect src = SDL_FRect{
            (float)gridSize * x,
            (float)gridSize * y,
            (float)gridSize,
            (float)gridSize
        };

        SDL_FRect dest = SDL_FRect{
            (float)(fontSize * i) + location.x,
            (float)location.y,
            (float)fontSize,
            (float)fontSize
        };

        if (alignment == TextAlignment::Center) {
            dest.x -= (float)(length / 2) * fontSize;
        }
        if (alignment == TextAlignment::Right) {
            dest.x -= length * fontSize;
        }

        SDL_SetTextureColorMod(fontTexture, color.r, color.g, color.b);
        SDL_RenderTexture(renderer, fontTexture, &src, &dest);
    }
}