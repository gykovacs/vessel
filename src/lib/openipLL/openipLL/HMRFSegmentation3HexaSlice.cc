#include <openipDS/Stopper.h>

#include <openipLL/HMRFSegmentation3HexaSlice.h>
#include <openipLL/imageIO.h>
#include <openipLL/Transform3Set.h>
#include <openipLL/Transform3Generator.h>

namespace openip
{
    HMRFSegmentation3HexaSlice::HMRFSegmentation3HexaSlice(Classifier<float>* c, Volume<float>& input, Volume<unsigned char>& mask, Volume<unsigned char>& output)
    : SimulatedAnnealing(100000000000, 0.0000001, 0.999999)
    {
        this->c= c;
        this->input= &input;
        this->output= &output;
        this->mask= &mask;

        output= 0;
        temperature= 0.1;
    }

    void HMRFSegmentation3HexaSlice::solve()
    {
        float rnd;

        init();

        objective= 0;
        int last= 0;
        double lastO= 0;
        tprintf("start optimization\n");
        while ( !stopCondition() )
        {
            sample();
            newObjective= evaluate();

            ++iteration;

            if ( !betterObjectiveFunction(newObjective) )
            {
                rnd= urfg.generate();
                if ( fabs(objective - newObjective) > 0.0 )
                if ( rnd > temperature/fabs(objective - newObjective) )
                {
                    revert();
                    continue;
                }
            }

            decreaseTemperature();

            objective= newObjective;

            if ( ( iteration - last ) > check )
            {
                tprintf("iteration: %ld objective: %f temperature: %f\n", iteration, objective, temperature );
                if ( fabs(objective - lastO) < 0.0000001 )
                    break;
                last= iteration;
                lastO= objective;
            }
        }
    }

    HMRFSegmentation3HexaSlice::HMRFSegmentation3HexaSlice(const HMRFSegmentation3HexaSlice& h)
    : SimulatedAnnealing(h), c(h.c), input(h.input), output(h.output), probabilities(h.probabilities)
    {
    }

    HMRFSegmentation3HexaSlice::~HMRFSegmentation3HexaSlice()
    {
    }

    void HMRFSegmentation3HexaSlice::segment(double beta, double temperature, double epsilon, double decrease, long int maxIt, int check)
    {
        this->beta= beta;
        this->gamma= gamma;
        this->temperature= temperature;
        this->epsilon= epsilon;
        this->annealing= decrease;
        this->maximumIteration= maxIt;
        this->check= check;

        tprintf("beta: %f temperature: %f epsilon: %f annealing: %f maximumIteration: %ld check: %d\n", beta, temperature, epsilon, decrease, maxIt, check); fflush(stdout);

        this->solve();
    }

    float HMRFSegmentation3HexaSlice::probabilityComponent(int i)
    {
        return ((*(probabilities[(*output)(i)]))(i));
    }

    float HMRFSegmentation3HexaSlice::connectivityComponent(int i)
    {
        float sum= 0;
        int s, r, c;
        output->getCoordinate3D(i, s, r, c);
        int n;
        
        if ( r % 2 == 0 )
        {
            output->getCoordinate1D(s, r-1, c-1, n );
            if ( n >= 0 && n < int(output->n) )
            if ( (*output)(i) != (*output)(s, r-1, c-1) )
                sum-= beta;
            
            output->getCoordinate1D(s, r-1, c, n );
            if ( n >= 0 && n < int(output->n) )
            if ( (*output)(i) != (*output)(s, r-1, c) )
                sum-= beta;
            
            output->getCoordinate1D(s, r, c-1, n );
            if ( n >= 0 && n < int(output->n) )
            if ( (*output)(i) != (*output)(s, r, c-1) )
                sum-= beta;
            
            output->getCoordinate1D(s, r, c+1, n );
            if ( n >= 0 && n < int(output->n) )
            if ( (*output)(i) != (*output)(s, r, c+1) )
                sum-= beta;
            
            output->getCoordinate1D(s, r+1, c-1, n );
            if ( n >= 0 && n < int(output->n) )
            if ( (*output)(i) != (*output)(s, r+1, c-1) )
                sum-= beta;
            
            output->getCoordinate1D(s, r+1, c, n );
            if ( n >= 0 && n < int(output->n) )
            if ( (*output)(i) != (*output)(s, r+1, c) )
                sum-= beta;
        }
        else
        {
            output->getCoordinate1D(s, r-1, c, n );
            if ( n >= 0 && n < int(output->n) )
            if ( (*output)(i) != (*output)(s, r-1, c) )
                sum-= beta;
            
            output->getCoordinate1D(s, r-1, c+1, n );
            if ( n >= 0 && n < int(output->n) )
            if ( (*output)(i) != (*output)(s, r-1, c+1) )
                sum-= beta;
            
            output->getCoordinate1D(s, r, c-1, n );
            if ( n >= 0 && n < int(output->n) )
            if ( (*output)(i) != (*output)(s, r, c-1) )
                sum-= beta;
            
            output->getCoordinate1D(s, r, c+1, n );
            if ( n >= 0 && n < int(output->n) )
            if ( (*output)(i) != (*output)(s, r, c+1) )
                sum-= beta;
            
            output->getCoordinate1D(s, r+1, c, n );
            if ( n >= 0 && n < int(output->n) )
            if ( (*output)(i) != (*output)(s, r+1, c) )
                sum-= beta;
            
            output->getCoordinate1D(s, r+1, c+1, n );
            if ( n >= 0 && n < int(output->n) )
            if ( (*output)(i) != (*output)(s, r+1, c+1) )
                sum-= beta;
        }
        return sum;
    }

