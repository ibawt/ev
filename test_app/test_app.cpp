#include "evil.h"
#include "application.h"
#include "texture.h"
#include "sprite_batch.h"
#include "sprite.h"
#include "utils.h"

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

    auto p = make_unique<Animation>();

    p->addFrame( spriteBatch.getSheet().frames["bats_fly1.png"]);
    p->addFrame( spriteBatch.getSheet().frames["bats_fly2.png"]);
    p->addFrame( spriteBatch.getSheet().frames["bats_fly3.png"]);
    p->setDelay(0.3f);
    sprite->setAnimation(p);
    
    return true;
}

TestApplication::~TestApplication()
{
}

void TestApplication::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    texture->bind();
    glEnable(GL_TEXTURE_2D);
    sprite->render();
    // glBegin(GL_QUADS);
    // glTexCoord2f(0,0);
    // glVertex2f(0,0);
    // glTexCoord2f(1,0);
    // glVertex2f(500,0);
    // glTexCoord2f(1,1);
    // glVertex2f(500,500);
    // glTexCoord2f(0,1);
    // glVertex2f(0,500);
    // glEnd();
    // glDisable(GL_TEXTURE_2D);
    //spriteBatch.render();
}

void TestApplication::update(float dt)
{
    sprite->update(dt);
}

int main(int argc, char **argv)
{
    TestApplication app(800,600);

    return app.main();
}
