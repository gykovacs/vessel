/**
 * @file SVMClassifier.h
 * @author Gyorgy Kovacs <gyuriofkovacs@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * SVMClassifier represents the Support Vector Machine classification method, based
 * on libsvm.
 */

#ifndef NEURALNETWORKCLASSIFIER_H
#define	NEURALNETWORKCLASSIFIER_H

#include <openipML/Classifier.h>
#include <openipML/fann/doublefann.h>
#include <openipML/fann/fann_cpp.h>
#include <openipML/Normalizer.h>

namespace openip
{
    typedef enum NNActivationFunction
    {
        NN_ACTIVATION_LINEAR,
        NN_ACTIVATION_SIGMOID,
        NN_ACTIVATION_SIGMOID_STEPWISE,
        NN_ACTIVATION_SIGMOID_SYMMETRIC,
        NN_ACTIVATION_GAUSSIAN,
        NN_ACTIVATION_GAUSSIAN_SYMMETRIC,
        NN_ACTIVATION_ELLIOT,
        NN_ACTIVATION_ELLIOT_SYMMETRIC,
        NN_ACTIVATION_LINEAR_PIECE,
        NN_ACTIVATION_LINEAR_PIECE_SYMMETRIC,
        NN_ACTIVATION_SIN_SYMMETRIC,
        NN_ACTIVATION_COS_SYMMETRIC
    } NNActivationFunction;

    typedef enum NNTrainingAlgorithm
    {
        NN_TRAIN_INCREMENTAL,
        NN_TRAIN_BATCH,
        NN_TRAIN_RPROP,
        NN_TRAIN_QUICKPROP
    } NNTrainingAlgorithm;

    /**
     * NeuralNetworkClassifier represents the Support Vector Machine classification method, based on libsvm.
     */
    template<typename T>
    class NeuralNetworkClassifier: public Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;
        using Classifier<T>::preprocessingNames;
        using Classifier<T>::featureNames;
        using Classifier<T>::classNames;

        /**
         * default constructor
         */
        NeuralNetworkClassifier(bool norm= true);

        NeuralNetworkClassifier(Vector<unsigned int> nn, NNActivationFunction nnaf, NNTrainingAlgorithm nnta, int nepochs, bool norm= true);

        /**
         * copy constructor
         * @param s instance to copy
         */
        NeuralNetworkClassifier(const NeuralNetworkClassifier& s);

        /**
         * destructor
         */
        ~NeuralNetworkClassifier();

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
        Vector<unsigned int> nl;
        NNActivationFunction nnaf;
        NNTrainingAlgorithm nnta;
        int nepochs;
        int numOfClasses;
        char filename[1000];
        bool directSave;

