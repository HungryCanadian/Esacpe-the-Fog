#include "Level.h"

void Level::StartStage() {
	mPlayer->AddLife();
	mSideBar->SetShips(mPlayer->Lives());
	mStageStarted = true;
}

Level::Level(int stage, SideBar* sideBar, Player* player) {
	mTimer = Timer::Instance();
	mSideBar = sideBar;
	mSideBar->SetLevel(stage);


	mStage = stage;
	mStageStarted = false;

	mLabelTimer = 0.0f;

	mStageLabel = new GLTexture("Level", "Pixel.otf", 52, { 0,0,0 });
	mStageLabel->Parent(this);
	mStageLabel->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.4f);

	mStageNumber = new Scoreboard({ 75,75,75 });
	mStageNumber->Score(mStage);
	mStageNumber->Parent(this);
	mStageNumber->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	mStageLabelOnScreen = 0.0f;
	mStageLabelOffScreen = 1.5f;

	mReadyLabel = new GLTexture("Get Ready", "Pixel.otf", 52, { 150,0,0 });
	mReadyLabel->Parent(this);
	mReadyLabel->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.4f);

	mReadyLabelOnScreen = mStageLabelOffScreen;
	mReadyLabelOffScreen = mReadyLabelOnScreen + 3.0f;

	mPlayer = player;
	mPlayerHit = false;
	mRespawnDelay = 3.0f;
	mRespawnLabelOnScreen = 2.0f;

	mGameOverLabel = new GLTexture("Game Over!", "emulogic.ttf", 32, { 150,0,0 });
	mGameOverLabel->Parent(this);
	mGameOverLabel->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	mGameOverDelay = 6.0f;
	mGameOverTimer = 0.0f;
	mGameOverLabelOnScreen = 1.0f;

	mCurrentState = Running;

	mFinishLineTimer = 0.0f;
	mFinishLineDelay = 60.0f;
	mFinishLine = new GLTexture("Finishline.png");
	mFinishLine->Scale(Vector2(0.0f, 0.0f));


	mSpawnDelay = 1.0f;
	mSpawnTimer = 0.0f;
	mSpawningFinished = false;

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
		if (mPlayer->WasHit()) {
			mSideBar->SetShips(mPlayer->Lives());

			mPlayerHit = true;
			mRespawnTimer = 0.0f;
			mPlayer->Active(false);
		}
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

void Level::SpawnFinishLine() {
	mFinishLineTimer += mTimer->DeltaTime();
	// Set the spawn boundaries
	Vector2 minBoundary(75.0f, 75.0f); // Top of the screen
	Vector2 maxBoundary(Graphics::SCREEN_WIDTH - 75.0f, Graphics::SCREEN_HEIGHT - 200.0f);
	if (mFinishLineTimer >= mFinishLineDelay) {
		float randX = RandomFloat(minBoundary.x, maxBoundary.x);
		float randY = RandomFloat(minBoundary.y, maxBoundary.y);
		mFinishLine = new GLTexture("Finishline.png");
		mFinishLine->Position(randX, randY);
		mFinishLine->Scale(Vector2(0.05f, 0.05f));

		mFinishLineTimer = 0.0f;
	}

}

bool Level::IsPlayerCrossingFinishLine() {
	// Get player's position
	Vector2 playerPosition = mPlayer->Position();

	// Get finish line's position and scale
	Vector2 finishLinePosition = mFinishLine->Position();
	Vector2 finishLineScale = mFinishLine->ScaledDimensions();

	// Check if the player has passed the finish line
	bool crossedX = playerPosition.x > finishLinePosition.x && playerPosition.x < finishLinePosition.x + finishLineScale.x;
	bool crossedY = playerPosition.y > finishLinePosition.y && playerPosition.y < finishLinePosition.y + finishLineScale.y;

	// If both conditions are met, player has crossed the finish line
	return crossedX && crossedY;
}

void Level::HandleEnemySpawning() {
	mSpawnTimer += mTimer->DeltaTime();

	// Set the spawn boundaries
	Vector2 minBoundary(75.0f, 75.0f); // Top of the screen
	Vector2 maxBoundary(Graphics::SCREEN_WIDTH - 75.0f, Graphics::SCREEN_HEIGHT - 200.0f);

	// Check if we have fewer than MAX_PIRATES and if enough time has passed
	if (mEnemies.size() < MAX_PIRATES && mSpawnTimer >= mSpawnDelay) {
		// Randomly calculate the position within the boundaries
		float randX = RandomFloat(minBoundary.x, maxBoundary.x);
		float randY = RandomFloat(minBoundary.y, maxBoundary.y);

		// Create a random Pirate (enemy) and spawn at a random location
		Pirate* newEnemy = new Pirate(0, false);

		newEnemy->Position(randX, randY);
		mEnemies.push_back(newEnemy);

		// Reset the spawn timer after spawning
		mSpawnTimer = 0.0f;
	}
}


void Level::Update() {
	if (!mStageStarted) {
		HandleStartLabels();
	}
	else {
		SpawnFinishLine();
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
			if (IsPlayerCrossingFinishLine()) {
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
	mFinishLine->Render();
}