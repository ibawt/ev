#include "evil.h"
#include "application.h"
#include "texture.h"
#include "sprite_batch.h"
#include "sprite.h"
#include "utils.h"

#include <stdlib.h>
#include <memory>
#include <vector>

using namespace evil;
using namespace std;

class TestApplication : public Application
{
public:
		TestApplication(uint32_t w, uint32_t h) : Application(w,h) {
				srand(time(NULL));
		}
		~TestApplication();
		bool init();
		void render();
		void update(float dt);
private:
		shared_ptr<Texture> texture;
		SpriteBatch spriteBatch;
		shared_ptr<Sprite> sprite;
		vector<shared_ptr<Sprite>> sprites;
};

float get_random(float min, float max) {
		double scaled = (double)rand()/RAND_MAX;

		return (max - min +1)*scaled + min;
}

bool TestApplication::init()
{
		texture = make_shared<Texture>();
		texture->load("/Users/ianquick/evil/evil/test_app/bats.png");

		spriteBatch.load("/Users/ianquick/evil/evil/test_app/bats.json");
		spriteBatch.setTexture(texture);

		for( int i = 0 ; i < 50000; ++i ) {
				auto s = make_shared<Sprite>();
				s->setPosition(get_random(0, getWidth()), get_random(0,getHeight()));
				//       s->setPosition(0, 0);
				auto p = make_unique<Animation>();

				p->addFrame( spriteBatch.getSheet().frames["sonic_boom.png"]);
				p->addFrame( spriteBatch.getSheet().frames["bats_fly2.png"]);
				p->addFrame( spriteBatch.getSheet().frames["bats_fly3.png"]);
				p->setDelay(0.2f);
				s->setAnimation(p);
				sprites.push_back(s);
				spriteBatch.addSprite(s);
		}

		spriteBatch.generateBuffer();

		Matrix4 matrix;
		matrix.setOrtho(0, getWidth(), getHeight(), 1, -1, 1 );
		spriteBatch.setTransform(matrix);

		return true;
}

TestApplication::~TestApplication()
{
}

void TestApplication::render()
{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		spriteBatch.render();
}

void TestApplication::update(float dt)
{
		for(auto& s: sprites) {
				s->update(dt);
		}
}

int main(int argc, char **argv)
{
		TestApplication app(800,600);

		return app.main();
}
