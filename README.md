Forgot to include the code breakdown in walkthrough
I will briefly describe them here

----- text rendering -----
I was trying to do text rendering in the codes as there are lines refering to those logics, in freetype way and SDL_ttf way
However the SDL_ttf way requires refreshing the screen with SDL, which comes with a conflict with OpenGL refresh and create a flashy screen in result

The freetype way is not tested
I think I should start a new project and test it out alone, to see how I can use it
But I was travelling in several cities in UK and Berlin, I have no time for that

So I leave them in my project but disable and unlink everything about text rendering

----- models -----
The project contains several models in format of .obj
7 of them are loaded in the codes
With independant textures mapped

----- controls -----
Number key 1 to 7 controls which models to load
Key B, N controls shading mode
Key I, K controls lighting
Key O switch between orthographic view and perspective view
Key L switch between line rendering and face rendering mode
Key <, > switch between BGMs
Key R to reset current position in the world space

----- fragment shader -----
I was thinking of making some researches about different shading mode (there is a website called shadertoy, mindblowing and don't know how they did that)
but it is far too complicated, way more than I thought
I gave up and only implement the clay shading and normals shading into my project

----- programming patterns -----
I have no idea what was I doing
At least I put them in different #pragma region so that they can be fold for a bit clearer reviewing experiences
