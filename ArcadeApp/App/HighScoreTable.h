#pragma once
#include "App/App.h"
#include <array>
#include <string>

class Screen;

struct ScoreInformation
{
	int score = 0;
	std::string PlayerName;
	bool operator < (const ScoreInformation& scoreb) const {
		return score > scoreb.score;
	}
};

class HighScoreTable
{
public:

	void Init();
	void UpdateTable(const ScoreInformation& scoreInfo);
	void SaveToFile();
	void PrintHighScoreTable();
	void LoadSavedFile();

	void ClearTable();
	bool IsHighScore(const ScoreInformation& scoreInfo);
	
	inline std::vector<ScoreInformation> GetTable() const { return m_Table; }
	inline const size_t Size() const { return m_Table.size(); }

private:
	static const int MAX_HIGH_SCORES = 10;
	const std::string m_ScoreFilePath = App::GetBasePath() + "Assets\\HighScores.txt";
	void CreateDefaultTable();
	std::vector<ScoreInformation> m_Table;
};

/* 

Player Name | Score
ABC			| 50
DEF			| 40
GHI			| 20
JKL			| 10

*/

