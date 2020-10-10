#ifndef LOGISTIC_REGRESSION_CLASSIFIER_H
#define LOGISTIC_REGRESSION_CLASSIFIER_H

namespace openip
{
      /**
     * SVMClassifier represents the Support Vector Machine classification method, based on libsvm.
     */
    template<typename T>
    class LogisticRegressionClassifier: public virtual Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;

        /**
         * default constructor
         */
        LogisticRegressionClassifier(int maxit= 100000, float mu= 1.0, float alpha= 1.0);

        /**
         * copy constructor
         * @param s instance to copy
         */
        LogisticRegressionClassifier(const LogisticRegressionClassifier& s);

        /**
         * destructor
         */
        ~LogisticRegressionClassifier();

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

	virtual double probability(Vector<T>& c);

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

	float alpha;
	float mu;
	int maxit;

	DataSet<T> data;

	Vector<T> w;
    };


    template<typename T>
    LogisticRegressionClassifier<T>::LogisticRegressionClassifier(int maxit, float mu, float alpha)
        : Classifier<T>()
    {
	this->mu= mu;
	this->alpha= alpha;
	this->maxit= maxit;
    }

    template<typename T>
    LogisticRegressionClassifier<T>::LogisticRegressionClassifier(const LogisticRegressionClassifier& s)
        : Classifier<T>(s)
    {
	this->mu= s.mu;
        this->alpha= s.alpha;
	this->maxit= s.maxit;
    }

    template<typename T>
    LogisticRegressionClassifier<T>::~LogisticRegressionClassifier()
    {
    }

    template<typename T>
    int LogisticRegressionClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
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
        
	w.resize(data(0).size());
	for ( unsigned int i= 0; i < data(0).size(); ++i )
	  w(i)= double(rand())/RAND_MAX;

	tprintf("starting main iteration\n");
	for ( int i= 1; i < maxit; ++i )
	{
	  alpha= 1.0/i;
	  for ( unsigned int j= 0; j < this->data.size(); ++j )
	    for ( unsigned int k= 0; k < this->data(j).size(); ++k )
	      w(k)= w(k) + alpha*(this->data(j).classIndex - probability(this->data(j)))*this->data(j)(k) - alpha*2*mu*w(k);
	}
	tprintf("main iteration finished\n");

        return 0;
    }

    template<typename T>
    double LogisticRegressionClassifier<T>::probability(Vector<T>& c)
    {
      double z= 0;
      for ( unsigned int i= 0; i < c.size(); ++i )
	z+= w(i)*c(i);
      return 1.0/(1.0 + exp(-z));
    }
    
    template<typename T>
    int LogisticRegressionClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        if ( probabilities == NULL )
            probabilities= &(this->probabilities);

        T tmp= probability(c);

        if ( tmp > 0.5 )
	{
	  (*probabilities)(0)= 1 - tmp;
	  (*probabilities)(1)= tmp;
	  return 1;
	}
	else
	{
	  (*probabilities)(0)= tmp;
	  (*probabilities)(1)= 1 - tmp;
	  return 0;
	}
    }

    template<typename T>
    int LogisticRegressionClassifier<T>::saveModelToFile(const char* /*filename*/)
    {
        /*strcpy(this->filename, filename);
        std::ofstream file;
        file.open(filename);
        saveModelToFile(file);*/
        return 0;
    }

    template<typename T>
    int LogisticRegressionClassifier<T>::openModelFromFile(const char* filename)
    {
        std::ifstream file;
        file.open(filename);
        openModelFromFile(file);

        return 0;
    }

    template<typename T>
    int LogisticRegressionClassifier<T>::saveModelToFile(ofstream& /*file*/)
    {
        /*file << "SequentialSVMClassifier" << std::endl;
        this->Classifier<T>::saveModelToFile(file);
        KernelFunctionIO<double> kfio;
        kfio.saveKernelFunctionToFile(kernel, file);*/
;

        return 0;
    }

    template<typename T>
    int LogisticRegressionClassifier<T>::openModelFromFile(ifstream& /*file*/)
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
    class MinibatchMulticlassLogisticRegressionClassifier: public virtual Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;

        /**
         * default constructor
         */
        MinibatchMulticlassLogisticRegressionClassifier(int maxit= 100000, float mu= 1.0, float alpha= 1.0, int batchSize= 10);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MinibatchMulticlassLogisticRegressionClassifier(const MinibatchMulticlassLogisticRegressionClassifier& s);

        /**
         * destructor
         */
        ~MinibatchMulticlassLogisticRegressionClassifier();

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

	virtual double probability(Vector<T>& c);
	
	virtual double pY(int i, Vector<T>& x);

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

	float alpha;
	float mu;
	int maxit;
	int batchSize;

	DataSet<T> data;

	Matrix<double> w;
	Vector<double> b;
    };


    template<typename T>
    MinibatchMulticlassLogisticRegressionClassifier<T>::MinibatchMulticlassLogisticRegressionClassifier(int maxit, float mu, float alpha, int batchSize)
        : Classifier<T>()
    {
	this->mu= mu;
	this->alpha= alpha;
	this->maxit= maxit;
	this->batchSize= batchSize;
    }

    template<typename T>
    MinibatchMulticlassLogisticRegressionClassifier<T>::MinibatchMulticlassLogisticRegressionClassifier(const MinibatchMulticlassLogisticRegressionClassifier& s)
        : Classifier<T>(s)
    {
	this->mu= s.mu;
        this->alpha= s.alpha;
	this->maxit= s.maxit;
	this->batchSize= s.batchSize;
	this->w= s.w;
	this->b= s.b;
    }

    template<typename T>
    MinibatchMulticlassLogisticRegressionClassifier<T>::~MinibatchMulticlassLogisticRegressionClassifier()
    {
    }

    template<typename T>
    double MinibatchMulticlassLogisticRegressionClassifier<T>::pY(int i, Vector<T>& x)
    {
      double tmp0= 0, tmp1= 0, tmp2= 0;
      for ( unsigned int j= 0; j < x.size(); ++j )
	tmp0+= w(j, i)*x(j);
      tmp0= exp(tmp0);
      
      for ( int j= 0; j < w.columns; ++j )
      {
	tmp1= 0;
	for ( unsigned int k= 0; k < x.size(); ++k )
	  tmp1+= w(k, j)*x(k);
	tmp1= exp(tmp1);
	tmp2+= tmp1;
      }
      
      return tmp0/tmp2;
    }
    
    template<typename T>
    int MinibatchMulticlassLogisticRegressionClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
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
        
	w.resize(this->data(0).size(), this->data.numberOfClasses);
	b.resize(this->data.numberOfClasses);
	
	w= 0;
	b= 0;
	
	/*for ( int i= 0; i < this->data(0).size(); ++i )
	  for ( int j= 0; j < this->data.numberOfClasses; ++j )
	    w(i,j)= double(rand())/RAND_MAX;
	for ( int i= 0; i < this->data.numberOfClasses; ++i )
	  b(i)= double(rand())/RAND_MAX;*/

	Matrix<double> grad(w.rows, w.columns);
	tprintf("starting main iteration\n");
	for ( int i= 1; i < maxit; ++i )
	{
	  alpha= 1.0/i;
	  grad= 0;
	  
	  /*printf("w\n");
	  for ( int l= 0; l < w.rows; ++l )
	  {
	      for ( int k= 0; k < w.columns; ++k )
		printf("%f ", w(l, k));
	      printf("\n");
	  }
	  
	  printf("%lf,%lf ", pY(0, this->data(0)), pY(1, this->data(0)));*/
	  
	  for ( int l= 0; l < w.columns; ++l )
	    for ( unsigned int j= 0; j < this->data.size(); ++j )
	      for ( unsigned int k= 0; k < this->data(j).size(); ++k )
		grad(k,l)= grad(k,l) + (this->data(j)(k)*((this->data(j).classIndex == l ? 1.0 : 0.0) -
		pY(l, this->data(j))));
	  
	  /*printf("g\n");
	  for ( int l= 0; l < w.rows; ++l )
	  {
	      for ( int k= 0; k < w.columns; ++k )
		printf("%f ", grad(l, k));
	      printf("\n");
	  }
	  
	  getchar();*/
	  
	  for ( int l= 0; l < w.rows; ++l )
	    for ( int k= 0; k < w.columns; ++k )
	      w(l, k)-= - 1.0/this->data.size() * mu * grad(l, k);
	    
	}
	tprintf("main iteration finished\n");

	printf("w\n");
	  for ( int l= 0; l < w.rows; ++l )
	  {
	      for ( int k= 0; k < w.columns; ++k )
		printf("%f ", w(l, k));
	      printf("\n");
	  }
	
        return 0;
    }

    template<typename T>
    double MinibatchMulticlassLogisticRegressionClassifier<T>::probability(Vector<T>& c)
    {
      double z= 0;
      for ( unsigned int i= 0; i < c.size(); ++i )
	z+= w(i)*c(i);
      return 1.0/(1.0 + exp(-z));
    }
    
    template<typename T>
    int MinibatchMulticlassLogisticRegressionClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        if ( probabilities == NULL )
            probabilities= &(this->probabilities);

	for ( int i= 0; i < w.columns; ++i )
	  (*probabilities)(i)= pY(i, c);
	
	int maxIdx= 0;
	for ( int i= 0; i < w.columns; ++i )
	  if ( (*probabilities)(i) > (*probabilities)(maxIdx) )
	    maxIdx= i;
	
	return maxIdx;
    }

    template<typename T>
    int MinibatchMulticlassLogisticRegressionClassifier<T>::saveModelToFile(const char* /*filename*/)
    {
        /*strcpy(this->filename, filename);
        std::ofstream file;
        file.open(filename);
        saveModelToFile(file);*/
        return 0;
    }

    template<typename T>
    int MinibatchMulticlassLogisticRegressionClassifier<T>::openModelFromFile(const char* filename)
    {
        std::ifstream file;
        file.open(filename);
        openModelFromFile(file);

        return 0;
    }

    template<typename T>
    int MinibatchMulticlassLogisticRegressionClassifier<T>::saveModelToFile(ofstream& /*file*/)
    {
        /*file << "SequentialSVMClassifier" << std::endl;
        this->Classifier<T>::saveModelToFile(file);
        KernelFunctionIO<double> kfio;
        kfio.saveKernelFunctionToFile(kernel, file);*/
;

        return 0;
    }

    template<typename T>
    int MinibatchMulticlassLogisticRegressionClassifier<T>::openModelFromFile(ifstream& /*file*/)
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
}

#endif