    float HMRFSegmentation3HexaSlice::connectivityComponent2(int i)
    {
        float sum= 0;
        if ( (*output)(i) == 0 && (*output)(i - output->columns) != 0 )
            sum-= beta;
        if ( (*output)(i) == 0 && (*output)(i + output->columns) != 0 )
            sum-= beta;
        if ( (*output)(i) == 0 && (*output)(i - 1) != 0 )
            sum-= beta;
        if ( (*output)(i) == 0 && (*output)(i + 1) != 0 )
            sum-= beta;
        if ( (*output)(i) != 0 && (*output)(i - output->columns) == 0 )
            sum-= beta;
        if ( (*output)(i) != 0 && (*output)(i + output->columns) == 0 )
            sum-= beta;
        if ( (*output)(i) != 0 && (*output)(i - 1) == 0 )
            sum-= beta;
        if ( (*output)(i) != 0 && (*output)(i + 1) == 0 )
            sum-= beta;

        return sum;
    }

    void HMRFSegmentation3HexaSlice::init()
    {
        tprintf("instantiate transform set\n");
        //Transform2Generator t2g;
        Transform3Set<float, float>& t3s= *(/*t2g.*/(generateTransform3Set<float, float>(c->featureNames)));
        Border3 b= t3s.getProposedBorder();
        b.borderMode= BORDER_MODE_MIRRORED;
        input->resizeBorder(b);
        output->resizeBorder(b);
        b.borderMode= BORDER_MODE_ZERO;
        mask->resizeBorder(b);
        //Transform2Set<float, float> t2s(c->featureNames);

        tprintf("create feature images\n");
        std::vector<Volume<float>* > featureImages;
        for ( unsigned int i= 0; i < c->featureNames.size(); ++i )
        {
            featureImages.push_back(new Volume<float>(*input));
            (*(featureImages[i]))= 0;
        }

        tprintf("create probability maps\n");
        for ( unsigned int i= 0; i < c->classNames.size(); ++i )
        {
            probabilities.push_back(new Volume<float>(*input));
            (*(probabilities[i]))= 0;
        }

        tprintf("extract features\n");
        for ( unsigned int i= 0; i < featureImages.size(); ++i )
        {
            t3s[i]->apply(*input, *(featureImages[i]), mask);
        }

        tprintf("compute probability maps\n");
        Vector<float> vf;
        vf.resize(featureImages.size());
        for ( unsigned int i= 0; i < input->n; ++i )
        {
            if ( (*mask)(i) > 0 )
            {
                for ( unsigned int j= 0; j < featureImages.size(); ++j )
                    vf(j)= (featureImages[j])->operator()(i);
                c->classify(vf);
                for ( unsigned int j= 0; j < c->classNames.size(); ++j )
                {
                    (*(probabilities[j]))(i)= c->probabilities[j];
                }
            }
        }
        tprintf("probability maps computed\n");
        /*probabilities[1]->normalize(0, 255);
        writeImage("b.png", *(probabilities[1]));*/

        srand(time(NULL));
        for ( unsigned int i= 0; i < output->n; ++i )
            if ( (*mask)(i) > 0 )
                (*output)(i)= rand()%probabilities.size();

        objective= 0;
    }

    void HMRFSegmentation3HexaSlice::sample()
    {
        smp= rand()%output->n;
        //while ( !(((mask->rows == 0) || (*mask)(smp) > 0) && (smp > mask->sliceSize + mask->columns + 1 && smp < mask->n - mask->sliceSize - mask->columns - 1)) )
        while ( !((mask->rows) == 0 || (*mask)(smp) > 0 ) )
            smp= rand()%output->n;
        newLabel= rand()%probabilities.size();
    }

    double HMRFSegmentation3HexaSlice::evaluate()
    {
        double tmp= objective;

        tmp-= probabilityComponent(smp);
        if ( beta > 0 )
            tmp-= connectivityComponent(smp);

        oldLabel= (*output)(smp);
        (*output)(smp)= newLabel;

        tmp+= probabilityComponent(smp);
        if ( beta > 0 )
            tmp+= connectivityComponent(smp);

        return tmp;
    }

    void HMRFSegmentation3HexaSlice::revert()
    {
        (*output)(smp)= oldLabel;
    }
}

