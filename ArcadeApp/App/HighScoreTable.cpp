#include <fstream>
#include <iostream>
#include <algorithm>

#include "HighScoreTable.h"
#include "Graphics/Screen.h"


void HighScoreTable::Init()
{
    // If there is a table that is already loaded delete it
    if (!m_Table.empty())
    {
        ClearTable();
    }

    std::ifstream file(m_ScoreFilePath);

    // Should check if we have a file to load, if we dont -> construct default with scores of 0
    if (file.is_open())
    {
        LoadSavedFile();
    }
    else
    {
        CreateDefaultTable();
        SaveToFile();
    }
}

void HighScoreTable::UpdateTable(const ScoreInformation& scoreInfo)
{
    if (IsHighScore(scoreInfo))
    {
        m_Table.back() = scoreInfo;
        std::sort(m_Table.begin(), m_Table.end());
    }
    else
    {
        return;
    }
}

void HighScoreTable::SaveToFile()
{
    std::ofstream file(m_ScoreFilePath);
    
    for (size_t i = 0; i < m_Table.size(); i++)
    {
        file << m_Table[i].PlayerName << " " << m_Table[i].score << std::endl;
    }

    file.close();
}

void HighScoreTable::PrintHighScoreTable()
{
    for (size_t i = 0; i < m_Table.size(); i++)
    {
        std::cout << m_Table[i].PlayerName << " " << m_Table[i].score << std::endl;
    }
    
    return;
}

void HighScoreTable::LoadSavedFile()
{
    std::ifstream file(m_ScoreFilePath);

    if (!file.is_open())
    {
        std::cout << "Could not open the file: " << m_ScoreFilePath << std::endl;
        return;
    }

    ScoreInformation scoreInfo;
    std::string line = "";

    while (!file.eof() || line != "")
    {
        std::getline(file, line);
        if (line != "")
        {
            std::string playerName = line.substr(0, 3);
            std::string score = line.substr(4, line.length()-4);
            
            ScoreInformation scoreInfo;
            scoreInfo.PlayerName = playerName;
            scoreInfo.score = std::stoi(score);

            m_Table.push_back(scoreInfo);
        }
    }
}

void HighScoreTable::ClearTable()
{
    m_Table.clear();
}

bool HighScoreTable::IsHighScore(const ScoreInformation& scoreInfo)
{
    // Assume the table is sorted
    return scoreInfo.score > m_Table.back().score;
}

void HighScoreTable::CreateDefaultTable()
{
    for (size_t i = 0; i < MAX_HIGH_SCORES; i++)
    {
        ScoreInformation scoreInfo;
        scoreInfo.PlayerName = "ZZZ";
        scoreInfo.score = 0.0f;
        m_Table.push_back(scoreInfo);
    }
}