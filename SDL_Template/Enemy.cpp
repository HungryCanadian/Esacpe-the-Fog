#include "Enemy.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"
#include "Level.h"


Player* Enemy::sPlayer = nullptr;
Formation* Enemy::sFormation = nullptr;


void Enemy::SetFormation(Formation* formation) {
	sFormation = formation;
}

Vector2 Enemy::WorldFormationPosition() {
	return sFormation->Position();
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
	PhysicsManager::Instance()->UnregisterEntity(mId);
	mCurrentState = Dead;
}

int Enemy::Index() {
	return mIndex;
}

void Enemy::HandleInFormationState() {
	Position();

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

Vector2 Enemy::GenerateRandomPosition(Vector2 minBoundary, Vector2 maxBoundary) {
	float randomX = RandomFloat(minBoundary.x, maxBoundary.x);
	float randomY = RandomFloat(minBoundary.y, maxBoundary.y);
	return Vector2(randomX, randomY);
}



Enemy::Enemy(int index, bool challenge) : mIndex(index), mChallengeStage(challenge) {
	mTimer = Timer::Instance();
	mTexture = nullptr;
	mSpeed = 75.0f;
	mIsMovingToTarget = false;

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Hostile);

	mDeathAnimation = new AnimatedTexture("EnemyExplosion.png", 0, 0, 128, 128, 5, 1.0f, Animation::Layouts::Horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Position(Vec2_Zero);
	mDeathAnimation->SetWrapMode(Animation::WrapModes::Once);


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

void Enemy::MoveToTarget() {
	Vector2 currentPosition = Position();
	Vector2 direction = mTargetPosition - currentPosition;
	float distance = direction.Length();

	// If the direction is very large, print a warning (this is just a safety check)
	if (distance > 10000.0f) {
		std::cout << "Warning: Large distance detected. Something may be wrong with the target or position.\n";
	}

	if (distance > 5.0f) {  // Continue moving until we are close enough
		direction = direction.Normalized();  // Normalize the direction so we move at a constant speed
		Vector2 movement = direction * mSpeed * mTimer->DeltaTime();
		Position(currentPosition + movement);

		// Calculate the angle to the target
		float angleToTarget = atan2(direction.x, -direction.y);  // This gives the angle in radians
		float angleInDegrees = angleToTarget * (180.0f / M_PI); // This converts it to degree's
		// Update the rotation of the enemy to face the target
		Rotation(angleInDegrees);
	}
	else {
		mIsMovingToTarget = false;  // Stop moving once we reach the target

		// Generate a new target position when we reach the current one
		mTargetPosition = GenerateRandomPosition(Vector2(75.0f, 75.0f), Vector2(Graphics::SCREEN_WIDTH - 75.0f, Graphics::SCREEN_HEIGHT - 200.0f));
		mIsMovingToTarget = true;  // Start moving to the new target
	}
}


void Enemy::Update() {
	if (Active()) {
		HandleStates();

		// If the enemy is not moving, pick a new target and start moving
		if (!mIsMovingToTarget) {
			mTargetPosition = GenerateRandomPosition(Vector2(75.0f, 75.0f), Vector2(Graphics::SCREEN_WIDTH - 75.0f, Graphics::SCREEN_HEIGHT - 200.0f)); // Update target
			mIsMovingToTarget = true;
		}

		// Move the enemy towards the target position
		if (mIsMovingToTarget) {
			MoveToTarget();
		}
	}
}

void Enemy::Render() {
	if (Active()) {
		RenderStates();
	}
	PhysEntity::Render();
}