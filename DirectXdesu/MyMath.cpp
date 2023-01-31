#include "MyMath.h"
#include <windows.h>
#include <random>
#include "KWinApp.h"

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

Matrix MyMath::LockAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    Vector3 baseX = up;
    Vector3 baseY;
    Vector3 baseZ = target - eye;

    baseZ.Normalize();

    baseX = baseX.Cross(baseZ);
    baseX.Normalize();

    baseY = baseZ;
    baseY = baseY.Cross(baseX);

    Matrix matView;

    matView.m[0][0] = baseX.x;
    matView.m[1][0] = baseX.y;
    matView.m[2][0] = baseX.z;

    matView.m[0][1] = baseY.x;
    matView.m[1][1] = baseY.y;
    matView.m[2][1] = baseY.z;

    matView.m[0][2] = baseZ.x;
    matView.m[1][2] = baseZ.y;
    matView.m[2][2] = baseZ.z;

    matView.m[3][0] = baseX.Dot(eye);
    matView.m[3][1] = baseY.Dot(eye);
    matView.m[3][2] = baseZ.Dot(eye);

    return matView;
}

Matrix MyMath::PerspectiveFov(float fovY, float nearZ, float farZ)
{
    assert(nearZ > 0.0f && farZ > 0.0f);

    float aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();

    float height = 1.0f / tanf(fovY / 2.0f);

    Matrix matProjection;
    matProjection.Identity();
    matProjection.m[0][0] = height;
    matProjection.m[1][1] = height * aspect;
    matProjection.m[2][2] = (float)(farZ + nearZ) / (farZ - nearZ);
    matProjection.m[2][3] = 1.0f;
    matProjection.m[3][2] = -2.0f * farZ * nearZ / (float)(farZ - nearZ);
    matProjection.m[3][3] = 0.0f;

    return matProjection;
}

Matrix MyMath::Ortho(float nearZ, float farZ)
{
    Matrix matProjection;
    matProjection.Identity();
    matProjection.m[0][0] = 2 / (float)KWinApp::GetWindowSizeW();
    matProjection.m[1][1] = 2 / (float)KWinApp::GetWindowSizeH();
    matProjection.m[2][2] = 1 / (float)(farZ - nearZ);
    matProjection.m[3][2] = nearZ / (float)(nearZ - farZ);

    return matProjection;
}

float MyMath::ConvertToRad(float angle)
{
    return angle / 180.0f * PI;
}
