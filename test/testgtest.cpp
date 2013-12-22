#include "gtest/gtest.h"
#include "evil.h"
#include "application.h"
#include "sprite_batch.h"

using namespace evil;

class ApplicationTest : public Application
{
public:
  ApplicationTest(uint32_t w, uint32_t h ) : Application(w,h) { }
  bool init() { initSDL(); initGL(); return true; }
  void render() { }
  void update(const float dt) { }
};

class SpriteBatchFixture : public ::testing::Test
{
public:
  ApplicationTest application;
  SpriteBatchFixture() : application(800,600) { }
  ~SpriteBatchFixture()  { }
  void SetUp() {
    application.init();
  }

  void TearDown() {
    application.quit();
  }
};


TEST( Application, construction )
{
    ApplicationTest test(5,5);
    EXPECT_EQ(5, test.getWidth());
    EXPECT_EQ(5, test.getHeight());
}

TEST(SpriteSheet, spriteSheet )
{
    SpriteSheet sheet;

    ASSERT_FALSE( sheet.load("this_file_doesn't_exist.json"));

    ASSERT_TRUE( sheet.load("../test/fixtures/bats.json") );

    ASSERT_EQ(5, sheet.getNumFrames());

    auto frame = sheet.getFrame("bats_fly1.png");

    EXPECT_EQ( 0, frame->sourceSize.x );
    EXPECT_EQ( 0, frame->sourceSize.y );
    EXPECT_EQ( 181, frame->sourceSize.w );
    EXPECT_EQ( 100, frame->sourceSize.h );

    EXPECT_TRUE( frame->trimmed );

    EXPECT_EQ( 0, frame->textureRect.x );
    EXPECT_EQ( 0, frame->textureRect.y );
    EXPECT_EQ( 181/512.0f, frame->textureRect.w );
    EXPECT_EQ( 100/512.0f, frame->textureRect.h );

    EXPECT_EQ( 0, frame->colorRect.x );
    EXPECT_EQ( 0, frame->colorRect.y );
    EXPECT_EQ( 181, frame->colorRect.w );
    EXPECT_EQ( 100, frame->colorRect.h );

    EXPECT_EQ( 0, frame->size.x );
    EXPECT_EQ( 0, frame->size.y );
    EXPECT_EQ( 181, frame->size.w );
    EXPECT_EQ( 100, frame->size.h );

    EXPECT_FALSE( frame->rotated );

    frame = sheet.getFrame("fire.png");

    EXPECT_EQ( 0, frame->sourceSize.x );
    EXPECT_EQ( 0, frame->sourceSize.y );
    EXPECT_EQ( 32, frame->sourceSize.w );
    EXPECT_EQ( 32, frame->sourceSize.h );

    EXPECT_TRUE( frame->trimmed );

    EXPECT_EQ( 183/512.0f, frame->textureRect.x );
    EXPECT_EQ( 0, frame->textureRect.y );
    EXPECT_EQ( 30/512.0f, frame->textureRect.w );
    EXPECT_EQ( 30/512.0f, frame->textureRect.h );

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
