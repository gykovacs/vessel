#ifndef BINARYMAJORITYCLASSIFIER_H
#define BINARYMAJORITYCLASSIFIER_H

#include <limits.h>

#include <openipML/Classifier.h>

namespace openip
{
    /**
     * KNNClassifier represents the k-Nearest Neighbour classification method.
     */
    template<typename T>
    class BinaryMajorityClassifier: public Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;
        using Classifier<T>::classNames;
        using Classifier<T>::featureNames;
        using Classifier<T>::saveModelToFile;
        using Classifier<T>::openModelFromFile;

        /**
         * constructor
         * @param k number of neighbours
         */
        BinaryMajorityClassifier();

        /**
         * copy constructor
         * @param k instance to copy
         */
        BinaryMajorityClassifier(const BinaryMajorityClassifier& k);

        /**
         * destructor
         */
        ~BinaryMajorityClassifier();

        /**
         * classifier training method
         * @param data training dataset
         * @param mask only those elements are trained, which have a corresponding non-zero element in the mask
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int train(DataSet<T>& data, Vector<unsigned char>* mask= NULL);

        /**
         * classifiaction method
         * @param c vector to classify
         * @param probabilities probability estimates
         * @return predicted class index
         */
        virtual int classify(Vector<T>& c, Vector<float>* probabilities= NULL);

        /**
         * saves the model to output file stream
         * @param file output file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveModelToFile(ofstream& file);

        /**
         * opens the model from input file stream
         * @param file input file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openModelFromFile(ifstream& file);

        /** distances */
        Vector<float> dist;
        /** pointer to training mask */
        Vector<unsigned char>* mask;
    };

    template<typename T>
    BinaryMajorityClassifier<T>::BinaryMajorityClassifier()
    {
    }

    template<typename T>
    BinaryMajorityClassifier<T>::BinaryMajorityClassifier(const BinaryMajorityClassifier& k)
    {
    }

    template<typename T>
    BinaryMajorityClassifier<T>::~BinaryMajorityClassifier()
    {
    }

    template<typename T>
    int BinaryMajorityClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
        this->classNames= data.classNames;
        this->featureNames= data.featureNames;
        this->preprocessingNames= data.preprocessingNames;

        data.init();

        probabilities.resize(2);
        this->mask= mask;

        return 0;
    }

    template<typename T>
    int BinaryMajorityClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        if ( probabilities == NULL )
            probabilities= &(this->probabilities);
        *probabilities= 0;

        unsigned int i;
        for ( i= 0; i < c.size(); ++i )
            if ( c(i) != 0 )
            {
                (*probabilities)(0)= 0;
                (*probabilities)(1)= 1;
                break;
            }
        if ( i == c.size() )
        {
            (*probabilities)(0)= 1;
            (*probabilities)(1)= 0;
        }

        int maxIdx= 0;
        for ( unsigned int i= 0; i < probabilities->size(); ++i )
        {
            if ( (*probabilities)(maxIdx) < (*probabilities)(i) )
                maxIdx= i;
        }

        return maxIdx;
    }

    template<typename T>
    int BinaryMajorityClassifier<T>::saveModelToFile(ofstream& file)
    {
        file << "BinaryMajorityClassifier" << std::endl;

        this->Classifier<T>::saveModelToFile(file);

        return 0;
    }

    template<typename T>
    int BinaryMajorityClassifier<T>::openModelFromFile(ifstream& file)
    {
        std::string tmp;
        FeatureVector<T> tmpfv;

        getline(file, tmp);

        tmpfv.resize(this->featureNames.size());
        if ( tmp.find("BinaryMajorityClassifier") == 0 )
        {
            this->Classifier<T>::openModelFromFile(file);

            probabilities.resize(2);
            this->mask= NULL;

            return 0;
        }

        return 1;
    }
}

#endif // BINARYMAJORITYCLASSIFIER_H
