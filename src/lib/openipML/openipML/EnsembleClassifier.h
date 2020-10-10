#ifndef ENSEMBLECLASSIFIER_H
#define ENSEMBLECLASSIFIER_H

#include <openipML/ClassifierIO.h>

namespace openip
{
    template<typename T>
    class ClassifierIO;

    enum DecisionRule
    {
        ENSEMBLE_MAJORITY= 1,
        ENSEMBLE_HIGHEST_PROBABILITY
    };

    template<typename T>
    class EnsembleClassifier: public Classifier<T>
    {
    public:

        using Classifier<T>::probabilities;
        using Classifier<T>::classNames;
        using Classifier<T>::preprocessingNames;
        using Classifier<T>::featureNames;

        EnsembleClassifier(int decisionRule= ENSEMBLE_MAJORITY, std::string filename= std::string(""));

        EnsembleClassifier(const EnsembleClassifier& e);

        ~EnsembleClassifier();

        virtual int train(DataSet<T>& data, Vector<unsigned char>* mask= NULL);

        virtual int classify(Vector<T>& c, Vector<float>* probabilities= NULL);
	
	virtual int classifyVaryingDimensions(int k, Vector<float>* probabilities= NULL);

        virtual int saveModelToFile(ofstream& file);

        virtual int openModelFromFile(ifstream& file);

        virtual int decide();
	
	virtual void init();
	
	virtual float evaluate(DataSet<T>& data, int measure);

        Vector<Classifier<T>*> classifiers;
        Vector<int> labels;
        Vector<Vector<float> > probabilities2;
        Vector<float>* p;
	
	Vector<DataSet<T> > fdatas;
	Vector<Vector<int> > masks;

        int decisionRule;
        std::string prefix;
    };

    template<typename T>
    EnsembleClassifier<T>::EnsembleClassifier(int decisionRule, std::string filename)
        : Classifier<T>()
    {
        this->decisionRule= decisionRule;
        int idx= filename.find_last_of("/");
        this->prefix= filename.substr(0,idx+1);
    }

    template<typename T>
    EnsembleClassifier<T>::EnsembleClassifier(const EnsembleClassifier &e)
        : Classifier<T>(e)
    {
        this->decisionRule= e.decisionRule;
        this->prefix= e.prefix;
    }

    template<typename T>
    EnsembleClassifier<T>::~EnsembleClassifier()
    {
    }

    template<typename T>
    int EnsembleClassifier<T>::train(DataSet<T> &data, Vector<unsigned char> *mask)
    {
        this->classNames= data.classNames;
        this->featureNames= data.featureNames;
        this->preprocessingNames= data.preprocessingNames;

        labels.resize(classifiers.size());
        probabilities2.resize(classifiers.size());
        probabilities.resize(classifiers(0)->classNames.size());

        for ( unsigned int i= 0; i < probabilities2.size(); ++i )
            probabilities2(i).resize(classNames.size());

        for ( unsigned int i= 0; i < classifiers.size(); ++i )
            classifiers(i)->train(data, mask);

        return 0;
    }

    template<typename T>
    int EnsembleClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        if ( probabilities )
            p= probabilities;
        else
            p= &(this->probabilities);
        if ( classifiers.size() == 0 )
            return 0;

	if ( masks.size() == 0 )
	{
	  for ( unsigned int i= 0; i < classifiers.size(); ++i )
	      labels(i)= classifiers(i)->classify(c, &(probabilities2(i)));
	}
	else
	{
	  FeatureVector<T> f;
	  for ( unsigned int k= 0; k < classifiers.size(); ++k )
	  {
	    f.resize(classifiers(k)->featureNames.size());
	    int n= 0;
	    for ( unsigned int l= 0; l < masks(k).size(); ++l )
	      if ( masks(k)(l) )
		f(n++)= c(l);
	    labels(k)= classifiers(k)->classify(f, &(probabilities2(k)));
	  }
	}

