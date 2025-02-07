#include "Player.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

Player::Player() {
	mTimer = Timer::Instance();
	mInput = InputManager::Instance();
	mAudio = AudioManager::Instance();

	mVisible = true;
	mAnimating = false;
	mWasHit = false;

	mScore = 0;
	mLives = 2;

	mTexture[0] = new GLTexture("Ship.png");
	mTexture[1] = new GLTexture("Ship1.png");
	mTexture[2] = new GLTexture("Ship2.png");
	mTexture[3] = new GLTexture("Ship3.png");

	for (int i = 0; i < 4; i++) {
		mTexture[i]->Scale(Vector2(0.5f, 0.5f));
		mTexture[i]->Parent(this);
		mTexture[i]->Position(Vec2_Zero);
		mTexture[i]->Flip(false, true);


		mMoveBoundsX = Vector2(0.0f + 35 + mTexture[i]->ScaledDimensions().x / 2, Graphics::SCREEN_WIDTH - 35 - mTexture[i]->ScaledDimensions().x / 2);
		mMoveBoundsY = Vector2(0.0f + 50 + mTexture[i]->ScaledDimensions().y / 2, Graphics::SCREEN_HEIGHT - 180 - mTexture[i]->ScaledDimensions().y / 2);
	}
	mMoveSpeed = 200.0f;

	mDeathAnimation = new AnimatedGLTexture("EnemyExplosion.png", 0, 0, 128, 128, 4, 1.0f, Animation::Layouts::Horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Position(Vec2_Zero);
	mDeathAnimation->SetWrapMode(Animation::WrapModes::Once);

	for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i] = new Bullet(true);
	}

	AddCollider(new BoxCollider(Vector2(16.0f, 67.0f)));
	AddCollider(new BoxCollider(Vector2(10.0f, 24.0f)), Vector2(9.0f, 5.0f));
	AddCollider(new BoxCollider(Vector2(10.0f, 24.0f)), Vector2(-9.0f, 5.0f));

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Friendly);
}

Player::~Player() {
	mTimer = nullptr;
	mInput = nullptr;
	mAudio = nullptr;

	for (int i = 0; i < 4; i++) {
		delete mTexture[i];
		mTexture[i] = nullptr;
	}

	delete mDeathAnimation;
	mDeathAnimation = nullptr;

	for (auto b : mBullets) {
		delete b;
	}
}

void Player::HandleMovement() {
	if (mInput->KeyDown(SDL_SCANCODE_D)) {
		Rotation(Rotation(GameEntity::Local) + 180.0f * mTimer->DeltaTime());
	}
	else if (mInput->KeyDown(SDL_SCANCODE_A)) {
		Rotation(Rotation(GameEntity::Local) + -180.0f * mTimer->DeltaTime());
	}

	if (mInput->KeyDown(SDL_SCANCODE_W)) {
		Translate(-Vec2_Up * mMoveSpeed * mTimer->DeltaTime(), Local);
	}

	Vector2 pos = Position(Local);
	if (pos.x < mMoveBoundsX.x) {
		pos.x = mMoveBoundsX.x;
	}
	else if (pos.x > mMoveBoundsX.y) {
		pos.x = mMoveBoundsX.y;
	}

	// Handle Y bounds
	if (pos.y < mMoveBoundsY.x) {
		pos.y = mMoveBoundsY.x;
	}
	else if (pos.y > mMoveBoundsY.y) {
		pos.y = mMoveBoundsY.y;
	}

	Position(pos);
}

void Player::HandleFiring() {
	if (mInput->KeyPressed(SDL_SCANCODE_SPACE)) {
		// Loop through bullet pool to find an inactive bullet
		for (int i = 0; i < MAX_BULLETS; ++i) {
			if (!mBullets[i]->Active()) {
				float rotation = Rotation();

				mBullets[i]->Fire(Position(), Rotation(Local) + 90.0f, Direction::Up);  // Fire upwards
				mBullets[i + 1]->Fire(Position(), Rotation(Local)+ 90.0f, Direction::Down);  // Fire downwards (if there's space)

				// Play the fire sound
				Mix_Volume(-1, 36);
				mAudio->PlaySFX("SFX/Fire.wav");
				break;
			}
		}
	}
}

void Player::Visible(bool visible) {
	mVisible = visible;
}

bool Player::IsAnimating() {
	return mAnimating;
}

int Player::Score() {
	return mScore;
}

int Player::Lives() {
	return mLives;
}

void Player::AddScore(int change) {
	mScore += change;
}

void Player::AddLife() {
	mLives += 1;
}

bool Player::IgnoreCollisions()
{
	return !mVisible || mAnimating || !Active();
}

void Player::Hit(PhysEntity* other) {
	mLives -= 1;
	mAnimating = true;
	mDeathAnimation->ResetAnimation();
	mAudio->PlaySFX("SFX/PlayerExplosion.wav");
	mWasHit = true;
}

bool Player::WasHit() {
	return mWasHit;
}

void Player::Update() {
	if (mAnimating) {

		if (mWasHit) {
			mWasHit = false;
		}

		mDeathAnimation->Update();
		mAnimating = mDeathAnimation->IsAnimating();
	}
	else {
		if (Active()) {
			HandleMovement();
			HandleFiring();
		}
	}

	for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i]->Update();
	}
}

void Player::Render() {
	if (mVisible) {
		if (mAnimating) {
			mDeathAnimation->Render();
		}
		else {
			mTexture[mCurrentTexture]->Render();
		}
	}

	for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i]->Render();
	}

	PhysEntity::Render();
}