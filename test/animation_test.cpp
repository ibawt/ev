#include "gtest/gtest.h"
#include "evil.h"
#include "application.h"
#include "animation.h"
#include "sprite_batch.h"

using namespace evil;
class AnimationTest : public ::testing::Test
{
public:
		Animation animation;

		void SetUp() {
				auto f1 = make_shared<SpriteFrame>();
				auto f2 = make_shared<SpriteFrame>();
				auto f3 = make_shared<SpriteFrame>();

				animation.addFrame(f1);
				animation.addFrame(f2);
				animation.addFrame(f3);
		}

		void TearDown() {
		}
};

TEST_F(AnimationTest, initialState)
{
		EXPECT_EQ(Animation::LOOP, animation.getMode());
		EXPECT_EQ(0, animation.getFrameIndex());
		EXPECT_EQ(0.0f, animation.getDelay());
}

TEST_F(AnimationTest, getNumFrames)
{
		EXPECT_EQ(3, animation.getNumFrames());
		EXPECT_EQ(0, animation.getFrameIndex());
		animation.setFrameIndex(4000);
		EXPECT_EQ( 2, animation.getFrameIndex());
}

TEST_F(AnimationTest, loopMode)
{
		EXPECT_EQ( Animation::LOOP, animation.getMode());

		animation.setDelay(1.0f);

		animation.update(1.0f);

		EXPECT_EQ( 1, animation.getFrameIndex());

		animation.update(0.5f);
		EXPECT_EQ( 1, animation.getFrameIndex());

		animation.update(1.0f);
		EXPECT_EQ( 2, animation.getFrameIndex());

		animation.update(.5f);
		EXPECT_EQ(0, animation.getFrameIndex());
}

TEST_F(AnimationTest, oneShot)
{
		animation.setMode(Animation::ONE_SHOT);

		animation.setDelay(1.0f);

		animation.update( 4.0f );

		EXPECT_EQ( 2, animation.getFrameIndex());

		animation.update(10.0f);

		EXPECT_EQ(2, animation.getFrameIndex());
}
