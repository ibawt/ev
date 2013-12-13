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

    auto sheet = batch.getSheet();
    
    ASSERT_EQ(5, batch.getSheet().frames.size());

    auto frame = sheet.frames["bats_fly1.png"];

    EXPECT_EQ( 0, frame->sourceSize.x );
    EXPECT_EQ( 0, frame->sourceSize.y );
    EXPECT_EQ( 181, frame->sourceSize.w );
    EXPECT_EQ( 100, frame->sourceSize.h );

    EXPECT_TRUE( frame->trimmed );
    
    EXPECT_EQ( 0, frame->textureRect.x );
    EXPECT_EQ( 0, frame->textureRect.y );
    EXPECT_EQ( 181, frame->textureRect.w );
    EXPECT_EQ( 100, frame->textureRect.h );

    EXPECT_EQ( 0, frame->colorRect.x );
    EXPECT_EQ( 0, frame->colorRect.y );
    EXPECT_EQ( 181, frame->colorRect.w );
    EXPECT_EQ( 100, frame->colorRect.h );

    EXPECT_EQ( 0, frame->size.x );
    EXPECT_EQ( 0, frame->size.y );
    EXPECT_EQ( 181, frame->size.w );
    EXPECT_EQ( 100, frame->size.h );

    EXPECT_FALSE( frame->rotated );

    frame = sheet.frames["fire.png"];
    
    EXPECT_EQ( 0, frame->sourceSize.x );
    EXPECT_EQ( 0, frame->sourceSize.y );
    EXPECT_EQ( 32, frame->sourceSize.w );
    EXPECT_EQ( 32, frame->sourceSize.h );

    EXPECT_TRUE( frame->trimmed );
    
    EXPECT_EQ( 183, frame->textureRect.x );
    EXPECT_EQ( 0, frame->textureRect.y );
    EXPECT_EQ( 30, frame->textureRect.w );
    EXPECT_EQ( 30, frame->textureRect.h );

    EXPECT_EQ( 1, frame->colorRect.x );
    EXPECT_EQ( 1, frame->colorRect.y );
    EXPECT_EQ( 30, frame->colorRect.w );
    EXPECT_EQ( 30, frame->colorRect.h );

    EXPECT_EQ( 0, frame->size.x );
    EXPECT_EQ( 0, frame->size.y );
    EXPECT_EQ( 30, frame->size.w );
    EXPECT_EQ( 30, frame->size.h );

    EXPECT_FALSE( frame->rotated );
}
        
