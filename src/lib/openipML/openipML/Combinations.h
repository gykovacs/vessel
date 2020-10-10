/* 
 * File:   Combinations.h
 * Author: gykovacs
 *
 * Created on 2011. május 30., 9:43
 */

#ifndef COMBINATIONS_H
#define	COMBINATIONS_H

#include <vector>

namespace openip
{
    class Combinations
    {
    public:
        Combinations(int n, int k);

        Combinations(const Combinations& c);

        ~Combinations();

        int hasNextCombination();

        void nextCombination(std::vector<int>& c);

        int n;
        int k;
        int idx;
        int total;
        std::vector<int> indices;
    };
}

#endif	/* COMBINATIONS_H */

