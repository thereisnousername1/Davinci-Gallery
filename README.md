Forgot to include the code breakdown in the walkthrough
I will briefly describe them here

----- text rendering -----

I was trying to do text rendering in the codes as lines are referring to those logics, in the FreeType way and SDL_ttf way
However, the SDL_ttf way requires refreshing the screen with SDL, which comes with a conflict with OpenGL refresh and creates a flashy screen in a result

The FreeType way is not tested
I think I should start a new project and test it out alone, to see how I can use it
But I was traveling in several cities in UK and Berlin, I have no time for that

So I leave them in my project but disable and unlink everything about text rendering

----- models -----

The project contains several models in the format of .obj
7 of them are loaded in the codes
With independent textures mapped

----- controls -----

Number key 1 to 7 controls which models to load
Key B, N controls shading mode
Key I, K controls lighting
Key O switches between the orthographic view and the perspective view
Key L switch between line rendering and face rendering mode
Key <, > switch between BGMs
Key R to reset current position in the world space

----- fragment shader -----

I was thinking of doing some research about different shading modes (there is a website called Shadertoy, mindblowing and don't know how they did that)
but it is far too complicated, way more than I thought
I gave up and only implemented clay shading and normals shading into my project

----- programming patterns -----

I have no idea what I was doing
At least I put them in different #pragma region so that they can be folded for a bit clearer reviewing experiences
