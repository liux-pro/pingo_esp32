#include "texture.h"
#include "math.h"

int texture_init( Texture *f, Vec2i size, Pixel *buf )
{
    if(size.x * size.y == 0)
        return 1; // 0 sized rect

    if(buf == 0)
        return 2; // null ptr buffer

    f->frameBuffer = (Pixel *)buf;
    f->size = size;

    return 0;
}

//#if defined(WIN32) || defined(__linux__)
void texture_draw(Texture *f, Vec2i pos, Pixel color)
{
//    f->frameBuffer[pos.x + pos.y * f->size.x] = color;
    *((uint8_t *)&(f->frameBuffer[pos.x + pos.y * f->size.x])) = *((uint8_t *)&color+1);
    *(((uint8_t *)&(f->frameBuffer[pos.x + pos.y * f->size.x]))+1) = *(uint8_t *)&color;
}
//#endif

Pixel texture_read(Texture *f, Vec2i pos)
{
    return f->frameBuffer[pos.x + pos.y * f->size.x];
}

Pixel texture_readF(Texture *f, Vec2f pos)
{
    uint16_t x = (uint16_t)(pos.x * f->size.x) % f->size.x;
    uint16_t y = (uint16_t)(pos.y * f->size.y) % f->size.y;
    uint32_t index = x + y * f->size.x;
    Pixel value = f->frameBuffer[index];
    return value;
}




