#pragma once
#include "Defines.h"

class Score
{
public:
	int score;
	int* highScore;
	int* scoreType;
	int numberHighScore;

	static enum
	{
		LARGE_CHICKEN,
		SMALL_CHICKEN,
		THIGH_CHICKEN
	};
	void SetScore(int score);
	void Init(const char * path, char *file);
	void ScoreUp(int ScoreChicken);
	void GetHighScore(const char * path,char *file);
	void SetHighScore(const char * path,char *file);
	Score(void);
	~Score(void);
};
