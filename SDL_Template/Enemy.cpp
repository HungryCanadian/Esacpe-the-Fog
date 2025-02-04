#include "Enemy.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

std::vector<std::vector<Vector2>> Enemy::sPaths;
Player * Enemy::sPlayer = nullptr;
Formation* Enemy::sFormation = nullptr;

void Enemy::CreatePaths() {
	int screenMidPoint = (int)(Graphics::Instance()->SCREEN_WIDTH * 0.4f);

	int currentPath = 0;
	BezierPath* path = new BezierPath();
	path->AddCurve({
		Vector2(screenMidPoint + 50.0f, -10.0f),
		Vector2(screenMidPoint + 50.0f, -20.0f),
		Vector2(screenMidPoint + 50.0f, 30.0f),
		Vector2(screenMidPoint + 50.0f, 20.0f) }, 1);
	path->AddCurve({
		Vector2(screenMidPoint + 50.0f, 20.0f),
		Vector2(screenMidPoint + 50.0f, 100.0f),
		Vector2(75.0f, 325.0f),
		Vector2(75.0f, 425.0f) }, 25);
	path->AddCurve({
		Vector2(75.0f, 425.0f),
		Vector2(75.0f, 650.0f),
		Vector2(350.0f, 650.0f),
		Vector2(350.0f, 425.0f) }, 25);

	sPaths.push_back(std::vector<Vector2>());
	path->Sample(&sPaths[currentPath]);
	delete path;

	currentPath = 1;
	path = new BezierPath();
	int fullScreen = screenMidPoint * 2;
	path->AddCurve({
		Vector2(screenMidPoint - 50.0f, -10.0f),
		Vector2(screenMidPoint - 50.0f, -20.0f),
		Vector2(screenMidPoint - 50.0f, 30.0f),
		Vector2(screenMidPoint - 50.0f, 20.0f) }, 1);
	path->AddCurve({
		Vector2(screenMidPoint - 50.0f, 20.0f),
		Vector2(screenMidPoint - 50.0f, 100.0f),
		Vector2(fullScreen - 75.0f, 325.0f),
		Vector2(fullScreen - 75.0f, 425.0f) }, 25);
	path->AddCurve({
		Vector2(fullScreen - 75.0f, 425.0f),
		Vector2(fullScreen - 75.0f, 650.0f),
		Vector2(fullScreen - 350.0f, 650.0f),
		Vector2(fullScreen - 350.0f, 425.0f) }, 25);

	sPaths.push_back(std::vector<Vector2>());
	path->Sample(&sPaths[currentPath]);
	delete path;

	currentPath = 2;
	float temp = screenMidPoint - 100.0f;

	path = new BezierPath();
	path->AddCurve({
		Vector2(-40.0f, 720.0f),
		Vector2(-50.0f, 720.0f),
		Vector2(10.0f, 720.0f),
		Vector2(0.0f, 720.0f) }, 1);
	path->AddCurve({
		Vector2(0.0f, 720.0f),
		Vector2(200.0f, 720.0f),
		Vector2(temp, 500.0f),
		Vector2(temp, 400.0f) }, 15);
	path->AddCurve({
		Vector2(temp, 400.0f),
		Vector2(temp, 200.0f),
		Vector2(40.0f, 200.0f),
		Vector2(40.0f, 400.0f) }, 15);
	path->AddCurve({
		Vector2(40.0f, 400.0f),
		Vector2(40.0f, 500.0f),
		Vector2(temp - 120.0f, 600.0f),
		Vector2(temp - 40.0f, 440.0f) }, 15);

	sPaths.push_back(std::vector<Vector2>());
	path->Sample(&sPaths[currentPath]);
	delete path;

	currentPath = 3;
	temp = screenMidPoint + 60.0f;
	float temp2 = fullScreen - 40.0f;

	path = new BezierPath();
	path->AddCurve({
		Vector2(temp2 + 40.0f, 720.0f),
		Vector2(temp2 + 50.0f, 720.0f),
		Vector2(temp2 + 10.0f, 720.0f),
		Vector2(temp2, 720.0f) }, 1);
	path->AddCurve({
		Vector2(temp2, 720.0f),
		Vector2(temp2 - 200.0f, 720.0f),
		Vector2(temp, 500.0f),
		Vector2(temp, 400.0f) }, 15);
	path->AddCurve({
		Vector2(temp, 400.0f),
		Vector2(temp, 200.0f),
		Vector2(temp2 - 40.0f, 200.0f),
		Vector2(temp2 - 40.0f, 400.0f) }, 15);
	path->AddCurve({
		Vector2(temp2 - 40.0f, 400.0f),
		Vector2(temp2 - 40.0f, 500.0f),
		Vector2(temp + 120.0f, 600.0f),
		Vector2(temp + 40.0f, 440.0f) }, 15);

	sPaths.push_back(std::vector<Vector2>());
	path->Sample(&sPaths[currentPath]);
	delete path;
}

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

void Enemy::HandleFlyInState() {
	if (mCurrentWaypoint < sPaths[mCurrentPath].size()) {
		Vector2 dist = sPaths[mCurrentPath][mCurrentWaypoint] - Position();
		Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
		Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f); //This was Rotate instead of Rotation (if beyblade mode still exists this wasnt it)

		if ((sPaths[mCurrentPath][mCurrentWaypoint] - Position()).MagnitudeSqr() < EPSILON * mSpeed / 25.0f) {
			mCurrentWaypoint++;
		}

		if (mCurrentWaypoint >= sPaths[mCurrentPath].size()) {
			//we have reached the end of our flyin path.
			PathComplete();
		}
	}
	else {
		Vector2 dist = WorldFormationPosition() - Position();
		Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
		Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

		if (dist.MagnitudeSqr() < EPSILON * mSpeed / 25.0f) {
			FlyInComplete();
		}
	}
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
	case FlyIn:
		HandleFlyInState();
		break;
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

void Enemy::RenderFlyInState() {
	mTexture->Render();

	for (int i = 0; i < sPaths[mCurrentPath].size() - 1; i++) {
		Graphics::Instance()->DrawLine(
			sPaths[mCurrentPath][i].x,
			sPaths[mCurrentPath][i].y,
			sPaths[mCurrentPath][i + 1].x,
			sPaths[mCurrentPath][i + 1].y
		);
	}
}

void Enemy::RenderInFormationState() {
	mTexture->Render();

	for (int i = 0; i < sPaths[mCurrentPath].size() - 1; i++) {
		Graphics::Instance()->DrawLine(
			sPaths[mCurrentPath][i].x,
			sPaths[mCurrentPath][i].y,
			sPaths[mCurrentPath][i + 1].x,
			sPaths[mCurrentPath][i + 1].y
		);
	}
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
	case FlyIn:
		RenderFlyInState();
		break;
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

Enemy::Enemy(int path, int index, bool challenge) : mCurrentPath(path), mIndex(index), mChallengeStage(challenge) {
	mTimer = Timer::Instance();

	mCurrentState = FlyIn;

	mCurrentWaypoint = 1;
	Position(sPaths[mCurrentPath][0]);

	mTexture = nullptr;

	mSpeed = 450.0f;

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

<<<<<<< Updated upstream
void Enemy::Dive(int type) {
	Parent(nullptr);
	mCurrentState = Diving;
	mDiveStartPosition = Position();
	mCurrentWaypoint = 1;
}

=======
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






>>>>>>> Stashed changes
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