#include <ctime>
#include <chrono>
#include <vector>
#include <string>
#include <random>

#include <iostream>
#include <yaml-cpp/yaml.h>

#include "../include/olcPixelGameEngine.h"

// #define REAL_INPUT
// #define NEG_TO_NEG

#ifdef REAL_INPUT
#define DTYPE double
#else
#define DTYPE int
#endif

enum InputMode
{
    randomized,
    manual
};

class Visualization : public olc::PixelGameEngine
{
public:
    Visualization();
    Visualization(const std::string &sConfigFileName);

public:
    size_t getCols() const;
    size_t getRows() const;

    void LoadConfig(const std::string &filename);
    void LogData(const std::string &data);

public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
    bool OnUserDestroy() override;

private:
    void DrawLineAt(size_t idx, olc::Pixel color);
    void InitArr();
    void SetSortingBounds(size_t a, size_t b);
    void ConfigureLogFile();

    void DisplayArr();
    bool DoSortMove();

    void HandlePause();
    void HandleChangeFPS();
    void HandleRestart();

private:
    std::vector<DTYPE> m_Arr;
    float m_Ticker{};

private:
    size_t m_Rows{}, m_Cols{};
    float m_FPS{};

    size_t m_SortFrom{};
    size_t m_SortTo{};

    size_t m_SortI{};
    size_t m_SortJ{};

    bool m_Swapped = false;
    bool m_Highlighted = false;
    bool m_Paused = false;
    bool m_Sorted = false;

private:
    InputMode m_InputMode;

private:
    std::string m_LogFileName;
    std::ofstream m_LogFile;
};

size_t indexNegElement(
    std::vector<DTYPE> arr,
    bool first = true);

template <typename T>
std::string stringify(const std::vector<T> &v);