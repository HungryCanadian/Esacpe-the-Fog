#pragma once
#include "Enemy.h"

class Pirate : public Enemy {
public:
	Pirate(int index, bool challenge);
	~Pirate();

	static void CreateDivePaths();

	//void Dive() override;

	void Hit(PhysEntity* other) override;

	virtual bool IgnoreCollisions() override;
	void RandomlySpawn(Vector2 minBoundary, Vector2 maxBoundary);

private:
	//static std::vector<std::vector<Vector2>> sDivePaths;

	//bool mEscort;

	void HandleDiveState() override;

	void RenderDiveState() override;

};