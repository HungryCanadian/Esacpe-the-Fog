#include "StartScreen.h"

StartScreen::StartScreen() {
	mTimer = Timer::Instance();
	mInput = InputManager::Instance();

	// top bar entities
	mTopBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, 80.0f);
	mPlayerOne = new GLTexture("1UP", "Pixel.otf", 52, { 200, 200, 200 });
	mPlayerOneExtrude = new GLTexture("1UP", "Extrude.otf", 52, { 50, 0, 200 });
	mHiScore = new GLTexture("HI SCORE", "Pixel.otf", 52, { 200, 200, 200 });
	mHiScoreExtrude = new GLTexture("HI SCORE", "Extrude.otf", 52, { 50, 0, 200 });
	mPlayerOneScore = new Scoreboard({ 0, 0, 200 });
	mTopScore = new Scoreboard({ 0, 0, 200 });

	mBackground = new GLTexture("4.png");

	mTopBar->Parent(this);
	mPlayerOne->Parent(mTopBar);
	mPlayerOneExtrude->Parent(mTopBar);
	mHiScore->Parent(mTopBar);
	mHiScoreExtrude->Parent(mTopBar);
	mPlayerOneScore->Parent(mTopBar);
	mTopScore->Parent(mTopBar);

	mPlayerOne->Position(-Graphics::SCREEN_WIDTH * 0.35f, 0.0f);
	mPlayerOneExtrude->Position(-Graphics::SCREEN_WIDTH * 0.35f, 0.0f);
	mHiScore->Position(-30.0f, 0.0f);
	mHiScoreExtrude->Position(-30.0f, 0.0f);

	mPlayerOneScore->Position(-Graphics::SCREEN_WIDTH * 0.23f, 40.0f);
	mTopScore->Position(Graphics::SCREEN_WIDTH * 0.05f, 40.0f);

	mTopScore->Score(308055);

	// logo entities
	mLogo = new GLTexture("CaspiraLogo.png", 0, 344, 360, 172);
	mLogo->Scale(Vector2(1.0f, 1.0f));
	/*mLogoExtrude = new GLTexture("Escape the Fog", "Extrude.otf", 100, { 50, 0, 200 });*/
	mAnimatedLogo = new AnimatedGLTexture("CaspiraLogo.png", 0, 0, 360, 172, 3, 0.5f, Animation::Layouts::Vertical);
	
	mLogo->Parent(this);
	/*mLogoExtrude->Parent(this);*/
	mAnimatedLogo->Parent(this);

	mLogo->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.32f);
	/*mLogoExtrude->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.32f);*/
	mAnimatedLogo->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.32f);

	// play mode entities
	mPlayModes = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.55f);
	mOnePlayerMode = new GLTexture("Start", "emulogic.ttf", 32, { 230, 230, 230 });
	mQuit = new GLTexture("Quit", "emulogic.ttf", 32, { 230, 230, 230 });
	mCursor = new GLTexture("Cursor.png");

	mPlayModes->Parent(this);
	mOnePlayerMode->Parent(mPlayModes);
	mQuit->Parent(mPlayModes);
	mCursor->Parent(mPlayModes);

	mOnePlayerMode->Position(0.0f, -35.0f);
	mQuit->Position(0.0f, 35.0f);
	mCursor->Position(-175.0f, -35.0f);

	mCursorStartPos = mCursor->Position(Local);
	mCursorOffset = Vector2(0.0f, 70.0f);
	mSelectedMode = 0;

	// bottom bar entities
	mBottomBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.7f);
	mNamco = new GLTexture("estate games", "namco__.ttf", 36, { 75, 0, 250 });
	mDates = new GLTexture("January 2025", "Pixel.otf", 72, { 230, 230, 230 });
	mRights = new GLTexture("Something something", "Pixel.otf", 72, { 230, 230, 230 });

	mBottomBar->Parent(this);
	mNamco->Parent(mBottomBar);
	mDates->Parent(mBottomBar);
	mRights->Parent(mBottomBar);

	mNamco->Position(Vec2_Zero);
	mDates->Position(0.0f, 90.0f);
	mRights->Position(0.0f, 170.0f);
	mBackground->Position(Graphics::Instance()->SCREEN_HEIGHT / 2,Graphics::Instance()->SCREEN_WIDTH / 2);

	// screen animation variables
	ResetAnimation();
}


