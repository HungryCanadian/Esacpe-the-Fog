#pragma once
#include "Timer.h"
#include "Scoreboard.h"
#include "AudioManager.h"

using namespace SDLFramework;

class SideBar : public GameEntity {
public:
	SideBar();
	~SideBar();

	void SetShips(int ships);
	void SetPlayerScore(int score);
	void SetHighScore(int score);

	void SetLevel(int level);

	void Update() override;
	void Render() override;
private:
	static const int MAX_SHIP_TEXTURES = 3;
	Timer* mTimer;
	AudioManager* mAudio;

	Texture* mBackground;

	Texture* mHighScorePanel;
	Texture* mHighLabel;
	Texture* mHighLabelExtrude;
	Texture* mScoreLabel;
	Texture* mScoreLabelExtrude;

	Texture* mLevel;
	Texture* mLevelExtrude;
	Texture* mLevelNumber;
	Texture* mLevelNumberExtrude;
	Scoreboard* mHighScoreBoard;

	Texture* mPowerLabel;
	Texture* mPowerLabelExtrude;

	Texture* mOneUpLabel;
	Texture* mPlayerScorePanel;
	float mBlinkTimer;
	float mBlinkInterval;
	bool mOneUpLabelVisible;

	Scoreboard* mPlayerOneScore;

	GameEntity* mShips;
	Texture* mLivesLabelExtrude;
	Texture* mLivesLabel;
	Texture* mShipTextures[MAX_SHIP_TEXTURES];
	Scoreboard* mTotalShipsLabel;
	int mTotalShips;

	GameEntity* mFlags;
	std::vector<Texture*> mFlagTextures;
	int mRemainingLevels;

	float mFlagXOffset;
	float mFlagYOffset;
	float mFlagTimer;
	float mFlagInterval;

	void ClearFlags();
	void AddNextFlag();
	void AddFlag(std::string filename, float width, int value);

};