#pragma once
#include <array>
#include <string>
#include <vector>

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

	void Init(std::string name);
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
	std::string m_ScoreFilePath;
	void CreateDefaultTable();
	std::vector<ScoreInformation> m_Table;
	std::string m_Name;
	
};

/* 

Player Name | Score
ABC			| 50
DEF			| 40
GHI			| 20
JKL			| 10

*/

