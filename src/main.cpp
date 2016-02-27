#include <iostream>

#include "main.hpp"
#include "viewer.h"

using namespace std;

int main()
{
    Viewer view(800, 600);
    view.Update();
    view.CleanUp();

    return 0;
}

