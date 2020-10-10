#ifndef GMMEM_H
#define GMMEM_H

#include <openipML/GaussianDistribution.h>
#include <openipML/ExpectationMaximization.h>

namespace openip
{
    template<typename T>
    class GMM
    {
    public:
        GMM();

        GMM(const GMM& gmm);

        ~GMM();

        double probability(Vector<T>& d);

        Vector<MultivariateGaussianDistribution<T> > dist;
        Vector<double> weights;
    };

    template<typename T>
    GMM<T>::GMM()
    {
    }

    template<typename T>
    GMM<T>::GMM(const GMM& gmm)
    {
    }

    template<typename T>
    GMM<T>::~GMM()
    {
    }

    template<typename T>
    double GMM<T>::probability(Vector<T>& d)
    {
        double p= 0;
        for ( int i= 0; i < dist.size(); ++i )
            p+= weights(i)*dist(i).probability(d);
        return p;
    }

    template<typename T>
    class GMMEM: public ExpectationMaximization
    {
    public:
        GMMEM();

        GMMEM(const GMMEM& em);

        ~GMMEM();

        virtual void init(Vector<Vector<T> >* d, Vector<MultivariateGaussianDistribution<T> >* dist, Vector<double>* w);

        virtual void expect();

        virtual void maximize();

        virtual int stopCondition();

        virtual void init(){ diff= 1; }

        Vector<Vector<T> >* d;
        Vector<MultivariateGaussianDistribution<T> >* dist;
        Vector<double>* w;
        Matrix<double> p;
        Vector<double> pj;
        double pT;
        double diff;
    };

    template<typename T>
    GMMEM<T>::GMMEM()
        : ExpectationMaximization()
    {
    }

    template<typename T>
    GMMEM<T>::GMMEM(const GMMEM& em)
        : ExpectationMaximization(em)
    {
    }

    template<typename T>
    GMMEM<T>::~GMMEM()
    {
    }

    template<typename T>
    void GMMEM<T>::init(Vector<Vector<T> >* d, Vector<MultivariateGaussianDistribution<T> >* dist, Vector<double>* w)
    {
        this->d= d;
        this->dist= dist;

        Vector<T> mean;
        Matrix<T> variance;
        mean.resize((*d)(0).size());
        variance.resize((*d)(0).size(), (*d)(0).size());

        mean= 0;
        variance= 0;

        for ( unsigned int i= 0; i < d->size(); ++i )
                mean= mean + (*d)(i);
        mean/= d->size();
        cout << mean << endl;


        variance= 0;
        for ( unsigned int j= 0; j < d->size(); ++j )
            variance= variance + ((*d)(j) - mean).columnRowProduct((*d)(j) - mean);
        variance/= d->size();
        cout << variance << endl;

        for ( unsigned int i= 0; i < dist->size(); ++i )
        {
            (*dist)(i).setParameters(mean, variance);
            for ( unsigned int j= 0; j < dist->size(); ++j )
                mean(j)-= 0.1;
        }

        w->resize(dist->size());
        for ( unsigned int i= 0; i < w->size(); ++i )
            (*w)(i)= 1.0/(w->size());

        p.resize(d->size(), dist->size());
        p= 0;
        pj.resize(dist->size());
    }

    template<typename T>
    void GMMEM<T>::expect()
    {
        tprintf("expect\n");
        pj= 0;
        pT= 0;
        for ( unsigned int i= 0; i < d->size(); ++i )
            for ( unsigned int j= 0; j < dist->size(); ++j )
            {
                p(i,j)= (*dist)(j).probability((*d)(i));
                pj(j)+= p(i,j);
                pT+= p(i,j);
            }

        cout << pj << endl;
        cout << pT << endl;
        tprintf("expect finished\n");
    }

    template<typename T>
    void GMMEM<T>::maximize()
    {
        tprintf("maximize\n");
        tprintf("compute means\n");
        // compute means
        Vector<Vector<T> > means;
        means.resize(dist->size());
        for ( unsigned int i= 0; i < means.size(); ++i )
            means(i).resize((*d)(0).size());

        for ( unsigned int i= 0; i < dist->size(); ++i )
        {
            means(i)= 0;
            for ( unsigned int j= 0; j < d->size(); ++j )
                means(i)= means(i) + (*d)(j)*p(j,i);
            means(i)/= pj(i);
        }

        for ( unsigned int i= 0; i < means.size(); ++i )
            cout << means(i) << std::endl;

        tprintf("means computed\n");
        // compute variances
        Vector<Matrix<T> > vars;
        vars.resize(dist->size());
        for ( unsigned int i= 0; i < vars.size(); ++i )
            vars(i).resize((*d)(0).size(), (*d)(0).size());

        for ( unsigned int i= 0; i < dist->size(); ++i )
        {
            vars(i)= 0;
            for ( unsigned int j= 0; j < d->size(); ++j )
                vars(i)= vars(i) + ((*d)(j) - means(i)).columnRowProduct((*d)(j) - means(i))*pj(i);
            vars(i)/= d->size();
        }

        for ( unsigned int i= 0; i < vars.size(); ++i )
            cout << vars(i) << std::endl;

        for ( unsigned int i= 0; i < dist->size(); ++i )
            (*w)(i)= pj(i)/pT;

        tprintf("vars computed\n");
        diff= 0;
        for ( unsigned int i= 0; i < means.size(); ++i )
            for ( unsigned int j= 0; j < means(i).size(); ++j )
                diff+= fabs(means(i)(j) - (*dist)(i).mean(j));

        tprintf("diffs computed\n");
        for ( unsigned int i= 0; i < dist->size(); ++i )
            (*dist)(i).setParameters(means(i), vars(i));
        tprintf("parameters updated\n");
    }

    template<typename T>
    int GMMEM<T>::stopCondition()
    {
        cout << "================ " << diff << " =================" << endl;
        if ( diff < 0.1 )
            return 1;
        return 0;
    }
}

#endif // GMMEM_H
