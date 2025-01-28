#include "Level.h"

void Level::StartStage() {
	mStageStarted = true;
}

Level::Level(int stage, SideBar* sideBar, Player* player) {
	mTimer = Timer::Instance();
	mSideBar = sideBar;
	mSideBar->SetLevel(stage);


	mStage = stage;
	mStageStarted = false;

	mLabelTimer = 0.0f;

	mStageLabel = new Texture("Level", "Pixel.otf", 52, { 0,0,0 });
	mStageLabel->Parent(this);
	mStageLabel->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.4f);

	mStageNumber = new Scoreboard({ 75,75,75 });
	mStageNumber->Score(mStage);
	mStageNumber->Parent(this);
	mStageNumber->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	mStageLabelOnScreen = 0.0f;
	mStageLabelOffScreen = 1.5f;

	mReadyLabel = new Texture("Get Ready", "Pixel.otf", 52, { 150,0,0 });
	mReadyLabel->Parent(this);
	mReadyLabel->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.4f);

	mReadyLabelOnScreen = mStageLabelOffScreen;
	mReadyLabelOffScreen = mReadyLabelOnScreen + 3.0f;

	mPlayer = player;
	mPlayerHit = false;
	mRespawnDelay = 3.0f;
	mRespawnLabelOnScreen = 2.0f;

	mGameOverLabel = new Texture("Game Over!", "emulogic.ttf", 32, { 150,0,0 });
	mGameOverLabel->Parent(this);
	mGameOverLabel->Position(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.5f);

	mGameOverDelay = 6.0f;
	mGameOverTimer = 0.0f;
	mGameOverLabelOnScreen = 1.0f;

	mCurrentState = Running;

	mButterflyCount = 0;

	mCurrentFlyInPriority = 0;
	mCurrentFlyInIndex = 0;
	mSpawnDelay = 0.2f;
	mSpawnTimer = 0.0f;
	mSpawningFinished = false;

	mDivingButterfly = nullptr;
	mSkipFirstbutterfly = false;
	mButterflyDiveDelay = 1.0f;
	mButterflyDiveTimer = 0.0f;

	Enemy::CurrentPlayer(mPlayer);
}

Level::~Level() {
	mTimer = nullptr;
	mSideBar = nullptr;
	mPlayer = nullptr;

	delete mStageLabel;
	delete mStageNumber;
	delete mReadyLabel;
	delete mGameOverLabel;

	// Clear out the random enemies
	for (auto enemy : mEnemies) {
		delete enemy;
	}
	mEnemies.clear();
}


Level::LevelStates Level::State() {
	return mCurrentState;
}

void Level::HandleStartLabels() {
	mLabelTimer += mTimer->DeltaTime();
	if (mLabelTimer >= mStageLabelOffScreen) {
		if (mStage > 1) {
			StartStage();
		}
		else {
			if (mLabelTimer >= mReadyLabelOffScreen) {
				StartStage();
				mPlayer->Active(true);
				mPlayer->Visible(true);
			}
		}
	}
}

void Level::HandleCollisions() {
	if (!mPlayerHit) {
		if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_X)) {
			mPlayer->WasHit();
			mSideBar->SetShips(mPlayer->Lives());

			mPlayerHit = true;
			mRespawnTimer = 0.0f;
			mPlayer->Active(false);
		}
		mSideBar->SetShips(mPlayer->Lives());
	}
}

void Level::HandlePlayerDeath() {
	if (!mPlayer->IsAnimating()) {
		if (mPlayer->Lives() > 0) {
			if (mRespawnTimer == 0.0f) {
				mPlayer->Visible(false);
			}

			mRespawnTimer += mTimer->DeltaTime();
			if (mRespawnTimer >= mRespawnDelay) {
				mPlayer->Active(true);
				mPlayer->Visible(true);
				mPlayerHit = false;
			}
		}
		else {
			if (mGameOverTimer == 0.0f) {
				mPlayer->Visible(false);
			}
			mGameOverTimer += mTimer->DeltaTime();
			if (mGameOverTimer >= mGameOverDelay) {
				mCurrentState = GameOver;
			}
		}
	}
}

