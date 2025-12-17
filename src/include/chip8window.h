#include <SDL2/SDL.h>

typedef struct Screen {
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * texture;
} Screen;

Screen * newScreen(const char * title, int height, int width, int textureWidth, int textureHeight);

void closeScreen(Screen * screen);

void update(Screen* screen, void const* buffer, int pitch);


bool takeInput(uint8_t * keys);