#ifndef SEQUENTIALSVM_H
#define SEQUENTIALSVM_H

#include <openipML/Classifier.h>
#include <openipML/Normalizer.h>
#include <openipML/KernelFunctionIO.h>

using namespace svm;

namespace openip
{
    /**
     * SVMClassifier represents the Support Vector Machine classification method, based on libsvm.
     */
    template<typename T>
    class SequentialTwoClassSVMClassifier: public virtual Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;

        /**
         * default constructor
         */
        SequentialTwoClassSVMClassifier(float C, float eps, float tol, KernelFunction<T>* kernel= NULL);

        /**
         * copy constructor
         * @param s instance to copy
         */
        SequentialTwoClassSVMClassifier(const SequentialTwoClassSVMClassifier& s);

        /**
         * destructor
         */
        ~SequentialTwoClassSVMClassifier();

        /**
         * classifier training method
         * @param data training dataset
         * @param mask only those elements are trained from the dataset, which have a corresponding non-zero element in the mask
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int train(DataSet<T>& data, Vector<unsigned char>* mask= NULL);

        /**
         * classification method
         * @param c vector to classify
         * @param probabilities probability estimation
         * @return predicted class index
         */
        virtual int classify(Vector<T>& c, Vector<float>* probabilities= NULL);

        virtual T evaluate(Vector<T>& c);

        virtual T b();

        virtual inline int SMO(int i, int j);

        virtual inline int check(int i);

        virtual T objective();

        /**
         * saves the model to file
         * @param filename output file
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveModelToFile(const char* filename);

        /**
         * opens the model from file
         * @param filename input file
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openModelFromFile(const char* filename);

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

        /** probabilities */
        Vector<double> probabilitiesD;

        Normalizer<T>* normalizer;
        KernelFunction<T> *kernel;
        Vector<T> alpha;
        DataSet<T> data;
        Vector<unsigned int> supports;
        Vector<T> w;
        Vector<float> errors;
        Vector<float> kaa;

