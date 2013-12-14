#ifndef EV_APPLICATION_H_
#define EV_APPLICATION_H_

#include <stdint.h>

#include "evil.h"

namespace evil {
	
class Application
{
public:
    Application(uint32_t w, uint32_t h);
    virtual ~Application() { }
    virtual bool init() = 0;
    virtual void render() = 0;
    virtual void update(const float) = 0;

    virtual void keyEvent(int keycode) { }
    virtual void mouseEvent(int x, int y, int button) { }

		uint32_t getWidth() const { return width; }
		uint32_t getHeight() const { return height; }
		
		float getFPS() const { return fps; }
    int  main();
    void quit();
private:
		bool initGL();
		bool initSDL();
		float         fps = 0.0f;
    uint32_t      width;
    uint32_t      height;
    SDL_Window   *window;
    SDL_GLContext context;
};
}
#endif
