#include <SDL.h>
#include <SDL_gamecontroller.h>
#include <SDL_video.h>
#include <SDL_log.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define windowHeight 500
#define windowWidth 700
void exit_with_error(const char *message)
{
    SDL_Log("%s\n", message, SDL_GetError());
    SDL_Quit();
    exit(1);
}
void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius)
{
    for (int angle = 0; angle <= 360; angle++)
    {
        double radians = angle * (M_PI / 180.0);
        int x = centerX + (int)(radius * cos(radians));
        int y = centerY + (int)(radius * sin(radians));

        SDL_RenderDrawPoint(renderer, x, y);
    }
}
void drawHalfCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius)
{
    for (int angle = 180; angle <= 360; angle++) // Draw from 180 to 360 degrees
    {
        double radians = angle * (M_PI / 180.0);
        int x = centerX - (int)(radius * cos(radians));
        int y = centerY + (int)(radius * sin(radians)); // Subtract instead of add

        SDL_RenderDrawPoint(renderer, x, y);
    }
}
void drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius)
{
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}
Uint32* savedPixels = NULL;
void savePixels(SDL_Renderer* renderer, Uint32* pixels, int x_start, int y_start, int x_end, int y_end) {
    int width = x_end - x_start;
    int height = y_end - y_start;
    SDL_Rect rect = {x_start, y_start, width, height};
    SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_ARGB8888, pixels, width * sizeof(Uint32));
}
void restorePixels(SDL_Renderer* renderer, Uint32* pixels, int x_start, int y_start, int x_end, int y_end) {
    // Calculate width and height of the region
    int width = x_end - x_start;
    int height = y_end - y_start;

    // Create a texture with the calculated width and height
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
     
    // Update the texture with the provided pixel data
    SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(Uint32));

    // Render the texture to the specified region
    SDL_Rect dest_rect = {x_start, y_start, width, height};
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);

    // Destroy the texture
    SDL_DestroyTexture(texture);
}
void handleTouchpadGesture(float deltaX, float deltaY) {
    // Set a threshold for gesture detection
    float threshold = 0.5;

    if (deltaX > threshold) {
        printf("Right gesture detected\n");
    } else if (deltaX < -threshold) {
        printf("Left gesture detected\n");
    }

    if (deltaY > threshold) {
        printf("Down gesture detected\n");
    } else if (deltaY < -threshold) {
        printf("Up gesture detected\n");
    }
}
int main(int argc, char **argv)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Rect cu;
    cu.x = 0;
    cu.y = 0;
    cu.w = 960;
    cu.h = 540;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0)
    {
        exit_with_error("SDL initialization failed");
    }
    SDL_bool isStickRightDownClicked = SDL_FALSE;
    SDL_bool isStickLeftUpClicked = SDL_FALSE;

    SDL_Joystick* joystick = NULL;
    int numJoysticks = SDL_NumJoysticks();
        joystick = SDL_JoystickOpen(0); // Open the first joystick
    window = SDL_CreateWindow("emux", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, 0);
    if(window == NULL)
    {
        exit_with_error("WINDOW = NULL");
    }
    SDL_GameController* controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                break;
            } else {
                SDL_Log("Could not open gamecontroller %i: %s\n", i, SDL_GetError());
            }
        }
    }

    if (!controller) 
    {
        exit_with_error("gamepad not found");
    }

    SDL_Event event;
    int quit = 0;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer == NULL)
        exit_with_error("create renderer");
    // Set the draw color to white
    if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
        exit_with_error("error to change color");

    // Clear the entire window with the white color
    if(SDL_RenderClear(renderer) != 0)
        exit_with_error("error clear renderer");

    // Draw the rectangle
    if(SDL_RenderDrawRect(renderer, &cu) != 0)
        exit_with_error("error draw cube");

    if(SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0)
        exit_with_error("error to change color");

    if(SDL_RenderDrawLine(renderer, 100, 60, 30, 420) != 0)
        exit_with_error("error draw line 1");

    if(SDL_RenderDrawLine(renderer, 600, 60, 670, 420) != 0)
        exit_with_error("error draw line 2");

    if(SDL_RenderDrawLine(renderer, 30, 420, 160, 340) != 0)
        exit_with_error("error draw line 3");

    if(SDL_RenderDrawLine(renderer, 160, 340, 540, 340) != 0)
        exit_with_error("error draw line 4");

    if(SDL_RenderDrawLine(renderer, 540, 340, 670, 420) != 0)
        exit_with_error("error draw line 5");

    if(SDL_RenderDrawLine(renderer, 530, 110, 530, 100) != 0)
        exit_with_error("error draw line 6");

    if(SDL_RenderDrawLine(renderer, 530, 100, 537, 92) != 0)
        exit_with_error("error draw line 7");

    if(SDL_RenderDrawLine(renderer, 530, 100, 523, 92) != 0)
        exit_with_error("error draw line 8");

    if(SDL_RenderDrawLine(renderer, 562, 142, 562, 118) != 0)
        exit_with_error("error draw line 9");    

    if(SDL_RenderDrawLine(renderer, 562, 130, 562, 130) != 0)
        exit_with_error("error draw line 10");    

    if(SDL_RenderDrawLine(renderer, 562, 142, 569, 142) != 0)
        exit_with_error("error draw line 11");    

    if(SDL_RenderDrawLine(renderer, 562, 118, 569, 118) != 0)
        exit_with_error("error draw line 12");    

    if(SDL_RenderDrawLine(renderer, 562, 130, 569, 130) != 0)
        exit_with_error("error draw line 13");  

    if(SDL_RenderDrawLine(renderer, 569, 118, 569, 142) != 0)
        exit_with_error("error draw line 14");       

    if(SDL_RenderDrawLine(renderer, 530, 148, 538, 166) != 0)
        exit_with_error("error draw line 15");

    if(SDL_RenderDrawLine(renderer, 530, 148, 522, 166) != 0)
        exit_with_error("error draw line 16");

    if(SDL_RenderDrawLine(renderer, 534, 157, 526, 157) != 0)
        exit_with_error("error draw line 17");

    if(SDL_RenderDrawLine(renderer, 488, 120, 507, 139) != 0)
        exit_with_error("error draw line 18");

    if(SDL_RenderDrawLine(renderer, 507, 120, 488, 139) != 0)
        exit_with_error("error draw line 19");

    if(SDL_RenderDrawLine(renderer, 100, 60, 600, 60) != 0)
        exit_with_error("error draw line 20");

    if(SDL_RenderDrawLine(renderer, 100, 60, 105, 20) != 0)
        exit_with_error("error draw line 21");
