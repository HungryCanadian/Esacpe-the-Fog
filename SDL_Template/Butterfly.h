#pragma once
#include "Enemy.h"

class Butterfly : public Enemy {
public:
	Butterfly(int index, bool challenge);
	~Butterfly();

	static void CreateDivePaths();

	//void Dive() override;

	void Hit(PhysEntity* other) override;

	virtual bool IgnoreCollisions() override;
	void RandomlySpawn(Vector2 minBoundary, Vector2 maxBoundary);

private:
	//static std::vector<std::vector<Vector2>> sDivePaths;

	//bool mEscort;

	

	Vector2 LocalFormationPosition() override;

	void HandleDiveState() override;

	void RenderDiveState() override;

};