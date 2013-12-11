#include "evil.h"
#include "application.h"
#include "texture.h"
#include "sprite_batch.h"
#include "sprite.h"

#include <memory>

using namespace evil;
using namespace std;

class TestApplication : public Application
{
public:
    TestApplication(uint32_t w, uint32_t h) : Application(w,h) {}
    ~TestApplication();
    bool init();
    void render();
    void update(float dt);
private:
    shared_ptr<Texture> texture;
    SpriteBatch spriteBatch;
    shared_ptr<Sprite> sprite;
};

bool TestApplication::init()
{
    texture = std::make_shared<Texture>();
    texture->load("../test_app/bats.png");
    
    spriteBatch.load("../test_app/bats.json");
    spriteBatch.setTexture(texture);

    sprite = spriteBatch.get("bats_fly1.png");
    sprite->setPosition(100.0f, 100.0f);
    
    return true;
}

TestApplication::~TestApplication()
{
}

void TestApplication::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    spriteBatch.render();
}

void TestApplication::update(float dt)
{
}

int main(int argc, char **argv)
{
    TestApplication app(800,600);

    return app.main();
}
