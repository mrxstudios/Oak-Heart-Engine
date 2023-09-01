#include "Physics.h"

Physics::Physics(Context* context) : context(context) {};

void Physics::Tick(double deltaTime) {
    for (size_t i = context->TOTAL_PIXELS - 1; i != SIZE_MAX; i--) {
        ParseSand(i);
    }
}

inline bool Physics::ParseSand(size_t index) {
    Pixel& pixel = context->raster->GetPixel(index);
    Pixel swapPixel = Pixel{};
    if (!pixel.Exists()) return false;
    if (pixel.IsDynamic() && pixel.IsAwake()) {
        if (index < context->TOTAL_PIXELS - 1 - context->VIEW_WIDTH) {
            Pixel& pixel_below = context->raster->GetPixel(index + context->VIEW_WIDTH);
            if (!pixel_below.Exists()) {
                SwapPixels(pixel, pixel_below);
            }
            else {
                Pixel& pixel_bottom_left = context->raster->GetPixel(index + context->VIEW_WIDTH - 1);
                Pixel& pixel_bottom_right = context->raster->GetPixel(index + context->VIEW_WIDTH + 1);
                if (pixel_bottom_left.Exists() && pixel_bottom_right.Exists()) {
                    pixel.SetAwake(false);
                }
                else {
                    if (!pixel_bottom_left.Exists() && !pixel_bottom_right.Exists()) {
                        if (std::rand() % 2 == 0) {
                            SwapPixels(pixel, pixel_bottom_left);
                        }
                        else {
                            SwapPixels(pixel, pixel_bottom_right);
                        }
                    }
                    else {
                        if (!pixel_bottom_left.Exists()) {
                            SwapPixels(pixel, pixel_bottom_left);
                        }
                        else if (!pixel_bottom_right.Exists()) {
                            SwapPixels(pixel, pixel_bottom_right);
                        }
                    }
                }
            }
        }
        else {
            pixel.SetAwake(false);
        }
    }
    return false;
};

void Physics::SwapPixels(Pixel& a, Pixel& b) {
    swapPixel = a;
    a = b;
    b = swapPixel;
}