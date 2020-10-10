/* 
 * File:   AveragedKFoldCrossValidator.h
 * Author: gykovacs
 *
 * Created on October 6, 2011, 9:53 PM
 */

#ifndef AVERAGEDKFOLDCROSSVALIDATOR_H
#define	AVERAGEDKFOLDCROSSVALIDATOR_H

#include <openipML/AveragedKFoldCrossValidator.h>

namespace openip
{
    /**
     * KFoldCrossValidator represents the k-fold cross validation method.
     */
    template<typename T>
    class AveragedKFoldCrossValidator: public Validator<T>
    {
    public:
        using Validator<T>::m;
        using Validator<T>::error;
        using Validator<T>::dumpResults;
        using Validator<T>::measure;
        using Validator<T>::evaluate;

        /**
         * constructor
         * @param k number of folds
         */
        AveragedKFoldCrossValidator(int runs= 4, typename Validator<T>::validator_measure measure= Validator<T>::VALIDATOR_ACC);

        /**
         * copy constructor
         * @param r instance to copy
         */
        AveragedKFoldCrossValidator(const AveragedKFoldCrossValidator& r);

        /**
         * destructor
         */
        ~AveragedKFoldCrossValidator();

        /**
         * do the validation
         * @param c classifier to validate
         * @param d dataset to validate with
         * @return accuracy of classification
         */
        virtual float validate(Classifier<T>* c, DataSet<T>& d);
        
        KFoldCrossValidator<T>* kfcv;

        /** number of folds */
        int runs;
    };

    template<typename T>
    AveragedKFoldCrossValidator<T>::AveragedKFoldCrossValidator(int runs, typename Validator<T>::validator_measure measure)
    : Validator<T>(measure)
    {
        this->runs= runs;
        kfcv= new KFoldCrossValidator<T>(10, measure);
    }

    template<typename T>
    AveragedKFoldCrossValidator<T>::AveragedKFoldCrossValidator(const AveragedKFoldCrossValidator& r)
    : Validator<T>(r)
    {
        this->runs= r.runs;
    }

    template<typename T>
    AveragedKFoldCrossValidator<T>::~AveragedKFoldCrossValidator()
    {
        delete kfcv;
    }

    template<typename T>
    float AveragedKFoldCrossValidator<T>::validate(Classifier<T>* c, DataSet<T>& data)
    {
        data.init();
        m.resize(data.numberOfClasses, data.numberOfClasses);
        error= 0;
        
        for ( int i= 0; i < runs; ++i )
        {
            tprintf("run %d\n", i);
            kfcv->validate(c, data);
            for ( int j= 0; j < kfcv->m.rows; ++j )
                for ( int k= 0; k < kfcv->m.columns; ++k )
                {
                    m(j,k)+= kfcv->m(j,k);
                    error+= kfcv->error;
                }
        }
        for ( int j= 0; j < kfcv->m.rows; ++j )
            for ( int k= 0; k < kfcv->m.columns; ++k )
                m(j,k)/= runs;
        error/= runs;

        return evaluate();
    }
        
}

#endif	/* AVERAGEDKFOLDCROSSVALIDATOR_H */

