#include "MyMath.h"
#include <windows.h>
#include <random>

using namespace std;

MyMath* MyMath::GetInstance()
{
    static MyMath instance;
    return &instance;
}

int MyMath::GetRand(int min, int max)
{
    std::random_device rd;
    std::mt19937_64 eng(rd());
    std::uniform_real_distribution<> distr(min, max);
    return distr(eng);
}

float MyMath::GetRand(float min, float max)
{
    std::random_device rd;
    std::mt19937_64 eng(rd());
    std::uniform_real_distribution<float> distr(min, max);
    return distr(eng);
}

double MyMath::GetRand(double min, float max)
{
    std::random_device rd;
    std::mt19937_64 eng(rd());
    std::uniform_real_distribution<double> distr(min, max);
    return distr(eng);
}
