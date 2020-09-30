#include "visualization.hpp"

Visualization::Visualization()
{
    sAppName = "BubbleSort Visualization";
}

Visualization::Visualization(const std::string &sConfigFileName)
{
    sAppName = "BubbleSort Visualization";
    LoadConfig("config.yml");
    InitArr();

#ifdef NEG_TO_NEG
    SetSortingBounds(indexNegElement(m_Arr, true) + 1, indexNegElement(m_Arr, false) - 1);
#else
    SetSortingBounds(0, m_Arr.size());
#endif
    m_SortI = m_SortFrom;
    m_SortJ = m_SortFrom;
}

size_t Visualization::getRows() const
{
    return m_Rows;
}

size_t Visualization::getCols() const
{
    return m_Cols;
}

bool Visualization::OnUserCreate()
{
    DisplayArr();
    return true;
}

bool Visualization::OnUserUpdate(float fElapsedTime)
{
    if (GetKey(olc::Key::P).bReleased)
    {
        HandlePause();
    }
    if (GetKey(olc::Key::LEFT).bReleased || GetKey(olc::Key::RIGHT).bReleased)
    {
        HandleChangeFPS();
    }
    if (GetKey(olc::Key::R).bReleased)
    {
        HandleRestart();
    }

    if (!m_Paused && !m_Sorted)
    {
        m_Ticker += fElapsedTime;
        if (m_Ticker > 1.f / m_FPS)
        {
            if (!DoSortMove())
            {
                for (size_t i = 0; i < m_Arr.size(); i++)
                {
                    DrawLineAt(i, olc::DARK_GREEN);
                }
                LogData("Sorted array: " + stringify<DTYPE>(m_Arr) + ".\n");
                m_Sorted = true;
            }
            m_Ticker -= 1.f / m_FPS;
        }
        if (m_Highlighted && m_Ticker > 0.5f / m_FPS)
        {
            Clear(olc::BLACK);
            DisplayArr();
            m_Highlighted ^= true;
        }
    }
    return true;
}

bool Visualization::OnUserDestroy()
{
    if (m_LogFile)
    {
        m_LogFile.close();
    }

    return true;
}

void Visualization::DrawLineAt(size_t idx, olc::Pixel color)
{
    size_t center = m_Rows / 2;

    DrawLine(idx, center,
             idx, center - (int)m_Arr[idx],
             color);
}

void Visualization::DisplayArr()
{
    for (size_t idx = 0; idx < m_Arr.size(); idx++)
    {
        DrawLineAt(idx, olc::WHITE);
    }
}

bool Visualization::DoSortMove()
{
    if (m_SortI == m_SortTo + 1)
    {
        return false;
    }
    if (m_SortJ == m_SortTo)
    {
        if (!m_Swapped)
        {
            return false;
        }
        m_Swapped ^= true;
        m_SortJ = m_SortFrom;
        m_SortI++;
    }

    if (m_Arr[m_SortJ] > m_Arr[m_SortJ + 1])
    {
        DrawLineAt(m_SortJ, olc::RED);
        DrawLineAt(m_SortJ + 1, olc::RED);
        std::swap(m_Arr[m_SortJ], m_Arr[m_SortJ + 1]);
        LogData("Swapping elements at indices {" + std::to_string(m_SortJ) + ", " + std::to_string(m_SortJ + 1) + "}.\n");
        LogData("Array: " + stringify<DTYPE>(m_Arr) + ".\n");
        m_Swapped = true;
    }
    else
    {
        DrawLineAt(m_SortJ, olc::GREEN);
        DrawLineAt(m_SortJ + 1, olc::GREEN);
    }
    m_Highlighted ^= true;
    ++m_SortJ;

    return true;
}

void Visualization::HandlePause()
{
    if (!m_Paused)
    {
        m_Paused ^= true;
        FillCircle(ScreenWidth() / 2, ScreenHeight() / 2, 2, olc::BLUE);
    }
    else
    {
        m_Paused ^= true;
        Clear(olc::BLACK);
        DisplayArr();
    }
}

