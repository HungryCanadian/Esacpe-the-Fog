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

	mBackground = new GLTexture("4.png");
	mBackground->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);
	mPlaySpace = new GLTexture("PlaySpace.png");
	mPlaySpace->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);
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
}

void PlayScreen::Update() {
	mPlayer->Update();
}

void PlayScreen::Render() {
	mBackground->Render();
	mPlaySpace->Render();
	mPlayer->Render();
	mSideBar->Render();
}
