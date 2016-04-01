#include <iostream>

#include <viewer.h>
#include <string>
#include <fluidSolver/SPHKernels.h>
#include <assert.h>
#include <thirdparty/easylogging++.h>

INITIALIZE_EASYLOGGINGPP

#define UNIT_TEST

using namespace std;

void TestKernels()
{
    float x[] = {0, 1.0f, 5.4f, 100.7f, -30.f, 36.f, -4.f};
    float h[] = {10.f, 1.0f, 2.4f, 140.23f, -22.f, -2.f, 20.f};
    glm::vec3 r[] = {
        glm::vec3(1,0,0),
        glm::vec3(0,1,0),
        glm::vec3(0,0,1),
        glm::vec3(1,0,0),
        glm::vec3(0,1,0),
        glm::vec3(0,1,0),
        glm::vec3(0,0,1)
    };

    LOG(INFO) << "TestKernels" << endl;

    int testLen = 7;
    for (int i = 0; i < testLen; ++i) {
        LOG(INFO) << "KernelPoly6, x: " + to_string(x[i]) + ", h: " + to_string(h[i]) << endl;
        float wPoly6 = KernelPoly6(x[i], h[i]);
        LOG(INFO) << wPoly6 << endl;

        // LOG(INFO) << "Grad" << endl;
        // glm::vec3 gradSpiky = GradKernelSpike(r, x, h);
        // LOG(INFO) << "x: " << gradSpiky.x << ", y: " << gradSpiky.y << ", z: " << gradSpiky.z << endl;

#ifdef TEST_SPIKY
        LOG(INFO) << "KernelSpiky: x: " + to_string(x[i]) + ", h: " + to_string(h[i]) << endl;
        float wSpiky = KernelSpiky(x[i], h[i]);
        LOG(INFO) << wSpiky << endl;
#endif

#ifdef TEST_VISCOUS
        LOG(INFO) << "KernelViscous: x: " + to_string(x[i]) + ", h: " + to_string(h[i]) << endl;
        float wViscous = KernelViscous(x[i], h[i]);
        LOG(INFO) << wSpiky << endl;
#endif
    }
}

void TestSuite()
{
    LOG(INFO) << "------------------------" << endl;
    LOG(INFO) << "UNIT TEST ENABLED" << endl;

    // @todo: NEED TO WRITE PROPER TEST SUITE!
    bool passed = false;
    TestKernels();

    LOG(INFO) << "END UNIT TEST" << endl;
    LOG(INFO) << "------------------------" << endl;
}

int
main(int argv, char* argc[]) {

#ifdef UNIT_TEST
    TestSuite();
#endif

    nanogui::ref<Viewer> app;

    try {
        nanogui::init();

        {
            app = new Viewer(1600, 900);
            app->drawAll();
            app->setVisible(true);
            nanogui::mainloop();
        }

        nanogui::shutdown();
    } catch (const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        #if defined(_WIN32)
            MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
        #else
            std::cerr << error_msg << endl;
        #endif
            app->CleanUp();
        return -1;
    }
    return 0;
}

