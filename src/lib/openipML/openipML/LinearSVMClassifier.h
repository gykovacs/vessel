#ifndef LINEARSVMCLASSIFIER_H
#define LINEARSVMCLASSIFIER_H

#include <openipML/Classifier.h>
#include <openipML/liblinear/linear.h>
#include <openipML/Normalizer.h>

namespace openip
{
    template<typename T>
    class LinearSVMClassifier: public virtual Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;

        /**
         * default constructor
         */
        LinearSVMClassifier(int kernelType= RBF, int degree= 3, double gamma= 1.0, double C= 1.0, double coef0= 1.0, double cacheSize= 100, double eps= 0.01, bool norm= true, KernelFunction<double>* kernel= NULL);

        /**
         * copy constructor
         * @param s instance to copy
         */
        LinearSVMClassifier(const LinearSVMClassifier& s);

        /**
         * destructor
         */
        ~LinearSVMClassifier();

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

        virtual void destroyProblem();

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

        /** svm problem */
        svm_problem* problem;
        /** svm model */
        svm_model* model;
        /** svm parameters */
        svm_parameter* parameter;
        /** svm nodes for input data */
        svm_node* node;

        /** probabilities */
        Vector<double> probabilitiesD;
        /** temporary filename */
        char filename[100];

        Normalizer<T>* normalizer;
    };

    template<typename T>
    LinearSVMClassifier<T>::LinearSVMClassifier(int kernelType, int degree, double gamma, double C, double coef0, double cacheSize, double eps, bool norm, KernelFunction<double>* kernel)
        : Classifier<T>()
    {
        parameter= new svm_parameter();
        parameter->svm_type= C_SVC;
        parameter->kernel_type= kernelType;
        parameter->degree= degree;
        parameter->gamma= gamma;
        parameter->coef0= coef0;
        parameter->cache_size= cacheSize;
        parameter->eps= eps;
        parameter->C= C;
        parameter->nr_weight= 0;
        parameter->weight_label= 0;
        parameter->weight= 0;
        //parameter.nu= ;
        //parameter->p= eps;
        parameter->shrinking= 0;
        parameter->probability= 1;
        problem= NULL;
        model= NULL;
        if ( norm )
            normalizer= new Normalizer<T>(0, 1);
        else
            normalizer= NULL;
        this->kernel= kernel;
    }

    template<typename T>
    LinearSVMClassifier<T>::LinearSVMClassifier(const LinearSVMClassifier& s)
        : Classifier<T>(s)
    {
        this->problem= NULL;
        this->model= NULL;
        this->parameter= s.parameter;
        if ( s.normalizer != NULL )
            this->normalizer= new Normalizer<T>(*(s.normalizer));
        else
            this->normalizer= NULL;
        this->kernel= s.kernel;
    }

    template<typename T>
    LinearSVMClassifier<T>::~LinearSVMClassifier()
    {
        svm_destroy_model(model);
        svm_destroy_param(parameter);
        delete node;
        if ( problem )
            destroyProblem();
        delete normalizer;
        delete kernel;
    }

    template<typename T>
    int LinearSVMClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
        //tprintf("training\n");
        //cout << kernel->descriptor << std::endl;
        openipKernel= kernel;
        this->classNames= data.classNames;
        this->featureNames= data.featureNames;
        this->preprocessingNames= data.preprocessingNames;

        if ( problem )
        {
            destroyProblem();
            svm_destroy_model(model);
            delete node;
        }

        problem= new svm_problem();

        //tprintf("normalizer\n");
        if ( normalizer )
            normalizer->train(data, mask);

        int sizeOfTrainingSet= data.size();
        if ( mask )
        {
            sizeOfTrainingSet= 0;
            for ( unsigned int i= 0; i < mask->size(); ++i )
                if ( (*mask)(i) )
                    ++sizeOfTrainingSet;
        }

        //tprintf("training: %d,%d\n", data.size(), sizeOfTrainingSet);
        problem->l= sizeOfTrainingSet;
        problem->y= new double[sizeOfTrainingSet];
        problem->x= new svm_node*[sizeOfTrainingSet];

        int i= 0;
        int dim= data(0).size();
        T tmp;

        //tprintf("build dataset\n");
        for ( unsigned int k= 0; k < data.size(); ++k )
        {
            if ( !mask || (*mask)(k) )
            {
                problem->y[i]= data(k).classIndex;
                problem->x[i]= new svm_node[dim + 1];
                for ( int j= 0; j < dim; ++j )
                {
                    problem->x[i][j].index= j;
                    if ( normalizer )
                    {
                        normalizer->normalize(data(k)(j), j, tmp);
                        problem->x[i][j].value= tmp;
                    }
                    else
                    {
                        problem->x[i][j].value= data(k)(j);
                    }
                }
                problem->x[i][dim].index= -1;
                i++;
            }
        }

        //tprintf("train\n");
        model= svm_train(problem, parameter);
        //tprintf("train finished\n");

        probabilities.resize(model->nr_class);
        probabilitiesD.resize(model->nr_class);
        node= new svm_node[dim+1];

        return 0;
    }

    template<typename T>
    void LinearSVMClassifier<T>::destroyProblem()
    {
        for ( int i= 0; i < problem->l; ++i )
        {
            delete problem->x[i];
        }
        delete problem->y;
        delete problem->x;
        delete problem;
    }

    template<typename T>
    int LinearSVMClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        openipKernel= this->kernel;
        int dim= c.size();

        if ( probabilities == NULL )
            probabilities= &(this->probabilities);

        //printf(".%d", dim); fflush(stdout);

        T tmp;
        for ( int i= 0; i < dim; ++i )
        {
            node[i].index= i;
            if ( normalizer )
            {
                normalizer->normalize(c(i), i, tmp);
                node[i].value= tmp;
            }
            else
                node[i].value= c(i);
        }
        node[dim].index= -1;

        //printf("s%d", probabilitiesD.size()); fflush(stdout);
        svm_predict_probability(model, node, &(probabilitiesD(0)));
        //printf("a "); fflush(stdout);

        for ( unsigned int i= 0; i < probabilitiesD.size(); ++i )
            (*probabilities)((model->label)[i])= probabilitiesD(i);

        int maxC= 0;
        for ( unsigned int i= 1; i < probabilities->size(); ++i )
            if ( (*probabilities)(i) > (*probabilities)(maxC) )
                maxC= i;

        return maxC;
    }

    template<typename T>
    int LinearSVMClassifier<T>::saveModelToFile(const char* filename)
    {
        strcpy(this->filename, filename);
        std::ofstream file;
        file.open("_svm_tmp_model.txt_a");
        saveModelToFile(file);
        return 0;
    }

    template<typename T>
    int LinearSVMClassifier<T>::openModelFromFile(const char* filename)
    {
        std::ifstream file;
        file.open(filename);
        openModelFromFile(file);

        return 0;
    }

    template<typename T>
    int LinearSVMClassifier<T>::saveModelToFile(ofstream& file)
    {
        file << "LinearSVMClassifier" << std::endl;
        this->Classifier<T>::saveModelToFile(file);
        KernelFunctionIO<double> kfio;
        kfio.saveKernelFunctionToFile(kernel, file);
        if ( normalizer )
        {
            file << 1 << std::endl;
            this->normalizer->saveToFile(file);
        }
        else
            file << 0 << std::endl;
        file.close();
        svm_save_model("_svm_tmp_model.txt_b", model);

        ofstream outputFile;
        outputFile.open(filename);
        cout << filename << endl;

        ifstream inputFile1;
        ifstream inputFile2;

        inputFile1.open("_svm_tmp_model.txt_a");
        inputFile2.open("_svm_tmp_model.txt_b");

        string line;

        while ( ! inputFile1.eof() )
        {
            getline(inputFile1, line);
            outputFile << line << endl;
        }
        outputFile << "end of openip model" << endl;
        while ( ! inputFile2.eof() )
        {
            getline(inputFile2, line);
            outputFile << line << endl;
        }

        inputFile1.close();
        inputFile2.close();
        outputFile.close();

        remove("_svm_tmp_model.txt_a");
        remove("_svm_tmp_model.txt_b");

        return 0;
    }

    template<typename T>
    int LinearSVMClassifier<T>::openModelFromFile(ifstream& file)
    {
        std::ofstream outputFile1;
        std::ofstream outputFile2;

        outputFile1.open("_svm_tmp_model.txt_a");
        outputFile2.open("_svm_tmp_model.txt_b");

        string line;

        int f= 0;
        while ( !file.eof() )
        {
            getline(file, line);
            if ( line.compare(string("")) == 0 )
                continue;
            if ( line.compare(string("end of openip model")) == 0 )
            {
                f= 1;
                continue;
            }

            if ( f == 0 )
                outputFile1 << line << endl;
            else
                outputFile2 << line << endl;
        }
        outputFile1.close();
        outputFile2.close();
        file.close();

        file.open("_svm_tmp_model.txt_a");
        std::string tmp;
        getline(file, tmp);

        if ( tmp.compare("LinearSVMClassifier") == 0 )
        {
            model= svm_load_model("_svm_tmp_model.txt_b");
            KernelFunctionIO<double> kfio;
            this->Classifier<T>::openModelFromFile(file);
            this->kernel= kfio.openKernelFunctionFromFile(file);
            openipKernel= this->kernel;
            cout << openipKernel->descriptor << std::cout;
            int flag;
            file >> flag;
            if ( flag )
            {
                this->normalizer= new Normalizer<T>();
                this->normalizer->openFromFile(file);
            }
            else
                this->normalizer= NULL;

            probabilities.resize(this->classNames.size());
            probabilitiesD.resize(this->classNames.size());
            node= new svm_node[this->featureNames.size()+1];

            remove("_svm_tmp_model.txt_a");
            remove("_svm_tmp_model.txt_b");

            return 1;
        }

        remove("_svm_tmp_model.txt_a");
        remove("_svm_tmp_model.txt_b");

        return 0;
    }
}

#endif // LINEARSVMCLASSIFIER_H
