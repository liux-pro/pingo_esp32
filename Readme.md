# pingo_esp32
This project is based on [pingo](https://github.com/fededevi/pingo/tree/47ec85dbd7c33d9e10e88886d4a2e6f79b251193) write by @fededevi.  
Pingo is a low level 2D/3D software graphics library.  
I modify it, try to make it even faster for my project. Some modification may be dirty, so I decided not to make a pull request.
# modification
1. add support for rgb565
2. get frame buffer pointer direct(reference by `extern` key word ), instead all function and return it. 
3. fix typo in function `texture_readF`
4. remove `texture_draw`