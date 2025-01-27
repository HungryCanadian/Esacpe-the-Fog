#pragma once
#include "Enemy.h"

class Butterfly : public Enemy {
public:
	Butterfly(int path, int index, bool challenge);
	~Butterfly();

	static void CreateDivePaths();

	void Dive(int type = 0) override;

	void Hit(PhysEntity* other) override;

	virtual bool IgnoreCollisions() override;

private:
	static std::vector<std::vector<Vector2>> sDivePaths;

	bool mEscort;

	Vector2 LocalFormationPosition() override;

	void HandleDiveState() override;

	void RenderDiveState() override;

};