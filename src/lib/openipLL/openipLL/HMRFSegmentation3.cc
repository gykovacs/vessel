#include <openipDS/Stopper.h>

#include <openipLL/HMRFSegmentation3.h>
#include <openipLL/imageIO.h>
#include <openipLL/Transform3Set.h>
#include <openipLL/Transform3Generator.h>

namespace openip
{
    HMRFSegmentation3::HMRFSegmentation3(Classifier<float>* c, Volume<float>& input, Volume<unsigned char>& mask, Volume<unsigned char>& support, Volume<unsigned char>& output)
    : SimulatedAnnealing(100000000000, 0.0000001, 0.999999)
    {
        this->c= c;
        this->input= &input;
        this->output= &output;
        this->mask= &mask;
	this->support= &support;
	
        output= 0;
        temperature= 0.1;
    }

    void HMRFSegmentation3::solve()
    {
        float rnd;

        init();

        objective= 0;
        int last= 0;
        double lastO= 0;
        tprintf("start optimization\n");
        /*printf("input: %d %d %d %d %d %d %d %d %d\n", input->leftBorder, input->rightBorder, input->topBorder, input->bottomBorder, input->frontBorder, input->backBorder, input->slices, input->rows, input->columns);
        printf("mask: %d %d %d %d %d %d %d %d %d\n", mask->leftBorder, mask->rightBorder, mask->topBorder, mask->bottomBorder, mask->frontBorder, mask->backBorder, mask->slices, mask->rows, mask->columns);
        printf("output: %d %d %d %d %d %d %d %d %d\n", output->leftBorder, output->rightBorder, output->topBorder, output->bottomBorder, output->frontBorder, output->backBorder, output->slices, output->rows, output->columns);*/
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
                {
                    tprintf("STOP: objective did not change\n");
                    break;
                }
                last= iteration;
                lastO= objective;
            }
        }
    }

    HMRFSegmentation3::HMRFSegmentation3(const HMRFSegmentation3& h)
    : SimulatedAnnealing(h), c(h.c), input(h.input), output(h.output), probabilities(h.probabilities)
    {
    }

    HMRFSegmentation3::~HMRFSegmentation3()
    {
    }

    void HMRFSegmentation3::segment(double beta, double temperature, double epsilon, double decrease, long int maxIt, int check)
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

    float HMRFSegmentation3::probabilityComponent(int i)
    {
        return ((*(probabilities[(*output)(i)]))(i));
    }

    float HMRFSegmentation3::connectivityComponent(int i)
    {
        if ( (*support)(i) == 10 )
	  return 0;
	
        float sum= 0;
        if ( (*output)(i) != (*output)(i - output->columns) )
            sum-= beta;
        if ( (*output)(i) != (*output)(i + output->columns) )
            sum-= beta;
        if ( (*output)(i) != (*output)(i - 1) )
            sum-= beta;
        if ( (*output)(i) != (*output)(i + 1) )
            sum-= beta;
        if ( (*output)(i) != (*output)(i - output->sliceSize) )
            sum-= 2*beta;
        if ( (*output)(i) != (*output)(i + output->sliceSize) )
            sum-= 2*beta;

        return sum;
    }

    float HMRFSegmentation3::connectivityComponent2(int i)
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

    void HMRFSegmentation3::init()
    {
        tprintf("instantiate transform set\n");
        //Transform2Generator t2g;
        Transform3Set<float, float>& t3s= *(/*t2g.*/(generateTransform3Set<float, float>(c->featureNames)));
        Border3 b1= t3s.getProposedBorder();
        Border3 b2(1,1,1,1,1,1,BORDER_MODE_ZERO);
        Border3 b= maximizeBorders(b1, b2);
        b.borderMode= BORDER_MODE_ZERO;
        input->resizeBorder(b);
        output->resizeBorder(b);
        b.borderMode= BORDER_MODE_ZERO;
	writeVolume("aaa.mnc", *mask);
        mask->resizeBorder(b);
	support->resizeBorder(b);
        //Transform2Set<float, float> t2s(c->featureNames);

        tprintf("create feature images\n");
        //std::vector<Volume<float>* > featureImages;
	featureImages.resize(t3s.dimensions());
	featureImages.resizeVolumes(*input);
        /*for ( unsigned int i= 0; i < c->featureNames.size(); ++i )
        {
            featureImages.push_back(new Volume<float>(*input));
            (*(featureImages[i]))= 0;
        }*/
	t3s.apply(*input, featureImages, mask, support);

        tprintf("create probability maps\n");
        for ( unsigned int i= 0; i < c->classNames.size(); ++i )
        {
            probabilities.push_back(new Volume<float>(*input));
            (*(probabilities[i]))= 0;
        }

        tprintf("extract features\n");
        /*for ( unsigned int i= 0; i < featureImages.size(); ++i )
        {
            t3s[i]->apply(*input, *(featureImages[i]), mask);
        }*/

        tprintf("compute probability maps\n");
        Vector<float> vf;
        vf.resize(t3s.dimensions());
        for ( unsigned int i= 0; i < input->n; ++i )
        {
            if ( (*mask)(i) > 0.5 )
            {
                for ( unsigned int j= 0; j < featureImages.size(); ++j )
                    vf(j)= (featureImages(j)).operator()(i);
                c->classify(vf);
                for ( unsigned int j= 0; j < c->classNames.size(); ++j )
                {
		  if ( c->probabilities[j] >= 0 && c->probabilities[j] <= 1 )
		    (*(probabilities[j]))(i)= c->probabilities[j];
		  else
                    (*(probabilities[j]))(i)= 0;
                }
            }
        }
        tprintf("probability maps computed, %f %f %f %f %f %f\n", probabilities[0]->getMin(), probabilities[0]->getMax(), probabilities[0]->getMean(), probabilities[1]->getMin(), probabilities[1]->getMax(), probabilities[1]->getMean());
	tprintf("%d %d %d %d\n", probabilities[0]->slices, probabilities[0]->rows, probabilities[0]->columns, probabilities[0]->n);

        srand(time(NULL));
        for ( unsigned int i= 0; i < output->n; ++i )
            if ( (*mask)(i) > 0.5 )
                (*output)(i)= rand()%probabilities.size();
	
        objective= 0;
    }

    void HMRFSegmentation3::sample()
    {
        smp= rand()%output->n;
        while (! (smp > int(output->sliceSize) && smp < int(output->n - output->sliceSize) && (*mask)(smp) > 0.5 ) )
            smp= rand()%output->n;
        newLabel= rand()%probabilities.size();
    }

    double HMRFSegmentation3::evaluate()
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

    void HMRFSegmentation3::revert()
    {
        (*output)(smp)= oldLabel;
    }
}