//x direction for
    if(SDL_RenderDrawLine(renderer, 105, 20, 205, 20) != 0)
        exit_with_error("error draw line 22");

    if(SDL_RenderDrawLine(renderer, 205, 20, 210, 60) != 0)
        exit_with_error("error draw line 23");
//R
    if(SDL_RenderDrawLine(renderer, 600, 60, 595, 20) != 0)
        exit_with_error("error draw line 24");

    if(SDL_RenderDrawLine(renderer, 595, 20, 495, 20) != 0)
        exit_with_error("error draw line 25");

    if(SDL_RenderDrawLine(renderer, 495, 20, 490, 60) != 0)
        exit_with_error("error draw line 26");
//end R
    if(SDL_RenderDrawLine(renderer, 150, 30, 150, 50) != 0)
        exit_with_error("error draw line 27");

    if(SDL_RenderDrawLine(renderer, 150, 50, 160, 50) != 0)
        exit_with_error("error draw line 29");

    if(SDL_RenderDrawLine(renderer, 165, 40, 170, 30) != 0)
        exit_with_error("error draw line 30");

    if(SDL_RenderDrawLine(renderer, 170, 30, 170, 50) != 0)
        exit_with_error("error draw line 31");

    if(SDL_RenderDrawLine(renderer, 150, 50, 160, 50) != 0)
        exit_with_error("error draw line 32");

    if(SDL_RenderDrawLine(renderer, 530, 30, 530, 50) != 0)
        exit_with_error("error draw line 33");

    if(SDL_RenderDrawLine(renderer, 530, 30, 540, 30) != 0)
        exit_with_error("error draw line 34");

    if(SDL_RenderDrawLine(renderer, 540, 30, 540, 40) != 0)
        exit_with_error("error draw line 35");

    if(SDL_RenderDrawLine(renderer, 540, 40, 530, 40) != 0)
        exit_with_error("error draw line 36");

    if(SDL_RenderDrawLine(renderer, 530, 40, 540, 50) != 0)
        exit_with_error("error draw line 37");

    if(SDL_RenderDrawLine(renderer, 550, 40, 555, 30) != 0)
        exit_with_error("error draw line 38");

    if(SDL_RenderDrawLine(renderer, 555, 30, 555, 50) != 0)
        exit_with_error("error draw line 39");
