#pragma once

//from: https://stackoverflow.com/questions/12194256/qt-how-to-organize-unit-test-with-more-than-one-class
#pragma warning(push)
#pragma warning(disable: 4127 4251 4481 4505 4512 4800)
#include <QtTest/QtTest>
#pragma warning(pop)

#include <memory>
#include <map>
#include <string>

namespace TestCollector
{
    typedef std::map<std::string, std::shared_ptr<QObject> > TestList;

    inline TestList& GetTestList()
    {
        static TestList list;
        return list;
    }

    inline int RunAllTests(int argc, char **argv)
    {
        QApplication app(argc, argv);

        app.setAttribute(Qt::AA_Use96Dpi, true);

        QTEST_DISABLE_KEYPAD_NAVIGATION

        int result = 0;

        for (auto it = GetTestList().cbegin(); it != GetTestList().cend(); ++it)
        {
            result += QTest::qExec(it->second.get(), argc, argv);
        }

        return result;
    }

    template <class T>
    class UnitTestClass
    {
        public:
            UnitTestClass(const std::string& pTestName)
            {
                auto& testList = TestCollector::GetTestList();

                if (0 == testList.count(pTestName))
                {
                    testList.insert(std::make_pair(pTestName, std::make_shared<T>()));
                }
            }
    };
}

#define ADD_TEST(className) static TestCollector::UnitTestClass<className> \
    test(#className);