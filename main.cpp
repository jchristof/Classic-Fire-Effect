#include <SDL2/SDL.h>

//Screen dimension constants
const int WINDOW_WIDTH = 288;
const int WINDOW_HEIGHT = 288;

//fire texture dimensions
const int FIRE_COLORS_NUM = 36;
const int FIRE_WIDTH = 72;

Uint32 firePaletteBuffer[FIRE_WIDTH * WINDOW_HEIGHT] = {0};
Uint32 fireTexturePixels[FIRE_WIDTH * FIRE_COLORS_NUM] = {0};

const SDL_Color palette[FIRE_COLORS_NUM] = {
        {16,  16,  16},
        {35,  16,  16},
        {49,  23,  16},
        {70,  23,  17},
        {85,  29,  18},
        {100, 36,  18},
        {115, 36,  19},
        {137, 43,  21},
        {152, 51,  22},
        {168, 66,  23},
        {184, 73,  25},
        {191, 73,  25},
        {215, 81,  27},
        {215, 89,  27},
        {215, 89,  27},
        {207, 96,  26},
        {208, 104, 31},
        {200, 112, 30},
        {200, 120, 29},
        {201, 127, 29},
        {201, 135, 34},
        {193, 135, 33},
        {194, 143, 33},
        {194, 151, 39},
        {187, 159, 38},
        {187, 159, 38},
        {187, 167, 44},
        {187, 167, 44},
        {188, 175, 51},
        {180, 175, 51},
        {181, 183, 51},
        {181, 183, 58},
        {205, 207, 111},
        {222, 223, 158},
        {238, 239, 198},
        {255, 255, 255},
};

void setupFireBuffer();
void spreadFire(int targetPixel);
void blowOutFire();

int main() {
    //initialize sdl
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);

    //create rendering surfaces
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Doom Fire");
    SDL_Surface* loadedSurface = SDL_LoadBMP( "doomguy.bmp" );
    SDL_SetWindowIcon(window, loadedSurface);

    SDL_Texture *fireSurface = SDL_CreateTexture(renderer,
                                                 SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, FIRE_WIDTH,FIRE_COLORS_NUM);

    //set the initial fire palette state
    setupFireBuffer();

    bool quit = false;
    int updateDelay = 50;
    while (!quit) {
        SDL_PollEvent(&event);

        switch(event.type){
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN:
                blowOutFire();
                updateDelay = 5;
                break;
        }

        SDL_RenderClear(renderer);

        SDL_UpdateTexture(fireSurface, NULL, fireTexturePixels, FIRE_WIDTH * sizeof(Uint32));
        SDL_RenderCopy(renderer, fireSurface, NULL, NULL);

        SDL_RenderPresent(renderer);
        for (int i = 0; i < ((FIRE_COLORS_NUM - 1) * FIRE_WIDTH); i++)
            spreadFire(i);

        SDL_Delay(updateDelay);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}


void setupFireBuffer() {
    for (int x = 0; x < FIRE_WIDTH; x++) {
        int pixelIndex = x + ((FIRE_COLORS_NUM - 1) * FIRE_WIDTH);
        firePaletteBuffer[pixelIndex] = FIRE_COLORS_NUM - 1;
        SDL_Color color = palette[firePaletteBuffer[pixelIndex]];
        fireTexturePixels[pixelIndex] = color.r << 16 | color.g << 8 | color.b;
    }
}

void blowOutFire(){
    for (int x = 0; x < FIRE_WIDTH; x++) {
        int pixelIndex = x + ((FIRE_COLORS_NUM - 1) * FIRE_WIDTH);
        firePaletteBuffer[pixelIndex] = 0;
        SDL_Color color = palette[firePaletteBuffer[pixelIndex]];
        fireTexturePixels[pixelIndex] = color.r << 16 | color.g << 8 | color.b;
    }
}

void spreadFire(int targetPixel) {
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    //get the color of the pixel directly below
    int color = firePaletteBuffer[targetPixel + FIRE_WIDTH];

    //randomly pick a slightly darker fire color
    color += (-2 * r);
    color = color < 0 ? 0 : color;

    //add a rtl horizontal wind effect
    r = r < .5f ? 0 : 1;
    int horizontalWindEffectOffset = targetPixel + (-3 * r);

    horizontalWindEffectOffset = horizontalWindEffectOffset < 0 ? targetPixel : horizontalWindEffectOffset;

    firePaletteBuffer[horizontalWindEffectOffset] = color;

    SDL_assert(horizontalWindEffectOffset >= 0);

    SDL_Color sdl_color = palette[color];
    fireTexturePixels[targetPixel] = sdl_color.r << 16 | sdl_color.g << 8 | sdl_color.b;
}

