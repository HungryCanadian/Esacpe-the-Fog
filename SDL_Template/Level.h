#pragma once
#include "SideBar.h"
#include "Player.h"
#include "Butterfly.h"
#include "tinyxml2.h"

using namespace SDLFramework;
using namespace tinyxml2;

class Level : public GameEntity {
public:
	enum LevelStates { Running, Finished, GameOver };

	Level(int stage, SideBar* sideBar, Player* player);
	~Level();

	LevelStates State();

	void Update();
	void Render();

	float RandomFloat(float min, float max);



private:
	Timer* mTimer;
	SideBar* mSideBar;

	Player* mPlayer;

	static const int MAX_BUTTERFLIES = 16;

	int mButterflyCount;

	std::vector<Enemy*> mEnemies;

	XMLDocument mSpawningPatterns;
	int mCurrentFlyInPriority;
	int mCurrentFlyInIndex;

	float mSpawnDelay;
	float mSpawnTimer;

	bool mSpawningFinished;

	Butterfly* mDivingButterfly;
	bool mSkipFirstbutterfly;
	float mButterflyDiveDelay;
	float mButterflyDiveTimer;

	int mStage;
	bool mChallengeStage;
	bool mStageStarted;

	Texture* mReadyLabel;
	float mReadyLabelOnScreen;
	float mReadyLabelOffScreen;

	Texture* mStageLabel;
	Scoreboard* mStageNumber;
	float mStageLabelOnScreen;
	float mStageLabelOffScreen;

	float mLabelTimer;

	bool mPlayerHit;
	float mRespawnDelay;
	float mRespawnTimer;
	float mRespawnLabelOnScreen;

	Texture* mGameOverLabel;
	float mGameOverDelay;
	float mGameOverTimer;
	float mGameOverLabelOnScreen;

	LevelStates mCurrentState;



	void HandleStartLabels();
	void HandleCollisions();
	void HandlePlayerDeath();

	void StartStage();

	bool EnemyFlyingIn();

	void HandleEnemySpawning();
	void HandleEnemyFormation();
	void HandleEnemyDiving();
};