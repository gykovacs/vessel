#include <openipDS/Feature2Set.h>
#include <openipDS/Stopper.h>

#include <openipLL/HMRFSegmentation.h>
#include <openipLL/imageIO.h>
#include <openipLL/Transform2Set.h>
#include <openipLL/Transform2Generator.h>
#include <openipLL/LowLevelTransforms.h>

namespace openip
{
    HMRFSegmentation::HMRFSegmentation(Classifier<float>* c, Image<float>& input, Image<unsigned char>& mask, Image<unsigned char>& output)
    : SimulatedAnnealing(100000000000, 0.0000001, 0.999999)
    {
        this->c= c;
        this->input= &input;
        this->output= &output;
        this->mask= &mask;

        output= 0;
        temperature= 0.1;
        shiftmax= false;
        shiftmin= false;
        sort= false;
        gauss= false;
    }

    void HMRFSegmentation::solve()
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

    HMRFSegmentation::HMRFSegmentation(const HMRFSegmentation& h)
    : SimulatedAnnealing(h), c(h.c), input(h.input), output(h.output), probabilities(h.probabilities)
    {
    }

    HMRFSegmentation::~HMRFSegmentation()
    {
    }

    void HMRFSegmentation::segment(double beta, double temperature, double epsilon, double decrease, long int maxIt, int check)
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

    float HMRFSegmentation::probabilityComponent(int i)
    {
        return ((*(probabilities[(*output)(i)]))(i));
    }

    float HMRFSegmentation::connectivityComponent(int i)
    {
        float sum= 0;
        if ( (*output)(i) != (*output)(i - output->columns) )
            sum-= beta;
        if ( (*output)(i) != (*output)(i + output->columns) )
            sum-= beta;
        if ( (*output)(i) != (*output)(i - 1) )
            sum-= beta;
        if ( (*output)(i) != (*output)(i + 1) )
            sum-= beta;

        return sum;
    }

    float HMRFSegmentation::connectivityComponent2(int i)
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

    float HMRFSegmentation::directionalComponent(int i)
    {
        float sum= 0;
        if ( (*orientation)(i) < M_PI/8  || (*orientation)(i) > M_PI - M_PI/8 )
        {
            if ( (*output)(i) == 0 && 0 != (*output)(i + 1) )
                sum-= gamma;
            if ( (*output)(i) == 0 && 0 != (*output)(i - 1) )
                sum-= gamma;
            if ( (*output)(i) != 0 && 0 == (*output)(i + 1) )
                sum-= gamma;
            if ( (*output)(i) != 0 && 0 == (*output)(i - 1) )
                sum-= gamma;
        }
        if ( (*orientation)(i) > M_PI/2 + M_PI/8 && (*orientation)(i) < M_PI - M_PI/8 )
        {
           if ( (*output)(i) == 0 && 0 != (*output)(i - output->columns - 1) )
                sum-= gamma;
            if ( (*output)(i) == 0 && 0 != (*output)(i + output->columns + 1) )
                sum-= gamma;
            if ( (*output)(i) != 0 && 0 == (*output)(i - output->columns - 1) )
                sum-= gamma;
            if ( (*output)(i) != 0 && 0 == (*output)(i + output->columns + 1) )
                sum-= gamma;
        }
        if ( (*orientation)(i) > M_PI/8 && (*orientation)(i) < M_PI/2 - M_PI/8 )
        {

             if ( (*output)(i) == 0 && 0 != (*output)(i - output->columns + 1) )
                sum-= gamma;
            if ( (*output)(i) == 0 && 0 != (*output)(i + output->columns - 1) )
                sum-= gamma;
            if ( (*output)(i) != 0 && 0 == (*output)(i - output->columns + 1) )
                sum-= gamma;
            if ( (*output)(i) != 0 && 0 == (*output)(i + output->columns - 1) )
                sum-= gamma;
        }
        if ( (*orientation)(i) > M_PI/2 - M_PI/8 && (*orientation)(i) < M_PI/2 + M_PI/8 )
        {
            if ( (*output)(i) == 0 && 0 != (*output)(i - output->columns) )
                sum-= gamma;
            if ( (*output)(i) == 0 && 0 != (*output)(i + output->columns) )
                sum-= gamma;
            if ( (*output)(i) != 0 && 0 == (*output)(i - output->columns) )
                sum-= gamma;
            if ( (*output)(i) != 0 && 0 == (*output)(i + output->columns) )
                sum-= gamma;
        }

        return sum;
    }

    void HMRFSegmentation::init()
    {
        tprintf("instantiate feature set\n");
        //Transform2Generator t2g;
        Transform2Set<float, float>& t2s= *(/*t2g.*/(generateTransform2Set<float, float>(c->featureNames)));
        //Transform2Set<float, float> t2s(c->featureNames);
        
        Border2 b= t2s.getProposedBorder();
        b.borderMode= BORDER_MODE_MIRRORED;
        input->resizeBorder(b);
        output->resizeBorder(b);
        b.borderMode= BORDER_MODE_ZERO;
        mask->resizeBorder(b);

        tprintf("create feature images\n");
        std::vector<Image<float>* > featureImages;
        for ( unsigned int i= 0; i < c->featureNames.size(); ++i )
        {
            featureImages.push_back(new Image<float>(*input));
            (*(featureImages[i]))= 0;
        }

        tprintf("create probability maps\n");
        for ( unsigned int i= 0; i < c->classNames.size(); ++i )
        {
            probabilities.push_back(new Image<float>(*input));
            (*(probabilities[i]))= 0;
        }

        tprintf("extract features\n");
        for ( unsigned int i= 0; i < featureImages.size(); ++i )
        {
            t2s[i]->apply(*input, *(featureImages[i]), mask);
        }

        tprintf("compute probability maps\n");
        FeatureVector<float> vf;
        vf.resize(featureImages.size());
        for ( unsigned int i= 0; i < input->n; ++i )
        {
            if ( (*mask)(i) > 0 )
            {
                for ( unsigned int j= 0; j < featureImages.size(); ++j )
                    vf(j)= (featureImages[j])->operator()(i);
                if ( gauss )
                    vf.gauss();
                if ( sort )
                    vf.sort();
                if ( shiftmax )
                    vf.shiftToMax();
                if ( shiftmin )
                    vf.shiftToMin();
                c->classify(vf);
                for ( unsigned int j= 0; j < c->classNames.size(); ++j )
                {
                    (*(probabilities[j]))(i)= (c->probabilities[j] / (c->probabilities[0] + c->probabilities[1]));
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

    void HMRFSegmentation::sample()
    {
        smp= rand()%output->n;
        while ( ! ((*mask)(smp) > 0 ) )
            smp= rand()%output->n;
        newLabel= rand()%probabilities.size();
    }

    double HMRFSegmentation::evaluate()
    {
        double tmp= objective;

        tmp-= probabilityComponent(smp);
        if ( beta > 0 )
            tmp-= connectivityComponent2(smp);

        oldLabel= (*output)(smp);
        (*output)(smp)= newLabel;

        tmp+= probabilityComponent(smp);
        if ( beta > 0 )
            tmp+= connectivityComponent2(smp);

        return tmp;
    }

    void HMRFSegmentation::revert()
    {
        (*output)(smp)= oldLabel;
    }
}
