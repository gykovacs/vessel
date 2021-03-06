//===////////////////////////////////////////////////////////////////////
//                                                                     //
//   SVMTorch II V1.77 [Ennastie]                                      //
//                                                                     //
//   Author: Ronan Collobert                                           //
//   Date: 20.11.2001                                                  //
//                                                                     //
//   Copyright (c) 2001  IDIAP & DIRO U. de Montreal                   //
//                       All rights reserved                           //
//                                                                     //
//   This software is available for non-commercial use only. It must   //
//   not be modified and distributed without prior permission of the   //
//   author. The author is not responsible for implications from the   //
//   use of this software.                                             //
//                                                                     //
////////////////////////////////////////////////////////////////////===//


#include "general.h"
#include "StandardSVM.h"
#include "Kernel.h"
#include "UserKernel.h"
#include "IOTorch.h"
#include <string.h>

// Tous les options du SVM doivent etre passees
// par des strings ////

typedef struct
{
    string regression_mode;
    bool multi_mode;
    int first_class;
    int last_class;
    string c;
    string eps_regression;
    string unshrink_mode;
    string n_max_unshrink;
    int  kernel_type;
    int  d;
    real std;
    real s;
    real r;
    string u;
    string cache_size;
    string eps_fin;
    string h;
    string eps_shrink;
    string eps_bornes;
    string sparse_mode;
    bool bin_mode;
    int  load;
} parametres;

void help(char **argv, parametres *params)
{
  cout << endl;
  cout << "==============================================================================================" << endl;
  cout << "* The Trebolloc Company presents:" << endl;
  cout << "* " << VERSION << endl;
  cout << "* \n* usage: " << argv[0] << " [options] <train file> <model file>" << endl;
  cout << "* \n* \n";
  cout << "* Arguments:" << endl;
  cout << "*   <train file> -> file with training data" << endl;
  cout << "*   <model file> -> file to save the model" << endl;
  cout << "* General options:" << endl;
  cout << "*   -help        -> this little help" << endl;
  cout << "*   -multi       -> multiclass mode" << endl;
  cout << "*   -mfc <int>   -> first class to learn in multiclass mode (optional)" << endl;
  cout << "*   -mlc <int>   -> last class to learn in multiclass mode (optional)" << endl;
  cout << "* Learning options:" << endl;
  cout << "*   -rm          -> regression mode" << endl;
  cout << "*   -c <float>   -> trade-off between training error" << endl;
  cout << "*                   and the margin [" << params->c << "]" << endl;
  cout << "*   -eps <float> -> the width of the error pipe in regression mode ["
       << params->eps_regression << "]" << endl;
  cout << "*   -unshrink    -> unshrink mode" << endl;
  cout << "* Kernel options:" << endl;
  cout << "*   -t <int>     -> type of kernel functions: [" << params->kernel_type << "]" << endl;
  cout << "*                     0 -> linear" << endl;
  cout << "*                     1 -> polynomial (s a*b + r)^d" << endl;
  cout << "*                     2 -> radial basis function exp(-|a-b|^2 / (std*std))" << endl;
  cout << "*                     3 -> sigmoid tanh(s a*b + r)" << endl;
  cout << "*                     4 -> user defined kernel [see UserKernel.cc and .h]" << endl;
  cout << "*   -d <int>     -> parameter d in polynomial kernel [" << params->d << "]" << endl;
  cout << "*   -std <float> -> parameter std in rbf kernel [" << params->std << "]" << endl;
  cout << "*   -s <float>   -> parameter s in sigmoid/poly kernel [" << params->r << "]" << endl;
  cout << "*   -r <float>   -> parameter r in sigmoid/poly kernel [" << params->s << "]" << endl;
  cout << "*   -u <string>  -> parameter of user defined kernel [" << params->u << "]" << endl;
  cout << "* Optimization options:" << endl;
  cout << "*   -m <float>   -> size of cache in mega byte [" << params->cache_size << "]" << endl;
  cout << "*   -e <float>   -> error allowed in the termination criterion [" << params->eps_fin << "]" << endl;
  cout << "*   -h <int>     -> minimal number of iterations a variable needs" << endl;
  cout << "*                   to be shrinked [" << params->h << "]" << endl;
  cout << "* Nightmare options:" << endl;
  cout << "*   -xs <float>  -> epsilon value related to shrink [" << params->eps_shrink << "]" << endl;
  cout << "*   -xb <float>  -> epsilon value for bounds precision [" << params->eps_bornes << "]" << endl;
  cout << "*   -xu <int>    -> after <int> unshrink, never shrink again [" << params->n_max_unshrink << "]" << endl;
  cout << "* Input option:" << endl;
  cout << "*   -sparse      -> sparse data" << endl;
  cout << "*   -bin         -> binary data" << endl;
  cout << "*   -load <int>  -> load only <int> examples" << endl;
  cout << "*" << endl;
  cout << "===========>> SVMTorch is (c) Ronan Collobert 2001 (IDIAP & DIRO U. de Montreal) <<===========" << endl;
  cout << "\n";
  exit(0);
}

