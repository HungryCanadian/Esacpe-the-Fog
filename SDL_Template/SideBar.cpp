#include "SideBar.h"


SideBar::SideBar() {
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mBackground = new GLTexture("Panel.png");
	mBackground->Parent(this);
	mBackground->Scale(Vector2(0.55f, 0.20f));
	mBackground->Position(0.0f, 0.0f);
	//High Score
	mHighLabel = new GLTexture("HIGHSCORE", "Pixel.otf", 32, { 150,150,150 });
	mHighLabel->Parent(this);
	mHighLabel->Position(-175.0f, -25.0f);
	mHighLabelExtrude = new GLTexture("HIGHSCORE", "Extrude.otf", 31, { 0,0,0 });
	mHighLabelExtrude->Parent(this);
	mHighLabelExtrude->Position(-175.0f, -25.0f);

	mLevel = new GLTexture("Level", "Pixel.otf", 32, { 150,150,150 });
	mLevel->Parent(this);
	mLevel->Position(190.0f, -25.0f);
	mLevelExtrude = new GLTexture("Level", "Extrude.otf", 31, { 0,0,0 });
	mLevelExtrude->Parent(this);
	mLevelExtrude->Position(190.0f, -25.0f);
	mLevelNumber = new Scoreboard();
	mLevelNumber->Parent(this);
	mLevelNumber->Position(190.0f, 0.0f);
	//mLevelNumberExtrude = new GLTexture("1", "Extrude.otf", 31, { 0,0,0 });
	//mLevelNumberExtrude->Parent(this);
	//mLevelNumberExtrude->Position(190.0f, 0.0f);

	mPowerLabel = new GLTexture("Power-Up", "Pixel.otf", 32, { 150,150,150 });
	mPowerLabel->Parent(this);
	mPowerLabel->Position(400.0f, -40.0f);
	mPowerLabelExtrude = new GLTexture("Power-Up", "Extrude.otf", 31, { 0,0,0 });
	mPowerLabelExtrude->Parent(this);
	mPowerLabelExtrude->Position(400.0f, -40.0f);

	mScoreLabel = new GLTexture("Score", "Pixel.otf", 50, { 150,150,150 });
	mScoreLabel->Parent(this);
	mScoreLabel->Position(0.0f, -100.0f);
	mScoreLabelExtrude = new GLTexture("Score", "Extrude.otf", 49, { 0,0,0 });
	mScoreLabelExtrude->Parent(this);
	mScoreLabelExtrude->Position(0.0f, -100.0f);

	

	mHighScorePanel = new GLTexture("Button.png", true);
	mHighScorePanel->Parent(this);
	mHighScorePanel->Position(0.0f, -85.0f);
	mHighScorePanel->Scale(Vector2(0.2f, 0.2f));

	mHighScoreBoard = new Scoreboard();
	mHighScoreBoard->Parent(this);
	mHighScoreBoard->Position(-130.0f, 0.0f);

	//One up
	mOneUpLabel = new GLTexture("1UP", "emulogic.ttf", 18, { 150,150,150 });
	mOneUpLabel->Parent(this);
	mOneUpLabel->Position(-45.0f, 0.0f);

	mBlinkTimer = 0.0f;
	mBlinkInterval = 0.5f;
	mOneUpLabelVisible = true; //TODO: Set to False when submitting

	mPlayerOneScore = new Scoreboard();
	mPlayerOneScore->Parent(this);
	mPlayerOneScore->Position(60.0f, -65.0f);

	mPlayerScorePanel = new GLTexture("PanelEmpty.png");
	mPlayerScorePanel->Parent(this);
	mPlayerScorePanel->Position(15.0f, 192.0f);
	mPlayerScorePanel->Scale(Vector2(0.1f, 0.1f));



	//Ships Lives (displaying the ships in connection with player lives)
	mLivesLabel = new GLTexture("Lives:", "Pixel.otf", 44, { 150,150,150 });
	mLivesLabel->Parent(this);
	mLivesLabel->Position(-400.0f, -40.0f);
	mLivesLabelExtrude = new GLTexture("Lives:", "Extrude.otf", 43, { 0,0,0 });
	mLivesLabelExtrude->Parent(this);
	mLivesLabelExtrude->Position(-400.0f, -40.0f);
	mShips = new GameEntity();
	mShips->Parent(this);
	mShips->Position(-450.0f, 0.0f);

	for (int i = 0; i < MAX_SHIP_TEXTURES; i++) {
		mShipTextures[i] = new GLTexture("Life.png");
		mShipTextures[i]->Parent(mShips);
		mShipTextures[i]->Scale(Vector2(0.5f, 0.5f));
		mShipTextures[i]->Position(40.0f * (i % 10), 25.0f * (i / 3));

	}

	mTotalShipsLabel = new Scoreboard();
	mTotalShipsLabel->Parent(mShips);
	mTotalShipsLabel->Position(115.0f, 0.0f);

	//Level Flags
	mFlags = new GameEntity();
	mFlags->Parent(this);
	mFlags->Position(-50.0f, 600.0f);

	mFlagTimer = 0.0f;
	mFlagInterval = 0.25f;

}

