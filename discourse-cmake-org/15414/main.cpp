#include <iostream>
#include <sstream>

#ifdef _WIN32
    #include <windows.h>
#endif

// better involve glad or some other OpenGL loader
#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#ifdef USING_PACKAGE_MANAGER
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif

int windowWidth = 1280,
    windowHeight = 800;

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("[ERROR] %s\n", SDL_GetError());
        return -1;
    }

    SDL_version builtWith;
    SDL_VERSION(&builtWith);
    std::cout << "Built with "
              << std::to_string(builtWith.major)
              << "." << std::to_string(builtWith.minor)
              << "." << std::to_string(builtWith.patch)
              << std::endl;

    SDL_version linkedWith;
    SDL_GetVersion(&linkedWith);
    std::cout << "Linked with "
              << std::to_string(linkedWith.major)
              << "." << std::to_string(linkedWith.minor)
              << "." << std::to_string(linkedWith.patch)
              << std::endl;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );
    
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(
        SDL_WINDOW_OPENGL
        | SDL_WINDOW_RESIZABLE
        | SDL_WINDOW_ALLOW_HIGHDPI
    );
    SDL_Window *window = SDL_CreateWindow(
        "SDL example",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        window_flags
    );
    SDL_SetWindowMinimumSize(window, 500, 300);
    
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == NULL)
    {
        std::cerr << "[ERROR] Failed to create a GL context: "
                  << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_GL_MakeCurrent(window, gl_context);
    
    // enable VSync
    SDL_GL_SetSwapInterval(1);

    std::cout << "OpenGL renderer: "
              << glGetString(GL_RENDERER)
              << std::endl;

    glViewport(0, 0, windowWidth, windowHeight);

    glClearColor(35/255.0f, 35/255.0f, 35/255.0f, 1.00f);

    bool loop = true;
    while (loop)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                loop = false;
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    windowWidth = event.window.data1;
                    windowHeight = event.window.data2;
                    glViewport(0, 0, windowWidth, windowHeight);
                    break;
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    loop = false;
                    break;
                }
                break;
            }
        }

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
