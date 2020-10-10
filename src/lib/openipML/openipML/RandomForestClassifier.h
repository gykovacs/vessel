#ifndef RANDOMFORESTCLASSIFIER_H
#define RANDOMFORESTCLASSIFIER_H

#include <openipML/Classifier.h>
#include <openipML/RandomForestTreeClassifier.h>

namespace openip
{
    template<typename T>
    class RandomForestClassifier: public Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;
        using Classifier<T>::saveModelToFile;
        using Classifier<T>::openModelFromFile;

        /**
         * default constructor
         */
        RandomForestClassifier(float bag= 0.1, int num= 10, float rate= -1);

        /**
         * copy constructor
         * @param c instance to copy
         */
        RandomForestClassifier(const RandomForestClassifier& c);

        /**
         * destructor
         */
        ~RandomForestClassifier();

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

        float bag;
        int num;
        float rate;
        Vector<RandomForestTreeClassifier<T>*> classifiers;
    };

    template<typename T>
    RandomForestClassifier<T>::RandomForestClassifier(float bag, int num, float rate)
    : Classifier<T>()
    {
        this->bag= bag;
        this->num= num;
        this->rate= rate;
    }

    template<typename T>
    RandomForestClassifier<T>::RandomForestClassifier(const RandomForestClassifier& c)
        : Classifier<T>(c), bag(c.bag), num(c.num), rate(c.rate)
    {
    }

    template<typename T>
    RandomForestClassifier<T>::~RandomForestClassifier()
    {
    }

    template<typename T>
    int RandomForestClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
        this->classNames= data.classNames;
        this->preprocessingNames= data.preprocessingNames;
        this->featureNames= data.featureNames;
        probabilities.resize(data.classNames.size());

        printf("delete existing classifiers\n"); fflush(stdout);
        printf("delete finished\n"); fflush(stdout);
        classifiers.clear();
        Vector<unsigned char> m;

        int numberOfVectors;

        if ( mask )
        {
            m.resize(mask->size());
            numberOfVectors= mask->numberOfNonZeroElements();
        }
        else
        {
            m.resize(data.size());
            numberOfVectors= m.size();
            m= 1;
        }

        //int numberOfVectors= mask->numberOfNonZeroElements();
        int sizeOfBags= bag*numberOfVectors;

        for ( int i= 0; i < num; ++i )
        {
            printf("training ensemble %d %d %d\n", i, sizeOfBags, num); fflush(stdout);
            m= 0;
            srand(time(NULL));
            int smp;
            int sizeOfActualBag= 0;
            while ( 1 )
            {
                smp= rand()%m.size();
                if ( (!mask || (*mask)(smp) > 0) && m(smp) == 0 )
                {
                    m(smp)= 1;
                    ++sizeOfActualBag;
                    if ( sizeOfActualBag == sizeOfBags )
                        break;
                }
            }

            classifiers.push_back(new RandomForestTreeClassifier<T>(rate));

            classifiers(classifiers.size()-1)->train(data, &m);
        }

        return 0;
    }

    template<typename T>
    int RandomForestClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        if ( probabilities == NULL )
            probabilities= &(this->probabilities);

        *probabilities= 0;
        for ( unsigned int i= 0; i < classifiers.size(); ++i )
        {
            (*probabilities)(classifiers(i)->classify(c))++;
        }

        (*probabilities)/= classifiers.size();

        int maxIdx= 0;
        for ( unsigned int i= 0; i < probabilities->size(); ++i )
            if ( (*probabilities)(i) > (*probabilities)(maxIdx) )
                maxIdx= i;

        return maxIdx;
    }

    template<typename T>
    int RandomForestClassifier<T>::saveModelToFile(std::ofstream& file)
    {
        file << "RandomForestClassifier" << std::endl;
        this->Classifier<T>::saveModelToFile(file);

        file << classifiers.size() << std::endl;
        for ( unsigned int i= 0; i < classifiers.size(); ++i )
        {
            //classifiers(i)->printTree(classifiers(i)->root, 10);
            classifiers(i)->saveModelToFile(file);
        }

        return 0;
    }

    template<typename T>
    int RandomForestClassifier<T>::openModelFromFile(std::ifstream& file)
    {
        std::string tmp;

        getline(file, tmp);

        if ( tmp.compare("RandomForestClassifier") == 0 )
        {
            printf("opening classifier"); fflush(stdout);
            this->Classifier<T>::openModelFromFile(file);

            int numberOfClassifiers;
            file >> numberOfClassifiers;
            classifiers.resize(numberOfClassifiers);

            for ( int i= 0; i < numberOfClassifiers; ++i )
            {
                printf("======================read model %d====================\n", i); fflush(stdout);
                std::string tmp;

                file >> tmp;

                cout << tmp << endl;

                classifiers(i)= new RandomForestTreeClassifier<T>();

                for ( unsigned int j= 0; j < tmp.length(); ++j )
                    file.unget();

                classifiers(i)->openModelFromFile(file);
                classifiers(i)->printTree(classifiers(i)->root, 10);
            }

            probabilities.resize(this->classNames.size());

            printf("bagged classifier read\n"); fflush(stdout);

            ofstream output;
            output.open("test.rf.model");
            saveModelToFile(output);

            return 0;
        }

        return 1;
    }
}

#endif // RANDOMFORESTCLASSIFIER_H