SideBar::~SideBar() {
	mTimer = nullptr;
	mAudio = nullptr;

	delete mBackground;
	mBackground = nullptr;

	delete mHighLabel;
	mHighLabel = nullptr;
	delete mHighLabelExtrude;
	mHighLabelExtrude = nullptr;

	delete mLevel;
	mLevel = nullptr;
	delete mLevelExtrude;
	mLevelExtrude = nullptr;
	delete mLevelNumber;
	mLevelNumber = nullptr;
	delete mLevelNumberExtrude;
	mLevelNumberExtrude = nullptr;

	delete mScoreLabel;
	mScoreLabel = nullptr;
	delete mScoreLabelExtrude;
	mScoreLabelExtrude = nullptr;

	delete mPowerLabel;
	mPowerLabel = nullptr;
	delete mPowerLabelExtrude;
	mPowerLabelExtrude = nullptr;

	delete mLivesLabel;
	mLivesLabel = nullptr;

	delete mHighScoreBoard;
	mHighScoreBoard = nullptr;

	delete mOneUpLabel;
	mOneUpLabel = nullptr;

	delete mPlayerOneScore;
	mPlayerOneScore = nullptr;

	delete mShips;
	mShips = nullptr;

	delete mTotalShipsLabel;
	mTotalShipsLabel = nullptr;

	for (auto texture : mShipTextures) {
		delete texture;
		texture = nullptr;
	}

	delete mFlags;
	mFlags = nullptr;

	ClearFlags();

}

void SideBar::ClearFlags() {
	for (int i = 0; i < mFlagTextures.size(); i++) {
		delete mFlagTextures[i];
		mFlagTextures[i] = nullptr;
	}

	mFlagTextures.clear();
}

void SideBar::AddNextFlag() {
	if (mRemainingLevels >= 50) {
		AddFlag("LevelFlags.png", 62, 50);
	}
	else if (mRemainingLevels >= 30) {
		AddFlag("LevelFlags.png", 62, 30);
	}
	else if (mRemainingLevels >= 20) {
		AddFlag("LevelFlags.png", 62, 20);
	}
	else if (mRemainingLevels >= 10) {
		AddFlag("LevelFlags.png", 54, 10);
	}
	else if (mRemainingLevels >= 5) {
		AddFlag("LevelFlags.png", 30, 5);
	}
	else {
		AddFlag("LevelFlags.png", 30, 1);
	}
}

void SideBar::AddFlag(std::string filename, float width, int value) {
	int index = (int)mFlagTextures.size();

	if (index > 0) {
		mFlagXOffset += width * 0.5f;
	}

	if (mFlagXOffset > 140) {
		mFlagYOffset += 66;
		mFlagXOffset = 0;
	}

	mRemainingLevels -= value;
	//The x position of the flag we want to cut from the texture
	int x = 0;

	switch (value) {
	case 50:
		x = 228;
		break;
	case 30:
		x = 168;
		break;
	case 20:
		x = 108;
		break;
	case 10:
		x = 56;
		break;
	case 5:
		x = 28;
		break;

	}

	mFlagTextures.push_back(new Texture(filename, x, 0, width - 2, 64));
	mFlagTextures[index]->Parent(mFlags);
	mFlagTextures[index]->Position(mFlagXOffset, mFlagYOffset);
	mFlagXOffset += width * 0.5f;

	mAudio->PlaySFX("FlagSound.wav", 0, -1);
}

void SideBar::SetShips(int ships) {
	mTotalShips = ships;

	if (ships > MAX_SHIP_TEXTURES) {
		mTotalShipsLabel->Score(ships);
	}
}

void SideBar::SetHighScore(int score) {
	mHighScoreBoard->Score(score);
}

void SideBar::SetPlayerScore(int score) {
	mPlayerOneScore->Score(score);
}

void SideBar::SetLevel(int level) {
	ClearFlags();
	mLevelNumber->Score(level);
	mRemainingLevels = level;
	mFlagXOffset = 0.0f;
}

void SideBar::Update() {
	mBlinkTimer += mTimer->DeltaTime();

	if (mBlinkTimer >= mBlinkInterval) {
		mOneUpLabelVisible = !mOneUpLabelVisible;
		mBlinkTimer = 0.0f;
	}

	if (mRemainingLevels > 0) {
		mFlagTimer += mTimer->DeltaTime();

		if (mFlagTimer >= mFlagInterval) {
			AddNextFlag();
			mFlagTimer = 0.0f;
		}
	}
}

void SideBar::Render() {
	mBackground->Render();
	mHighScorePanel->Render();
	mLivesLabelExtrude->Render();
	mLivesLabel->Render();
	mPlayerScorePanel->Render();
	mHighLabelExtrude->Render();
	mHighLabel->Render();
	mLevelExtrude->Render();
	mLevel->Render();
	//mLevelNumberExtrude->Render();
	mLevelNumber->Render();
	mPowerLabelExtrude->Render();
	mPowerLabel->Render();
	mScoreLabelExtrude->Render();
	mScoreLabel->Render();
	mHighScoreBoard->Render();

	if (mOneUpLabelVisible == true) {
		mOneUpLabel->Render();
	}
	mPlayerOneScore->Render();

	for (int i = 0; i < MAX_SHIP_TEXTURES && i < mTotalShips; i++) {
		mShipTextures[i]->Render();
	}

	if (mTotalShips > MAX_SHIP_TEXTURES) {
		mTotalShipsLabel->Render();
	}

	for (auto flag : mFlagTextures) {
		flag->Render();
	}
}