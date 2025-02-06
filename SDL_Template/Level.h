#pragma once
#include "SideBar.h"
#include "Pirate.h"
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
	void RemoveEnemy(Enemy* enemy);



private:
	Timer* mTimer;
	SideBar* mSideBar;
	Enemy* mEnemy;
	Player* mPlayer;

	static const int MAX_PIRATES = 8;

	int mPirateCount;

	std::vector<Enemy*> mEnemies;

	float mFinishLineDelay;
	float mFinishLineTimer;
	Texture* mFinishLine;

	float mSpawnDelay;
	float mSpawnTimer;

	bool mSpawningFinished;

	Pirate* mRushingPirate;
	float mPirateRushDelay;
	float mPirateRushTimer;

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

	bool IsPlayerCrossingFinishLine();
	void HandleStartLabels();
	void HandleCollisions();
	void HandlePlayerDeath();

	void SpawnFinishLine();

	void StartStage();

	bool EnemyFlyingIn();

	void HandleEnemySpawning();
	void HandleEnemyFormation();
	void HandleEnemyDiving();
};