//l2
    if(SDL_RenderDrawLine(renderer, 205, 20, 305, 20) != 0)
        exit_with_error("error draw line 40");

    if(SDL_RenderDrawLine(renderer, 305, 20, 315, 60) != 0)
        exit_with_error("error draw line 41");

    if(SDL_RenderDrawLine(renderer, 240, 30, 240, 50) != 0)
        exit_with_error("error draw line 42");

    if(SDL_RenderDrawLine(renderer, 240, 50, 255, 50) != 0)
        exit_with_error("error draw line 43");

    drawHalfCircle(renderer, 275, 40, 10);

    if(SDL_RenderDrawLine(renderer, 285, 40, 265, 50) != 0)
        exit_with_error("error draw line 44");

    if(SDL_RenderDrawLine(renderer, 265, 50, 285, 50) != 0)
        exit_with_error("error draw line 45");
//r2

    if(SDL_RenderDrawLine(renderer, 495, 20, 395, 20) != 0)
        exit_with_error("error draw line 46");

    if(SDL_RenderDrawLine(renderer, 395, 20, 385, 60) != 0)
        exit_with_error("error draw line 47");

    if(SDL_RenderDrawLine(renderer, 420, 30, 420, 50) != 0)
        exit_with_error("error draw line 48");

    if(SDL_RenderDrawLine(renderer, 420, 30, 430, 30) != 0)
        exit_with_error("error draw line 49");

    if(SDL_RenderDrawLine(renderer, 430, 30, 430, 40) != 0)
        exit_with_error("error draw line 50");

    if(SDL_RenderDrawLine(renderer, 430, 40, 420, 40) != 0)
        exit_with_error("error draw line 51");

    if(SDL_RenderDrawLine(renderer, 420, 40, 430, 50) != 0)
        exit_with_error("error draw line 52");
    drawHalfCircle(renderer, 450, 40, 10);

    if(SDL_RenderDrawLine(renderer, 460, 40, 440, 50) != 0)
        exit_with_error("error draw line 53");

    if(SDL_RenderDrawLine(renderer, 440, 50, 460, 50) != 0)
        exit_with_error("error draw line 54");
//end r2
// draw touchpad
    drawCircle(renderer, 210, 280, 50);
    if(SDL_RenderDrawLine(renderer, 200, 240, 200, 320) != 0)
        exit_with_error("error draw line 55");
    if(SDL_RenderDrawLine(renderer, 220, 240, 220, 320) != 0)
        exit_with_error("error draw line 56");

    if(SDL_RenderDrawLine(renderer, 200, 240, 220, 240) != 0)
        exit_with_error("error draw line 57");

    if(SDL_RenderDrawLine(renderer, 200, 320, 220, 320) != 0)
        exit_with_error("error draw line 58");

    if(SDL_RenderDrawLine(renderer, 170, 270, 250, 270) != 0)
        exit_with_error("error draw line 59");

    if(SDL_RenderDrawLine(renderer, 170, 290, 250, 290) != 0)
        exit_with_error("error draw line 60");

    if(SDL_RenderDrawLine(renderer, 170, 270, 170, 290) != 0)
        exit_with_error("error draw line 61");

    if(SDL_RenderDrawLine(renderer, 250, 270, 250, 290) != 0)
        exit_with_error("error draw line 62");
//end draw touchpad
//draw stick right-down
    drawCircle(renderer, 450, 280, 40);
    drawCircle(renderer, 450, 280, 30);
