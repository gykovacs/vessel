/* 
 * File:   DecisionTreeClassifier.h
 * Author: gykovacs
 *
 * Created on November 4, 2011, 1:58 PM
 */

#ifndef DECISIONTREECLASSIFIER_H
#define	DECISIONTREECLASSIFIER_H

#include <openipDS/DataSet.h>
#include <openipDS/mathFunctions.h>

#include <openipML/BinaryDecisionTreeNode.h>
#include <openipML/Classifier.h>

namespace openip
{
    struct SortingRecord
    {
    public:
        SortingRecord();
        
        SortingRecord(float d, int c);
        
        float data;
        int classIndex;
    };
    
    bool sortByData(const SortingRecord& lhs, const SortingRecord& rhs);
    
    template<typename T>
    class DecisionTreeClassifier: public Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;
        using Classifier<T>::saveModelToFile;
        using Classifier<T>::openModelFromFile;
	using Classifier<T>::unknown;
	using Classifier<T>::unknownSet;
	using Classifier<T>::classNames;

        /**
         * default constructor
         */
        DecisionTreeClassifier(int unknown= 28);

        /**
         * copy constructor
         * @param c instance to copy
         */
        DecisionTreeClassifier(const DecisionTreeClassifier& c);

        /**
         * destructor
         */
        ~DecisionTreeClassifier();

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
        
        virtual float informationGain(DataSet<T>& data, Vector<unsigned char>& mask, int coordinate, T& threshold);
        
        void determineSplit(DataSet<T>& data, Vector<unsigned char>& mask, int& attribute, T& threshold);
        
        void buildTree(BinaryDecisionTreeNode<T>* &root, DataSet<T>& data, Vector<unsigned char>& mask);

        virtual float conditionalH(Vector<int>& classFrequencies, Vector<int>& xlt, Matrix<int>& cxlt, int numberOfClasses, int numberOfVectors, int j);
        
        int getLabel(BinaryDecisionTreeNode<T>* node, Vector<T>& c);

        void deleteTree(BinaryDecisionTreeNode<T>** root);
        
