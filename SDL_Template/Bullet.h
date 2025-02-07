#ifndef __BULLET_H
#define __BULLET_H
#include "PhysEntity.h"
#include "Timer.h"

using namespace SDLFramework;

enum class Direction {
	Up,
	Down
};

class Bullet : public PhysEntity {
private:
	static const int TOP_BUFFER = 75;
	static const int BOTTOM_BUFFER = 200;
	static const int LEFT_BUFFER = 50;
	static const int RIGHT_BUFFER = 25;
public:

	Bullet(bool friendly);
	~Bullet();

	void Fire(Vector2 pos, float rot, Direction direction);
	void SetDirection(Direction dir) { mDirection = dir; }
	void Reload();

	// Inherited from PhysEntity
	void Hit(PhysEntity* other) override;

	void Update() override;
	void Render() override;
	Direction mDirection;
private:
	Timer* mTimer;

	Texture* mTexture;

	float mSpeed;

	//Inherited from PhysEntity
	bool IgnoreCollisions() override;

};
#endif