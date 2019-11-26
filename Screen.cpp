//
//  Screen.cpp
//  cpp test
//
//  Created by Patrick Iacob on 6/21/19.
//  Copyright © 2019 Patrick Iacob. All rights reserved.
//
#include <iostream>
#include "Screen.h"
using namespace std;

class Screen{

private:
    SDL_Surface* screenSurface=NULL;
    SDL_Renderer *renderer;
    static const uint8_t SCREEN_WIDTH = 64;
    static const uint8_t SCREEN_HEIGHT = 32;
    SDL_Rect screen_rect;
    int PIXEL_SIZE=10;
    int timer;

public:
    SDL_Window *window=NULL;
    Screen(){
        window = SDL_CreateWindow("Stackunderfl0w 8080", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 224, 256, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        screen_rect.x = 0;
        screen_rect.y = 0;
        screen_rect.w = SCREEN_WIDTH;
        screen_rect.h = SCREEN_HEIGHT;
        //cout<<timer<<endl;
        update();
    }
    ~Screen(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

    }
    void update(){
        //Get window surface
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

        SDL_RenderClear( renderer );

        // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
        SDL_Rect r={timer%640,50,50,50};

        timer++;

        // Set render color to blue ( rect will be rendered in this color )
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );

        // Render rect
        SDL_RenderFillRect( renderer, &r );
        for(int x=0; x<64;x++){
            for(int y=0; y<32;y++){
                r={x*10,y*10,10,10};
                if((x+y)%2==0){
                    SDL_RenderFillRect( renderer, &r );
                }
            }
        }

        // Render the rect to the screen
        SDL_RenderPresent(renderer);
    }
    void render(const unsigned char *disp){
        int width;
        int height;
        SDL_GetWindowSize(window, &width, &height);
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
        for (int y = 0; y < 224; y++) {
            for (int x = 0; x < 32; x++) {
                for (int z = 0; z<8;z++){
                    if((disp[0x2400+32*y+x]>>z)%2==1){
                        //SDL_Rect r={y, 256-(x*8+z), 1, 1};
                        //SDL_RenderFillRect( renderer, &r );
                        int newx = (int) (y * width / 224.0);
                        int newy = (int) ((256-(x*8+z)) * height / 256.0);
                        int pixelwidth = (int) (((y + 1) * width / 224.0) - newx);
                        int pixelheight = (int) ((((256-(x*8+z)) + 1) * height / 256.0) - newy);
                        SDL_Rect r={newx, newy, pixelwidth, pixelheight};
                        SDL_RenderFillRect( renderer, &r );
                    }
                }
            }
        }
        /*for(int x=0; x<64;x++){
            for(int y=0; y<32;y++){
                if(disp[y]){
                    int newx = (int) (x * width / 64);
                    int newy = (int) (y * height / 32);
                    int pixelwidth = (int) (((x + 1) * width / 64) - newx);
                    int pixelheight = (int) (((y + 1) * height / 32) - newy);
                    SDL_Rect r={newx, newy, pixelwidth, pixelheight};
                    SDL_RenderFillRect( renderer, &r );
                    //g.fillRect(newx, newy, pixelwidth, pixelheight);
                }
            }
        }*/
        SDL_RenderPresent(renderer);
    }
};