void Visualization::HandleChangeFPS()
{
    if (GetKey(olc::Key::LEFT).bReleased)
    {
        m_FPS /= 2;
    }
    else
    {
        m_FPS *= 2;
    }
}

void Visualization::HandleRestart()
{
    InitArr();
#ifdef NEG_TO_NEG
    SetSortingBounds(indexNegElement(m_Arr, true) + 1, indexNegElement(m_Arr, false) - 1);
#else
    SetSortingBounds(0, m_Arr.size());
#endif
    m_SortI = m_SortFrom;
    m_SortJ = m_SortFrom;
    m_Sorted = false;
    DisplayArr();
}

void Visualization::InitArr()
{
    m_Arr.resize(m_Cols);

    if (m_InputMode == randomized)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
#ifdef REAL_INPUT
        std::uniform_real_distribution<> ud(-(double)(m_Rows / 2.), m_Rows / 2.);
#else
        std::uniform_int_distribution<> ud(-m_Rows / 2, m_Rows / 2);
#endif
        for (size_t i = 0; i < m_Cols; i++)
        {
            m_Arr[i] = ud(gen);
        }
        LogData("Generated array: " + stringify<DTYPE>(m_Arr) + ".\n");
        LogData("Starting to sort...\n");
    }
    else
    {
        std::cout << "Enter " << m_Cols << " numbers: ";
        for (size_t i = 0; i < m_Cols; i++)
        {
            std::cin >> m_Arr[i];
        }
        LogData("Input array: " + stringify<DTYPE>(m_Arr) + ".\n");
        LogData("Starting to sort...\n");
    }
}

void Visualization::SetSortingBounds(size_t a, size_t b)
{
    LogData("Defining sorting bounds as " + std::to_string(a) + " and " + std::to_string(b) + ".\n");

    m_SortFrom = a;
    m_SortTo = b;
}

void Visualization::LoadConfig(const std::string &filename)
{
    auto config = YAML::LoadFile(filename);
    auto window = config["window"];
    if (window)
    {
        auto width = window["width"];
        auto height = window["height"];

        if (width && height)
        {
            m_Cols = width.as<size_t>();
            m_Rows = height.as<size_t>();
        }
    }

    auto fps = config["fps"];
    if (fps)
        m_FPS = fps.as<float>();

    auto logFile = config["logfile"];
    auto logFileName = config["logfile-fname"];
    if (logFile)
    {
        if (logFile.as<bool>())
        {
            if (logFileName)
            {
                m_LogFileName = logFileName.as<std::string>();
            }
            else
            {
                m_LogFileName = "session.log";
            }
        }
    }
    ConfigureLogFile();

    auto input = config["input"];
    if (input)
    {
        if (input.as<std::string>() == "randomized")
            m_InputMode = randomized;
        else if (input.as<std::string>() == "manual")
            m_InputMode = manual;
    }
}

void Visualization::LogData(const std::string &data)
{
    if (m_LogFile)
        m_LogFile << data;
    else
        std::cout << data;
}

void Visualization::ConfigureLogFile()
{
    m_LogFile.open(m_LogFileName);
}

size_t indexNegElement(
    std::vector<DTYPE> arr,
    bool first)
{
    size_t res = -1;
    if (first)
    {
        for (size_t i = 0; i < arr.size(); i++)
            if (arr[i] < 0)
            {
                res = i;
                break;
            }
    }
    else
    {
        for (size_t i = arr.size() - 1; i >= 0; i--)
        {
            if (arr[i] < 0)
            {
                res = i;
                break;
            }
        }
    }
    return res;
}

void clearFile(std::string fname)
{
    std::ofstream ofs;
    ofs.open(fname, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

template <typename T>
std::string stringify(const std::vector<T> &v)
{
    std::stringstream ss;
    ss << '[';
    for (auto &el : v)
    {
        ss << el << ", ";
    }
    ss << ']';

    return ss.str();
}