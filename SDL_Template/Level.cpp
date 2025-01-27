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

	std::string fullPath = SDL_GetBasePath();
	fullPath.append("Data/Level1.xml");
	mSpawningPatterns.LoadFile(fullPath.c_str());

	mChallengeStage = mSpawningPatterns.FirstChildElement("Level")->FirstChildElement()->BoolAttribute("value");
	if (!mChallengeStage) {
		//Create our Formation
		//Initialize our Enemy Arrays
		mFormation = new Formation();
		mFormation->Position(Graphics::SCREEN_WIDTH * 0.4f, 150.0f);
		Enemy::SetFormation(mFormation);

		for (int i = 0; i < MAX_BUTTERFLIES; i++) {
			mFormationButterflies[i] = nullptr;
		}

	}
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
	mStageLabel = nullptr;
	delete mStageNumber;
	mStageNumber = nullptr;
	delete mReadyLabel;
	mReadyLabel = nullptr;

	delete mGameOverLabel;
	mGameOverLabel = nullptr;

	delete mFormation;
	mFormation = nullptr;

	for (auto enemy : mEnemies) {
		delete enemy;
		enemy = nullptr;
	}
	for (int i = 0; i < MAX_BUTTERFLIES; i++) {
		delete mFormationButterflies[i];
		mFormationButterflies[i] = nullptr;
	}

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

void Level::HandleEnemySpawning() {
	mSpawnTimer += mTimer->DeltaTime();
	if (mSpawnTimer >= mSpawnDelay) {
		XMLElement* element = mSpawningPatterns.FirstChildElement("Level")->FirstChild()->NextSiblingElement();
		bool spawned = false;
		bool priorityFound = false;

		while (element != nullptr) {
			int priority = element->IntAttribute("priority");

			if (mCurrentFlyInPriority == priority) {
				priorityFound = true;
				int path = element->IntAttribute("path");
				XMLElement* child = element->FirstChildElement();

				for (int i = 0; i < mCurrentFlyInIndex && child != nullptr; i++) {
					child = child->NextSiblingElement();
				}

				if (child != nullptr) {
					std::string type = child->Attribute("type");
					int index = child->IntAttribute("index");

					if (type.compare("Butterfly") == 0) {
						if (!mChallengeStage) {
							mFormationButterflies[index] = new Butterfly(path, index, false);
							mButterflyCount += 1;
						}
						else {
							mEnemies.push_back(new Butterfly(path, index, false));
						}
					}
					spawned = true;
				}
			}

			element = element->NextSiblingElement();
		}

		if (!priorityFound) {
			// no priorities found means no more Spawn elements
			mSpawningFinished = true;
		}
		else {
			if (!spawned) {
				// We have Spawn elements waiting, but we didn't spawn anything
				if (!EnemyFlyingIn()) {
					mCurrentFlyInPriority += 1;
					mCurrentFlyInIndex = 0;
				}
			}
			else {
				// We haven't finished spawning this element's enemies, next index!
				mCurrentFlyInIndex += 1;
			}
		}
		mSpawnTimer = 0.0f;
	}
}

bool Level::EnemyFlyingIn() {
	for (Butterfly* b : mFormationButterflies) {
		if (b != nullptr && b->CurrentState() == Enemy::FlyIn) {
			return true;
		}
	}
	return false;
}

void Level::HandleEnemyFormation() {
	mFormation->Update();

	bool levelCleared = mSpawningFinished;

	for (Butterfly* butterfly : mFormationButterflies) {
		if (butterfly != nullptr) {
			butterfly->Update();
			if (butterfly->CurrentState() != Enemy::Dead || butterfly->InDeathAnimation()) {
				levelCleared = false;
			}
		}
	}

	if (!mFormation->Locked()) {
		if (mButterflyCount == MAX_BUTTERFLIES && mWaspCount == MAX_WASPS && mBossCount == MAX_BOSSES) {
			if (!EnemyFlyingIn()) {
				mFormation->Lock();
			}
		}
	}
	else {
		HandleEnemyDiving();
	}

	if (levelCleared) {
		mCurrentState = Finished;
	}

}

void Level::HandleEnemyDiving() {
	if (mDivingButterfly == nullptr) {
		mButterflyDiveTimer += mTimer->DeltaTime();

		if (mButterflyDiveTimer >= mButterflyDiveDelay) {
			bool skipped = false;

			for (int i = MAX_BUTTERFLIES - 1; i >= 0; i--) {
				if (mFormationButterflies[i] != nullptr && mFormationButterflies[i]->CurrentState() == Enemy::InFormation) {
					if (!mSkipFirstbutterfly || (mSkipFirstbutterfly && skipped)) {
						mDivingButterfly = mFormationButterflies[i];
						mDivingButterfly->Dive();
						mSkipFirstbutterfly = !mSkipFirstbutterfly;
						break;
					}
				}
			}
			mButterflyDiveTimer = 0.0f;
		}
	}
	else {
		if (mDivingButterfly->CurrentState() != Enemy::Diving) {
			mDivingButterfly = nullptr;
		}
	}
}




void Level::Update() {
	if (!mStageStarted) {
		HandleStartLabels();
	}
	else {
		if (!mSpawningFinished) {
			HandleEnemySpawning();
		}

		if (!mChallengeStage) {
			HandleEnemyFormation();
		}
		else {
			for (auto enemy : mEnemies) {
				enemy->Update();
			}
		}

		HandleCollisions();

		if (mPlayerHit) {
			HandlePlayerDeath();
		}
		else {
			//TODO: Temporary logic until enemeies implemented
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
		if (!mChallengeStage) {
			for (Butterfly* butterfly : mFormationButterflies) {
				if (butterfly != nullptr) {
					butterfly->Render();
				}
			}
		}
		else {
			for (auto enemy : mEnemies) {
				enemy->Render();
			}
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