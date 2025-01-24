#include "PlayScreen.h"

PlayScreen::PlayScreen() {
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mPlayer = new Player();
	mPlayer->Parent(this);
	mPlayer->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	mSideBar = new SideBar();
	mSideBar->Parent(this);
	mSideBar->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.93f);
	mSideBar->SetShips(5);

	mStartLabel = new Texture("Start", "emulogic.ttf", 32, { 150,0,0 });
	mStartLabel->Parent(this);
	mStartLabel->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	mLevel = nullptr;
	mLevelStartDelay = 1.0f;
	mLevelStarted = false;

	mBackground = new GLTexture("4.png");
	mBackground->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);
	mPlaySpace = new GLTexture("PlaySpace.png");
	mPlaySpace->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	Butterfly::CreateDivePaths();
	Enemy::CreatePaths();
}

PlayScreen::~PlayScreen() {
	mTimer = nullptr;
	mAudio = nullptr;

	delete mPlayer;
	mPlayer = nullptr;

	delete mSideBar;
	mSideBar = nullptr;

	delete mBackground;
	mBackground = nullptr;
	delete mPlaySpace;
	mPlaySpace = nullptr;

	delete mStartLabel;
	mStartLabel = nullptr;

	delete mLevel;
	mLevel = nullptr;
}

void PlayScreen::StartNewGame() {
	mSideBar->SetHighScore(3080);
	mGameStarted = false;
	mLevelStarted = false;
	mLevelStartTimer = 0.0f;
	mCurrentStage = 0;
	mAudio->PlayMusic("/Music/Pirate 2.wav", 0);
	Mix_VolumeMusic(12);

	delete mPlayer;
	mPlayer = new Player();
	mPlayer->Parent(this);
	mPlayer->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);
	mPlayer->Active(false);

	mSideBar->SetPlayerScore(mPlayer->Score());
	mSideBar->SetShips(mPlayer->Lives());
	mSideBar->SetLevel(0);


}

void PlayScreen::StartNextLevel() {
	mCurrentStage++;
	mLevelStartTimer = 0.0f;
	mLevelStarted = true;

	delete mLevel;
	mLevel = new Level(mCurrentStage, mSideBar, mPlayer);
}

bool PlayScreen::GameOver() {
	//This is essentially an if statement as a return on a single line
	//If mLevelStarts == false, return false. Otherwise we return true or false if the state == gameover
	return !mLevelStarted ? false : (mLevel->State() == Level::GameOver);
}

void PlayScreen::Update() {
	if (mGameStarted) {
		if (!mLevelStarted) {
			mLevelStartTimer += mTimer->DeltaTime();
			if (mLevelStartTimer >= mLevelStartDelay) {
				StartNextLevel();
			}
		}
		else {
			//the level has started or is in session.
			mLevel->Update();

			if (mLevel->State() == Level::Finished) {
				mLevelStarted = false;
			}
		}

		if (mCurrentStage > 0) {
			mSideBar->Update();
		}
		mPlayer->Update();
	}
	else {
		mGameStarted = true;	
	}


}

void PlayScreen::Render() {
	mBackground->Render();
	mPlaySpace->Render();
	if (!mGameStarted) {
		mStartLabel->Render();
	}

	if (mGameStarted) {
		if (mLevelStarted) {
			mLevel->Render();
		}
		mPlayer->Render();
	}
	mSideBar->Render();
}
