#ifndef ADABOOSTCLASSIFIER_H
#define ADABOOSTCLASSIFIER_H
#include <openipML/Classifier.h>
#include <openipML/GeneralizedRandomForestTreeClassifier.h>
#include <openipML/NeuralNetworkClassifier.h>

namespace openip
{
    template<typename T>
    class AdaBoostClassifier: public Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;
        using Classifier<T>::saveModelToFile;
        using Classifier<T>::openModelFromFile;

        /**
         * default constructor
         */
        AdaBoostClassifier(float bag= 0.1, int num= 10, Classifier<T>* sc= new DiscreteNaiveBayesClassifier<T>());

        /**
         * copy constructor
         * @param c instance to copy
         */
        AdaBoostClassifier(const AdaBoostClassifier& c);

        /**
         * destructor
         */
        ~AdaBoostClassifier();

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
        Vector<Classifier<T>*> classifiers;
        Classifier<T>* sampleClassifier;
    };

    template<typename T>
    AdaBoostClassifier<T>::AdaBoostClassifier(float bag, int num, Classifier<T>* sc)
    : Classifier<T>()
    {
        this->bag= bag;
        this->num= num;
        this->sampleClassifier= sc;
    }

    template<typename T>
    AdaBoostClassifier<T>::AdaBoostClassifier(const AdaBoostClassifier& c)
        : Classifier<T>(c), bag(c.bag), num(c.num)
    {
    }

    template<typename T>
    AdaBoostClassifier<T>::~AdaBoostClassifier()
    {
    }

    template<typename T>
    int AdaBoostClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
//        printf("training bagged classifier %d\n", mask->numberOfNonZeroElements()); fflush(stdout);
        this->classNames= data.classNames;
        this->preprocessingNames= data.preprocessingNames;
        this->featureNames= data.featureNames;
        probabilities.resize(data.classNames.size());

        printf("delete existing classifiers\n"); fflush(stdout);
        /*for ( int i= 0; i < classifiers.size(); ++i )
            delete classifiers[i];*/
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

            if ( dynamic_cast<DiscreteNaiveBayesClassifier<T>*>(sampleClassifier) != 0 )
            {
                classifiers.push_back(new DiscreteNaiveBayesClassifier<T>(*dynamic_cast<DiscreteNaiveBayesClassifier<T>*>(sampleClassifier)));
            }
            else if ( dynamic_cast<AODEClassifier<T>*>(sampleClassifier) != 0 )
            {
                classifiers.push_back(new AODEClassifier<T>(*dynamic_cast<AODEClassifier<T>*>(sampleClassifier)));
            }
            else if ( dynamic_cast<ContinuousNaiveBayesClassifier<T>*>(sampleClassifier) != 0 )
            {
                classifiers.push_back(new ContinuousNaiveBayesClassifier<T>(*dynamic_cast<ContinuousNaiveBayesClassifier<T>*>(sampleClassifier)));
            }
            else if ( dynamic_cast<DecisionTreeClassifier<T>*>(sampleClassifier) != 0 )
            {
                classifiers.push_back(new DecisionTreeClassifier<T>(*dynamic_cast<DecisionTreeClassifier<T>*>(sampleClassifier)));
            }
            else if ( dynamic_cast<EpsilonSVMClassifier<T>*>(sampleClassifier) != 0 )
            {
                classifiers.push_back(new EpsilonSVMClassifier<T>(*dynamic_cast<EpsilonSVMClassifier<T>*>(sampleClassifier)));
            }
            else if ( dynamic_cast<KernelKNNClassifier<T>*>(sampleClassifier) != 0 )
            {
                classifiers.push_back(new KernelKNNClassifier<T>(*dynamic_cast<KernelKNNClassifier<T>*>(sampleClassifier)));
            }
            else if ( dynamic_cast<GeneralizedRandomForestTreeClassifier<T>*>(sampleClassifier) != 0 )
            {
                classifiers.push_back(new GeneralizedRandomForestTreeClassifier<T>(*dynamic_cast<GeneralizedRandomForestTreeClassifier<T>*>(sampleClassifier)));
            }
            else if ( dynamic_cast<NeuralNetworkClassifier<T>* >(sampleClassifier) != 0 )
            {
                classifiers.push_back(new NeuralNetworkClassifier<T>(*dynamic_cast<NeuralNetworkClassifier<T>*>(sampleClassifier)));
            }
            else
            {
                printf("dynamic cast not succeeded in AdaBoostClassifier\n"); fflush(stdout); return 0;
            }

            classifiers(classifiers.size()-1)->train(data, &m);
        }

        return 0;
    }

    template<typename T>
    int AdaBoostClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
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
    int AdaBoostClassifier<T>::saveModelToFile(std::ofstream& file)
    {
        file << "AdaBoostClassifier" << std::endl;
        this->Classifier<T>::saveModelToFile(file);

        file << classifiers.size() << std::endl;
        for ( unsigned int i= 0; i < classifiers.size(); ++i )
            classifiers(i)->saveModelToFile(file);

        return 0;
    }

    template<typename T>
    int AdaBoostClassifier<T>::openModelFromFile(std::ifstream& file)
    {
        std::string tmp;

        getline(file, tmp);

        if ( tmp.compare("AdaBoostClassifier") == 0 )
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

                if ( tmp.find("KNNClassifier") == 0  )
                    classifiers(i)= new KNNClassifier<T>(1);
                else if ( tmp.find("EpsilonSVMClassifier") == 0 )
                    classifiers(i)= new EpsilonSVMClassifier<T>();
                else if ( tmp.find("NuSVMClassifier") == 0 )
                    classifiers(i)= new NuSVMClassifier<T>();
                else if ( tmp.find("DiscreteNaiveBayesClassifier") == 0 )
                    classifiers(i)= new DiscreteNaiveBayesClassifier<T>();
                else if ( tmp.find("ContinuousNaiveBayesClassifier") == 0 )
                    classifiers(i)= new ContinuousNaiveBayesClassifier<T>();
                else if ( tmp.find("AODEClassifier") == 0 )
                    classifiers(i)= new AODEClassifier<T>();
                else if ( tmp.find("DiscreteMultivariateBayesClassifier") == 0 )
                    classifiers(i)= new DiscreteMultivariateBayesClassifier<T>();
                else if ( tmp.find("KernelKNNClassifier") == 0 )
                    classifiers(i)= new KernelKNNClassifier<T>(1);
                else if ( tmp.find("GeneralizedRandomForestTreeClassifier") == 0 )
                    classifiers(i)= new GeneralizedRandomForestTreeClassifier<T>();
                else if ( tmp.find("DecisionTreeClassifier") == 0 )
                    classifiers(i)= new DecisionTreeClassifier<T>();
                else if ( tmp.find("BaggedClassifier") == 0 )
                    classifiers(i)= new BaggedClassifier<T>();

                for ( unsigned int j= 0; j < tmp.length(); ++j )
                    file.unget();

                classifiers(i)->openModelFromFile(file);
            }

            probabilities.resize(this->classNames.size());

            printf("boosted classifier read\n"); fflush(stdout);

            return 0;
        }

        return 1;
    }
}
#endif // ADABOOSTCLASSIFIER_H
