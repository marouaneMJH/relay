

// #include "ui/start_test.hpp"
// #include <SDL2/SDL.h>
// #include <iostream>

// bool start_test_window()
// {
//     if (SDL_Init(SDL_INIT_VIDEO) != 0)
//     {
//         std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
//         return false;
//     }

//     SDL_Window *window = SDL_CreateWindow(
//         "Benchmark Control",
//         SDL_WINDOWPOS_CENTERED,
//         SDL_WINDOWPOS_CENTERED,
//         600, 200,
//         SDL_WINDOW_SHOWN);

//     if (!window)
//     {
//         std::cerr << "Window creation failed\n";
//         SDL_Quit();
//         return false;
//     }

//     bool start = false;
//     bool running = true;

//     while (running)
//     {
//         SDL_Event e;
//         while (SDL_PollEvent(&e))
//         {
//             if (e.type == SDL_QUIT)
//                 running = false;

//             if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
//                 start = running = false;

//             if (e.type == SDL_MOUSEBUTTONDOWN)
//                 start = running = false;
//         }

//         SDL_Delay(16); // ~60 FPS idle loop
//     }

//     SDL_DestroyWindow(window);
//     SDL_Quit();
//     return start;
// }
