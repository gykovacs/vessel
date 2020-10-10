#ifndef GMMCLASSIFIER_H
#define GMMCLASSIFIER_H

#include "Classifier.h"


#include <iostream>
#include <fstream>

#include <openipDS/Vector.h>
#include <openipDS/DataSet.h>

#include <openipML/Classifier.h>
//#include <openipML/GaussianDistributionVector.h>
#include <openipML/JointDistribution.h>
#include <openipML/GaussianDistribution.h>
#include <openipML/GMMEM.h>
//#include <openipML/UnivariateGaussianDistributionSet.h>

namespace openip
{

    template<typename T>
    class GMMClassifier: public Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;
        using Classifier<T>::saveModelToFile;
        using Classifier<T>::openModelFromFile;

        /**
         * default constructor
         */
        GMMClassifier(int nfg, int nbg);

        /**
         * copy constructor
         * @param c instance to copy
         */
        GMMClassifier(const GMMClassifier& c);

        /**
         * destructor
         */
        ~GMMClassifier();

        /**
         * training method of the classifier
         * @param data training dataset
         * @param mask only those elements of the dataset are trained, which have a corresponding non-zero element in the mask
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int train(DataSet<T>& data, Vector<unsigned char>* mask= NULL);

        /**
         * classifies the parameter vector
         * @param c vector to classify
         * @param probabilities probability estimates
         * @return predicted class index of the vector
         */
        virtual int classify(Vector<T>& c, Vector<float>* probabilities= NULL);

        /**
         * saves the model to filestream
         * @param file output filestream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveModelToFile(std::ofstream& file);

        /**
         * opens the model from filestream
         * @param file input filestream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openModelFromFile(std::ifstream& file);

        /** model of the classifier */
        Vector<MultivariateGaussianDistribution<T> > foreground;
        Vector<MultivariateGaussianDistribution<T> > background;
        Vector<double> wfg;
        Vector<double> wbg;
        int nfg;
        int nbg;
    };

    template<typename T>
    GMMClassifier<T>::GMMClassifier(int nfg, int nbg)
    : Classifier<T>()
    {
        this->nfg= nfg;
        this->nbg= nbg;
        foreground.resize(nfg);
        wfg.resize(nfg);
        background.resize(nbg);
        wbg.resize(nbg);
    }

    template<typename T>
    GMMClassifier<T>::GMMClassifier(const GMMClassifier& c)
    : Classifier<T>(c)
    {
        this->nfg= c.nfg;
        this->nbg= c.nbg;
    }

    template<typename T>
    GMMClassifier<T>::~GMMClassifier()
    {
    }

    template<typename T>
    int GMMClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* /*mask*/)
    {
        this->classNames= data.classNames;
        this->featureNames= data.featureNames;
        this->preprocessingNames= data.preprocessingNames;

        data.init();
        probabilities.resize(data.numberOfClasses);

        wfg.resize(nfg);
        wbg.resize(nbg);

        foreground.resize(nfg);
        background.resize(nbg);

        GMMEM<T> gmmem;

        Vector<Vector<T> > dataset;
        for ( unsigned int i= 0; i < data.size(); ++i )
            if ( data(i).classIndex == 0 )
                dataset.push_back(data(i));

        tprintf("init gmmmem %d %d\n", nfg, nbg);
        gmmem.init(&dataset, &background, &wbg);
        tprintf("gmmem init finished\n");

        tprintf("starting gmmem\n");
        gmmem.doEM();
        tprintf("gmmem finished\n");

        for ( unsigned int i= 0; i < data.size(); ++i )
            if ( data(i).classIndex != 0 )
                dataset.push_back(data(i));

        tprintf("init gmmem\n");
        gmmem.init(&dataset, &foreground, &wfg);
        tprintf("gmmem init finished\n");

        tprintf("starting gmmem\n");
        gmmem.doEM();
        tprintf("gmmem finished\n");

        for ( unsigned int i= 0; i < foreground.size(); ++i )
        {
            cout << foreground(i).mean << endl;
            cout << foreground(i).covariance << endl;
        }
        cout << wfg << endl;

        for ( unsigned int i= 0; i < background.size(); ++i )
        {
            cout << background(i).mean << endl;
            cout << background(i).covariance << endl;
        }
        cout << wbg << endl;

        return 0;
    }

    template<typename T>
    int GMMClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        cout << "."; fflush(stdout);
        float sum= 0;
        if ( probabilities == NULL )
            probabilities= &(this->probabilities);

        double probfg= 0, probbg= 0;

        for ( unsigned int i= 0; i < foreground.size(); ++i )
            probfg+= wfg(i)*foreground(i).probability(c);

        for ( unsigned int i= 0; i < background.size(); ++i )
            probbg+= wbg(i)*background(i).probability(c);

        (*probabilities)(0)= probbg;
        (*probabilities)(1)= probfg;

        for ( unsigned int i= 0; i < probabilities->size(); ++i )
            sum+= (*probabilities)(i);

        //printf("%f %f %f; ", c(0), probabilities->operator()(0), probabilities->operator()(1)); fflush(stdout);

        for ( unsigned int i= 0; i < probabilities->size(); ++i )
        {
            (*probabilities)(i)/= sum;
        }

        int maxIdx= 0;
        for ( unsigned int i= 1; i < probabilities->size(); ++i )
            if ( (*probabilities)(i) > (*probabilities)(maxIdx) )
                maxIdx= i;
        return maxIdx;
    }

    template<typename T>
    int GMMClassifier<T>::saveModelToFile(std::ofstream& file)
    {
        file << "GMMClassifier" << std::endl;
        this->Classifier<T>::saveModelToFile(file);

        /*file << model.size() << std::endl;
        printf("model size: %d\n", (int)(model.size())); fflush(stdout);
        for ( unsigned int i= 0; i < model.size(); ++i )
        {
            //printf("save model to file\n"); fflush(stdout);
            model(i)->saveDistributionToFile(file);
        }*/

        return 0;
    }

    template<typename T>
    int GMMClassifier<T>::openModelFromFile(std::ifstream& /*file*/)
    {
        std::string tmp;

        /*getline(file, tmp);

        if ( tmp.compare("GMMClassifier") == 0 )
        {
            this->Classifier<T>::openModelFromFile(file);
            int k;
            file >> k;



            for ( int i= 0; i < k; ++i )
            {
                UnivariateGaussianJointDistribution<T>* ugjd= new UnivariateGaussianJointDistribution<T>();
                ugjd->openDistributionFromFile(file);
                model.push_back(ugjd);
            }

            probabilities.resize(model.size());

            return 0;
        }*/

        return 1;
    }
}

#endif // GMMCLASSIFIER_H