StartScreen::~StartScreen() {
	// top bar entities
	delete mTopBar;
	mTopBar = nullptr;
	delete mPlayerOne;
	mPlayerOne = nullptr;
	delete mPlayerOneExtrude;
	mPlayerOneExtrude = nullptr;
	delete mQuit;
	mQuit = nullptr;
	delete mHiScore;
	mHiScore = nullptr;
	delete mHiScoreExtrude;
	mHiScoreExtrude = nullptr;
	delete mPlayerOneScore;
	mPlayerOneScore = nullptr;
	delete mTopScore;
	mTopScore = nullptr;

	// logo entities
	delete mLogo;
	mLogo = nullptr;
	delete mLogoExtrude;
	mLogoExtrude = nullptr;
	delete mAnimatedLogo;
	mAnimatedLogo = nullptr;

	// play mode entities
	delete mPlayModes;
	mPlayModes = nullptr;
	delete mOnePlayerMode;
	mOnePlayerMode = nullptr;
	delete mCursor;
	mCursor = nullptr;

	// bottom bar entities
	delete mBottomBar;
	mBottomBar = nullptr;
	delete mNamco;
	mNamco = nullptr;
	delete mDates;
	mDates = nullptr;
	delete mRights;
	mRights = nullptr;

	mTimer = nullptr;
	mInput = nullptr;
}

void StartScreen::ResetAnimation() {
	mAnimationStartPos = Vector2(0.0f, Graphics::SCREEN_HEIGHT);
	mAnimationEndPos = Vec2_Zero;
	mAnimationTotalTime = 5.0f;
	mAnimationTimer = 0.0f;
	mAnimationDone = false;

	Position(mAnimationStartPos);
}

int StartScreen::SelectedMode() {
	return mSelectedMode;
}

void StartScreen::ChangeSelectedMode(int change) {
	mSelectedMode += change;

	if (mSelectedMode < 0) {
		mSelectedMode = 1;
	}
	else if (mSelectedMode > 1) {
		mSelectedMode = 0;
	}

	mCursor->Position(mCursorStartPos + mCursorOffset * (float)mSelectedMode);
}

void StartScreen::Update() {
	if (!mAnimationDone) {
		mAnimationTimer += mTimer->DeltaTime();
		Position(Lerp(mAnimationStartPos, mAnimationEndPos, mAnimationTimer / mAnimationTotalTime));

		if (mAnimationTimer >= mAnimationTotalTime) {
			mAnimationDone = true;
		}

		if (mInput->KeyPressed(SDL_SCANCODE_S) || mInput->KeyPressed(SDL_SCANCODE_W)) {
			mAnimationTimer = mAnimationTotalTime;
		}
	}
	else {
		mAnimatedLogo->Update();

		if (mInput->KeyPressed(SDL_SCANCODE_S)) {
			ChangeSelectedMode(1);
		}
		else if (mInput->KeyPressed(SDL_SCANCODE_W)) {
			ChangeSelectedMode(-1);
		}
	}
}

void StartScreen::Render() {
	mBackground->Render();
	mPlayerOneExtrude->Render();
	mPlayerOne->Render();
	mHiScoreExtrude->Render();
	mHiScore->Render();
	mPlayerOneScore->Render();
	mTopScore->Render();

	//mLogoExtrude->Render();
	//mLogo->Render();

	if (!mAnimationDone) {
		mLogo->Render();
	}
	else {
		mAnimatedLogo->Render();
	}

	mOnePlayerMode->Render();
	mQuit->Render();
	mCursor->Render();

	mNamco->Render();
	mDates->Render();
	mRights->Render();
}