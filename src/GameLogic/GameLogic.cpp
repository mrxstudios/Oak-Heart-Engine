#include "GameLogic.h"

GameLogic::GameLogic(Context* context) : context(context) {};

GameLogic::~GameLogic() {
    // Destroy whatever has been created in here
}

void GameLogic::InitGame() {

    SpawnRectangleOfSand();
    /*for (size_t i = 0; i < sdl_data.PIXEL_LENGTH; i++) {
        unsigned char gradient = ((float)i / (float)sdl_data.PIXEL_LENGTH) * 255.f;
        unsigned char inv_gradient = 255 - gradient;
        unsigned char noise = std::rand() % 100;
        raster.emplace_back(Pixel{ std::rand() % 5 == 0 , ARGB{255,255,noise,noise} });
        //raster.emplace_back(Pixel{ std::rand() % 5 == 0 , BGRA{255,255,255,255} });
    }*/

    /*const int centerX = sdl_data.RASTER_WIDTH / 4;
    const int centerY = sdl_data.RASTER_HEIGHT / 4;
    const int radius = 50; // Adjust this radius as needed

    for (int y = 0; y < sdl_data.RASTER_WIDTH; ++y) {
        for (int x = 0; x < sdl_data.RASTER_HEIGHT; ++x) {
            int distance = static_cast<int>(std::sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY)));

            if (distance <= radius) {
                if (std::rand() % 5 == 0) {
                    coord c = coord{ x,y };
                    raster.GetPixel(c).SetValue(11);
                }
            }
        }
    }*/

    /*for (int y = 0; y < sdl_data.RASTER_HEIGHT; ++y) {
        for (int x = 0; x < sdl_data.RASTER_WIDTH; ++x) {
            if (x % 10 > 5) {
                coord c = coord{ x,y };
                raster.GetPixel(c).SetValue(11);
            }
        }
    }*/
}

void GameLogic::Tick(double deltaTime) {

}

void GameLogic::SpawnRectangleOfSand() {
    const int rectLeft = 200;  // Left boundary of the rectangle
    const int rectRight = 500; // Right boundary of the rectangle
    for (int x = 0; x < context->RASTER_WIDTH; x++) {
        for (int y = 0; y < context->RASTER_HEIGHT; y++) {
            if (x >= rectLeft && x <= rectRight) {
                unsigned char noise = std::rand() % 50;
                if (std::rand() % 5 == 0) {
                    coord c = coord{ x,y };
                    //context->raster->GetPixel(c).SetValue(11);
                    //context->raster->GetPixel(c).SetColor(context->palette->sandColors[rand() % 4]);
                    context->raster->SetPixel(c, PIXEL_EXISTS_UPDATED_DYNAMIC, context->palette->sandColors[rand() % 4]);
                }
            }
        }
    }
}