        Normalizer<T>* normalizer;
    };

    template<typename T>
    NeuralNetworkClassifier<T>::NeuralNetworkClassifier(bool norm)
        : Classifier<T>()
    {
        directSave= false;

        if ( norm )
            normalizer= new Normalizer<T>(0, 1);
        else
            normalizer= NULL;
    }

    template<typename T>
    NeuralNetworkClassifier<T>::NeuralNetworkClassifier(Vector<unsigned int> nl, NNActivationFunction nnaf, NNTrainingAlgorithm nnta, int nepochs, bool norm)
        : Classifier<T>()
    {
        this->nl= nl;
        this->nn= NULL;
        this->nnaf= nnaf;
        this->nnta= nnta;
        this->nepochs= nepochs;
        this->directSave= false;

        if ( norm )
            normalizer= new Normalizer<T>(0, 1);
        else
            normalizer= NULL;
    }

    template<typename T>
    NeuralNetworkClassifier<T>::NeuralNetworkClassifier(const NeuralNetworkClassifier& s)
        : Classifier<T>(s)
    {
        this->nl= s.nl;
        //this->nn= s.nn;
        this->nn= NULL;
        this->nnaf= s.nnaf;
        this->nnta= s.nnta;
        this->nepochs= s.nepochs;
        this->numberOfClasses= s.numberOfClasses;
        this->numOfClasses= s.numOfClasses;
        this->directSave= false;
        strcpy(this->filename, s.filename);

        if ( s.normalizer )
            this->normalizer= new Normalizer<T>(*(s.normalizer));
        else
            this->normalizer= NULL;
    }

    template<typename T>
    NeuralNetworkClassifier<T>::~NeuralNetworkClassifier()
    {
        if ( this->nn )
            nn->destroy();
    }

    template<typename T>
    int NeuralNetworkClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
        this->preprocessingNames= data.preprocessingNames;
        this->featureNames= data.featureNames;
        this->classNames= data.classNames;
        this->probabilities.resize(data(0).size());

        if ( this->normalizer )
            normalizer->train(data);

        this->numOfClasses= this->numberOfClasses= data.classNames.size();
        if ( this->nn )
            this->nn->destroy();
        this->nn= new FANN::neural_net();
        unsigned int nn2[this->nl.size()+1];
        for ( unsigned int i= 0; i < this->nl.size(); ++i )
            nn2[i]= nl(i);
        nn2[this->nl.size()]= data.numberOfClasses;
        nn2[0]= data(0).size();
	for ( unsigned int i= 0; i < this->nl.size()+1; ++i )
	  printf("%d ", nn2[i]);
	printf("\n");
        this->nn->create_standard_array(this->nl.size()+1, nn2);

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

        FeatureVector<T> fv;
        fv.resize(featureNames.size());

        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            if ( !mask || (*mask)(i) )
            {
                if ( !normalizer )
                {
                    inputp[m]= input + m*data(0).size();
                    for ( unsigned int j= 0; j < data(i).size(); ++j )
                        inputp[m][j]= data(i)(j);
                    ++m;
                }
                else
                {
                    normalizer->normalize(data(i), fv);
                    inputp[m]= input + m*data(0).size();
                    for ( unsigned int j= 0; j < data(i).size(); ++j )
                        inputp[m][j]= fv(j);
                    ++m;
                }
            }
        }


        output= (fann_type*)malloc(sizeof(fann_type)*(n*numberOfClasses));
        outputp= (fann_type**)malloc(sizeof(fann_type*)*n);
        m= 0;
        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            if ( !mask || (*mask)(i) )
            {
                //output[m]= (fann_type*)malloc(sizeof(fann_type)*1);
                outputp[m]= output + m * numberOfClasses;
                for ( int j= 0; j < numberOfClasses; ++j )
                {
                    outputp[m][j]= 0;
                }
                //outputp[m][0]= (fann_type)(float(data(i).classIndex)/float(data.numberOfClasses - 1));
                outputp[m][data(i).classIndex]= 1;
                ++m;
            }
        }

        FANN::training_data td;
        td.set_train_data(n, data(0).size(), inputp, 1, outputp);

        FANN::activation_function_enum af= FANN::LINEAR;
        FANN::training_algorithm_enum ta= FANN::TRAIN_QUICKPROP;
        if ( nnaf == NN_ACTIVATION_LINEAR )
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
            af= FANN::COS_SYMMETRIC;

        if ( nnta == NN_TRAIN_INCREMENTAL )
            ta= FANN::TRAIN_INCREMENTAL;
        else if ( nnta == NN_TRAIN_BATCH )
            ta= FANN::TRAIN_BATCH;
        else if ( nnta == NN_TRAIN_RPROP )
            ta= FANN::TRAIN_RPROP;
        else if ( nnta == NN_TRAIN_QUICKPROP )
            ta= FANN::TRAIN_QUICKPROP;

        nn->set_activation_function_hidden(af);
        nn->set_activation_function_output(af);
        //nn->set_activation_function_output(FANN::LINEAR_PIECE);
        nn->set_training_algorithm(ta);
        nn->set_train_error_function(FANN::ERRORFUNC_TANH);

        nn->randomize_weights(-0.1, 0.1);

        int nnepochs= nepochs/20;
        if ( nnepochs > 100 )
            nnepochs= 100;
        nn->train_on_data(td, nepochs, nnepochs, 0.00001);

        td.destroy_train();

        free(input);
        free(output);
        free(inputp);
        free(outputp);

        return 0;
    }

    template<typename T>
    int NeuralNetworkClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        if ( probabilities == 0 )
        {
            probabilities= &(this->probabilities);
        }

        fann_type* input;
        input= (fann_type*)malloc(sizeof(fann_type)*c.size());
        if ( !normalizer )
        {
            for ( unsigned int i= 0; i < c.size(); ++i )
                input[i]= c(i);
        }
        else
        {
            FeatureVector<T> fv;
            fv.resize(c.size());
            normalizer->normalize(c, fv);
            for ( unsigned int i= 0; i < fv.size(); ++i )
                input[i]= fv(i);
        }

        fann_type* output= nn->run(input);

        free(input);

        cout << "(";
        for ( int i= 0; i < numberOfClasses; ++i )
            cout << output[i] << ",";
        cout << ") " << endl;

        int maxI= 0;
        for ( int i= 0; i < numberOfClasses; ++i )
            if ( output[i] > output[maxI] )
                maxI= i;

        for ( unsigned int i= 0; i < probabilities->size(); ++i )
        {
            probabilities->operator()(i)= output[i] < 0 ? 0 : output[i];
        }

        return maxI;
        /*double od= output[0];
        float of= od;

        if ( od != od )
            return 0;

        of*= (this->numberOfClasses - 1);
        of= floor(of + 0.5);

        if ( of < 0 )
            of= 0;
        if ( of > this->numberOfClasses - 1)
            of= this->numberOfClasses - 1;

        if ( od < 0 )
            od= 0;
        if ( od > 1 )
            od= 1;

        (*probabilities)(0)= 1.0 - od;
        (*probabilities)(1)= od;

        return int(of);*/
    }

    template<typename T>
    int NeuralNetworkClassifier<T>::saveModelToFile(const char* filename)
    {
        strcpy(this->filename, filename);
        std::ofstream file;
        //file.open("_nn_tmp_model.txt_a");
        file.open(filename);
        directSave= true;
        saveModelToFile(file);

        return 0;
    }

    template<typename T>
    int NeuralNetworkClassifier<T>::openModelFromFile(const char* filename)
    {
        std::ifstream file;
        file.open(filename);
        openModelFromFile(file);

        return 0;
    }

    template<typename T>
    int NeuralNetworkClassifier<T>::saveModelToFile(ofstream& file)
    {
        /*if ( directSave )
        {
            file << "NeuralNetworkClassifier" << std::endl;
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
        }
        else*/
        {
            file << "NeuralNetworkClassifier" << std::endl;
            this->Classifier<T>::saveModelToFile(file);

            if ( normalizer )
            {
                file << "1" << std::endl;
                normalizer->saveToFile(file);
            }
            else
                file << "0" << std::endl;

            file << directSave << std::endl;
            long int p= clock();
            long int p2= (long int)this;
            stringstream ss;
            ss << "nn_model_";
            ss << p;
            ss << p2;
            ss << ".model";
            string filename= ss.str();
            nn->save(filename);
            file << filename << std::endl;
        }

        return 0;
    }

    template<typename T>
    int NeuralNetworkClassifier<T>::openModelFromFile(ifstream& file)
    {
        /*std::ofstream outputFile1;
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
        file.close();*/

        //file.open("_nn_tmp_model.txt_a");
        tprintf("reading one line\n");
        std::string tmp;
        getline(file, tmp);

        if ( tmp.compare("NeuralNetworkClassifier") == 0 )
        {
            this->Classifier<T>::openModelFromFile(file);


            int flag;
            file >> flag;
            if ( flag )
            {
                normalizer= new Normalizer<T>(0,1);
                normalizer->openFromFile(file);
            }
            else
                normalizer= NULL;

            file.get();
            bool directSave;
            file >> directSave;
            /*if ( directSave )
            {
                nn= new FANN::neural_net();
                nn->create_from_file(std::string("_nn_tmp_model.txt_b"));
            }
            else*/
            {
                string filename;
                file >> filename;
                //cout << "filename read non-direct save " << filename << std::endl;
                nn= new FANN::neural_net();
                nn->create_from_file(filename);
            }

            for ( unsigned int i= 0; i < preprocessingNames.size(); ++i )
                cout << preprocessingNames(i) << std::endl;
            for ( unsigned int i= 0; i < featureNames.size(); ++i )
                cout << featureNames(i) << std::endl;
            for ( unsigned int i= 0; i < classNames.size(); ++i )
                cout << classNames(i) << std::endl;

            probabilities.resize(this->classNames.size());
            this->numberOfClasses= classNames.size();
            //probabilitiesD.resize(this->classNames.size());

            //remove("_nn_tmp_model.txt_a");
            //remove("_nn_tmp_model.txt_b");

            return 1;
        }

        //remove("_nn_tmp_model.txt_a");
        //remove("_nn_tmp_model.txt_b");

        return 0;
    }
}

#endif	/* SVMCLASSIFIER_H */