//end draw stick right-down
//draw stick left-up
    drawCircle(renderer, 180, 120, 40);
    drawCircle(renderer, 180, 120, 30);
//end draw stick left-up    
//draw menu button 
    drawCircle(renderer, 410, 120, 10);
    if(SDL_RenderDrawLine(renderer, 404, 116, 416, 116) != 0)
        exit_with_error("error draw line 63");

    if(SDL_RenderDrawLine(renderer, 404, 120, 416, 120) != 0)
        exit_with_error("error draw line 64");

    if(SDL_RenderDrawLine(renderer, 404, 124, 416, 124) != 0)
        exit_with_error("error draw line 65");
//end draw menu button
//draw change view
    drawCircle(renderer, 280, 120, 10);

    if(SDL_RenderDrawLine(renderer, 274, 118, 278, 118) != 0)
        exit_with_error("error draw line 66");

    if(SDL_RenderDrawLine(renderer, 278, 118, 278, 120) != 0)
        exit_with_error("error draw line 67");

    if(SDL_RenderDrawLine(renderer, 274, 118, 274, 123) != 0)
        exit_with_error("error draw line 68");

    if(SDL_RenderDrawLine(renderer, 274, 123, 276, 123) != 0)
        exit_with_error("error draw line 69");

    if(SDL_RenderDrawLine(renderer, 279, 123, 283, 123) != 0)
        exit_with_error("error draw line 70");

    if(SDL_RenderDrawLine(renderer, 283, 123, 283, 127) != 0)
        exit_with_error("error draw line 71");

    if(SDL_RenderDrawLine(renderer, 279, 123, 279, 127) != 0)
        exit_with_error("error draw line 72");

    if(SDL_RenderDrawLine(renderer, 279, 127, 283, 127) != 0)
        exit_with_error("error draw line 73");
