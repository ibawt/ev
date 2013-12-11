#include "gtest/gtest.h"
#include "evil.h"
#include "application.h"
#include "sprite_batch.h"

using namespace evil;

class ApplicationTest : public Application
{
public:
    ApplicationTest(uint32_t w, uint32_t h ) : Application(w,h) { }
    bool init() { return true; }
    void render() { }
    void update(const float dt) { }
};

TEST( Application, construction )
{
    ApplicationTest test(5,5);
    EXPECT_EQ(5, test.getWidth());
    EXPECT_EQ(5, test.getHeight());
}


TEST( SpriteBatch, spriteSheet )
{
    SpriteBatch batch;

    ASSERT_FALSE( batch.load("this_file_doesn't_exist.json"));
    
    ASSERT_TRUE( batch.load("../test/fixtures/bats.json") );

}
        
