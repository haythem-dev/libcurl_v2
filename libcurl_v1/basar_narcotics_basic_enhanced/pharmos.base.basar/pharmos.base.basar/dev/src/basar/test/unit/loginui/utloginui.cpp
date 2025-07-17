// utloginui.cpp : Defines the entry point for the console application.
//

//define QT_WIDGETS_LIB and QT_GUI_LIB in project settings, else gui will not be testable

#include "qtestcollection.h"
#include <iostream>

int main(int argc, char *argv[])
{
    int failedTests = TestCollector::RunAllTests(argc, argv);

    std::cout << "Total number of failed tests: " << failedTests << std::endl;

    return failedTests;
}