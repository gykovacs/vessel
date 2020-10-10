#ifndef _SEQUENTIAL_SV_REGRESSION_H_
#define _SEQUENTIAL_SV_REGRESSION_H_

#include <openipDS/Vector.h>
#include <openipDS/RegressionDataSet.h>

#include <openipML/KernelFunctions.h>
#include <openipML/KernelFunctionIO.h>
#include <openipML/Normalizer.h>
#include <openipML/Regression.h>

#include <CL/opencl.h>

#include <omp.h>

#define ERROR(a,b) {if(a != CL_SUCCESS) errorMessage(stdout, a, b);}

void errorMessage(FILE* out, int err, const char* prefix)
{
  switch(err)
  {
    case CL_DEVICE_NOT_FOUND: fprintf(out, "%s: Device not found.\n", prefix); break;
    case CL_DEVICE_NOT_AVAILABLE: fprintf(out, "%s: Device not available.\n", prefix); break;
    case CL_COMPILER_NOT_AVAILABLE: fprintf(out, "%s: Compiler not available.\n", prefix); break;
    case CL_MEM_OBJECT_ALLOCATION_FAILURE: fprintf(out, "%s: Memory object application failure.\n", prefix); break;
    case CL_OUT_OF_RESOURCES: fprintf(out, "%s: Out of resources.\n", prefix); break;
    case CL_OUT_OF_HOST_MEMORY: fprintf(out, "%s: Out of host memory.\n", prefix); break;
    case CL_PROFILING_INFO_NOT_AVAILABLE: fprintf(out, "%s: Profiling information not available.\n", prefix); break;
    case CL_MEM_COPY_OVERLAP: fprintf(out, "%s: Memory copy overlap.\n", prefix); break;
    case CL_IMAGE_FORMAT_MISMATCH: fprintf(out, "%s: Image format mismatch.\n", prefix); break;
    case CL_IMAGE_FORMAT_NOT_SUPPORTED: fprintf(out, "%s: Image format not supported.\n", prefix); break;
    case CL_BUILD_PROGRAM_FAILURE: fprintf(out, "%s: Program build failure.\n", prefix); break;
    case CL_MAP_FAILURE: fprintf(out, "%s: Map failure.\n", prefix); break;
    case CL_INVALID_VALUE: fprintf(out, "%s: Invalid value.\n", prefix); break;
    case CL_INVALID_DEVICE_TYPE: fprintf(out, "%s: Invalid device type.\n", prefix); break;
    case CL_INVALID_PLATFORM: fprintf(out, "%s: Invalid platform.\n", prefix); break;
    case CL_INVALID_DEVICE: fprintf(out, "%s: Invalid device.\n", prefix); break;
    case CL_INVALID_CONTEXT: fprintf(out, "%s: Invalid context.\n", prefix); break;
    case CL_INVALID_QUEUE_PROPERTIES: fprintf(out, "%s: Invalid queue properties.\n", prefix); break;
    case CL_INVALID_COMMAND_QUEUE: fprintf(out, "%s: Invalid command queue.\n", prefix); break;
    case CL_INVALID_HOST_PTR: fprintf(out, "%s: Invalid host pointer.\n", prefix); break;
    case CL_INVALID_MEM_OBJECT: fprintf(out, "%s: Invalid memory object.\n", prefix); break;
    case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: fprintf(out, "%s: Invalid image format descriptor.\n", prefix); break;
    case CL_INVALID_IMAGE_SIZE: fprintf(out, "%s: Invalid image size.\n", prefix); break;
    case CL_INVALID_SAMPLER: fprintf(out, "%s: Invalid sampler.\n", prefix); break;
    case CL_INVALID_BINARY: fprintf(out, "%s: Invalid binary.\n", prefix); break;
    case CL_INVALID_BUILD_OPTIONS: fprintf(out, "%s: Invalid build options.\n", prefix); break;
    case CL_INVALID_PROGRAM: fprintf(out, "%s: Invalid program.\n", prefix); break;
    case CL_INVALID_PROGRAM_EXECUTABLE: fprintf(out, "%s: Invalid program executable.\n", prefix); break;
    case CL_INVALID_KERNEL_NAME: fprintf(out, "%s: Invalid kernel name.\n", prefix); break;
    case CL_INVALID_KERNEL_DEFINITION: fprintf(out, "%s: Invalid kernel definition.\n", prefix); break;
    case CL_INVALID_KERNEL: fprintf(out, "%s: Invalid kernel.\n", prefix); break;
    case CL_INVALID_ARG_INDEX: fprintf(out, "%s: Invalid argument index.\n", prefix); break;
    case CL_INVALID_ARG_VALUE: fprintf(out, "%s: Invalid argument value.\n", prefix); break;
    case CL_INVALID_ARG_SIZE: fprintf(out, "%s: Invalid argument size.\n", prefix); break;
    case CL_INVALID_KERNEL_ARGS: fprintf(out, "%s: Invalid kernel arguments.\n", prefix); break;
    case CL_INVALID_WORK_DIMENSION: fprintf(out, "%s: Invalid work dimension.\n", prefix); break;
    case CL_INVALID_WORK_GROUP_SIZE: fprintf(out, "%s: Invalid work group size.\n", prefix); break;
    case CL_INVALID_WORK_ITEM_SIZE: fprintf(out, "%s: Invalid work item size.\n", prefix); break;
    case CL_INVALID_GLOBAL_OFFSET: fprintf(out, "%s: Invalid global offset.\n", prefix); break;
    case CL_INVALID_EVENT_WAIT_LIST: fprintf(out, "%s: Invalid event wait list.\n", prefix); break;
    case CL_INVALID_EVENT: fprintf(out, "%s: Invalid event.\n", prefix); break;
    case CL_INVALID_OPERATION: fprintf(out, "%s: Invalid operation.\n", prefix); break;
    case CL_INVALID_GL_OBJECT: fprintf(out, "%s: Invalid OpenGL object.\n", prefix); break;
    case CL_INVALID_BUFFER_SIZE: fprintf(out, "%s: Invalid buffer size.\n", prefix); break;
    case CL_INVALID_MIP_LEVEL: fprintf(out, "%s: Invalid mip-map level.\n", prefix); break;
    default: fprintf(out, "%s: Unknown error.\n", prefix); break;
  }
  fflush(out);
}

