#pragma once
#include "Level.h"
#include "Player.h"

using namespace SDLFramework;

class PlayScreen : public GameEntity {
public:
	PlayScreen();
	~PlayScreen();

	void StartNewGame();
	void StartNextLevel();

	void Update() override;
	void Render() override;

	bool GameOver();

private:
	Timer* mTimer;
	AudioManager* mAudio;
	SideBar* mSideBar;


	Texture* mStartLabel;
	Player* mPlayer;
	Texture* mBackground;
	Texture* mPlaySpace;

	float mLevelStartTimer;
	float mLevelStartDelay;

	bool mGameStarted;

	Level* mLevel;
	bool mLevelStarted;
	int mCurrentStage;
};