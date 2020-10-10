/**
 * @file HMRFSegmentation.h
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
 * Hidden Markov Random Fields based segmentation.
 */

#ifndef HMRFSEGMENTATION_H
#define	HMRFSEGMENTATION_H

#include <openipDS/Image.h>

#include <openipML/Classifier.h>
#include <openipML/SimulatedAnnealing.h>

#include <openipLL/LowLevelTransforms.h>

namespace openip
{
    /**
     * HMRFSegmentation represents a Hidden Markov Random Fields based segmentation method
     */
    class HMRFSegmentation: public SimulatedAnnealing
    {
    public:
        /**
         * constructor
         * @param c classifier object
         * @param input input image
         * @param mask the segmentation is carried out in the foreground (non-zero) region of the mask
         * @param output result of the segmentation
         */
        HMRFSegmentation(Classifier<float>* c, Image<float>& input, Image<unsigned char>& mask, Image<unsigned char>& output);

        /**
         * copy constructor
         * @param h instance to copy
         */
        HMRFSegmentation(const HMRFSegmentation& h);

        /**
         * destructor
         */
        ~HMRFSegmentation();

        /**
         * do the segmentation with the specified parameters
         * @param beta connectivity penalty
         * @param temperature starting temperature of the simulated annealing process
         * @param epsilon stopping criteria of the simulated annealing (temperature smaller than epsilon)
         * @param decrease annealing constant, the temperature is multiplied with this in each iteration
         * @param maxIt maximum number of iterations
         * @param check output will be dumped in every check iteration
         */
        virtual void segment(double beta, double temperature= 4.0, double epsilon= 0.0001, double decrease= 0.99999, long int maxIt= 100000000, int check= 1000000);

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

        /**
         * direction dependent connectivity component of the target function
         * @param i pixel index in row-major representation
         * @return the connectivity penalty of the segmentation in position i
         */
        virtual inline float directionalComponent(int i);

        /** classifier object */
        Classifier<float>* c;
        /** input image */
        Image<float>* input;
        /** output image */
        Image<unsigned char>* output;
        /** mask image */
        Image<unsigned char>* mask;
        /** orientation image */
        Image<float>* orientation;
        /** probability maps */
        vector<Image<float>* > probabilities;

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

        bool shiftmax;
        bool shiftmin;
        bool sort;
        bool gauss;
    };
}

#endif	/* HMRFSEGMENTATION_H */