void scan_cmd(int argc, char **argv, parametres *params)
{  
  int i = 1;
  int erreur;
  int maxa;
  
  if(argc < 3)
    help(argv, params);
  
  maxa = argc-2;
  while(i < maxa)
  {
    erreur = 1;
    
    if(!strcmp(argv[i], "-help"))
      help(argv, params);

    if(!strcmp(argv[i], "-multi"))
    {
      params->multi_mode = true;
      erreur = 0;
    }    

    if(!strcmp(argv[i], "-mfc"))
    {
      i++;
      if(i < maxa)
        params->first_class = atoi(argv[i]);
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-mlc"))
    {
      i++;
      if(i < maxa)
        params->last_class = atoi(argv[i]);
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-rm"))
    {
      params->regression_mode = "1";
      erreur = 0;
    }

    if(!strcmp(argv[i], "-c"))
    {
      i++;
      if(i < maxa)
        params->c = argv[i];
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-eps"))
    {
      i++;
      if(i < maxa)
        params->eps_regression = argv[i];
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-unshrink"))
    {
      params->unshrink_mode = "1";
      erreur = 0;
    }

    if(!strcmp(argv[i], "-t"))
    {
      i++;
      if(i < maxa)
        params->kernel_type = atoi(argv[i]);
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-d"))
    {
      i++;
      if(i < maxa)
        params->d = atoi(argv[i]);
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-std"))
    {
      i++;
      if(i < maxa)
        params->std = atof(argv[i]);
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-s"))
    {
      i++;
      if(i < maxa)
        params->s = atof(argv[i]);
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-r"))
    {
      i++;
      if(i < maxa)
        params->r = atof(argv[i]);
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-u"))
    {
      i++;
      if(i < maxa)
        params->u = argv[i];
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-m"))
    {
      i++;
      if(i < maxa)
        params->cache_size = argv[i];
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-e"))
    {
      i++;
      if(i < maxa)
        params->eps_fin = argv[i];
      else
        help(argv, params);
      
      erreur = 0;
    }
    
    if(!strcmp(argv[i], "-h"))
    {
      i++;
      if(i < maxa)
        params->h = argv[i];
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-xs"))
    {
      i++;
      if(i < maxa)
        params->eps_shrink = argv[i];
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-xb"))
    {
      i++;
      if(i < maxa)
        params->eps_bornes = argv[i];
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-xu"))
    {
      i++;
      if(i < maxa)
        params->n_max_unshrink = argv[i];
      else
        help(argv, params);
      
      erreur = 0;
    }

    if(!strcmp(argv[i], "-sparse"))
    {
      params->sparse_mode = "1";
      erreur = 0;
    }

    if(!strcmp(argv[i], "-bin"))
    {
      params->bin_mode = true;
      erreur = 0;
    }

    if(!strcmp(argv[i], "-load"))
    {
      i++;
      if(i < maxa)
        params->load = atoi(argv[i]);
      else
        help(argv, params);
      
      erreur = 0;
    }

    i++;    
    if(erreur)
      help(argv, params);
  }
}

void carbure(real **data, sreal **sdata, real *y, int l, int c, parametres *params, string file_out, string comment)
{
  if(params->regression_mode != "1")
  {
    cout << "# Checking class...";
    cout.flush();
    for(int i = 0; i < l; i++)
    {
      if( (y[i] != 1) && (y[i] != -1) )
      {
        cout << "\n$ Error: class label must be -1 or +1\n" << endl;
        exit(0);
      }
    }
    cout << "OK" << endl;
  }

  StandardSVM estelle;
  
  estelle.setOption("regression_mode", params->regression_mode);
  estelle.setOption("C", params->c);
  estelle.setOption("eps_regression", params->eps_regression);
  estelle.setOption("unshrink_mode", params->unshrink_mode);
  estelle.setOption("n_max_unshrink", params->n_max_unshrink);
  
  estelle.setOption("cache_size_meg", params->cache_size);
  estelle.setOption("eps_fin", params->eps_fin);
  estelle.setOption("n_iter_min_to_shrink", params->h);
  estelle.setOption("eps_shrink", params->eps_shrink);
  estelle.setOption("eps_bornes", params->eps_bornes);
  estelle.setOption("sparse_mode", params->sparse_mode);
  
  // Le kernel [Genre de truc a setter apres le svm]
  Kernel *delphine;
  switch(params->kernel_type)
  {
    case 0:
      delphine = new DotKernel(estelle);
       break;
    case 1:
      delphine = new PolynomialKernel(estelle);
      ((PolynomialKernel *)delphine)->setParametres(params->d, params->s, params->r);
      break;
    case 2:
      delphine = new GaussianKernel(estelle);
       ((GaussianKernel *)delphine)->setParametres(params->std);
       break;
    case 3:
      delphine = new SigmoidKernel(estelle);
      ((SigmoidKernel *)delphine)->setParametres(params->s, params->r);
      break;
    case 4:
      delphine = new UserKernel(estelle);
      ((UserKernel *)delphine)->setParametres(params->u);
      break;
    default:
      cout << "$ What's this kernel ?!\n\n";
      exit(0);
  }
  
  estelle.train(data, sdata, y, l, c);
  estelle.save(file_out, comment);
}

int main(int argc, char **argv)
{

  parametres params;

  params.regression_mode = "0";
  params.multi_mode = false;
  params.first_class = -1;
  params.last_class = -1;
  params.c = "100";
  params.eps_regression = "0.5";
  params.unshrink_mode = "0";
  params.n_max_unshrink = "2";
  params.kernel_type = 2;
  params.d = 2;
  params.std = 10;
  params.s = 1;
  params.r = 1;
  params.u = "10";
  params.cache_size = "50";
  params.eps_fin = "0.01";
  params.h = "100";
#ifdef USEDOUBLE
  params.eps_shrink = "1E-9";
  params.eps_bornes = "1E-12";
#else
  params.eps_shrink = "1E-4";
  params.eps_bornes = "1E-4";
#endif
  params.sparse_mode = "0";
  params.bin_mode = false;    
  params.load = -1;

  scan_cmd(argc, argv, &params);

  // Let's go boy //////////////////////////////////
  real **data = NULL;
  sreal **sdata = NULL;
  real *y = NULL;
  int l, c;

  cout << "# Loading data :" << endl;
  IOTorch melanie;
  if(params.sparse_mode == "1")
    melanie.loadData(argv[argc-2], &sdata, &y, l, c, params.bin_mode, params.load);
  else
    melanie.loadData(argv[argc-2], &data, &y, l, c, params.bin_mode, params.load);

  cout << "  " << l << " training examples" << endl;
  cout << "  input dimension is " << c << endl;

  string comment = argv[0];;
  for(int i = 1; i < argc; i++)
  {
    string s1, s2;
    s1 = " ";
    s2 = argv[i];
    comment += s1 + s2;
  }

  if(params.multi_mode)
  {
    if(params.regression_mode == "1")
    {
      cout << "$ RhhAHHhaa. Multiclass = multi + CLASS !!!\n" << endl;
      exit(0);
    }
  
    cout << "# Scanning for classes...";
    cout.flush();
    int n_class = -1;
    for(int i = 0; i < l; i++)
    {
      if(y[i] > (real)n_class)
        n_class = (int)y[i];
    }
    n_class++;
    cout << n_class << " classes found" << endl;
    
    if( (params.first_class < 0) || (params.first_class >= n_class) )
      params.first_class = 0;
    if( (params.last_class < 0) || (params.last_class >= n_class) )
      params.last_class = n_class-1;
    if(params.last_class < params.first_class)
      params.last_class = params.first_class;

#ifdef I_WANT_TIME    
    long t_debut = getRuntime();
#endif

    real *y_temp = new real[l];
    for(int cl = params.first_class; cl <= params.last_class; cl++)
    {
      cout << "\n# Training class " << cl << " against the others\n";
      int n_temp = 0;
      for(int i = 0; i < l; i++)
      {
        if((int)y[i] == cl)
        {
          y_temp[i]=  1;
          n_temp++;
        }
        else
          y_temp[i]= -1;
      }
      cout << "# class " << cl << " has " << n_temp << " examples" << endl;
      string file_out = argv[argc-1];
      {
        char julie[10];
        sprintf(julie, ".%d", cl);
        string sjulie = julie;
        file_out += sjulie;
      }
      carbure(data, sdata, y_temp, l, c, &params, file_out, comment);
    }

#ifdef I_WANT_TIME
    long t_fin = getRuntime();    
    cout << endl;
    cout << "# Total time in CPU-seconds = " << (double)(t_fin-t_debut)/((double)CLOCKS_PER_SEC) << endl;
#endif

    
    delete[] y_temp;
  }
  else
    carbure(data, sdata, y, l, c, &params, argv[argc-1], comment);

  return(0);
}




