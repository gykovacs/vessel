#ifndef VALIDATORIO_H
#define VALIDATORIO_H

#include <openipML/KFoldCrossValidator.h>
#include <openipML/LeaveOneOutCrossValidator.h>
#include <openipML/RandomSubSamplingValidator.h>

namespace openip
{
    template<typename T>
    class ValidatorIO
    {
    public:
        ValidatorIO();

        ValidatorIO(const ValidatorIO& v);

        ~ValidatorIO();

        typename Validator<float>::validator_measure targetFromString(string target);

        Validator<T>* createValidatorFromString(std::string& str);
    };

    template<typename T>
    ValidatorIO<T>::ValidatorIO()
    {
    }

    template<typename T>
    ValidatorIO<T>::ValidatorIO(const ValidatorIO& v)
    {
    }

    template<typename T>
    ValidatorIO<T>::~ValidatorIO()
    {
    }

    template<typename T>
    typename Validator<float>::validator_measure ValidatorIO<T>::targetFromString(string target)
    {
        typename Validator<float>::validator_measure vm= Validator<float>::VALIDATOR_ACC;
        if ( target.find("acc") == 0 )
            vm= Validator<float>::VALIDATOR_ACC;
        else if ( target.find("f1") == 0 )
            vm= Validator<float>::VALIDATOR_F1;
        else if ( target.find("prec") == 0 )
            vm= Validator<float>::VALIDATOR_PREC;
        else if ( target.find("spec") == 0 )
            vm= Validator<float>::VALIDATOR_SPEC;
        else if ( target.find("sens") == 0 )
            vm= Validator<float>::VALIDATOR_SENS;
        else if ( target.find("ppv") == 0 )
            vm= Validator<float>::VALIDATOR_PPV;
        else if ( target.find("npv") == 0 )
            vm= Validator<float>::VALIDATOR_NPV;
	else if ( target.find("mse") == 0 )
	    vm= Validator<float>::VALIDATOR_MSE;
        return vm;
    }

    template<typename T>
    Validator<T>* ValidatorIO<T>::createValidatorFromString(std::string& str)
    {
        stringstream ss;
        ss.str(str);
        string name;
        ss >> name;
        if ( name.find("KFoldCrossValidator") == 0 )
        {
            int k;
            ss >> k;
            string target;
            ss >> target;

            return new KFoldCrossValidator<T>(k, targetFromString(target));
        }
        else if ( name.find("LeaveOneOutCrossValidator") == 0 )
        {
            string target;
            ss >> target;

            return new LeaveOneOutCrossValidator<T>(targetFromString(target));
        }
        else if ( name.find("RandomSubSamplingValidator") == 0 )
        {
            int n;
            float ratio;
            ss >> n;
            ss >> ratio;

            string target;
            ss >> target;

            return new RandomSubSamplingValidator<T>(n, ratio, targetFromString(target));
        }
        return NULL;
    }
}


#endif // VALIDATORIO_H
