#pragma once
#include "GameEntity.h"
#include "Timer.h"

using namespace SDLFramework;

class Formation : public GameEntity {
public:
	Formation();
	~Formation();

	void Update() override;

	int GetTick();

	Vector2 GridSize();

	bool Locked();
	void Lock();

private:
	Timer* mTimer;

	//SidetoSide animation while 'waiting'
	float mOffsetAmount;
	float mOffsetTimer;
	float mOffsetDelay;
	int mOffsetCounter;
	int mOffsetDirection;

	//Breathing Animation when Formation is locked.
	float mSpreadTimer;
	float mSpreadDelay;
	int mSpreadCounter;
	int mSpreadDirection;

	Vector2 mGridSize;

	bool mLocked;
};