float Level::RandomFloat(float min, float max) {
	float scale = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); // Random number between 0 and 1
	return min + scale * (max - min);  // Scale it to the desired range
}

void Level::HandleEnemySpawning() {
	mSpawnTimer += mTimer->DeltaTime();
	if (mSpawnTimer >= mSpawnDelay) {
		bool spawned = false;
		if (!mSpawningFinished) {
			if (mEnemies.size() < MAX_BUTTERFLIES) {
				// Set the spawn boundaries
				Vector2 minBoundary(75.0f, -75.0f); // Top of the screen
				Vector2 maxBoundary(Graphics::SCREEN_WIDTH - 75.0f, Graphics::SCREEN_HEIGHT - 200.0f);

				

				// Randomly calculate the position within the boundaries
				float randX = RandomFloat(minBoundary.x, maxBoundary.x);
				float randY = RandomFloat(minBoundary.y, maxBoundary.y);
				// Create a random Butterfly and spawn at a random location
				Butterfly* newEnemy = new Butterfly(0, false);

				newEnemy->RandomlySpawn(minBoundary, maxBoundary);
				std::cout << "Butterfly spawned at position: "
					<< randX << ", " << randY << std::endl;
				mEnemies.push_back(newEnemy);
				
				spawned = true;
			}
		}

		if (!spawned) {
			// Set flag for finishing spawning
			mSpawningFinished = true;
		}

		mSpawnTimer = 0.0f;
	}
}


//void Level::HandleEnemyDiving() {
//	if (mDivingButterfly == nullptr) {
//		mButterflyDiveTimer += mTimer->DeltaTime();
//
//		if (mButterflyDiveTimer >= mButterflyDiveDelay) {
//			bool skipped = false;
//
//			for (int i = MAX_BUTTERFLIES - 1; i >= 0; i--) {
//				if (mFormationButterflies[i] != nullptr && mFormationButterflies[i]->CurrentState() == Enemy::InFormation) {
//					if (!mSkipFirstbutterfly || (mSkipFirstbutterfly && skipped)) {
//						mDivingButterfly = mFormationButterflies[i];
//						mDivingButterfly->Dive();
//						mSkipFirstbutterfly = !mSkipFirstbutterfly;
//						break;
//					}
//				}
//			}
//			mButterflyDiveTimer = 0.0f;
//		}
//	}
//	else {
//		if (mDivingButterfly->CurrentState() != Enemy::Diving) {
//			mDivingButterfly = nullptr;
//		}
//	}
//}




void Level::Update() {
	if (!mStageStarted) {
		HandleStartLabels();
	}
	else {
		if (!mSpawningFinished) {
			HandleEnemySpawning();  // Handles the random spawning logic now
		}

		// Update all enemies, they are now stored in mEnemies
		for (auto enemy : mEnemies) {
			enemy->Update();
		}

		HandleCollisions();

		if (mPlayerHit) {
			HandlePlayerDeath();
		}
		else {
			// Temporary logic for level ending
			if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_N)) {
				mCurrentState = Finished;
			}
		}
	}
}


void Level::Render() {
	if (!mStageStarted) {
		if (mLabelTimer > mStageLabelOnScreen && mLabelTimer < mStageLabelOffScreen) {
			mStageLabel->Render();
			mStageNumber->Render();
		}
		else if (mLabelTimer > mReadyLabelOnScreen && mLabelTimer < mReadyLabelOffScreen) {
			mReadyLabel->Render();
		}
	}
	else {
		// Render all enemies in the random spawn list
		for (auto enemy : mEnemies) {
			enemy->Render();
		}

		if (mPlayerHit) {
			if (mRespawnTimer >= mRespawnLabelOnScreen) {
				mReadyLabel->Render();
			}

			if (mGameOverTimer >= mGameOverLabelOnScreen) {
				mGameOverLabel->Render();
			}
		}
	}
}