//end draw change view
    //a
    drawCircle(renderer, 530, 160, 15);
    //b
    drawCircle(renderer, 562, 130, 15);
    //x
    drawCircle(renderer, 498, 130, 15);
    //y
    drawCircle(renderer, 530, 100, 15);
    //test--------------------------------------------------------------------
    printf("Number of joysticks available: %d\n", SDL_NumJoysticks());
    for (int i = 0; i < SDL_NumJoysticks(); ++i) 
        printf("Joystick %d: %s\n", i, SDL_JoystickNameForIndex(i));

    while(1) 
    {
        SDL_GameControllerUpdate();

        float x, y, pressure;
        Uint8 state;
        int result = SDL_GameControllerGetTouchpadFinger(controller, 0, 0, &state, &x, &y, &pressure);
        if (result != 0) 
        {
            printf("Error getting touchpad finger information: %s\n", SDL_GetError());
            break;
        }

        if (state == SDL_RELEASED) 
            handleTouchpadGesture(0, 0);
        else 
        {
            // Finger moved, calculate gesture
            static float lastX = 0, lastY = 0;
            float deltaX = x - lastX;
            float deltaY = y - lastY;
            handleTouchpadGesture(deltaX, deltaY);
            lastX = x;
            lastY = y;
        }
    }   
    //test--------------------------------------------------------------------
    printf("Controller opened successfully: %s\n", SDL_GameControllerName(controller));
    SDL_RenderPresent(renderer);
    SDL_bool emux_launch=SDL_TRUE;
    Uint32* pixels = (Uint32*)malloc(windowWidth * windowHeight * sizeof(Uint32));
    if (pixels == NULL) 
        exit_with_error("Failed to allocate memory");
    Uint32* pixels_R1 = (Uint32*)malloc(windowWidth * windowHeight * sizeof(Uint32));
    if (pixels == NULL) 
        exit_with_error("Failed to allocate memory");
    Uint32* pixels_L1 = (Uint32*)malloc(windowWidth * windowHeight * sizeof(Uint32));
    if (pixels == NULL) 
        exit_with_error("Failed to allocate memory");
    Uint32* pixels_A = (Uint32*)malloc(windowWidth * windowHeight * sizeof(Uint32));
    if (pixels == NULL) 
        exit_with_error("Failed to allocate memory");
    Uint32* pixels_B = (Uint32*)malloc(windowWidth * windowHeight * sizeof(Uint32));
    if (pixels == NULL) 
        exit_with_error("Failed to allocate memory");
    Uint32* pixels_X = (Uint32*)malloc(windowWidth * windowHeight * sizeof(Uint32));
    if (pixels == NULL) 
        exit_with_error("Failed to allocate memory");
    Uint32* pixels_Y = (Uint32*)malloc(windowWidth * windowHeight * sizeof(Uint32));
    if (pixels == NULL) 
        exit_with_error("Failed to allocate memory");
    Uint32* pixels_BACK = (Uint32*)malloc(windowWidth * windowHeight * sizeof(Uint32));
    if (pixels == NULL) 
        exit_with_error("Failed to allocate memory");
    Uint32* pixels_START = (Uint32*)malloc(windowWidth * windowHeight * sizeof(Uint32));
    if (pixels == NULL) 
        exit_with_error("Failed to allocate memory");
    Uint32* pixels_L3 = (Uint32*)malloc(windowWidth * windowHeight * sizeof(Uint32));
    if (pixels == NULL) 
        exit_with_error("Failed to allocate memory");
    Uint32* pixels_R3 = (Uint32*)malloc(windowWidth * windowHeight * sizeof(Uint32));
    if (pixels == NULL) 
        exit_with_error("Failed to allocate memory");
    SDL_RenderPresent(renderer);
    int running = 1;
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event) != 0) 
        {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    printf("X %d / Y%d\n", event.button.x, event.button.y);
                    break;
            }
            if (event.type == SDL_QUIT) {
                running = 0;
            }                
            else if (event.type == SDL_CONTROLLERBUTTONDOWN) 
            {
                switch (event.cbutton.button) 
                {
                    printf("Button pressed: %d\n", event.cbutton.button);
                
                    case SDL_CONTROLLER_BUTTON_A:
                        printf("Button A pressed\n");
                        savePixels(renderer, pixels_A, 515, 145, 545, 175);
                        drawFilledCircle(renderer, 530, 160, 10);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDL_CONTROLLER_BUTTON_B:
                        printf("Button B pressed\n");
                        savePixels(renderer, pixels_B, 547, 115, 577, 145);
                        drawFilledCircle(renderer, 562, 130, 10);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDL_CONTROLLER_BUTTON_X:
                        printf("Button X pressed\n");
                        savePixels(renderer, pixels_X, 483, 115, 513, 145);
                        drawFilledCircle(renderer, 498, 130, 10);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDL_CONTROLLER_BUTTON_Y:
                        printf("Button Y pressed\n");
                        savePixels(renderer, pixels_Y, 515, 85, 545, 115);
                        drawFilledCircle(renderer, 530, 100, 10);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_UP:
                        printf("DPad-Up Pressed\n");
                        savePixels(renderer, pixels, 204, 245, 216, 256);
                        drawFilledCircle(renderer, 210, 250, 5);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        savePixels(renderer, pixels, 204, 304, 216, 316);
                        drawFilledCircle(renderer, 210, 310, 5);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                        printf("DPad Left Pressed\n");
                        savePixels(renderer, pixels, 174, 274, 186, 286);
                        drawFilledCircle(renderer, 180, 280, 5);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                        printf("DPad Right Pressed\n");
                        savePixels(renderer, pixels, 234, 274, 246, 286);
                        drawFilledCircle(renderer, 240, 280, 5);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDL_CONTROLLER_BUTTON_BACK:
                        printf("button select(change view) are pressed\n");
                        savePixels(renderer, pixels_BACK, 274, 114, 286, 126);
                        drawFilledCircle(renderer, 280, 120, 5);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDL_CONTROLLER_BUTTON_START:
                        printf("button start(menu) are pressed\n");
                        savePixels(renderer, pixels_START, 404, 114, 416, 126);
                        drawFilledCircle(renderer, 410, 120, 5);
                        SDL_RenderPresent(renderer);
                        break;                    
                    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                        printf("button L1 are pressed\n");
                        savePixels(renderer, pixels_L1, 119, 29, 141, 51);
                        drawFilledCircle(renderer, 130, 40, 10);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDL_CONTROLLER_BUTTON_LEFTSTICK:
                        printf("button L3(button joystick left) are pressed\n");
                        savePixels(renderer, pixels_L3, 171, 111, 189, 129);
                        drawFilledCircle(renderer, 180, 120, 8);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                        printf("button R1 are pressed\n");
                        savePixels(renderer, pixels_R1, 564, 29, 586, 51);
                        drawFilledCircle(renderer, 575, 40, 10);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
                        printf("button R3(button joystick right) are pressed\n");
                        savePixels(renderer, pixels_R3, 441, 271, 459, 289);
                        drawFilledCircle(renderer, 450, 280, 8);
                        SDL_RenderPresent(renderer);
                        break;
                    // Add cases for other buttons as needed
                    default:
                        break;
                }
            }
            else if (event.type == SDL_CONTROLLERBUTTONUP) 
            {
                if(event.cbutton.button==0)
                {
                    printf("Button A released\n");
                    restorePixels(renderer, pixels_A, 515, 145, 545, 175);
                    SDL_RenderPresent(renderer);
                }
                if(event.cbutton.button==1)
                {
                    printf("Button B released\n");
                    restorePixels(renderer, pixels_B, 547, 115, 577, 145);
                    SDL_RenderPresent(renderer);
                }
                if(event.cbutton.button==2)
                {
                    printf("Button X released\n");
                    restorePixels(renderer, pixels_X, 483, 115, 513, 145);
                    SDL_RenderPresent(renderer);
                }
                if(event.cbutton.button==3)
                {
                    printf("Button Y released\n");
                    restorePixels(renderer, pixels_Y, 515, 85, 545, 115);
                    SDL_RenderPresent(renderer);
                }
                
                if(event.cbutton.button==4)
                {
                    printf("Button select(change view) released\n");
                    restorePixels(renderer, pixels_BACK, 274, 114, 286, 126);
                    SDL_RenderPresent(renderer);
                }
                if(event.cbutton.button==6)
                {
                    printf("Button start(menu) released\n");
                    restorePixels(renderer, pixels_START, 404, 114, 416, 126);
                    SDL_RenderPresent(renderer);
                }
                if(event.cbutton.button==7)
                {
                    printf("Button L3(button joystick left) released\n");
                    restorePixels(renderer, pixels_L3, 171, 111, 189, 129);
                    SDL_RenderPresent(renderer);
                }
                if(event.cbutton.button==8)
                {
                    printf("Button R3(button joystick right) released\n");
                    restorePixels(renderer, pixels_R3, 441, 271, 459, 289);
                    SDL_RenderPresent(renderer);
                }
                if(event.cbutton.button==11)
                {
                    printf("Button dpad-up released\n");
                    restorePixels(renderer, pixels, 204, 245, 216, 256);
                    SDL_RenderPresent(renderer);
                }
                if(event.cbutton.button==12)
                {
                    printf("Button dpad-down released\n");
                    restorePixels(renderer, pixels, 204, 304, 216, 316);
                    SDL_RenderPresent(renderer);                    
                }
                if(event.cbutton.button==13)
                {
                    printf("Button dpad-left released\n");
                    restorePixels(renderer, pixels, 174, 274, 186, 286);
                    SDL_RenderPresent(renderer);                         
                }
                if(event.cbutton.button==14)
                {
                    printf("Button dpad-right released\n");
                    restorePixels(renderer, pixels, 234, 274, 246, 286);
                    SDL_RenderPresent(renderer);                    
                }
                if(event.cbutton.button==9)
                {
                    printf("Button L1 released\n");
                    restorePixels(renderer, pixels_L1, 119, 29, 141, 51);
                    SDL_RenderPresent(renderer);
                }
                if(event.cbutton.button==10)
                {
                    printf("Button R1 released\n");
                    restorePixels(renderer, pixels_R1, 564, 29, 586, 51);
                    SDL_RenderPresent(renderer);                    
                }
                printf("Button released: %d\n", event.cbutton.button);
            }
        }
    }
// Clean up resources
    SDL_JoystickClose(joystick);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(pixels);
    return 0;
}

//gcc src/main.c -o bin/emux.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2