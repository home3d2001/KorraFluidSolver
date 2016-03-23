#include <iostream>

#include <viewer.h>
#include <string>
#include <fluidSolver/SPHKernels.h>
#include <assert.h>

#define UNIT_TEST

using namespace std;

void TestKernels()
{
    float x[] = {0, 1.0f, 5.4f, 100.7f, -30.f, 36.f, -4.f};
    float h[] = {10.f, 1.0f, 2.4f, 140.23f, -22.f, -2.f, 20.f};
    float expectedPoly6[] = {0.f, 0.0f, 0.f, 100.7f, 0.f, 0.f, 0.f};

    cout << "TestKernels" << endl;

    int testLen = 7;
    for (int i = 0; i < testLen; ++i) {
        cout << "KernelPoly6, x: " + to_string(x[i]) + ", h: " + to_string(h[i]) << endl;
        float wPoly6 = KernelPoly6(x[i], h[i]);
        cout << wPoly6 << endl;

#ifdef TEST_SPIKY
        cout << "KernelSpiky: x: " + to_string(x[i]) + ", h: " + to_string(h[i]) << endl;
        float wSpiky = KernelSpiky(x[i], h[i]);
        cout << wSpiky << endl;
#endif

#ifdef TEST_VISCOUS
        cout << "KernelViscous: x: " + to_string(x[i]) + ", h: " + to_string(h[i]) << endl;
        float wViscous = KernelViscous(x[i], h[i]);
        cout << wSpiky << endl;
#endif
    }
}

void TestSuite()
{
    cout << "------------------------" << endl;
    cout << "UNIT TEST ENABLED" << endl;

    // @todo: NEED TO WRITE PROPER TEST SUITE!
    bool passed = false;
    TestKernels();

    cout << "END UNIT TEST" << endl;
    cout << "------------------------" << endl;
}

int main()
{
#ifdef UNIT_TEST
    TestSuite();
#endif

    Viewer view(800, 600);
    view.Update();
    view.CleanUp();

    return 0;
}