        bool bInit;
        T bValue;
        T eps;
        T tol;
        T C;
    };

    template<typename T>
    class SequentialSVMClassifier: public virtual Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;

        /**
         * default constructor
         */
        SequentialSVMClassifier(float C, float eps= 0.1, float tol= 0.1, KernelFunction<T>* kernel= NULL);

        /**
         * copy constructor
         * @param s instance to copy
         */
        SequentialSVMClassifier(const SequentialSVMClassifier& s);

        /**
         * destructor
         */
        ~SequentialSVMClassifier();

        /**
         * classifier training method
         * @param data training dataset
         * @param mask only those elements are trained from the dataset, which have a corresponding non-zero element in the mask
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int train(DataSet<T>& data, Vector<unsigned char>* mask= NULL);

        /**
         * classification method
         * @param c vector to classify
         * @param probabilities probability estimation
         * @return predicted class index
         */
        virtual int classify(Vector<T>& c, Vector<float>* probabilities= NULL);

        /**
         * saves the model to file
         * @param filename output file
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveModelToFile(const char* filename);

        /**
         * opens the model from file
         * @param filename input file
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openModelFromFile(const char* filename);

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

        /** probabilities */
        Vector<double> probabilitiesD;
        KernelFunction<T> *kernel;

        Vector<SequentialTwoClassSVMClassifier<T>* > classifiers;

        bool bInit;
        float bValue;
        float eps;
        float tol;
        float C;
        Vector<int> classa;
        Vector<int> classb;
        Vector<int> results;
        int numberOfClasses;
    };

    template<typename T>
    SequentialTwoClassSVMClassifier<T>::SequentialTwoClassSVMClassifier(float C, float eps, float tol, KernelFunction<T>* kernel)
        : Classifier<T>()
    {
       KernelFunctionIO<T> kfio;
       this->kernel= kfio.createKernelFunctionFromString(kernel->descriptor);
       this->C= C;
       bInit= false;
       bValue= 0;
       this->eps= eps;
       this->tol= tol;
    }

    template<typename T>
    SequentialTwoClassSVMClassifier<T>::SequentialTwoClassSVMClassifier(const SequentialTwoClassSVMClassifier& s)
        : Classifier<T>(s)
    {
        KernelFunctionIO<T> kfio;
        this->kernel= kfio.createKernelFunctionFromString(s.kernel.descriptor);
        this->C= s.C;
        this->eps= s.eps;
        this->tol= s.tol;
        this->kaa= s.kaa;
    }

    template<typename T>
    SequentialTwoClassSVMClassifier<T>::~SequentialTwoClassSVMClassifier()
    {
        delete kernel;
    }

    template<typename T>
    int SequentialTwoClassSVMClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
        this->classNames= this->data.classNames= data.classNames;
        this->featureNames= this->data.featureNames= data.featureNames;
        this->preprocessingNames= this->data.preprocessingNames= data.preprocessingNames;

        for ( unsigned int i= 0; i < data.size(); ++i )
            if ( !mask || (*mask)(i) )
            {
                this->data.push_back(data(i));
            }

        this->data.init();
        kaa.resize(this->data.size());
        for ( unsigned int i= 0; i < this->data.size(); ++i )
            kaa(i)= kernel->evaluate(this->data.operator()(i), this->data.operator()(i));

        vector<int> labels;

        set<int>::iterator it= this->data.classLabels.begin();
        while ( it != this->data.classLabels.end() )
        {
            labels.push_back(*it);
            ++it;
        }

        sort(labels.begin(), labels.end());

        for ( unsigned int i= 0; i < this->data.size(); ++i )
        {
            if ( this->data(i).classIndex == labels[0] )
                this->data(i).classIndex= -1;
            else
                this->data(i).classIndex= 1;
        }

        alpha.resize(this->data.size());
        alpha= 0;

        errors.resize(this->data.size());
        for ( unsigned int i= 0; i < errors.size(); ++i )
            errors(i)= evaluate(data(i)) - data(i).classIndex;

        int numChanged= 0;
        int examineAll= 1;

        while ( numChanged > 0 || examineAll )
        {
            numChanged= 0;
            if ( examineAll )
                for ( unsigned int i= 0; i < this->data.size(); ++i )
                    numChanged+= check(i);
            else
                for ( unsigned int i= 0; i < this->data.size(); ++i )
                    if ( alpha(i) != 0 && alpha(i) != C )
                        numChanged+= check(i);

            if ( examineAll == 1 )
                examineAll= 0;
            else if ( numChanged == 0 )
                examineAll= 1;
        }

        return 0;
    }

    template<typename T>
    int SequentialTwoClassSVMClassifier<T>::SMO(int i, int j)
    {
        if ( i == j )
            return 0;

        T alpha1= alpha(i);
        T alpha2= alpha(j);
        int y1= data(i).classIndex, y2= data(j).classIndex;
        T E1= errors(i);
        T E2= errors(j);
        T s= y1*y2;

        T L, H;
        if ( y1 != y2 )
        {
          L= 0 > (alpha2 - alpha1) ? 0 : (alpha2 - alpha1);
          H= (C < C + alpha2 - alpha1) ? C : (C + alpha2 - alpha1);
        }
        else
        {
            L= (0 > (alpha2 + alpha1 - C)) ? 0 : (alpha2 + alpha1 - C);
            H= (C < (alpha2 + alpha1)) ? C : (alpha2 + alpha1);
        }

        if ( L == H )
            return 0;

        T k11= kaa(i);
        T k12= kernel->evaluate(data(i), data(j));
        T k22= kaa(j);

        T eta= k11 + k22 - 2*k12;

        T a1, a2, LObj, HObj;

        if ( eta > 0 )
        {
            a2= alpha2 + y2*(E1-E2)/eta;
            if ( a2 < L )
                a2= L;
            else if ( a2 > H )
                a2= H;
        }
        else
        {
            alpha(j)= L;
            LObj= objective();
            alpha(j)= H;
            HObj= objective();
            alpha(j)= alpha2;

            if ( LObj < HObj - eps )
                a2= L;
            else if ( LObj > HObj + eps )
                a2= H;
            else
                a2= alpha2;
        }

        if ( fabs(a2 - alpha2) < eps * (a2 + alpha2 + eps) )
            return 0;

        a1= alpha1 + s*(alpha2 - a2);

        T b1= E1 + y1*(a1 - alpha(i))*k11 + y2*(a2 - alpha2)*k12 + b();
        T b2= E2 + y1*(a1 - alpha(i))*k12 + y2*(a2 - alpha2)*k22 + b();

        if ( a1 > 0 && a1 < C )
        {
            if ( a2 > 0 && a2 < C )
                bValue= (b1 + b2)/2.0;
            else
                bValue= b1;
        }
        else
            bValue= b2;

        alpha(i)= a1;
        alpha(j)= a2;

        for ( unsigned int i= 0; i < data.size(); ++i )
            errors(i)= evaluate(data(i)) - data(i).classIndex;

        return 1;
    }

    template<typename T>
    int SequentialTwoClassSVMClassifier<T>::check(int i)
    {
        int y2= data(i).classIndex;
        T alpha2= alpha(i);
        T E2= errors(i);
        T r2= E2*y2;

        Vector<int> cands;

        if ((r2 < -tol && alpha2 < C) || (r2 > tol && alpha2 > 0 ))
        {
            for ( unsigned int j= 0; j < alpha.size(); ++j )
                if ( alpha(j) != 0 && alpha(j) != C )
                    cands.push_back(j);
                    //++a;

            if ( cands.size() > 1 )
            {
                int extr= 0;
                if ( E2 < 0 )
                    for ( unsigned int j= 0; j < cands.size(); ++j )
                        if ( errors(extr) < errors(cands(j)) )
                            extr= cands(j);
                if ( E2 >= 0 )
                    for ( unsigned int j= 0; j < cands.size(); ++j )
                        if ( errors(extr) > errors(cands(j)) )
                            extr= cands(j);

                if ( SMO(extr, i) )
                    return 1;
            }

            for ( unsigned int j= 0; j < alpha.size(); ++j )
                if ( alpha(j) != 0 && alpha(j) != C )
                    if ( SMO(j,i) )
                        return 1;
            for ( unsigned int j= 0; j < alpha.size(); ++j )
                if ( SMO(j,i) )
                    return 1;
        }

        return 0;
    }

    template<typename T>
    T SequentialTwoClassSVMClassifier<T>::objective()
    {
        T obj= 0;
        for ( unsigned int i= 0; i < data.size(); ++i )
            for ( unsigned int j= 0; j < data.size(); ++j )
                obj+= data(i).classIndex*data(j).classIndex*kernel->evaluate(data(i), data(j))*alpha(i)*alpha(j);
        for ( unsigned int i= 0; i < alpha.size(); ++i )
            obj-= alpha(i);
        return obj;
    }

    template<typename T>
    T SequentialTwoClassSVMClassifier<T>::evaluate(Vector<T>& c)
    {
        double res= 0;
        for ( unsigned int i= 0; i < data.size(); ++i )
            res+= data(i).classIndex*alpha(i)*kernel->evaluate(data(i), c);
        return res - b();
    }

    template<typename T>
    T SequentialTwoClassSVMClassifier<T>::b()
    {
        return bValue;
    }

    template<typename T>
    int SequentialTwoClassSVMClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        if ( probabilities == NULL )
            probabilities= &(this->probabilities);

        T tmp;

        tmp= evaluate(c);

        if ( fabs(tmp - (-1) ) < fabs(tmp - 1) )
            return -1;
        else
            return 1;
    }

    template<typename T>
    int SequentialTwoClassSVMClassifier<T>::saveModelToFile(const char* /*filename*/)
    {
        /*strcpy(this->filename, filename);
        std::ofstream file;
        file.open(filename);
        saveModelToFile(file);*/
        return 0;
    }

    template<typename T>
    int SequentialTwoClassSVMClassifier<T>::openModelFromFile(const char* filename)
    {
        std::ifstream file;
        file.open(filename);
        openModelFromFile(file);

        return 0;
    }

    template<typename T>
    int SequentialTwoClassSVMClassifier<T>::saveModelToFile(ofstream& /*file*/)
    {
        /*file << "SequentialSVMClassifier" << std::endl;
        this->Classifier<T>::saveModelToFile(file);
        KernelFunctionIO<double> kfio;
        kfio.saveKernelFunctionToFile(kernel, file);*/
;

        return 0;
    }

    template<typename T>
    int SequentialTwoClassSVMClassifier<T>::openModelFromFile(ifstream& /*file*/)
    {
        /*if ( tmp.compare("SequentialSVMClassifier") == 0 )
        {
            model= svm_load_model("_svm_tmp_model.txt_b");
            KernelFunctionIO<double> kfio;
            this->Classifier<T>::openModelFromFile(file);
            this->kernel= kfio.openKernelFunctionFromFile(file);

            return 1;
        }*/


        return 0;
    }


    template<typename T>
    SequentialSVMClassifier<T>::SequentialSVMClassifier(float C, float eps, float tol, KernelFunction<T>* kernel)
    {
        this->C= C;
        this->eps= eps;
        this->tol= tol;
        this->kernel= kernel;
    }

    template<typename T>
    SequentialSVMClassifier<T>::SequentialSVMClassifier(const SequentialSVMClassifier& s)
    {
        this->C= s.C;
        this->eps= s.eps;
        this->tol= s.tol;
        this->kernel= s.kernel;
    }

    template<typename T>
    SequentialSVMClassifier<T>::~SequentialSVMClassifier()
    {

    }

    template<typename T>
    int SequentialSVMClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
        classifiers.clear();
        data.init();
        numberOfClasses= data.numberOfClasses;
        classa.clear();
        classb.clear();

        vector<int> labels;

        set<int>::iterator it= data.classLabels.begin();
        while ( it != data.classLabels.end() )
        {
            labels.push_back(*it);
            ++it;
        }

        sort(labels.begin(), labels.end());

        for ( int i= 0; i < data.numberOfClasses; ++i )
            for ( int j= 0; j < i; ++j )
            {
                SequentialTwoClassSVMClassifier<float>* svmtc= new SequentialTwoClassSVMClassifier<float>(C, eps, tol, kernel);
                classifiers.push_back(svmtc);

                classa.push_back(labels[j]);
                classb.push_back(labels[i]);
            }

        #pragma omp parallel for
        for ( unsigned int i= 0; i < classifiers.size(); ++i )
        {
            Vector<unsigned char> maskTmp;
            maskTmp.resize(data.size());

            for ( unsigned int k= 0; k < maskTmp.size(); ++k )
            {
                if ( (data(k).classIndex == classa[i] || data(k).classIndex == classb[i]) && (!mask || (*mask)(k)) )
                    maskTmp(k)= 1;
                else
                    maskTmp(k)= 0;
            }

            //tprintf("training classifier for labels: %d %d\n", classa[i], classb[i]);
            classifiers[i]->train(data, &maskTmp);
            //tprintf("trining of classifier %d %d finished\n", classa[i], classb[i]);
        }
        
        return 0;
    }

    template<typename T>
    int SequentialSVMClassifier<T>::classify(Vector<T>& c, Vector<float>* /*probabilities*/)
    {
        results.resize(numberOfClasses);
        results= 0;

        int d;
        for ( unsigned int i= 0; i < classifiers.size(); ++i )
        {
            d= classifiers(i)->classify(c);
            results[ d == -1 ? classa[i] : classb[i] ]++;
            /*if ( d == -1 )
                results[classa[i]]++;
            else
                results[classb[i]]++;*/
        }

        int maxLabel= 0;
        for ( int i= 0; i < numberOfClasses; ++i )
            if ( results(maxLabel) < results(i) )
                maxLabel= i;

        return maxLabel;
    }

    template<typename T>
    int SequentialSVMClassifier<T>::saveModelToFile(const char* /*filename*/)
    {
        return 0;
    }

    template<typename T>
    int SequentialSVMClassifier<T>::openModelFromFile(const char* /*filename*/)
    {
        return 0;
    }

    template<typename T>
    int SequentialSVMClassifier<T>::saveModelToFile(ofstream& /*file*/)
    {
        return 0;
    }

    template<typename T>
    int SequentialSVMClassifier<T>::openModelFromFile(ifstream& /*file*/)
    {
        return 0;
    }
}

#endif // SEQUENTIALSVM_H
