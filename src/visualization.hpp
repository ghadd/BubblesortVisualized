#include <boost/variant.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "../include/olcPixelGameEngine.h"

typedef boost::variant<int, double> DataType;

class my_visitor : public boost::static_visitor<double> {
public:
  inline double operator()(int i) const { return i; }
  inline double operator()(double i) const { return i; }
};

enum InputMode { randomized, manual };
enum DataTypeMode { integer, real };

class Visualization : public olc::PixelGameEngine {
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
  std::vector<DataType> m_Arr;
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

public:
  InputMode m_InputMode;
  DataTypeMode m_DataTypeMode;

  bool m_NegToNeg;
  bool m_WithFlag;
  bool m_Maximized;

private:
  std::string m_LogFileName;
  std::ofstream m_LogFile;
};

size_t indexNegElement(std::vector<DataType> arr, bool first = true);

template <typename T> std::string stringify(const std::vector<T> &v);