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

#ifndef EPSILONSVMCLASSIFIER_H
#define	EPSILONSVMCLASSIFIER_H

#include <openipML/Classifier.h>
#include <openipML/svm.h>
#include <openipML/Normalizer.h>
#include <openipML/KernelFunctionIO.h>

using namespace svm;

namespace openip
{
    /**
     * SVMClassifier represents the Support Vector Machine classification method, based on libsvm.
     */
    template<typename T>
    class EpsilonSVMClassifier: public virtual Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;

        /**
         * default constructor
         */
        EpsilonSVMClassifier(int kernelType= RBF, int degree= 3, double gamma= 1.0, double C= 1.0, double coef0= 1.0, double cacheSize= 100, double eps= 0.01, bool norm= true, KernelFunction<double>* kernel= NULL);

        /**
         * copy constructor
         * @param s instance to copy
         */
        EpsilonSVMClassifier(const EpsilonSVMClassifier& s);

        /**
         * destructor
         */
        ~EpsilonSVMClassifier();

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
        KernelFunction<double> *kernel;
    };

    template<typename T>
    EpsilonSVMClassifier<T>::EpsilonSVMClassifier(int kernelType, int degree, double gamma, double C, double coef0, double cacheSize, double eps, bool norm, KernelFunction<double>* kernel)
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
        if ( kernel == NULL )
            this->kernel= openipKernel;
        else
            this->kernel= kernel;
    }

    template<typename T>
    EpsilonSVMClassifier<T>::EpsilonSVMClassifier(const EpsilonSVMClassifier& s)
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
    EpsilonSVMClassifier<T>::~EpsilonSVMClassifier()
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
    int EpsilonSVMClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
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

        //tprintf("train finished 2\n");

        return 0;
    }

    template<typename T>
    void EpsilonSVMClassifier<T>::destroyProblem()
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
    int EpsilonSVMClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
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
    int EpsilonSVMClassifier<T>::saveModelToFile(const char* filename)
    {
        strcpy(this->filename, filename);
        std::ofstream file;
        file.open("_svm_tmp_model.txt_a");
        saveModelToFile(file);
        return 0;
    }

    template<typename T>
    int EpsilonSVMClassifier<T>::openModelFromFile(const char* filename)
    {
        std::ifstream file;
        file.open(filename);
        openModelFromFile(file);
        
        return 0;
    }

    template<typename T>
    int EpsilonSVMClassifier<T>::saveModelToFile(ofstream& file)
    {
        file << "EpsilonSVMClassifier" << std::endl;
        this->Classifier<T>::saveModelToFile(file);
        KernelFunctionIO<double> kfio;
        printf("aaa"); fflush(stdout);
        kfio.saveKernelFunctionToFile(kernel, file);
        printf("bbb"); fflush(stdout);
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
    int EpsilonSVMClassifier<T>::openModelFromFile(ifstream& file)
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
        remove("_svm_tmp_model.txt_b");
        
        return 0;
    }

    /*int svm_save_model(FILE *fp, const svm_model *model)
    {
	//FILE *fp = fopen(model_file_name,"w");
	if(fp==NULL) return -1;

	const svm_parameter& param = model->param;

	fprintf(fp,"svm_type %s\n", svm_type_table[param.svm_type]);
	fprintf(fp,"kernel_type %s\n", kernel_type_table[param.kernel_type]);

	if(param.kernel_type == POLY)
		fprintf(fp,"degree %d\n", param.degree);

	if(param.kernel_type == POLY || param.kernel_type == RBF || param.kernel_type == SIGMOID)
		fprintf(fp,"gamma %g\n", param.gamma);

	if(param.kernel_type == POLY || param.kernel_type == SIGMOID)
		fprintf(fp,"coef0 %g\n", param.coef0);

	int nr_class = model->nr_class;
	int l = model->l;
	fprintf(fp, "nr_class %d\n", nr_class);
	fprintf(fp, "total_sv %d\n",l);

	{
		fprintf(fp, "rho");
		for(int i=0;i<nr_class*(nr_class-1)/2;i++)
			fprintf(fp," %g",model->rho[i]);
		fprintf(fp, "\n");
	}

	if(model->label)
	{
		fprintf(fp, "label");
		for(int i=0;i<nr_class;i++)
			fprintf(fp," %d",model->label[i]);
		fprintf(fp, "\n");
	}

	if(model->probA) // regression has probA only
	{
		fprintf(fp, "probA");
		for(int i=0;i<nr_class*(nr_class-1)/2;i++)
			fprintf(fp," %g",model->probA[i]);
		fprintf(fp, "\n");
	}
	if(model->probB)
	{
		fprintf(fp, "probB");
		for(int i=0;i<nr_class*(nr_class-1)/2;i++)
			fprintf(fp," %g",model->probB[i]);
		fprintf(fp, "\n");
	}

	if(model->nSV)
	{
		fprintf(fp, "nr_sv");
		for(int i=0;i<nr_class;i++)
			fprintf(fp," %d",model->nSV[i]);
		fprintf(fp, "\n");
	}

	fprintf(fp, "SV\n");
	const double * const *sv_coef = model->sv_coef;
	const svm_node * const *SV = model->SV;

	for(int i=0;i<l;i++)
	{
		for(int j=0;j<nr_class-1;j++)
			fprintf(fp, "%.16g ",sv_coef[j][i]);

		const svm_node *p = SV[i];

		if(param.kernel_type == PRECOMPUTED)
			fprintf(fp,"0:%d ",(int)(p->value));
		else
			while(p->index != -1)
			{
				fprintf(fp,"%d:%.8g ",p->index,p->value);
				p++;
			}
		fprintf(fp, "\n");
	}
	if (ferror(fp) != 0 || fclose(fp) != 0) return -1;
	else return 0;
    }*/

    /*static char *line = NULL;
    static int max_line_len;

    static char* readline(FILE *input)
    {
            int len;

            if(fgets(line,max_line_len,input) == NULL)
                    return NULL;

            while(strrchr(line,'\n') == NULL)
            {
                    max_line_len *= 2;
                    line = (char *) realloc(line,max_line_len);
                    len = (int) strlen(line);
                    if(fgets(line+len,max_line_len-len,input) == NULL)
                            break;
            }
            return line;
    }

    svm_model *svm_load_model(const char *model_file_name)
    {
	FILE *fp = fopen(model_file_name,"rb");
	if(fp==NULL) return NULL;

	// read parameters

	svm_model *model = Malloc(svm_model,1);
	svm_parameter& param = model->param;
	model->rho = NULL;
	model->probA = NULL;
	model->probB = NULL;
	model->label = NULL;
	model->nSV = NULL;

	char cmd[81];
	while(1)
	{
		fscanf(fp,"%80s",cmd);

		if(strcmp(cmd,"svm_type")==0)
		{
			fscanf(fp,"%80s",cmd);
			int i;
			for(i=0;svm_type_table[i];i++)
			{
				if(strcmp(svm_type_table[i],cmd)==0)
				{
					param.svm_type=i;
					break;
				}
			}
			if(svm_type_table[i] == NULL)
			{
				fprintf(stderr,"unknown svm type.\n");
				free(model->rho);
				free(model->label);
				free(model->nSV);
				free(model);
				return NULL;
			}
		}
		else if(strcmp(cmd,"kernel_type")==0)
		{
			fscanf(fp,"%80s",cmd);
			int i;
			for(i=0;kernel_type_table[i];i++)
			{
				if(strcmp(kernel_type_table[i],cmd)==0)
				{
					param.kernel_type=i;
					break;
				}
			}
			if(kernel_type_table[i] == NULL)
			{
				fprintf(stderr,"unknown kernel function.\n");
				free(model->rho);
				free(model->label);
				free(model->nSV);
				free(model);
				return NULL;
			}
		}
		else if(strcmp(cmd,"degree")==0)
			fscanf(fp,"%d",&param.degree);
		else if(strcmp(cmd,"gamma")==0)
			fscanf(fp,"%lf",&param.gamma);
		else if(strcmp(cmd,"coef0")==0)
			fscanf(fp,"%lf",&param.coef0);
		else if(strcmp(cmd,"nr_class")==0)
			fscanf(fp,"%d",&model->nr_class);
		else if(strcmp(cmd,"total_sv")==0)
			fscanf(fp,"%d",&model->l);
		else if(strcmp(cmd,"rho")==0)
		{
			int n = model->nr_class * (model->nr_class-1)/2;
			model->rho = Malloc(double,n);
			for(int i=0;i<n;i++)
				fscanf(fp,"%lf",&model->rho[i]);
		}
		else if(strcmp(cmd,"label")==0)
		{
			int n = model->nr_class;
			model->label = Malloc(int,n);
			for(int i=0;i<n;i++)
				fscanf(fp,"%d",&model->label[i]);
		}
		else if(strcmp(cmd,"probA")==0)
		{
			int n = model->nr_class * (model->nr_class-1)/2;
			model->probA = Malloc(double,n);
			for(int i=0;i<n;i++)
				fscanf(fp,"%lf",&model->probA[i]);
		}
		else if(strcmp(cmd,"probB")==0)
		{
			int n = model->nr_class * (model->nr_class-1)/2;
			model->probB = Malloc(double,n);
			for(int i=0;i<n;i++)
				fscanf(fp,"%lf",&model->probB[i]);
		}
		else if(strcmp(cmd,"nr_sv")==0)
		{
			int n = model->nr_class;
			model->nSV = Malloc(int,n);
			for(int i=0;i<n;i++)
				fscanf(fp,"%d",&model->nSV[i]);
		}
		else if(strcmp(cmd,"SV")==0)
		{
			while(1)
			{
				int c = getc(fp);
				if(c==EOF || c=='\n') break;
			}
			break;
		}
		else
		{
			fprintf(stderr,"unknown text in model file: [%s]\n",cmd);
			free(model->rho);
			free(model->label);
			free(model->nSV);
			free(model);
			return NULL;
		}
	}

	// read sv_coef and SV

	int elements = 0;
	long pos = ftell(fp);

	max_line_len = 1024;
	line = Malloc(char,max_line_len);
	char *p,*endptr,*idx,*val;

	while(readline(fp)!=NULL)
	{
		p = strtok(line,":");
		while(1)
		{
			p = strtok(NULL,":");
			if(p == NULL)
				break;
			++elements;
		}
	}
	elements += model->l;

	fseek(fp,pos,SEEK_SET);

	int m = model->nr_class - 1;
	int l = model->l;
	model->sv_coef = Malloc(double *,m);
	int i;
	for(i=0;i<m;i++)
		model->sv_coef[i] = Malloc(double,l);
	model->SV = Malloc(svm_node*,l);
	svm_node *x_space = NULL;
	if(l>0) x_space = Malloc(svm_node,elements);

	int j=0;
	for(i=0;i<l;i++)
	{
		readline(fp);
		model->SV[i] = &x_space[j];

		p = strtok(line, " \t");
		model->sv_coef[0][i] = strtod(p,&endptr);
		for(int k=1;k<m;k++)
		{
			p = strtok(NULL, " \t");
			model->sv_coef[k][i] = strtod(p,&endptr);
		}

		while(1)
		{
			idx = strtok(NULL, ":");
			val = strtok(NULL, " \t");

			if(val == NULL)
				break;
			x_space[j].index = (int) strtol(idx,&endptr,10);
			x_space[j].value = strtod(val,&endptr);

			++j;
		}
		x_space[j++].index = -1;
	}
	free(line);

	if (ferror(fp) != 0 || fclose(fp) != 0)
		return NULL;

	model->free_sv = 1;	// XXX
	return model;
    }*/
}

#endif	/* SVMCLASSIFIER_H */

