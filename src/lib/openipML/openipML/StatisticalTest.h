#ifndef STATISTICALTEST_H
#define STATISTICALTEST_H

namespace openip
{
    template<typename T>
    class StatisticalTest
    {
    public:
        StatisticalTest(double ssig= 0.95);

        StatisticalTest(const StatisticalTest& st);

        ~StatisticalTest();

        double significance;
    };

    template<typename T>
    StatisticalTest<T>::StatisticalTest(double ssig)
    {
        this->significance= ssig;
    }

    template<typename T>
    StatisticalTest<T>::StatisticalTest(const StatisticalTest& st)
    {
        this->significance= st.significance;
    }

    template<typename T>
    StatisticalTest<T>::~StatisticalTest()
    {
    }
}

#endif // STATISTICALTEST_H