        return decide();
    }
    
    template<typename T>
    int EnsembleClassifier<T>::classifyVaryingDimensions(int k, Vector<float>* probabilities)
    {
        if ( probabilities )
            p= probabilities;
        else
            p= &(this->probabilities);
        if ( classifiers.size() == 0 )
            return 0;

        for ( unsigned int i= 0; i < classifiers.size(); ++i )
            labels(i)= classifiers(i)->classify(fdatas(i)(k), &(probabilities2(i)));

        return decide();
    }

    template<typename T>
    int EnsembleClassifier<T>::decide()
    {
        if ( decisionRule & ENSEMBLE_MAJORITY )
        {
            Vector<float> labelHist;
            labelHist.resize(classifiers(0)->classNames.size());
            labelHist= 0;
            for ( unsigned int i= 0; i < labels.size(); ++i )
                labelHist(labels(i))++;
            int max= 0;
            for ( unsigned int i= 0; i < labelHist.size(); ++i )
                if ( labelHist(max) < labelHist(i) )
                    max= i;
            labelHist/= classifiers.size();
            for ( unsigned int i= 0; i < labelHist.size(); ++i )
                p->operator()(i)= labelHist(i);

            return max;
        }
        return 0;
    }

    template<typename T>
    void EnsembleClassifier<T>::init()
    {
      masks.resize(classifiers.size());
      fdatas.resize(classifiers.size());

      for ( unsigned int k= 0; k < classifiers.size(); ++k )
      {
	masks(k).resize(featureNames.size());
	masks(k)= 0;
	for ( unsigned int i= 0; i < featureNames.size(); ++i )
	{
	  int flag= 0;
	  for ( unsigned int j= 0; j < classifiers(k)->featureNames.size(); ++j )
	    if ( featureNames(i).compare(classifiers(k)->featureNames(j)) == 0 )
	      flag= 1;
	  masks(k)(i)= flag;
	}
	cout << masks(k) << endl;
      }
    }
    
    template<typename T>
    float EnsembleClassifier<T>::evaluate(DataSet<T>& data, int measure)
    {
	tprintf("starting evaluation in ensemble classifier\n");
        Validator<T> v(measure);
        tprintf("init dataset\n");

	masks.resize(classifiers.size());
	fdatas.resize(classifiers.size());

	cout << "data features" << endl;
	for ( unsigned int i= 0; i < data.featureNames.size(); ++i )
	  cout << data.featureNames(i) << endl;
	for ( unsigned int k= 0; k < classifiers.size(); ++k )
	{
	  masks(k).resize(data(0).size());
	  masks(k)= 0;
	  cout << "classifier " << k << endl;
	  for ( unsigned int i= 0; i < classifiers(k)->featureNames.size(); ++i )
	    cout << classifiers(k)->featureNames(i) << endl;
	  for ( unsigned int i= 0; i < data.featureNames.size(); ++i )
	  {
	    int flag= 0;
	    for ( unsigned int j= 0; j < classifiers(k)->featureNames.size(); ++j )
	      if ( data.featureNames(i).compare(classifiers(k)->featureNames(j)) == 0 )
	      {
		cout << data.featureNames(i) << " compared to " << classifiers(k)->featureNames(j) << endl;
		flag= 1;
	      }
	    masks(k)(i)= flag;
	  }
	  cout << masks(k) << endl;
	}
	printf("aaa"); fflush(stdout);
	for ( unsigned int k= 0; k < classifiers.size(); ++k )
	{
	  fdatas(k).classNames= classifiers(k)->classNames;
	  fdatas(k).featureNames= classifiers(k)->featureNames;
	  fdatas(k).preprocessingNames= classifiers(k)->preprocessingNames;
	}
	printf("bbb"); fflush(stdout);
	for ( unsigned int k= 0; k < classifiers.size(); ++k )
	{
	  printf("fs: %zd\n", classifiers(k)->featureNames.size()); fflush(stdout);
	  FeatureVector<T> f;
	  f.resize(classifiers(k)->featureNames.size());
	  for ( unsigned int i= 0; i < data.size(); ++i )
	  {
	    int n= 0;
	    for ( unsigned int j= 0; j < data(i).size(); ++j )
	    {
	      if ( masks(k)(j) )
		f(n++)= data(i)(j);
	    }
	    //printf("%d ", n);
	    f.classIndex= data(i).classIndex;
	    fdatas(k).push_back(f);
	  }
	}
	printf("ccc"); fflush(stdout);
	for ( unsigned int k=0; k < fdatas.size(); ++k )
	  fdatas(k).init();
	
        tprintf("initialization finished\n");
        v.m.resize(fdatas(0).numberOfClasses, fdatas(0).numberOfClasses);

        tprintf("%d %d\n", fdatas(0).numberOfClasses, (int)(fdatas(0).size()));
        v.m= 0;
        tprintf("classification... (%d,%d)\n", v.m.rows, v.m.columns);
        int label;
        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            label= classifyVaryingDimensions(i);
            v.m(data(i).classIndex, label)= v.m(data(i).classIndex, label ) + 1;
        }
        tprintf("classification finished\n");

        v.dumpResults();

        return v.evaluate();
    }
    
    template<typename T>
    int EnsembleClassifier<T>::saveModelToFile(ofstream& file)
    {
        file << "EnsembleClassifier" << std::endl;
        file << classifiers.size() << std::endl;
        file << decisionRule << std::endl;
        char filename[100];
        for ( unsigned int i= 0; i < classifiers.size(); ++i )
        {
            sprintf(filename, "ensemble%02d.model", i);
            classifiers(i)->saveModelToFile(filename);
            file << filename << std::endl;
        }

        return 0;
    }

    template<typename T>
    int EnsembleClassifier<T>::openModelFromFile(ifstream& file)
    {
        std::string tmp;

        getline(file, tmp);

        if ( tmp.compare("EnsembleClassifier") == 0 )
        {
            //this->DiscreteClassifier<T>::openModelFromFile(file);

            int num= 0;
            file >> num;
            tprintf("%d\n", num);
            file >> decisionRule;
            tprintf("%d\n", decisionRule);
            classifiers.resize(num);
            ClassifierIO<T> cio;
            std::string line;
            getline(file, line);
            for ( int i= 0; i < num; ++i )
            {
                getline(file, line);
                std::string filename= prefix;
                filename+= line;
                cout << filename << std::endl;
                classifiers(i)= cio.openClassifierFromFile(filename);
                cout << filename << " opened" << std::endl;
            }

            this->classNames= classifiers(0)->classNames;
            this->featureNames= classifiers(0)->featureNames;
            this->preprocessingNames= classifiers(0)->preprocessingNames;

            labels.resize(classifiers.size());
            probabilities2.resize(classifiers.size());
            probabilities.resize(classifiers(0)->classNames.size());

            for ( unsigned int i= 0; i < probabilities2.size(); ++i )
                probabilities2(i).resize(classNames.size());

	    init();
            tprintf("open finished\n");
            return 0;
        }

        return 1;
    }
}

#endif // ENSEMBLECLASSIFIER_H
