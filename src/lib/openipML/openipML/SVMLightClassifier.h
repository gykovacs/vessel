#ifndef SVMLIGHTCLASSIFIER_H
#define SVMLIGHTCLASSIFIER_H

#include <openipML/Classifier.h>
#include <openipML/svmlight/svm_common.h>
#include <openipML/svmlight/svm_learn.h>
#include <openipML/Normalizer.h>
#include <openipML/KernelFunctionIO.h>

using namespace svm;

namespace openip
{
    /**
     * SVMClassifier represents the Support Vector Machine classification method, based on libsvm.
     */
    template<typename T>
    class SVMLightClassifier: public virtual Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;

        /**
         * default constructor
         */
        SVMLightClassifier(int kernelType= RBF, int degree= 3, double gamma= 1.0, double C= 1.0, double coef0= 1.0, double cacheSize= 100, double eps= 0.01, bool norm= true, KernelFunction<double>* kernel= NULL);

        /**
         * copy constructor
         * @param s instance to copy
         */
        SVMLightClassifier(const SVMLightClassifier& s);

        /**
         * destructor
         */
        ~SVMLightClassifier();

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

        /** probabilities */
        Vector<double> probabilitiesD;
        /** temporary filename */
        char filename[100];

        Normalizer<T>* normalizer;
        KernelFunction<double> *kernel;

        DOC** docs;
        double* classes;
        int totdoc;
        int totwords;
        LEARN_PARM learn_parm;
        KERNEL_PARM kernel_parm;
        KERNEL_CACHE* kernel_cache;
        MODEL* model;
        double* alpha;
    };

    template<typename T>
    SVMLightClassifier<T>::SVMLightClassifier(int kernelType, int degree, double gamma, double C, double coef0, double cacheSize, double eps, bool norm, KernelFunction<double>* kernel)
        : Classifier<T>()
    {


        if ( norm )
            normalizer= new Normalizer<T>(0, 1);
        else
            normalizer= NULL;
        this->kernel= kernel;
    }

    template<typename T>
    SVMLightClassifier<T>::SVMLightClassifier(const SVMLightClassifier& s)
        : Classifier<T>(s)
    {

        this->normalizer= new Normalizer<T>(*(s.normalizer));
        this->kernel= s.kernel;
    }

    template<typename T>
    SVMLightClassifier<T>::~SVMLightClassifier()
    {

        delete normalizer;
        delete kernel;
    }

    template<typename T>
    int SVMLightClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
        //tprintf("training\n");
        //cout << kernel->descriptor << std::endl;
        //openipKernel= kernel;
        this->classNames= data.classNames;
        this->featureNames= data.featureNames;
        this->preprocessingNames= data.preprocessingNames;



        //probabilities.resize(model->nr_class);
        probabilitiesD.resize(model->nr_class);

        return 0;
    }

    template<typename T>
    void SVMLightClassifier<T>::destroyProblem()
    {

    }

    template<typename T>
    int SVMLightClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        openipKernel= this->kernel;


        /*for ( unsigned int i= 0; i < probabilitiesD.size(); ++i )
            (*probabilities)((model->label)[i])= probabilitiesD(i);*/

        int maxC= 0;
        /*for ( unsigned int i= 1; i < probabilities->size(); ++i )
            if ( (*probabilities)(i) > (*probabilities)(maxC) )
                maxC= i;*/

        return maxC;
    }

    template<typename T>
    int SVMLightClassifier<T>::saveModelToFile(const char* filename)
    {
        /*strcpy(this->filename, filename);
        std::ofstream file;
        file.open("_svm_tmp_model.txt_a");
        saveModelToFile(file);*/
        return 0;
    }

    template<typename T>
    int SVMLightClassifier<T>::openModelFromFile(const char* filename)
    {
        /*std::ifstream file;
        file.open(filename);
        openModelFromFile(file);*/

        return 0;
    }

    template<typename T>
    int SVMLightClassifier<T>::saveModelToFile(ofstream& file)
    {
        /*file << "SVMLightClassifier" << std::endl;
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
        remove("_svm_tmp_model.txt_b");*/

        return 0;
    }

    template<typename T>
    int SVMLightClassifier<T>::openModelFromFile(ifstream& file)
    {
        /*std::ofstream outputFile1;
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

        if ( tmp.compare("EpsilonSVMClassifier") == 0 )
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
        remove("_svm_tmp_model.txt_b");*/

        return 0;
    }
}

#endif // SVMLIGHTCLASSIFIER_H