#define MAX_SOURCE_LENGTH 10000

void readSourceProgram(char const* filename, char** source, size_t* length)
{
  FILE* input= fopen(filename, "rt");
  *length= 0;
  *source= (char*)malloc(sizeof(char)*MAX_SOURCE_LENGTH);
  while ( !feof(input) )
  {
    char* ret= fgets(*source + *length, MAX_SOURCE_LENGTH, input);
    if ( ret == NULL )
        printf("error in fgets\n");
    *length= strlen(*source);
  }
  *source= (char*)realloc(*source, sizeof(char)*strlen(*source));
}

namespace openip
{
    template<typename T>
    class SequentialSVRegression: public Regression<T>
    {
    public:
        /**
         * default constructor
         */
        SequentialSVRegression(float C, float eps, float tol, KernelFunction<T>* kernel= NULL, KernelFunction<T>* crossKernel= NULL, int maxIteration= 1000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        SequentialSVRegression(const SequentialSVRegression& s);

        /**
         * destructor
         */
        ~SequentialSVRegression();

        /**
         * classifier training method
         * @param data training dataset
         * @param mask only those elements are trained from the dataset, which have a corresponding non-zero element in the mask
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int train(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL);
	
	virtual int trainWithPrecomputedKernel(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL, Vector<Vector<double>* >* precomputedKernel= NULL);

        /**
         * classification method
         * @param c vector to classify
         * @param probabilities probability estimation
         * @return predicted class index
         */
        virtual float regress(Vector<T>& c, Vector<float>* probabilities= NULL);
	
	virtual float regress(Vector<T>& c, int k);

	/**
	 * evaluate regression at vector
	 * @param c input vector
	 * @return response
	 */ 
        inline T evaluate(Vector<T>& c);
	
	inline T evaluate(Vector<T>& c, int k);
	
	/**
	 * compute gradient at sample i
	 * @param i index of gradient
	 * @return gradient value
	 */ 
	inline T gradient(int i);
	
	/**
	 * evaluate kernel at indices i, j
	 * @param i index
	 * @param j index
	 * @return kernel response
	 */ 
	inline T Q(unsigned int i, unsigned int j);
	
	/**
	 * evaluate p at index i
	 * @param i index
	 * @return p vector value
	 */ 
	inline T p(unsigned int i);

	/**
	 * get b offset value
	 * @return b value
	 */ 
        inline T b();
	
	/**
	 * compute b offset value
	 */
	virtual void computeB();
	
	virtual void computeBestB();
	
	/**
	 * select working set
	 * @param i output working set component
	 * @param j output working set component
	 */ 
	inline void selectWorkingSet(int& i, int& j);

	/**
	 * sequential minimization
	 * @param i working set index
	 * @param j working set index
	 * @return boolean success code
	 */ 
        inline int SMO(int i, int j);

	/**
	 * objective
	 * @return objective function value
	 */ 
        virtual T objective();
	
	/**
	 * extract final support vectors
	 */ 
	inline void extractSupportVectors();
	
	inline void extractBestSupportVectors();
	
	/**
	 * refill gradient cache
	 */ 
	inline void refillGradientCache();
	
	/**
	 * update updateGradients
	 */ 
	inline void updateGradients(double alpha_i_old, double alpha_j_old, int i, int j);
	
	/**
	 * fill cache
	 */ 
	inline void fillCache();
	
	/**
	 * init temporary vectors
	 */ 
	virtual void initTemporaryVectors();
	
	/**
	 * check stopping stopping condition
	 * @param iteration iteration value
	 */ 
	inline int stoppingCondition(int iteration);

	virtual void swapToBestModel();
	
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
	KernelFunction<T> *crossKernel;
	
        Vector<T> alpha;
        RegressionDataSet<T> data;
	
	Vector<T> supportVectorsAlpha;
	RegressionDataSet<T> supportVectors;
	Vector<T> bestSupportVectorsAlpha;
	RegressionDataSet<T> bestSupportVectors;
	
	Vector<double> y;
	
	Vector<Vector<double> > cache;
	Vector<Vector<double>* >* precomputedKernel;
	
	Vector<double> gradientCache;

        bool bInit;
        T bValue;
	T bestBValue;
        T eps;
        T tol;
        T C;
	int maxIteration;
	float gap;
    };
  
    template<typename T>
    SequentialSVRegression<T>::SequentialSVRegression(float C, float eps, float tol, KernelFunction<T>* kernel, KernelFunction<T>* crossKernel, int maxIteration)
        : Regression<T>()
    {
	this->kernel= kernel;
	
	if ( crossKernel != NULL )
	  this->crossKernel= crossKernel;
	
	if ( kernel != NULL && crossKernel == NULL )
	  this->crossKernel= kernel;
	
	
	this->C= C;
	bInit= false;
	bValue= 0;
	this->eps= eps;
	this->tol= tol;
	this->maxIteration= maxIteration;
	this->precomputedKernel= NULL;
	this->gap= 100000.0;
    }

    template<typename T>
    SequentialSVRegression<T>::SequentialSVRegression(const SequentialSVRegression& s)
        : Regression<T>(s)
    {
        KernelFunctionIO<T> kfio;
        this->kernel= kfio.createKernelFunctionFromString(s.kernel.descriptor);
	this->crossKernel= kfio.createKernelFunctionFromString(s.crossKernel.descriptor);
        this->C= s.C;
        this->eps= s.eps;
        this->tol= s.tol;
        this->kaa= s.kaa;
	this->maxIteration= s.maxIteration;
	this->precomputedKernel= s.precomputedKernel;
	this->gap= s.gap;
    }

