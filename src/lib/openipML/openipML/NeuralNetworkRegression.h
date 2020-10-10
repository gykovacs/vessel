#ifndef _NEURAL_NETWORK_REGRESSION_H_
#define _NEURAL_NETWORK_REGRESSION_H_

#include <string>

#include <openipDS/Vector.h>
#include <openipDS/RegressionDataSet.h>
#include <openipML/Regression.h>
#include <openipSC/matrixOperations.h>
#include <openipML/fann/doublefann.h>
#include <openipML/fann/fann_cpp.h>
#include <openipML/NeuralNetworkClassifier.h>


namespace openip
{
  template<typename T>
  class NeuralNetworkRegression: public Regression<T>
  {
    public:
      using Regression<T>::probabilities;
      using Regression<T>::preprocessingNames;
      using Regression<T>::featureNames;
      
      NeuralNetworkRegression(Vector<unsigned int> nn, NNActivationFunction nnaf, NNTrainingAlgorithm nnta, int nepochs);

      NeuralNetworkRegression(NeuralNetworkRegression& r);

      ~NeuralNetworkRegression();

      virtual int train(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL);

      virtual float regress(Vector<T>& c, Vector<float>* probabilities= NULL);

      virtual int saveModelToFile(ofstream& file);

      virtual int openModelFromFile(ifstream& file);
      
      FANN::neural_net* nn;
      Vector<unsigned int> nl;
      NNActivationFunction nnaf;
      NNTrainingAlgorithm nnta;
      int nepochs;
      char filename[1000];
      bool directSave;
      
      Vector<float> betaHat;
  };
  
  template<typename T>
  NeuralNetworkRegression<T>::NeuralNetworkRegression(Vector<unsigned int> nl, NNActivationFunction nnaf, NNTrainingAlgorithm nnta, int nepochs)
  : Regression<T>()
  {
    this->nl= nl;
    this->nn= NULL;
    this->nnaf= nnaf;
    this->nnta= nnta;
    this->nepochs= nepochs;
    this->directSave= false;
  }
  
  template<typename T>
  NeuralNetworkRegression<T>::NeuralNetworkRegression(NeuralNetworkRegression& s)
  : Regression<T>(s)
  {
    this->nl= s.nl;
    //this->nn= s.nn;
    this->nn= NULL;
    this->nnaf= s.nnaf;
    this->nnta= s.nnta;
    this->nepochs= s.nepochs;
    this->directSave= false;
    strcpy(this->filename, s.filename);
  }
  
  template<typename T>
  NeuralNetworkRegression<T>::~NeuralNetworkRegression()
  {
    if ( this->nn )
      nn->destroy();
  }
  
  template<typename T>
  int NeuralNetworkRegression<T>::train(RegressionDataSet<T>& data, Vector<unsigned char>* mask)
  {
    data.init();
    
    this->preprocessingNames= data.preprocessingNames;
    this->featureNames= data.featureNames;

    this->probabilities.resize(data(0).size());

    printf("a"); fflush(stdout);
    if ( this->nn )
	this->nn->destroy();
    this->nn= new FANN::neural_net();
    unsigned int nn2[this->nl.size()+1];
    for ( unsigned int i= 0; i < this->nl.size(); ++i )
	nn2[i]= nl(i);
    nn2[this->nl.size()]= 1;
    nn2[0]= data(0).size();

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

    printf("b"); fflush(stdout);
    int m= 0;
    input= (fann_type*)malloc(sizeof(fann_type)*n*data(0).size());
    inputp= (fann_type**)malloc(sizeof(fann_type*)*n);

    FeatureVector<T> fv;
    fv.resize(featureNames.size());

    for ( unsigned int i= 0; i < data.size(); ++i )
    {
      if ( !mask || (*mask)(i) )
      {
	inputp[m]= input + m*data(0).size();
	for ( unsigned int j= 0; j < data(i).size(); ++j )
	    inputp[m][j]= data(i)(j);
	++m;
      }
    }

    printf("c"); fflush(stdout);
    output= (fann_type*)malloc(sizeof(fann_type)*n);
    outputp= (fann_type**)malloc(sizeof(fann_type*)*n);
    m= 0;
    for ( unsigned int i= 0; i < data.size(); ++i )
    {
	if ( !mask || (*mask)(i) )
	{
	    outputp[m]= output + m * 1;
	    for ( unsigned int j= 0; j < 1; ++j )
	    {
		outputp[m][j]= 0;
	    }
	    //outputp[m][0]= (fann_type)(float(data(i).classIndex)/float(data.numberOfClasses - 1));
	    outputp[m][0]= data(i).targetValue;
	    ++m;
	}
    }

    printf("d"); fflush(stdout);
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
  float NeuralNetworkRegression<T>::regress(Vector<T>& c, Vector<float>* /*probabilities*/)
  {
    fann_type* input;
    input= (fann_type*)malloc(sizeof(fann_type)*c.size());
    
    for ( unsigned int i= 0; i < c.size(); ++i )
      input[i]= c(i);
    
    fann_type* output= nn->run(input);

    free(input);

    /*cout << "(";
    for ( int i= 0; i < numberOfClasses; ++i )
	cout << output[i] << ",";
    cout << ") " << endl;*/

    /*int maxI= 0;
    for ( int i= 0; i < numberOfClasses; ++i )
	if ( output[i] > output[maxI] )
	    maxI= i;*/

    return output[0];
  }

  template<typename T>
  int NeuralNetworkRegression<T>::saveModelToFile(ofstream& /*file*/)
  {
    /*Regression<T>::saveModelToFile(file);
    
    file << betaHat.size() << endl;
    for ( int i= 0; i < betaHat.size(); ++i )
      file << betaHat(i) << endl;*/
    
    return 0;
  }

  template<typename T>
  int NeuralNetworkRegression<T>::openModelFromFile(ifstream& /*file*/)
  {
    /*Regression<T>::openModelFromFile(file);
    
    int n;
    file >> n;
    float b;
    betaHat.resize(n);
    for ( int i= 0; i < n; ++i )
      file >> betaHat(i);*/
    
    return 0;
  }
}

#endif