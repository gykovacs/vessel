#include <openipML/DiscreteCorrelationFeatureSelection.h>

#include "DiscreteCorrelationFeatureSelection.h"

namespace openip
{
    CFSState::CFSState()
    {

    }

    CFSState::CFSState(DiscreteCorrelationFeatureSelection* d)
    {
        this->d= d;
        selection.resize(d->classFeatureCorrelations.size());
        this->selection= 0;
    }

    CFSState::CFSState(const CFSState& c)
    {
        this->d= c.d;
        this->selection.resize(c.selection.size());
        this->selection= c.selection;
    }

    CFSState::~CFSState()
    {
    }

    float CFSState::evaluate()
    {
        return d->evaluate(this->selection);
    }

    CFSState& CFSState::operator=(const CFSState& c)
    {
        this->selection.resize(c.selection.size());
        this->selection= c.selection;
        this->d= c.d;
        
        return (*this);
    }

    bool CFSState::operator==(CFSState& c)
    {
        for ( unsigned int i= 0; i < selection.size(); ++i )
            if ( selection[i] != c.selection[i] )
                return false;
        return true;
    }

    void CFSState::dump()
    {
    }

    CFSOperator::CFSOperator(int k)
    {
        this->k= k;
    }

    CFSOperator::CFSOperator(const CFSOperator& c)
    {
        this->k= c.k;
    }

    CFSOperator::~CFSOperator()
    {
    }

    bool CFSOperator::operator==(CFSOperator& c)
    {
        return this->k == c.k;
    }

    CFSState CFSOperator::apply(CFSState& c)
    {
        CFSState tmp(c);
        if ( tmp.selection[k] == 1 )
            tmp.selection[k]= 0;
        else
            tmp.selection[k]= 1;
        return tmp;
    }

    DiscreteCorrelationFeatureSelection::DiscreteCorrelationFeatureSelection()
    : FilterBasedFeatureSelection<int>()
    {
    }

    DiscreteCorrelationFeatureSelection::DiscreteCorrelationFeatureSelection(const DiscreteCorrelationFeatureSelection& c)
    : FilterBasedFeatureSelection<int>(c)
    {
    }

    DiscreteCorrelationFeatureSelection::~DiscreteCorrelationFeatureSelection()
    {
    }

    int DiscreteCorrelationFeatureSelection::select(DataSet<int>& d)
    {
        d.init();
        classFeatureCorrelations.resize(d.dimension);
        featureFeatureCorrelations.resize(d.dimension, d.dimension);
        selection.resize(d.dimension);

        for ( int i= 0; i < d.dimension; ++i )
            classFeatureCorrelations(i)= computeCorrelation(d, -1, i);

        for ( int i= 0; i < d.dimension; ++i )
            for ( int j= 0; j < d.dimension; ++j )
                featureFeatureCorrelations(i,j)= computeCorrelation(d, i, j);

        BestFirstSearch<CFSState, CFSOperator> bfs;
        bfs.actualNode= CFSState(this);
        for ( unsigned int i= 0; i < classFeatureCorrelations.size(); ++i )
            bfs.operators.push_back(CFSOperator(i));
        CFSState act(this);

        bfs.search(act);

        this->selection= bfs.actualNode.selection;

        return 0;
    }

    float DiscreteCorrelationFeatureSelection::computeCorrelation(DataSet<int>& d, int x, int y)
    {
        Vector<int> a(d.size());
        Vector<int> b(d.size());

        if ( x == -1 )
        {
            for ( unsigned int i= 0; i < d.size(); ++i )
                a(i)= d(i).classIndex;
        }
        else
        {
            for ( unsigned int i= 0; i < d.size(); ++i )
                a(i)= d(i)(x);
        }

        if ( y == -1 )
        {
            for ( unsigned int i= 0; i < d.size(); ++i )
                b(i)= d(i).classIndex;
        }
        else
        {
            for ( unsigned int i= 0; i < d.size(); ++i )
                b(i)= d(i)(y);
        }

        return computeCorrelation(a, b);
    }

    float DiscreteCorrelationFeatureSelection::computeCorrelation(Vector<int>& a, Vector<int>& b)
    {
        int amax= a(0), amin= a(0);
        int bmax= b(0), bmin= b(0);

        for ( unsigned int i= 0; i < a.size(); ++i )
        {
            if ( a(i) < amin )
                amin= a(i);
            if ( a(i) > amax )
                amax= a(i);
            if ( b(i) < bmin )
                bmin= b(i);
            if ( b(i) > bmax )
                bmax= b(i);
        }

        Vector<float> ahist(amax - amin + 1);
        Vector<float> bhist(bmax - bmin + 1);
        Matrix<float> bahist(bmax - bmin + 1, amax - amin + 1);

        ahist= bhist.size();
        bhist= ahist.size();
        bahist= 1.0f;

        for ( unsigned int i= 0; i < a.size(); ++i )
        {
            ahist(a(i) - amin)+= 1.0f;
            bhist(b(i) - bmin)+= 1.0f;
            bahist(b(i) - bmin, a(i) - amin)+= 1.0f;
        }

        ahist/= a.size() + ahist.size()*bhist.size();
        bhist/= b.size() + bhist.size()*ahist.size();
        bahist/= a.size() + ahist.size() * bhist.size();

        float entropyA= 0.0f;
        float entropyB= 0.0f;
        float entropyAB= 0.0f;

        for ( unsigned int i= 0; i < ahist.size(); ++i )
            entropyA-= ahist(i)*log2(ahist(i));
        for ( unsigned int i= 0; i < bhist.size(); ++i )
            entropyB-= bhist(i)*log2(bhist(i));
        for ( unsigned int i= 0; i < ahist.size(); ++i )
            for ( unsigned int j= 0; j < bhist.size(); ++j )
                entropyAB-= bahist(j,i)*log2(bahist(j,i));

        return 2 * (entropyA + entropyB - entropyAB) / (entropyA + entropyB);
    }

    float DiscreteCorrelationFeatureSelection::evaluate(Vector<int> selection)
    {
        float k= 0;
        float a= 0;
        float b= 0;
        for ( unsigned int i= 0; i < selection.size(); ++i )
            if ( selection[i] != 0 )
                k+= 1.0f;

        for ( unsigned int i= 0; i < selection.size(); ++i )
            if ( selection[i] != 0 )
                a+= classFeatureCorrelations(i);
        a/= k;

        for ( unsigned int i= 0; i < selection.size(); ++i )
            for ( unsigned int j= 0; j < selection.size(); ++j )
                if ( selection[i] != 0 && selection[j] != 0 )
                    b+= featureFeatureCorrelations(i,j);
        b/= k*k;

        if ( k + k*(k-1)*b >= 0 )
        return k*a/sqrt(k + k*(k-1)*b);
        else
            return 0;
    }
}
