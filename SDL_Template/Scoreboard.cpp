#include "Scoreboard.h"

Scoreboard::Scoreboard() 
	: Scoreboard({230, 230, 230}) {
}

Scoreboard::Scoreboard(SDL_Color color) {
	mColor = color;
	Score(0);
}

Scoreboard::~Scoreboard() {
	ClearBoard();
}

void Scoreboard::Score(int score) {
	ClearBoard();

	mScoreValue = score;

	if (score == 0) {
		for (int i = 0; i < 2; i++) {
			mScore.push_back(new GLTexture("0", "emulogic.ttf", 24, mColor));
			mScore[i]->Parent(this);
			mScore[i]->Position(Vector2(-25.0f * i, 0.0f));
		}
	}
	else {
		std::string str = std::to_string(score);
		unsigned lastIndex = (unsigned)str.length() - 1;

		for (unsigned i = 0; i <= lastIndex; i++) {
			mScore.push_back(new GLTexture(str.substr(i, 1), "emulogic.ttf", 24, mColor));
			mScore[i]->Parent(this);
			mScore[i]->Position(Vector2(-25.0f * (lastIndex - i), 0.0f));
		}
	}
}

int Scoreboard::Score() {
	return mScoreValue;
}

void Scoreboard::Render() {
	for (auto digit : mScore) {
		digit->Render();
	}
}

void Scoreboard::ClearBoard() {
	for (auto s : mScore) {
		delete s;
	}

	mScore.clear();
}
