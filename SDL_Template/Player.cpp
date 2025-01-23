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

	mScore = 32456;
	mLives = 3;
	
	mTexture = new GLTexture("Ship.png");
	mTexture->Scale(Vector2(0.75f, 0.75f));
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero);
	mTexture->Flip(false, true);

	mMoveSpeed = 300.0f;
	mMoveBoundsX = Vector2(0.0f + 35 +mTexture->ScaledDimensions().x/2, Graphics::SCREEN_WIDTH - 35 - mTexture->ScaledDimensions().x/2);
	mMoveBoundsY = Vector2(0.0f + 50 + mTexture->ScaledDimensions().y / 2, Graphics::SCREEN_HEIGHT - 180 - mTexture->ScaledDimensions().y / 2);

	mDeathAnimation = new AnimatedGLTexture("EnemyExplosion.png", 0, 0, 128, 128, 4, 1.0f, Animation::Layouts::Horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Position(Vec2_Zero);
	mDeathAnimation->SetWrapMode(Animation::WrapModes::Once);

	for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i] = new Bullet(true);
	}

	AddCollider(new BoxCollider(Vector2(16.0f, 67.0f)));
	AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2( 18.0f, 10.0f));
	AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2(-18.0f, 10.0f));

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Friendly);
}

Player::~Player() {
	mTimer = nullptr;
	mInput = nullptr;
	mAudio = nullptr;

	delete mTexture;
	mTexture = nullptr;

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


	if (mInput->KeyPressed(SDL_SCANCODE_X)) {
		mAnimating = true;
		mDeathAnimation->ResetAnimation();
		//mAudio->PlaySFX("SFX/PlayerExplosion.wav");
		mWasHit = true;
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
		for (int i = 0; i < MAX_BULLETS; ++i) {
			if (!mBullets[i]->Active()) {
				mBullets[i]->Fire(Position());
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

bool Player::IgnoreCollisions()
{
	return !mVisible || mAnimating;
}

void Player::Hit(PhysEntity * other) {
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
			mTexture->Render();
		}
	}

	for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i]->Render();
	}

	PhysEntity::Render();
}