    template<typename T>
    SequentialSVRegression<T>::~SequentialSVRegression()
    {
	/*if ( kernel != crossKernel && crossKernel != NULL )
	  delete crossKernel;
	if ( kernel != NULL )
	  delete kernel;*/
    }

    template<typename T>
    void SequentialSVRegression<T>::fillCache()
    {
      if ( dynamic_cast<PETKernel<T>* >(kernel) == NULL )
      {
	for ( unsigned int i= 0; i < data.size(); ++i )
	{
	  Vector<double> cacheline;
	  for ( unsigned int j= 0; j < data.size(); ++j )
	  {
	    cacheline.push_back(kernel->evaluate(data(i),data(j)));
	  }
	  cache.push_back(cacheline);
	}
	
	return;
      }
      int useOpenCL= 0;
      
      if ( ! useOpenCL )
      {
	  if ( precomputedKernel != NULL )
	  {
	    tprintf("precomputed kernel != NULL\n"); 
	    tprintf("precomputed kernel: %d %d\n", precomputedKernel->size(), precomputedKernel->operator()(0)->size());
	    
	    for ( unsigned int i= 0; i < precomputedKernel->size(); ++i )
	    {
	      cache.push_back(*(precomputedKernel->operator()(i)));
	      /*for ( int j= 0; j < precomputedKernel->operator()(i)->size(); ++j )
		printf("%f ", cache(i)(j));*/
	      delete precomputedKernel->operator()(i);
	    }
	    
	    return;
	  }
	  
	  // reading kernel cache
	  FILE* cacheFile;
	  string filename;
	  stringstream ss;
	  
	  int resolution= -1;
	  
	  if ( dynamic_cast<PETKernel<T>* >(kernel) != NULL )
	    resolution= dynamic_cast<PETKernel<T>* >(kernel)->resolution;
	  
	  ss << "kernel_cache_" << data.size() << "_" << resolution << ".data";
	  filename= ss.str();
	  cacheFile= fopen(filename.c_str(), "r");
	  
	  if ( cacheFile != NULL && precomputedKernel == NULL )
	  {
	    tprintf("reading cached kernel matrix\n");
	    int size= 0;
	    int ret= fscanf(cacheFile, "%d", &size);
            if ( !ret ) printf("error in fscanf\n");
	    double f;
	    for ( int i= 0; i < size; ++i )
	    {
	      Vector<double> cacheline;
	      for ( int j= 0; j < size; ++j )
	      {
		int ret= fscanf(cacheFile, "%lf", &f);
                if ( !ret ) printf("error in fscanf\n");
		cacheline.push_back(f);
	      }
	      cache.push_back(cacheline);
	    }
	    
	    fclose(cacheFile);
	    
	    tprintf("reading of kernel matrix finished\n");
	    
	    return;
	  }
	  
	  if ( precomputedKernel == NULL )
	  {
	  // computing kernel cache
	    cache.resize(data.size());
	    int n= 0;
	    #pragma omp parallel for shared(n), schedule(dynamic, 10)
	    for ( unsigned int i= 0; i < data.size(); ++i )
	    {
	      #pragma omp atomic
		++n;
	      if ( omp_get_thread_num() == 0 )
	      {
		printf("\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r");
		tprintf("%5d/%5d", n, data.size()); 
	      }
	      Vector<double> cacheline;
	      for ( unsigned int j= 0; j < i; ++j )
		cacheline.push_back(0);
	      for ( unsigned int j= i; j < data.size(); ++j )
	      {
		cacheline.push_back(kernel->evaluate(data(i), data(j)));
		if ( isnan(cacheline(cacheline.size()-1)) )
		{
		  printf("nan: %d, %d, %f, %f, %f, %f\n", i, j, data(i)(0), data(i)(1), data(j)(0), data(j)(1));
		}
	      }
	      cache(i)= cacheline;
	      /*if ( i % 100 == 0 )
	      {
		KernelFunction<float>* k2= new PETKernelMC<float>(resolution);
		
		for ( int j= i; j < data.size(); ++j )
		{
		  if ( j % 100 == 0 )
		  {
		    float tmp= k2->evaluate(data(i),data(j));
		    if ( fabs(cache(i)(j) - tmp) > 0.1 )
		      printf("%d,%d,%f,%f,%f,%f,%f,%f ", i, j, data(i)(0), data(i)(1), data(j)(0), data(j)(1), cache(i)(j), tmp);
		  }
		}
		printf("\n");
	      }*/
	      
	    }
	    
	    for ( unsigned int i= 0; i < data.size(); ++i )
	      for ( unsigned int j= 0; j < i; ++j )
		cache(i)(j)= cache(j)(i);
	  }
	      if ( precomputedKernel == NULL )
	      {
	  // writing out kernel cache
	  tprintf("writing kernel cache: %s\n", filename.c_str());
	  cacheFile= fopen(filename.c_str(), "w");
	  fprintf(cacheFile, "%zd\n", data.size());
	  for ( unsigned int i= 0; i < cache.size(); ++i )
	  {
	    for ( unsigned int j= 0; j < cache(i).size(); ++j )
	      fprintf(cacheFile, "%f ", cache(i)(j));
	    fprintf(cacheFile, "\n");
	  }
	  fclose(cacheFile);
	      }
      }
      else
      {
	FILE* cacheFile;
	string filename;
	stringstream ss;
	
	int resolution= -1;
	
	if ( dynamic_cast<PETKernel<T>* >(kernel) != NULL )
	  resolution= dynamic_cast<PETKernel<T>* >(kernel)->resolution;
	
	ss << "kernel_cache_" << data.size() << "_" << resolution << ".data";
	filename= ss.str();
	cacheFile= fopen(filename.c_str(), "r");
	
	if ( cacheFile == NULL )
	{
	  cl_int err;
	  cl_context context;
	  cl_command_queue queue;
	  cl_mem mem_random, mem_ms, mem_mus, mem_output, mem_smus, mem_cmus;
	  cl_event event;
	  cl_program program;
	  cl_kernel kernel;
	  cl_uint numDevices;
	  cl_device_id devices[10];
	  cl_platform_id platforms[10];
	  cl_context_properties properties[10*2]= {0};
	  cl_uint numPlatforms;
	  size_t size;
	  
	  err= clGetPlatformIDs(10, platforms, &numPlatforms);
	  ERROR(err, "clGetPlatformIDs")
	  
	  unsigned int i;
	  for ( i= 0; i < numPlatforms; ++i )
	  {
	    properties[i*2]= (cl_context_properties)CL_CONTEXT_PLATFORM;
	    properties[i*2+1]= (cl_context_properties)(platforms[i]);
	  }
	  properties[i*2]= 0,
	  
	  context= clCreateContextFromType(properties, CL_DEVICE_TYPE_GPU, NULL, NULL, &err);
	  ERROR(err, "clCreateContextFromType")
	  
	  err= clGetContextInfo(context, CL_CONTEXT_DEVICES, 10*sizeof(cl_device_id), devices, &size);
	  ERROR(err, "clGetContextInfo")
	  err= clGetContextInfo(context, CL_CONTEXT_NUM_DEVICES, sizeof(cl_uint), &numDevices, &size);
	  ERROR(err, "clGetContextInfo")
	  
	  queue= clCreateCommandQueue(context, devices[0], 0, &err);
	  ERROR(err, "clCreateCommandQueue")

	  const int RANDOM_NUMBERS= 20000000;
	  double* randomNumbers= (double*)malloc(sizeof(double)*RANDOM_NUMBERS);
	  for ( int i= 0; i < RANDOM_NUMBERS; ++i )
	    randomNumbers[i]= double(rand())/double(RAND_MAX);
	  
	  double* ms= (double*)malloc(sizeof(double)*data.size());
	  double* mus= (double*)malloc(sizeof(double)*data.size());
	  double* smus= (double*)malloc(sizeof(double)*data.size());
	  double* cmus= (double*)malloc(sizeof(double)*data.size());
	  double* output= (double*)malloc(sizeof(double)*data.size());
	  
	  for ( unsigned int i= 0; i < data.size(); ++i )
	  {
	    ms[i]= data(i)(0);
	    mus[i]= data(i)(1);
	    smus[i]= sin(mus[i]);
	    cmus[i]= cos(mus[i]);
	  }
	  
	  mem_random= clCreateBuffer(context, 0, RANDOM_NUMBERS*sizeof(double), NULL, &err);
	  ERROR(err, "clCreateBuffer")
	  mem_ms= clCreateBuffer(context, 0, data.size()*sizeof(double), NULL, &err);
	  ERROR(err, "clCreateBuffer")
	  mem_mus= clCreateBuffer(context, 0, data.size()*sizeof(double), NULL, &err);
	  ERROR(err, "clCreateBuffer")
	  mem_smus= clCreateBuffer(context, 0, data.size()*sizeof(double), NULL, &err);
	  ERROR(err, "clCreateBuffer")
	  mem_cmus= clCreateBuffer(context, 0, data.size()*sizeof(double), NULL, &err);
	  ERROR(err, "clCreateBuffer")
	  mem_output= clCreateBuffer(context, 0, data.size()*sizeof(double), NULL, &err);
	  ERROR(err, "clCreateBuffer")
	  
	  err= clEnqueueWriteBuffer(queue, mem_random, 1, 0, sizeof(double)*RANDOM_NUMBERS, randomNumbers, 0, NULL, NULL);
	  ERROR(err, "clWriteBuffer")
	  err= clEnqueueWriteBuffer(queue, mem_ms, 1, 0, sizeof(double)*data.size(), ms, 0, NULL, NULL);
	  ERROR(err, "clWriteBuffer")
	  err= clEnqueueWriteBuffer(queue, mem_mus, 1, 0, sizeof(double)*data.size(), mus, 0, NULL, NULL);
	  ERROR(err, "clWriteBuffer")
	  err= clEnqueueWriteBuffer(queue, mem_smus, 1, 0, sizeof(double)*data.size(), smus, 0, NULL, NULL);
	  ERROR(err, "clWriteBuffer")
	  err= clEnqueueWriteBuffer(queue, mem_cmus, 1, 0, sizeof(double)*data.size(), cmus, 0, NULL, NULL);
	  ERROR(err, "clWriteBuffer")
	  
	  char* kernelSource;
	  size_t kernelLength;
	  readSourceProgram("openclKernel.k", &kernelSource, &kernelLength);
	  program= clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &err);
	  ERROR(err, "clCreateProgramWithSource")
	  
	  char options[100];
	  //sprintf(options, "-DRESOLUTION=%d -DCACHESIZE=512", resolution*resolution);
	  sprintf(options, "-DRESOLUTION=%d -DCACHESIZE=256", resolution);
	  err= clBuildProgram(program, numDevices, devices, options, NULL, NULL);
	  ERROR(err, "clBuildProgram")
	  
	  kernel= clCreateKernel(program, "PETKernelMC", &err);
	  ERROR(err, "clCreateKernel")

	  int rowIdx= 0;
	  
	  err= clSetKernelArg(kernel, 0, sizeof(cl_mem), &mem_random);
	  ERROR(err, "clSetKernelArg")
	  err= clSetKernelArg(kernel, 1, sizeof(int), &RANDOM_NUMBERS);
	  ERROR(err, "clSetKernelArg")
	  err= clSetKernelArg(kernel, 2, sizeof(cl_mem), &mem_ms);
	  ERROR(err, "clSetKernelArg")
	  err= clSetKernelArg(kernel, 3, sizeof(cl_mem), &mem_mus);
	  ERROR(err, "clSetKernelArg")
	  err= clSetKernelArg(kernel, 4, sizeof(cl_mem), &mem_smus);
	  ERROR(err, "clSetKernelArg")
	  err= clSetKernelArg(kernel, 5, sizeof(cl_mem), &mem_cmus);
	  ERROR(err, "clSetKernelArg")
	  
	  err= clSetKernelArg(kernel, 7, sizeof(cl_mem), &mem_output);
	  ERROR(err, "clSetKernelArg")
	  
	  size_t global_work_size[1];
	  //size_t local_work_size[1];
	  global_work_size[0]= data.size();
	  //local_work_size[0]= 256;
	  
	  for ( unsigned int i= 0; i < data.size(); ++i )
	  {
	    rowIdx= i;
	    global_work_size[0]= data.size()-i;
	    if ( global_work_size[0] == 0 )
	      break;
	    
	    //global_work_size[0]= ((int)(global_work_size[0]/256 + 1))*256;
	    
	    int realSize= data.size()-i;
	    printf("%d/%zd, gws: %zd, rs: %d", i, data.size(), global_work_size[0], realSize); fflush(stdout);
	    err= clSetKernelArg(kernel, 6, sizeof(int), &rowIdx);
	    ERROR(err, "clSetKernelArg")
	    
	    /*err= clSetKernelArg(kernel, 6, sizeof(int), &realSize);
	    ERROR(err, "clSetKernelArg")*/
	    
	    //clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, &event);
	    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, &event);
	    ERROR(err, "clEnqueueNDRangeKernel")
	    
	    clWaitForEvents(1, &event);
	    
	    err= clEnqueueReadBuffer(queue, mem_output, 1, 0, sizeof(double)*data.size(), output, 0, NULL, NULL);
	    ERROR(err, "clEnqueueReadBuffer")
	    
	    Vector<double> cacheline;
	    cacheline.resize(data.size());
	    cacheline= 0;
	    for ( unsigned int j= i; j < data.size(); ++j )
	      cacheline(j)= output[j];
	    cache.push_back(cacheline);
	    
	    printf("\r"); fflush(stdout);
	    
	    if ( i % 1000 == 0 )
	      for ( unsigned int j= rowIdx; j < data.size(); j+= 100 )
	      {
		printf("%f,%f,%f,%f,%f,%f,%f ", float(j), data(rowIdx)(0), data(rowIdx)(1), data(j)(0), data(j)(1), this->kernel->evaluate(data(rowIdx),data(j)), output[j]);
		fflush(stdout);
	      }
	    printf("\n");
	  }
	  
	  
	  
	  for ( unsigned int i= 0; i < data.size(); ++i )
	    for ( unsigned int j= 0; j < i; ++j )
	      cache(i)(j)= cache(j)(i);
	    
	  // writing out kernel cache
	  tprintf("writing kernel cache: %s\n", filename.c_str());
	  cacheFile= fopen(filename.c_str(), "w");
	  fprintf(cacheFile, "%zd\n", data.size());
	  for ( unsigned int i= 0; i < cache.size(); ++i )
	  {
	    for ( unsigned int j= 0; j < cache(i).size(); ++j )
	      fprintf(cacheFile, "%.10f ", cache(i)(j));
	    fprintf(cacheFile, "\n");
	  }
	  fclose(cacheFile);
	}
	else
	{
	  tprintf("reading cached kernel matrix\n");
	  int size= 0;
	  int ret= fscanf(cacheFile, "%d", &size);
          if ( !ret ) printf("error in fscanf\n");
	  float f;
	  for ( int i= 0; i < size; ++i )
	  {
	    Vector<double> cacheline;
	    for ( int j= 0; j < size; ++j )
	    {
	      int ret= fscanf(cacheFile, "%f", &f);
              if ( !ret ) printf("error in fscanf\n");
	      cacheline.push_back(f);
	    }
	    cache.push_back(cacheline);
	  }
	  
	  fclose(cacheFile);
	  
	  tprintf("reading of kernel matrix finished\n");
	  
	  return;
	}
      }
    }
    
    template<typename T>
    void SequentialSVRegression<T>::initTemporaryVectors()
    {
      y.resize(this->data.size()*2);
      y= 1;
      for ( unsigned int i= y.size()/2; i < y.size(); ++i )
	y(i)= -1;
      
      alpha.resize(this->data.size()*2);
      alpha= 0;
      
      gradientCache.resize(alpha.size());
    }
    
    template<typename T>
    int SequentialSVRegression<T>::stoppingCondition(int iteration)
    {
      float M= -FLT_MAX;
      float m= FLT_MAX;
      
      for ( unsigned int k= 0; k < alpha.size(); ++k )
      {
	if ( (alpha(k) == 0 && y(k) == -1) || (alpha(k) == C && y(k) == 1 ) )
	{
	  if ( y(k)*gradientCache(k) > M )
	    M= y(k)*gradientCache(k);
	}
	if ( (alpha(k) == 0 && y(k) == 1) || (alpha(k) == C && y(k) == -1 ) )
	{
	  if ( y(k)*gradientCache(k) < m )
	    m= y(k)*gradientCache(k);
	}
      }
      M= -M;
      m= -m;

      gap= m - M;
      
      if ( m - M < tol || iteration > maxIteration)
      {
	if ( m - M < tol )
	  return 1;
	else if ( iteration > maxIteration )
	  return 2;
	else
	  return 3;
      }
      else
	return 0;
    }
    
    template<typename T>
    int SequentialSVRegression<T>::train(RegressionDataSet<T>& data, Vector<unsigned char>* mask)
    {
	tprintf("SequentialSVRegression - data.size(): %d, dimensions: %d\n", data.size(), data(0).size());
      
	tprintf("initialize regression\n");
	this->data.clear();
	this->supportVectors.clear();
	this->supportVectorsAlpha.clear();
	this->alpha.clear();
      
        this->featureNames= this->data.featureNames= data.featureNames;
        this->preprocessingNames= this->data.preprocessingNames= data.preprocessingNames;

	tprintf("filter input data\n");
        for ( unsigned int i= 0; i < data.size(); ++i )
            if ( !mask || (*mask)(i) )
                this->data.push_back(data(i));

        this->data.init();

	tprintf("fill kernel cache, this->data.size()= %d\n", this->data.size());
	fillCache();
	tprintf("main diagonal:\n");
	/*for ( int i= 0; i < data.size(); ++i )
	  printf("%f ", cache(i)(i));*/
	
	Vector<int> toremove;
	for ( unsigned int i= 0; i < this->data.size(); ++i )
	  if ( fabs(sin(this->data(i)(1))) < 0.01 || fabs(cos(this->data(i)(1))) < 0.01 )
	    toremove.push_back(i);
	  
	for ( int i= toremove.size()-1; i >= 0; --i )
	{
	  this->data.erase(this->data.begin() + toremove(i));
	  for ( unsigned int j= 0; j < cache.size(); ++j )
	    cache(j).erase(cache(j).begin() + toremove(i));
	  cache.erase(cache.begin() + toremove(i));
	}
	
	tprintf("init temporary vectors\n");
	initTemporaryVectors();

	int i, j, sc, iteration= 0;
	float minObj= FLT_MAX;
	
	refillGradientCache();
	
	tprintf("start sequential minimal optimization\n");
	double at, bt, ct, dt, s0= 0, s1= 0, s2= 0;
        while ( 1 )
        {
	  at= omp_get_wtime();
          selectWorkingSet(i,j);
	  bt= omp_get_wtime();
	  SMO(i,j);
	  ct= omp_get_wtime();
	  s0+= bt - at;
	  s1+= ct - bt;
	  
	  if ( iteration % 10000 == 0 )
	  {
	    float obj= objective();
	    if ( obj < minObj ) 
	    {
	      minObj= obj;
	      extractBestSupportVectors();
	      computeBestB();
	      //printf("\n%d,%f\n", bestSupportVectors.size(), minObj);
	    }
	    printf("\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r");
	    tprintf("objective: %6.6f, iteration: %10d, gap: %5.6f", obj, iteration, gap);
	    fflush(stdout);
	  }
	  
	  ++iteration;
	  
	  if ( iteration % 10000 == 0 && (sc= stoppingCondition(iteration)) )
	    break;
	  
	  dt= omp_get_wtime();
	  s2+= dt - at;
	  /*if ( iteration % 10000 == 0 )
	  {
	    printf("%f %f %f\n", s0, s1, s2);
	    s0= s1= s2= 0;
	  }*/
        }
        
        printf("\n");
        tprintf("sequential minimal optimization finished\n");
	tprintf("current objective value: %f, minimum objective value: %f\n", objective(), minObj);
	
	if ( sc == 1 )
	  tprintf("m-M < tolerance (%f)\n", tol);
	else if ( sc == 2 )
	  tprintf("maximum iteration %d reached\n", maxIteration);
	
	tprintf("computing the b offset value\n");
        computeB();
	tprintf("offset b: %f\n", b());

	tprintf("extracting support vectors\n");
	extractSupportVectors();
	tprintf("number of support vectors: %d\n", supportVectors.size());
	
        return 0;
    }
    
    template<typename T>
    int SequentialSVRegression<T>::trainWithPrecomputedKernel(RegressionDataSet<T>& data, Vector<unsigned char>* mask, Vector<Vector<double>* >* precomputedKernel)
    {
      this->precomputedKernel= precomputedKernel;
      return train(data, mask);
    }

    template<typename T>
    void SequentialSVRegression<T>::extractSupportVectors()
    {
      for ( unsigned int i= 0; i < data.size(); ++i )
	{
	  if ( fabs(alpha(i) - alpha(i + data.size())) > 0 )
	  {
	    supportVectorsAlpha.push_back(alpha(i) - alpha(i + data.size()));
	    supportVectors.push_back(data(i));
	  }
	}
    }
    
    template<typename T>
    void SequentialSVRegression<T>::extractBestSupportVectors()
    {
      bestSupportVectorsAlpha.clear();
      bestSupportVectors.clear();
      
      for ( unsigned int i= 0; i < data.size(); ++i )
	{
	  if ( alpha(i) - alpha(i + data.size()) != 0 )
	  {
	    bestSupportVectorsAlpha.push_back(alpha(i) - alpha(i + data.size()));
	    bestSupportVectors.push_back(data(i));
	  }
	}
    }
    
    template<typename T>
    void SequentialSVRegression<T>::swapToBestModel()
    {
      Vector<T> supportVectorsAlphaTmp;
      RegressionDataSet<T> supportVectorsTmp;
      
      supportVectorsAlphaTmp= supportVectorsAlpha;
      supportVectorsTmp= supportVectors;
      
      supportVectorsAlpha= bestSupportVectorsAlpha;
      supportVectors= bestSupportVectors;
      
      bestSupportVectorsAlpha= supportVectorsAlphaTmp;
      bestSupportVectors= supportVectorsTmp;
      
      tprintf("number of best support vectors: %d, original: %d\n", supportVectors.size(), bestSupportVectors.size());
    }
    
    template<typename T>
    void SequentialSVRegression<T>::computeB()
    {
      float bavg= 0;
      int n= 0;
      for ( unsigned int k= 0; k < alpha.size(); ++k )
	if ( alpha(k) > 0 && alpha(k) < C )
	{
	  bavg+= y(k)*gradient(k);
	  ++n;
	}
      
      if ( n > 0 )
	bValue= bavg/n;
      else
      {
	float M= -FLT_MAX;
	float m= FLT_MAX;
	
	for ( unsigned int k= 0; k < alpha.size(); ++k )
	{
	  if ( (alpha(k) == 0 && y(k) == -1) || (alpha(k) == C && y(k) == 1 ) )
	  {
	    if ( y(k)*gradient(k) > M )
	      M= y(k)*gradient(k);
	  }
	  if ( (alpha(k) == 0 && y(k) == 1) || (alpha(k) == C && y(k) == -1 ) )
	  {
	    if ( y(k)*gradient(k) < m )
	      m= y(k)*gradient(k);
	  }
	}
	if ( M == -FLT_MAX || m == FLT_MAX )
	  bValue= 0;
	else
	{
	  M= M;
	  m= m;
	  bValue= (M + m)/2;
	}
      }
    }
    
    template<typename T>
    void SequentialSVRegression<T>::computeBestB()
    {
      float bavg= 0;
      int n= 0;
      for ( unsigned int k= 0; k < alpha.size(); ++k )
	if ( alpha(k) > 0 && alpha(k) < C )
	{
	  bavg+= y(k)*gradient(k);
	  ++n;
	}
      
      if ( n > 0 )
	bestBValue= bavg/n;
      else
      {
	float M= -FLT_MAX;
	float m= FLT_MAX;
	
	for ( unsigned int k= 0; k < alpha.size(); ++k )
	{
	  if ( (alpha(k) == 0 && y(k) == -1) || (alpha(k) == C && y(k) == 1 ) )
	  {
	    if ( y(k)*gradient(k) > M )
	      M= y(k)*gradient(k);
	  }
	  if ( (alpha(k) == 0 && y(k) == 1) || (alpha(k) == C && y(k) == -1 ) )
	  {
	    if ( y(k)*gradient(k) < m )
	      m= y(k)*gradient(k);
	  }
	}
	if ( M == -FLT_MAX || m == FLT_MAX )
	  bestBValue= 0;
	else
	{
	  M= M;
	  m= m;
	  bestBValue= (M + m)/2;
	}
      }
    }
    
    template<typename T>
    int SequentialSVRegression<T>::SMO(int i, int j)
    {
	double TAU=0.0000001;
      
	double alpha_i_old= alpha(i);
	double alpha_j_old= alpha(j);
	
	if ( y(i) != y(j) )
	{
	  double qc= Q(i,i) + Q(j,j) + 2*Q(i,j);
	  if (qc <= 0)
	    qc = TAU;
	  
	  double delta = (-gradientCache(i)-gradientCache(j))/qc;
	  double diff = alpha(i) - alpha(j);
	  alpha(i) += delta;
	  alpha(j) += delta;
	  
	  if(diff > 0)
	  {
	    if(alpha(j) < 0)
	    {
	      alpha(j) = 0;
	      alpha(i) = diff;
	    }
	  }
	  else
	  {
	    if(alpha(i) < 0)
	    {
	      alpha(i) = 0;
	      alpha(j) = -diff;
	    }
	  }
	  if(diff > 0)
	  {
	    if(alpha(i) > C)
	    {
	      alpha(i) = C;
	      alpha(j) = C - diff;
	    }
	  }
	  else
	  {
	    if(alpha(j) > C)
	    {
	      alpha(j) = C;
	      alpha(i) = C + diff;
	    }
	  }
	}
	else
	{
	  double qc = Q(i,i)+Q(j,j)-2*Q(i,j);
	  
	  if (qc <= 0)
	    qc = TAU;
	  
	  double delta = (gradientCache(i)-gradientCache(j))/qc;
	  double sum = alpha(i) + alpha(j);
	  alpha(i) -= delta;
	  alpha(j) += delta;

	  if(sum > C)
	  {
	    if(alpha(i) > C)
	    {
	      alpha(i) = C;
	      alpha(j) = sum - C;
	    }
	  }
	  else
	  {
	    if(alpha(j) < 0)
	    {
	      alpha(j) = 0;
	      alpha(i) = sum;
	    }
	  }
	  if(sum > C)
	  {
	    if(alpha(j) > C)
	    {
	      alpha(j) = C;
	      alpha(i) = sum - C;
	    }
	  }
	  else
	  {
	    if(alpha(i) < 0)
	    {
	      alpha(i) = 0;
	      alpha(j) = sum;
	    }
	  }
	}
	
	if ( alpha(i) != alpha_i_old || alpha(j) != alpha_j_old )
	  updateGradients(alpha_i_old, alpha_j_old, i, j);
	//refillGradientCache();
	
        return 1;
    }

    template<typename T>
    T SequentialSVRegression<T>::objective()
    {
        T obj= 0;
        for ( unsigned int i= 0; i < alpha.size(); ++i )
	  for ( unsigned int j= 0; j < alpha.size(); ++j )
	    obj+= alpha(i)*Q(i,j)*alpha(j);
	obj*= 0.5;
        for ( unsigned int i= 0; i < alpha.size(); ++i )
            obj+= alpha(i)*p(i);

        return obj;
    }

    template<typename T>
    T SequentialSVRegression<T>::evaluate(Vector<T>& c)
    {
        double res= 0;
	
	for ( unsigned int i= 0; i < supportVectors.size(); ++i )
	  res+= supportVectorsAlpha(i)*(crossKernel->evaluate(supportVectors(i), c));

        return res - b();
    }
    
    template<typename T>
    T SequentialSVRegression<T>::evaluate(Vector<T>& c, int k)
    {
        double res= 0;
	
	res+= supportVectorsAlpha(k)*(crossKernel->evaluate(supportVectors(k), c));

        return res;
    }

    template<typename T>
    T SequentialSVRegression<T>::b()
    {
        return bValue;
    }

    template<typename T>
    T SequentialSVRegression<T>::gradient(int i)
    {
      double gradient= 0;
      for ( unsigned int j= 0; j < alpha.size(); ++j )
	gradient+= alpha(j)*Q(i,j);
	
      return gradient + p(i);
    }
    
    template<typename T>
    void SequentialSVRegression<T>::refillGradientCache()
    {
      //#pragma omp parallel for
      for ( unsigned int j= 0; j < alpha.size(); ++j )
	gradientCache(j)= gradient(j);
    }
    
    template<typename T>
    void SequentialSVRegression<T>::updateGradients(double alpha_i_old, double alpha_j_old, int i, int j)
    {
      double alpha_i_diff= alpha(i) - alpha_i_old;
      double alpha_j_diff= alpha(j) - alpha_j_old;
      #pragma omp parallel for num_threads(2)
      for ( unsigned int k= 0; k < gradientCache.size(); ++k )
      {
	gradientCache(k)+= alpha_i_diff*Q(i,k);
	gradientCache(k)+= alpha_j_diff*Q(j,k);
      }
    }
    
    template<typename T>
    void SequentialSVRegression<T>::selectWorkingSet(int& s, int& t)
    {
      double TAU=0.001;
      
      double maxgrad= -FLT_MAX;
      int maxIdx= 0;
      double yg;
      
      #pragma omp parallel for private(yg), shared(maxgrad, maxIdx), num_threads(2)
      for ( unsigned int i= 0; i < alpha.size(); ++i )
      {
	yg= -y(i)*gradientCache(i);
	if ( ((alpha(i) < C && y(i) == 1) || (alpha(i) > 0 && y(i) == -1)) && yg > maxgrad )
	{
	  #pragma omp critical
	  {
	    maxgrad= yg;
	    maxIdx= i;
	  }
	}
      }
      
      double minvalue= FLT_MAX;
      int minIdx= 0;
      double ygmax= -y(maxIdx)*gradientCache(maxIdx);
      double ast, bst, asthat, tmp;
      
      #pragma omp parallel for private(yg, ast, bst, asthat, tmp), shared(minvalue, minIdx), num_threads(2)
      for ( unsigned int i= 0; i < alpha.size(); ++i )
      {
	yg= -y(i)*gradientCache(i);
	if ( ((alpha(i) < C && y(i) == -1) || (alpha(i) > 0 && y(i) == 1)) && yg < ygmax)
	{
	  ast= Q(i,i) + Q(maxIdx,maxIdx) - 2*Q(i,maxIdx);
	  bst= ygmax - yg;
	  asthat= ast > 0 ? ast : TAU;
	  tmp= -bst*bst/asthat;
	  
	  if ( tmp < minvalue )
	  {
	    minvalue= tmp;
	    minIdx= i;
	  }
	}
      }
      
      s= minIdx;
      t= maxIdx;
    }
    
    template<typename T>
    T SequentialSVRegression<T>::Q(unsigned int i, unsigned int j)
    {
      //if ( precomputedKernel == NULL )
      {
	if ( i < data.size() && j < data.size())
	  return cache(i)(j);
	else if ( i < data.size() && j >= data.size())
	  return -cache(i)(j - data.size());
	else if ( i >= data.size() && j < data.size())
	  return -cache(i - data.size())(j);
	else
	  return cache(i - data.size())(j - data.size());
      }
      /*else
      {
	if ( i < data.size() && j < data.size())
	  return (*precomputedKernel)(i)(j);
	else if ( i < data.size() && j >= data.size())
	  return -(*precomputedKernel)(i)(j - data.size());
	else if ( i >= data.size() && j < data.size())
	  return -(*precomputedKernel)(i - data.size())(j);
	else
	  return (*precomputedKernel)(i - data.size())(j - data.size());
      }*/
    }
    
    template<typename T>
    T SequentialSVRegression<T>::p(unsigned int i)
    {
      if ( i >= data.size() )
	return (eps + data(i - data.size()).targetValue);
      else
	return (eps - data(i).targetValue);
    }
    
    template<typename T>
    float SequentialSVRegression<T>::regress(Vector<T>& c, Vector<float>* probabilities)
    {
        if ( probabilities == NULL )
            probabilities= &(this->probabilities);

        T tmp;
        tmp= evaluate(c);

        return tmp;
    }
    
    template<typename T>
    float SequentialSVRegression<T>::regress(Vector<T>& c, int k)
    {
        T tmp;
        tmp= evaluate(c, k);

        return tmp;
    }

    template<typename T>
    int SequentialSVRegression<T>::saveModelToFile(ofstream& file)
    {
        file << "SequentialSVRegression" << std::endl;
        this->Regression<T>::saveModelToFile(file);
	
        KernelFunctionIO<T> kfio;
        kfio.saveKernelFunctionToFile(kernel, file);
	file << supportVectors.size() << endl;
	file << supportVectors(0).size() << endl;
	for ( unsigned int i= 0; i < supportVectors.size(); ++i )
	  file << supportVectors(i) << endl;
	for ( unsigned int i= 0; i < supportVectorsAlpha.size(); ++i )
	  file << supportVectorsAlpha(i) << " ";
	file << endl;
	file << bValue;

        return 0;
    }

    template<typename T>
    int SequentialSVRegression<T>::openModelFromFile(ifstream& file)
    {
      string tmp;
      getline(file, tmp);
      
      if ( tmp.find("SequentialSVRegression") == 0 )
      {
	KernelFunctionIO<T> kfio;
	this->Regression<T>::openModelFromFile(file);
	/*this->kernel= kfio.openKernelFunctionFromFile(file);
	this->crossKernel= kernel;

	int n;

	file >> n;
	int dim;
	file >> dim;
	RegressionFeatureVector<double> rfv;
	rfv.resize(dim);

	for ( int i= 0; i < n; ++i )
	{
	  file >> rfv;
	  supportVectors.push_back(rfv);
	}

	double tmp;
	for ( int i= 0; i < n; ++i )
	{
	  file >> tmp;
	  supportVectorsAlpha.push_back(tmp);
	}
	file >> tmp;
	bValue= tmp;*/
      
	return 1;
      }

      return 0;
    }
}

#endif