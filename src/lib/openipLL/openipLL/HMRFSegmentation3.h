/* 
 * File:   HMRFSegmentation3.h
 * Author: gykovacs
 *
 * Created on July 23, 2011, 1:39 PM
 */

#ifndef HMRFSEGMENTATION3_H
#define	HMRFSEGMENTATION3_H

#include <openipDS/Volume.h>
#include <openipDS/VolumeVector.h>

#include <openipML/Classifier.h>
#include <openipML/SimulatedAnnealing.h>

namespace openip
{
    class HMRFSegmentation3: public SimulatedAnnealing
    {
    public:
        /**
         * constructor
         * @param c classifier object
         * @param input input image
         * @param mask the segmentation is carried out in the foreground (non-zero) region of the mask
         * @param output result of the segmentation
         */
        HMRFSegmentation3(Classifier<float>* c, Volume<float>& input, Volume<unsigned char>& mask, Volume<unsigned char>& support, Volume<unsigned char>& output);

        /**
         * copy constructor
         * @param h instance to copy
         */
        HMRFSegmentation3(const HMRFSegmentation3& h);

        /**
         * destructor
         */
        ~HMRFSegmentation3();

        /**
         * do the segmentation with the specified parameters
         * @param beta connectivity penalty
         * @param temperature starting temperature of the simulated annealing process
         * @param epsilon stopping criteria of the simulated annealing (temperature smaller than epsilon)
         * @param decrease annealing constant, the temperature is multiplied with this in each iteration
         * @param maxIt maximum number of iterations
         * @param check output will be dumped in every check iteration
         */
        virtual void segment(double beta, double temperature= 3.0, double epsilon= 0.000000000001, double decrease= 0.9999999, long int maxIt= 100000000, int check= 1000000);

        /**
         * initialization of the simulated annealing
         */
        virtual void init();

        /**
         * sampling function
         */
        virtual inline void sample();

        /**
         * evaluation of the actual segmentation
         * @return measure of goodness of segmentation
         */
        virtual inline double evaluate();

        /**
         * reverting last change
         */
        virtual inline void revert();

        /**
         * start the iteration
         */
        virtual void solve();

        /**
         * probability component of the target function
         * @param i pixel index in row-major representation
         * @return the probability of right segmentation in position i
         */
        virtual inline float probabilityComponent(int i);

        /**
         * connectivity component of the target function
         * @param i pixel index in row-major representation
         * @return the connectivity penalty of the segmentation in position i
         */
        virtual inline float connectivityComponent(int i);

        /**
         * test: connectivity component of the target function (8-connectivity)
         * @param i pixel index in row-major representation
         * @return the connectivity penalty of the segmentation in position i
         */
        virtual inline float connectivityComponent2(int i);

        /** classifier object */
        Classifier<float>* c;
        /** input image */
        Volume<float>* input;
        /** output image */
        Volume<unsigned char>* output;
        /** mask image */
        Volume<unsigned char>* mask;
        /** probability maps */
        vector<Volume<float>* > probabilities;
	Volume<unsigned char>* support;
	VolumeVector<float> featureImages;

        /** connectivity penalty parameter */
        float beta;
        /** orientation dependent connectivity penalty parameter */
        float gamma;
        /** check parameter */
        int check;

        /** sample variable */
        int smp;
        /** new label */
        int newLabel;
        /** old label */
        int oldLabel;
        /** minimum */
        int min;
        /** maximum */
        int max;
    };
}
#endif	/* HMRFSEGMENTATION3_H */

