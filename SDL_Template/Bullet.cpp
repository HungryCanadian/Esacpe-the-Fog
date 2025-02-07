#include "Bullet.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

bool Bullet::IgnoreCollisions() {
	return !Active();
}

Bullet::Bullet(bool friendly) {
	mTimer = Timer::Instance();

	mTexture = new GLTexture("Bullet.png");
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero);

	mSpeed = 500;

	Reload();

	AddCollider(new BoxCollider(mTexture->ScaledDimensions()));

	if (friendly) {
		mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::FriendlyProjectiles);
	}
	else {
		mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::HostileProjectiles);
	}
}

Bullet::~Bullet() {
	mTimer = nullptr;

	delete mTexture;
	mTexture = nullptr;
}

void Bullet::Fire(Vector2 pos, float rot, Direction direction) {
	Position(pos);
	Active(true);
	Rotation(rot);

	// Set direction based on parameter passed from Player
	SetDirection(direction);

}



void Bullet::Reload() {
	Active(false);
}

void Bullet::Hit(PhysEntity* other) {
	Reload();
}

void Bullet::Update() {
	if (Active()) {
		// Move the bullet based on its direction
		if (mDirection == Direction::Up) {
			Translate(Vec2_Up * mSpeed * mTimer->DeltaTime());
		}
		else if (mDirection == Direction::Down) {
			Translate(-Vec2_Up * mSpeed * mTimer->DeltaTime());
		}

		// Check if the bullet has gone off screen and reload it
		Vector2 pos = Position();
		if (pos.y < TOP_BUFFER || pos.y > Graphics::SCREEN_HEIGHT - BOTTOM_BUFFER) {
			Reload();
		}
		if (pos.x < LEFT_BUFFER || pos.x > Graphics::SCREEN_WIDTH - RIGHT_BUFFER) {
			Reload();
		}
	}
}

void Bullet::Render() {
	if (Active()) {
		mTexture->Render();
		PhysEntity::Render();
	}
}