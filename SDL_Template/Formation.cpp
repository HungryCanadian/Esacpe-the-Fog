#include <cstdlib> // For std::rand()
#include <ctime>   // For std::time()

#include "Formation.h"

Formation::Formation() {
	mTimer = Timer::Instance();

	mOffsetAmount = 10.0f;
	mOffsetDelay = 0.4f;
	mOffsetTimer = 0.0f;
	mOffsetDirection = 1;
	mOffsetCounter = 4;

	mSpreadTimer = 0.0f;
	mSpreadDelay = 0.6f;
	mSpreadCounter = 0;
	mSpreadDirection = 1;

	mLocked = false;

	mGridSize = Vector2(32.0f, 64.0f);

	// Initialize random number generator
	std::srand(static_cast<unsigned int>(std::time(0)));

	// Define spawn boundaries
	mSpawnMin = Vector2(100.0f, 100.0f);  // Min x and y boundaries
	mSpawnMax = Vector2(400.0f, 600.0f);  // Max x and y boundaries
}

Formation::~Formation() {
	mTimer = nullptr;
}

Vector2 Formation::GridSize() {
	return mGridSize;
}

int Formation::GetTick() {
	if (!mLocked || mOffsetCounter != 4) {
		return mOffsetCounter;
	}
	else {
		return mSpreadCounter;
	}
}

void Formation::Lock() {
	mLocked = true;
}

bool Formation::Locked() {
	return mLocked && mOffsetCounter == 4;
}

// Generate random float between min and max
float Formation::RandomFloat(float min, float max) {
	return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

// Randomly spawn pirate ships within defined boundaries
void Formation::RandomlySpawnShips() {
	// Example of randomly positioning a ship within the spawn area
	// Each ship could have its own random position generated
	float randomX = RandomFloat(mSpawnMin.x, mSpawnMax.x);
	float randomY = RandomFloat(mSpawnMin.y, mSpawnMax.y);

	// Assuming you have a function to create or move a ship to a position:
	// CreateShip(Vector2(randomX, randomY));  // Or something like this

	// Or just set the new position if already created:
	// mShip->SetPosition(Vector2(randomX, randomY));
}

void Formation::Update() {
	if (!mLocked || mOffsetCounter != 4) {
		mOffsetTimer += mTimer->DeltaTime();

		if (mOffsetTimer >= mOffsetDelay) {
			mOffsetCounter++;
			Translate(Vec2_Right * (float)mOffsetDirection * mOffsetAmount, World);

			if (mOffsetCounter == 8) {
				mOffsetCounter = 0;
				mOffsetDirection *= -1;
			}

			mOffsetTimer = 0.0f;
		}

	}
	else {
		// Our breathing animation is playing
		mSpreadTimer += mTimer->DeltaTime();

		if (mSpreadTimer >= mSpreadDelay) {
			mSpreadCounter += mSpreadDirection;

			mGridSize.x += (mSpreadDirection * ((mSpreadCounter % 2) ? 1 : 2));

			if (mSpreadCounter == 4 || mSpreadCounter == 0) {
				mSpreadDirection *= -1;
			}

			mSpreadTimer = 0.0f;
		}
	}

	// Call random spawn function during update
	RandomlySpawnShips();
}