        /**
         * saves the model to filestream
         * @param file output filestream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveModelToFile(std::ofstream& file);

        virtual int writeTreeIntoFile(BinaryDecisionTreeNode<T>* p, std::ofstream& file);

        /**
         * opens the model from filestream
         * @param file input filestream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openModelFromFile(std::ifstream& file);

        virtual int readTreeFromFile(BinaryDecisionTreeNode<T>** p, std::ifstream& file);

        BinaryDecisionTreeNode<T>* root;

        T oldThreshold;
        int oldCoordinate;
	Vector<int> unknownLabels;
	Vector<float> unknownWeights;
    };
    
    template<typename T>
    DecisionTreeClassifier<T>::DecisionTreeClassifier(int unknown)
    : Classifier<T>(unknown)
    {
        root= NULL;
    }

    template<typename T>
    DecisionTreeClassifier<T>::DecisionTreeClassifier(const DecisionTreeClassifier& c)
    : Classifier<T>(c)
    {
        root= c.root;
    }

    template<typename T>
    DecisionTreeClassifier<T>::~DecisionTreeClassifier()
    {
        deleteTree(&root);
    }

    template<typename T>
    void DecisionTreeClassifier<T>::deleteTree(BinaryDecisionTreeNode<T> **root)
    {
        //printf("delete tree %p\n", *root); fflush(stdout);
        if ( *root )
        {
            if ( (*root)->childrenNodes[0] )
                deleteTree(&((*root)->childrenNodes[0]));
            if ( (*root)->childrenNodes[1] )
                deleteTree(&((*root)->childrenNodes[1]));
            delete *root;
            *root= NULL;
        }
    }

    template<typename T>
    int DecisionTreeClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
        tprintf("train dtc %lu %lu %lu\n", data.classNames.size(), data.featureNames.size(), data.preprocessingNames.size()); fflush(stdout);
        deleteTree(&root);

        this->classNames= data.classNames;
        this->featureNames= data.featureNames;
        this->preprocessingNames= data.preprocessingNames;

        oldThreshold= FLT_MAX;
        oldCoordinate= -1;

        if ( mask )
        {
            DataSet<T> tmp;
            for ( unsigned int i= 0; i < mask->size(); ++i )
                if ( (*mask)(i) )
                    tmp.push_back(data(i));
            Vector<unsigned char> tmpMask(tmp.size());
            tmpMask= 1;
            
            tmp.init();
            
            buildTree(root, tmp, tmpMask);
        }
        else
        {
            Vector<unsigned char> tmpMask(data.size());
            tmpMask= 1;
            
            data.init();
            
            buildTree(root, data, tmpMask);
        }
        //printf("trining finished "); fflush(stdout);
        return 0;
    }

    template<typename T>
    int DecisionTreeClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        if ( unknownSet )
	  unknownLabels.clear();
        if ( probabilities == NULL )
            probabilities= &(this->probabilities);
        return getLabel(root, c);
    }

    template<typename T>
    int DecisionTreeClassifier<T>::getLabel(BinaryDecisionTreeNode<T>* node, Vector<T>& c)
    {
        if ( node->terminal )
            return node->label;
	if ( unknownSet && c(node->attribute) == unknown )
	{
	  unknownLabels.push_back(getLabel(node->left(), c));
	  unknownLabels.push_back(getLabel(node->right(), c));
	  
	  Vector<int> majority;
	  majority.resize(classNames.size());
	  majority= 0;
	  for ( unsigned int i= 0; i < unknownLabels.size(); ++i )
	    majority(unknownLabels(i))++;
	  int maxIdx= 0;
	  for ( unsigned int i= 0; i < majority.size(); ++i )
	    if ( majority(i) > majority(maxIdx)/* || (majority(i) == majority(maxIdx) && majority(i) > majority(maxIdx))*/ )
	      maxIdx= i;
	  return maxIdx;
	}
        if ( c(node->attribute) < node->threshold )
            return getLabel(node->left(), c);
        else
            return getLabel(node->right(), c);
    }
    
    template<typename T>
    void DecisionTreeClassifier<T>::buildTree(BinaryDecisionTreeNode<T>* &root, DataSet<T>& data, Vector<unsigned char>& mask)
    {
        int firstLabel= -1;
        Vector<T> firstValue;
        bool terminal= true;
        bool unique= true;
        for ( unsigned int i= 0; i < mask.size(); ++i )
        {
            if ( mask(i) && firstLabel == -1 )
            {
                firstLabel= data(i).classIndex;
                firstValue= data(i);
            }
            if ( firstLabel != -1 && mask(i) && data(i).classIndex != firstLabel)
            {
                terminal= false;
            }
            if ( firstLabel != -1 && mask(i) && data(i) != firstValue )
            {
                unique= false;
            }
        }
        
        if ( unique )
        {
            Vector<int> labels(data.numberOfClasses);
	    int n= 0;
            for ( unsigned int i= 0; i < mask.size(); ++i )
                if ( mask(i) )
		{
                    labels(data(i).classIndex)++;
		    ++n;
		}
            int maxLabel= 0;
            for ( unsigned int i= 0; i < labels.size(); ++i )
                if ( labels(maxLabel) < labels(i) )
                    maxLabel= i;
            root= new BinaryDecisionTreeNode<T>(true, maxLabel);
	    root->weight= double(maxLabel)/double(n);
        }
        else if ( terminal )
        {
            root= new BinaryDecisionTreeNode<T>(true, firstLabel);
        }
        else
        {
            int attribute;
            T threshold;
            determineSplit(data, mask, attribute, threshold);

            if ( attribute == oldCoordinate && threshold == oldThreshold )
            {
                Vector<int> labels(data.numberOfClasses);
                for ( unsigned int i= 0; i < mask.size(); ++i )
                    if ( mask(i) )
                        labels(data(i).classIndex)++;
                int maxLabel= 0;
                for ( unsigned int i= 0; i < labels.size(); ++i )
                    if ( labels(maxLabel) < labels(i) )
                        maxLabel= i;
                root= new BinaryDecisionTreeNode<T>(true, labels(maxLabel));

                return;
            }
            oldCoordinate= attribute;
            oldThreshold= threshold;

            root= new BinaryDecisionTreeNode<T>(false, attribute, threshold);

            Vector<unsigned char> maskLeft(data.size());
            Vector<unsigned char> maskRight(data.size());
            int left= 0, right= 0;
            for ( unsigned int i= 0; i < mask.size(); ++i )
                if ( mask(i) && data(i)(attribute) <= threshold )
                {
                    maskLeft(i)= 1;
                    maskRight(i)= 0;
                    ++left;
                }
                else if ( mask(i) && data(i)(attribute) > threshold )
                {
                    maskRight(i)= 1;
                    maskLeft(i)= 0;
                    ++right;
                }
                else
                {
                    maskLeft(i)= maskRight(i)= 0;
                }
            buildTree(root->childrenNodes[0], data, maskLeft);
            buildTree(root->childrenNodes[1], data, maskRight);
        }

    }
    
    template<typename T>
    void DecisionTreeClassifier<T>::determineSplit(DataSet<T>& data, Vector<unsigned char>& mask, int& attribute, T& threshold)
    {
        //printf("f"); fflush(stdout);
        int bestAttribute= 0;
        float bestThreshold= 0;
        float bestIG= FLT_MAX;
        
        float tmpThreshold;
        float tmpIG;
        
        for ( unsigned int i= 0; i < data(0).size(); ++i )
        {
            //printf("ig "); fflush(stdout);
            tmpIG= informationGain(data, mask, i, tmpThreshold);
            //printf("eig "); fflush(stdout);
            if ( tmpIG < bestIG )
            {
                bestIG= tmpIG;
                bestAttribute= i;
                bestThreshold= tmpThreshold;
            }
        }
        attribute= bestAttribute;
        threshold= bestThreshold;
        //printf("g"); fflush(stdout);
    }
    
    template<typename T>
    float DecisionTreeClassifier<T>::informationGain(DataSet<T>& data, Vector<unsigned char>& mask, int coordinate, T& threshold)
    {
        int numberOfVectors= 0;
        for ( unsigned int i= 0; i < mask.size(); ++i )
            if ( mask(i) && (!unknownSet || data(i)(coordinate != unknown)) )
                ++numberOfVectors;
        //printf("%d ", numberOfVectors); fflush(stdout);
        
        Vector<SortingRecord> sample(numberOfVectors);
        Vector<int> classFrequencies(data.numberOfClasses);
        classFrequencies= 0;
        int s= 0;
        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            if ( mask(i) && (!unknownSet || data(i)(coordinate) != unknown) )
            {
                sample(s++)= SortingRecord(data(i)(coordinate), data(i).classIndex);
                classFrequencies(data(i).classIndex)++;
            }
        }

        sort(sample.begin(), sample.end(), sortByData);
        
        Vector<T> thresholds;
        for ( int i= 0; i < numberOfVectors-1; ++i )
            if ( sample(i).data != sample(i+1).data )
                thresholds.push_back((sample(i).data + sample(i+1).data)/2.0);
        if ( thresholds.size() == 0 )
        {
            return FLT_MAX;
        }
        //printf("a%d ", thresholds.size()); fflush(stdout);
        Vector<int> xlt(thresholds.size());
        Matrix<int> cxlt(thresholds.size(), data.numberOfClasses);
        xlt= 0;
        cxlt= 0;
        unsigned int j= 0;
        for ( int i= 0; i < numberOfVectors && j < thresholds.size(); ++i )
            if ( sample(i).data < thresholds(j) )
            {
                //printf("."); fflush(stdout);
                xlt(j)++;
                cxlt(j,sample(i).classIndex)++;
            }
            else
            {
                //printf("_"); fflush(stdout);
                while ( j < thresholds.size() - 1 && sample(i).data >= thresholds(j) )
                {
                    xlt(j+1)= xlt(j);
                    for ( int k= 0; k < data.numberOfClasses; ++k )
                        cxlt(j+1,k)= cxlt(j,k);
                    ++j;
                }
                if ( j < thresholds.size() - 1 && sample(i).data < thresholds(j) )
                {
                    xlt(j)++;
                    cxlt(j,sample(i).classIndex)++;
                }
            }
                    
        float minIG= FLT_MAX;
        float tmpIG;
        int bestThreshold= 0;
        //printf("h"); fflush(stdout);
        for ( unsigned int i= 0; i < thresholds.size(); ++i )
        {
            //printf("H "); fflush(stdout);
            tmpIG= conditionalH(classFrequencies, xlt, cxlt, data.numberOfClasses, numberOfVectors, i);
            //printf("eH "); fflush(stdout);
            //printf("threshold: %d, IG: %f\n", i, tmpIG);
            if ( tmpIG < minIG )
            {
                minIG= tmpIG;
                bestThreshold= i;
            }
        }
        //printf("i"); fflush(stdout);
        //printf("[%f,%f] ", bestThreshold, minIG);
        //printf("[%d,%f", thresholds.size(), sample(0).data); fflush(stdout);
        threshold= thresholds(bestThreshold);
        return minIG;
    }
    
    template<typename T>
    float DecisionTreeClassifier<T>::conditionalH(Vector<int>& classFrequencies, Vector<int>& xlt, Matrix<int>& cxlt, int numberOfClasses, int numberOfVectors, int j)
    {
        float HYXjlt= 0;
        for ( int i= 0; i < numberOfClasses; ++i )
            if ( cxlt(j,i) > 0 && xlt(j) != 0 && float((cxlt(j,i))/float(xlt(j))) > 0 )
                HYXjlt+= -float(cxlt(j,i))/float(xlt(j))*log2(float(cxlt(j,i))/float(xlt(j)));
        //printf("."); fflush(stdout);
        float HYXjgt= 0;
        for ( int i= 0; i < numberOfClasses; ++i )
        {
            //printf("%d,%d", cxlt(j,i), xlt(j));
            if ( (classFrequencies(i) - cxlt(j,i) > 0) && ((numberOfVectors - xlt(j)) != 0) && (float(classFrequencies(i) - cxlt(j,i))/float(numberOfVectors - xlt(j))) > 0)
                HYXjgt+= -float(classFrequencies(i) - cxlt(j,i))/float(numberOfVectors - xlt(j))*log2(float(classFrequencies(i) - cxlt(j,i))/float(numberOfVectors - xlt(j)));
        }

        //printf(","); fflush(stdout);
        return HYXjlt + HYXjgt;
    }
    
    template<typename T>
    int DecisionTreeClassifier<T>::saveModelToFile(std::ofstream& file)
    {
        file << "DecisionTreeClassifier" << std::endl;
        this->Classifier<T>::saveModelToFile(file);

        writeTreeIntoFile(root, file);

        return 0;
    }

    template<typename T>
    int DecisionTreeClassifier<T>::writeTreeIntoFile(BinaryDecisionTreeNode<T>* p, std::ofstream& file)
    {
        if ( p == 0 )
        {
            file << "0" << std::endl;
        }
        else
        {
            file << "1 " << p->attribute << " " << p->label << " " << p->numberOfChildren << " " << p->terminal << " " << p->threshold << std::endl;
            writeTreeIntoFile(p->left(), file);
            writeTreeIntoFile(p->right(), file);
        }

        return 0;
    }

    template<typename T>
    int DecisionTreeClassifier<T>::openModelFromFile(std::ifstream& file)
    {
        std::string tmp;

        tprintf("dtc: open model from file\n");
        getline(file, tmp);

        if ( tmp.compare("DecisionTreeClassifier") == 0 )
        {
            tprintf("dtc: open: in if\n");
            this->Classifier<T>::openModelFromFile(file);
            readTreeFromFile(&root, file);

            return 0;
        }

        probabilities.resize(this->classNames.size());
        tprintf("decision tree read successfully\n");
        return 1;
    }

    template<typename T>
    int DecisionTreeClassifier<T>::readTreeFromFile(BinaryDecisionTreeNode<T>** p, std::ifstream &file)
    {
        //tprintf("read tree from file\n");
        //bool isNumber;

        if ( file.eof() )
        {
            *p= NULL;
            return 0;
        }

        int token;
        file >> token;

        int attribute;
        int label;
        bool terminal;
        int numberOfChildren;
        T threshold;

        if ( token == 0 )
        {
            *p= 0;
            return 0;
        }
        else
        {
            file >> attribute;
            file >> label;
            file >> numberOfChildren;
            file >> terminal;
            file >> threshold;

            *p= new BinaryDecisionTreeNode<T>(terminal, attribute, threshold);
            (*p)->label= label;
            (*p)->numberOfChildren= numberOfChildren;

            readTreeFromFile(&((*p)->childrenNodes[0]), file);
            readTreeFromFile(&((*p)->childrenNodes[1]), file);
        }

        return 0;
    }

}

#endif	/* DECISIONTREECLASSIFIER_H */

