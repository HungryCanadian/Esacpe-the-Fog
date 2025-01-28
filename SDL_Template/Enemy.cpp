#include "Enemy.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

Player * Enemy::sPlayer = nullptr;
Formation* Enemy::sFormation = nullptr;


void Enemy::SetFormation(Formation* formation) {
	sFormation = formation;
}

Vector2 Enemy::WorldFormationPosition() {
	return sFormation->Position() + LocalFormationPosition();
}

void Enemy::FlyInComplete() {
	if (mChallengeStage) {
		mCurrentState = Dead;
	}
	else {
		JoinFormation();
	}
}

void Enemy::JoinFormation() {
	Position(WorldFormationPosition());
	Parent(sFormation);
	mCurrentState = InFormation;
}

void Enemy::PathComplete() {
	if (mChallengeStage) {
		mCurrentState = Dead;
	}
}

Enemy::Types Enemy::Type() {
	return mType;
}

void Enemy::Hit(PhysEntity* other) {
	if (mCurrentState == InFormation) {
		Parent(nullptr);
	}

	mCurrentState = Dead;
}

int Enemy::Index() {
	return mIndex;
}

void Enemy::HandleInFormationState() {
	Position(LocalFormationPosition());

	float rotation = Rotation();
	if (rotation != 0.0f) {
		if (rotation > 5.0f) {
			float rotationSpeed = 250.0f;
			float rotationDir = (rotation >= 180.0f) ? 1.0f : -1.0f;
			Rotate(rotationDir * mTimer->DeltaTime() * rotationSpeed);
		}
		else {
			Rotation(0.0f);
		}
	}
}


void Enemy::HandleStates() {
	switch (mCurrentState) {
	case InFormation:
		HandleInFormationState();
		break;
	case Diving:
		HandleDiveState();
		break;
	case Dead:
		HandleDeadState();
		break;

	}
}

void Enemy::RenderInFormationState() {
	mTexture->Render();
}

void Enemy::RenderDeadState() {
	if (mDeathAnimation->IsAnimating()) {
		mDeathAnimation->Render();
	}
}

void Enemy::HandleDeadState() {
	if (mDeathAnimation->IsAnimating()) {
		mDeathAnimation->Update();
	}
}

void Enemy::CurrentPlayer(Player* player) {
	sPlayer = player;
}

void Enemy::RenderStates() {
	switch (mCurrentState) {
	case InFormation:
		RenderInFormationState();
		break;
	case Diving:
		RenderDiveState();
		break;
	case Dead:
		RenderDeadState();
		break;

	}
}

// Random float function to generate positions within a range
float Enemy::RandomFloat(float min, float max) {
	return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

// Method to randomly spawn the enemy within a given boundary
void Enemy::RandomlySpawn(Vector2 minBoundary, Vector2 maxBoundary) {
	// Generate random position within the boundaries
	float randomX = RandomFloat(minBoundary.x, maxBoundary.x);
	float randomY = RandomFloat(minBoundary.y, maxBoundary.y);
	Position(Vector2(randomX, randomY));  // Set the enemy's position
}


Enemy::Enemy(int index, bool challenge) : mIndex(index), mChallengeStage(challenge) {
	mTimer = Timer::Instance();
	mTexture = nullptr;
	mSpeed = 450.0f;

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Hostile);

	mDeathAnimation = new AnimatedTexture("EnemyExplosion.png", 0, 0, 128, 128, 5, 1.0f, Animation::Layouts::Horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Position(Vec2_Zero);
	mDeathAnimation->SetWrapMode(Animation::WrapModes::Once);

	Vector2 minBoundary(-100.0f, 300.0f);  // Example minimum boundary
	Vector2 maxBoundary(800.0f, 600.0f);   // Example maximum boundary
	RandomlySpawn(minBoundary, maxBoundary);
}

Enemy::~Enemy() {
	mTimer = nullptr;

	delete mTexture;
	mTexture = nullptr;


	delete mDeathAnimation;
	mDeathAnimation = nullptr;
}

Enemy::States Enemy::CurrentState() {
	return mCurrentState;
}

bool Enemy::InDeathAnimation() {
	return mDeathAnimation->IsAnimating();
}

//void Enemy::Dive(int type) {
//	Parent(nullptr);
//	mCurrentState = Diving;
//	mDiveStartPosition = Position();
//	mCurrentWaypoint = 1;
//}

void Enemy::Update() {
	if (Active()) {
		HandleStates();
	}
}

void Enemy::Render() {
	if (Active()) {
		RenderStates();
	}
}