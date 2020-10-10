#ifndef CASCADEDNEURALNETWORKCLASSIFIER_H
#define CASCADEDNEURALNETWORKCLASSIFIER_H

#include <openipML/Classifier.h>
#include <openipML/fann/doublefann.h>
#include <openipML/fann/fann_cpp.h>
#include <openipML/NeuralNetworkClassifier.h>

namespace openip
{
    /**
     * CascadedNeuralNetworkClassifier represents the Support Vector Machine classification method, based on libsvm.
     */
    template<typename T>
    class CascadedNeuralNetworkClassifier: public Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;
        using Classifier<T>::preprocessingNames;
        using Classifier<T>::featureNames;
        using Classifier<T>::classNames;

        /**
         * default constructor
         */
        CascadedNeuralNetworkClassifier();

        CascadedNeuralNetworkClassifier(NNActivationFunction nnaf, NNTrainingAlgorithm nnta, int nepochs);

        /**
         * copy constructor
         * @param s instance to copy
         */
        CascadedNeuralNetworkClassifier(const CascadedNeuralNetworkClassifier& s);

        /**
         * destructor
         */
        ~CascadedNeuralNetworkClassifier();

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

        FANN::neural_net* nn;
        int numberOfClasses;
        NNActivationFunction nnaf;
        NNTrainingAlgorithm nnta;
        int nepochs;
        int numOfClasses;
        char filename[1000];
    };

    template<typename T>
    CascadedNeuralNetworkClassifier<T>::CascadedNeuralNetworkClassifier()
        : Classifier<T>()
    {
    }

    template<typename T>
    CascadedNeuralNetworkClassifier<T>::CascadedNeuralNetworkClassifier(NNActivationFunction nnaf, NNTrainingAlgorithm nnta, int nepochs)
        : Classifier<T>()
    {
        this->nn= NULL;
        this->nnaf= nnaf;
        this->nnta= nnta;
        this->nepochs= nepochs;
    }

    template<typename T>
    CascadedNeuralNetworkClassifier<T>::CascadedNeuralNetworkClassifier(const CascadedNeuralNetworkClassifier& s)
        : Classifier<T>(s)
    {
        this->nn= s.nn;
        this->nnaf= s.nnaf;
        this->nnta= s.nnta;
        this->nepochs= s.nepochs;
    }

    template<typename T>
    CascadedNeuralNetworkClassifier<T>::~CascadedNeuralNetworkClassifier()
    {
        if ( this->nn )
            nn->destroy();
    }

    template<typename T>
    int CascadedNeuralNetworkClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
        this->preprocessingNames= data.preprocessingNames;
        this->featureNames= data.featureNames;
        this->classNames= data.classNames;

        this->numOfClasses= data.numberOfClasses;
        if ( this->nn )
            this->nn->destroy();
        this->nn= new FANN::neural_net();
        /*unsigned int nn2[this->nl.size()];
        for ( unsigned int i= 0; i < this->nl.size(); ++i )
            nn2[i]= nl(i);
        nn2[0]= data(0).size();
        this->nn->create_standard_array(this->nl.size(), nn2);*/
        unsigned int nl[2];
        nl[0]= data(0).size();
        nl[1]= 1;
        this->nn->create_standard_array(2, nl);

        numberOfClasses= data.classNames.size();

        int n;
        if ( mask )
            n= mask->numberOfNonZeroElements();
        else
            n= data.size();

        tprintf("learning data: %d\n", n);

        fann_type* input;
        fann_type* output;
        fann_type** inputp;
        fann_type** outputp;

        int m= 0;
        input= (fann_type*)malloc(sizeof(fann_type)*n*data(0).size());
        inputp= (fann_type**)malloc(sizeof(fann_type*)*n);

        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            if ( !mask || (*mask)(i) )
            {
                //input[m]= (fann_type*)malloc(sizeof(fann_type)*data(0).size());
                inputp[m]= input + m*data(0).size();
                for ( unsigned int j= 0; j < data(i).size(); ++j )
                    inputp[m][j]= data(i)(j);
                ++m;
            }
        }


        output= (fann_type*)malloc(sizeof(fann_type)*n);
        outputp= (fann_type**)malloc(sizeof(fann_type*)*n);
        m= 0;
        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            if ( !mask || (*mask)(i) )
            {
                //output[m]= (fann_type*)malloc(sizeof(fann_type)*1);
                outputp[m]= output + m;
                outputp[m][0]= float(data(i).classIndex)/float(data.numberOfClasses - 1);
                ++m;
            }
        }

        FANN::training_data td;
        td.set_train_data(n, data(0).size(), inputp, 1, outputp);

        //FANN::activation_function_enum af= FANN::LINEAR;
        FANN::training_algorithm_enum ta= FANN::TRAIN_QUICKPROP;
        /*if ( nnaf == NN_ACTIVATION_LINEAR )
            af= FANN::LINEAR;
        else if ( nnaf == NN_ACTIVATION_SIGMOID )
            af= FANN::SIGMOID;
        else if ( nnaf == NN_ACTIVATION_SIGMOID_STEPWISE )
            af= FANN::SIGMOID_STEPWISE;
        else if ( nnaf == NN_ACTIVATION_SIGMOID_SYMMETRIC )
            af= FANN::SIGMOID_SYMMETRIC;
        else if ( nnaf == NN_ACTIVATION_GAUSSIAN )
            af= FANN::GAUSSIAN;
        else if ( nnaf == NN_ACTIVATION_GAUSSIAN_SYMMETRIC )
            af= FANN::GAUSSIAN_SYMMETRIC;
        else if ( nnaf == NN_ACTIVATION_ELLIOT )
            af= FANN::ELLIOT;
        else if ( nnaf == NN_ACTIVATION_ELLIOT_SYMMETRIC )
            af= FANN::ELLIOT_SYMMETRIC;
        else if ( nnaf == NN_ACTIVATION_LINEAR_PIECE )
            af= FANN::LINEAR_PIECE;
        else if ( nnaf == NN_ACTIVATION_LINEAR_PIECE_SYMMETRIC )
            af= FANN::LINEAR_PIECE_SYMMETRIC;
        else if ( nnaf == NN_ACTIVATION_SIN_SYMMETRIC )
            af= FANN::SIN_SYMMETRIC;
        else if ( nnaf == NN_ACTIVATION_COS_SYMMETRIC )
            af= FANN::COS_SYMMETRIC;*/

        if ( nnta == NN_TRAIN_INCREMENTAL )
            ta= FANN::TRAIN_INCREMENTAL;
        else if ( nnta == NN_TRAIN_BATCH )
            ta= FANN::TRAIN_BATCH;
        else if ( nnta == NN_TRAIN_RPROP )
            ta= FANN::TRAIN_RPROP;
        else if ( nnta == NN_TRAIN_QUICKPROP )
            ta= FANN::TRAIN_QUICKPROP;

        FANN::activation_function_enum afs[1];
        //afs[0]= FANN::SIGMOID_SYMMETRIC;
        //afs[1]= FANN::ELLIOT_SYMMETRIC;
        //afs[2]= FANN::LINEAR_PIECE_SYMMETRIC;
        afs[0]= FANN::SIN_SYMMETRIC;
        nn->set_cascade_activation_functions(afs, 1);
        //nn->set_activation_function_output(af);
        nn->set_training_algorithm(ta);
        nn->set_train_error_function(FANN::ERRORFUNC_TANH);

        nn->randomize_weights(-0.1, 0.1);

        int nnepochs= nepochs/20;
        if ( nnepochs > 100 )
            nnepochs= 100;

        nn->cascadetrain_on_data(td, 30, 1, 0.00001);
        nn->train_on_data(td, nepochs, nnepochs, 0.00001);

        td.destroy_train();

        free(input);
        free(output);
        free(inputp);
        free(outputp);

        return 0;
    }

    template<typename T>
    int CascadedNeuralNetworkClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        if ( probabilities == 0 )
            probabilities= &(this->probabilities);

        fann_type* input;
        input= (fann_type*)malloc(sizeof(fann_type)*c.size());
        for ( unsigned int i= 0; i < c.size(); ++i )
            input[i]= c(i);

        fann_type* output= nn->run(input);

        free(input);

        double od= output[0];
        float of= od;

        if ( od != od )
            return 0;

        of*= this->numberOfClasses;
        of= floor(of + 0.5);

        if ( of < 0 )
            of= 0;
        if ( of > this->numberOfClasses - 1)
            of= this->numberOfClasses - 1;

        (*probabilities)(0)= 0;
        (*probabilities)(1)= 0;

        return int(of);
    }

    template<typename T>
    int CascadedNeuralNetworkClassifier<T>::saveModelToFile(const char* filename)
    {
        strcpy(this->filename, filename);
        std::ofstream file;
        file.open("_nn_tmp_model.txt_a");
        saveModelToFile(file);

        return 0;
    }

    template<typename T>
    int CascadedNeuralNetworkClassifier<T>::openModelFromFile(const char* filename)
    {
        std::ifstream file;
        file.open(filename);
        openModelFromFile(file);

        return 0;
    }

    template<typename T>
    int CascadedNeuralNetworkClassifier<T>::saveModelToFile(ofstream& file)
    {
        file << "CascadedNeuralNetworkClassifier" << std::endl;
        this->Classifier<T>::saveModelToFile(file);
        file.close();

        nn->save(std::string("_nn_tmp_model.txt_b"));

        ofstream outputFile;
        outputFile.open(filename);
        cout << filename << endl;

        ifstream inputFile1;
        ifstream inputFile2;

        inputFile1.open("_nn_tmp_model.txt_a");
        inputFile2.open("_nn_tmp_model.txt_b");

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

        remove("_nn_tmp_model.txt_a");
        remove("_nn_tmp_model.txt_b");

        return 0;
    }

    template<typename T>
    int CascadedNeuralNetworkClassifier<T>::openModelFromFile(ifstream& file)
    {
        std::ofstream outputFile1;
        std::ofstream outputFile2;

        outputFile1.open("_nn_tmp_model.txt_a");
        outputFile2.open("_nn_tmp_model.txt_b");

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

        file.open("_nn_tmp_model.txt_a");
        std::string tmp;
        getline(file, tmp);

        if ( tmp.compare("CascadedNeuralNetworkClassifier") == 0 )
        {
            nn= new FANN::neural_net();
            nn->create_from_file(std::string("_nn_tmp_model.txt_b"));
            this->Classifier<T>::openModelFromFile(file);

            for ( unsigned int i= 0; i < preprocessingNames.size(); ++i )
                cout << preprocessingNames(i) << std::endl;
            for ( unsigned int i= 0; i < featureNames.size(); ++i )
                cout << featureNames(i) << std::endl;
            for ( unsigned int i= 0; i < classNames.size(); ++i )
                cout << classNames(i) << std::endl;

            probabilities.resize(this->classNames.size());
            this->numberOfClasses= classNames.size();
            //probabilitiesD.resize(this->classNames.size());

            remove("_nn_tmp_model.txt_a");
            remove("_nn_tmp_model.txt_b");

            return 1;
        }

        remove("_nn_tmp_model.txt_a");
        remove("_nn_tmp_model.txt_b");

        return 0;
    }
}

#endif	/* CascadedNeuralNetworkClassifier */

