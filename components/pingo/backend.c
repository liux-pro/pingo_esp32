#include <esp_heap_caps.h>
#include "backend.h"

#include "render/renderer.h"
#include "render/texture.h"
#include "render/pixel.h"
#include "render/depth.h"




Vec4i rect;
Vec2i totalSize;

PingoDepth * zetaBuffer;
Pixel * frameBuffer;




void init( Renderer * ren, BackEnd * backEnd, Vec4i _rect) {

}

void beforeRender( Renderer * ren, BackEnd * backEnd) {
}



void afterRender( Renderer * ren,  BackEnd * backEnd) {


}

Pixel * getFrameBuffer( Renderer * ren,  BackEnd * backEnd) {
    return frameBuffer;
}

PingoDepth * getZetaBuffer( Renderer * ren,  BackEnd * backEnd) {
    return zetaBuffer;
}


void linuxWindowBackEndInit( LinuxWindowBackEnd * this, Vec2i size) {
    totalSize = size;
    this->backend.init = &init;
    this->backend.beforeRender = &beforeRender;
    this->backend.afterRender = &afterRender;
    this->backend.getFrameBuffer = &getFrameBuffer;
    this->backend.getZetaBuffer = &getZetaBuffer;
    this->backend.drawPixel = 0;

    zetaBuffer = malloc(size.x*size.y*sizeof (PingoDepth));
    frameBuffer = malloc(size.x*size.y*sizeof (Pixel));
    
}
