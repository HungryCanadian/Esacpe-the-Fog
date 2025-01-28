#pragma once
#include "AnimatedTexture.h"
#include "BezierPath.h"
#include "Player.h"
#include "PhysEntity.h"
#include "Formation.h"

class Enemy : public PhysEntity {
public:
	enum States { InFormation, Diving, Dead };
	enum Types { Butterfly, Boss };

	static void SetFormation(Formation* formation);

	bool InDeathAnimation();

	Enemy(int index, bool challenge);
	virtual ~Enemy();

	//virtual void Dive();

	States CurrentState();
	static void CurrentPlayer(Player* player);


	Types Type();
	int Index();

	virtual void Hit(PhysEntity* other) override;

	void Update() override;
	void Render() override;

	void RandomlySpawn(Vector2 minBoundary, Vector2 maxBoundary);
protected:
	static Formation* sFormation;
	static Player* sPlayer;
	Timer* mTimer;

	Texture* mTexture;
	AnimatedTexture* mDeathAnimation;

	States mCurrentState;
	Types mType;

	int mIndex;

	bool mChallengeStage;

	Vector2 mDiveStartPosition;

	unsigned mCurrentPath;
	unsigned mCurrentWaypoint;

	const float EPSILON = 50.0f;
	float mSpeed;

	virtual void PathComplete();
	virtual void FlyInComplete();

	void JoinFormation();

	virtual Vector2 WorldFormationPosition();
	virtual Vector2 LocalFormationPosition() = 0;

	virtual void HandleInFormationState();
	virtual void HandleDiveState() = 0;
	virtual void HandleDeadState();

	void HandleStates();

	virtual void RenderInFormationState();
	virtual void RenderDiveState() = 0;
	virtual void RenderDeadState();

	void RenderStates();

	// Function to generate random float between min and max
	float RandomFloat(float min, float max);

	// Function to set random spawn position within a boundary
	
private:
	Enemy* mEnemy;
};
