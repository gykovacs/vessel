#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <float.h>
#include <time.h>

#include <openipML/SimulatedAnnealing.h>
#include <openipDS/Stopper.h>

using namespace std;

namespace openip
{
    SimulatedAnnealing::SimulatedAnnealing(long int maximumIt, double eps, double ann, double temp, int out)
        : objective(0.0), newObjective(0.0 + 2 * eps), lastObjective(FLT_MAX), temperature(temp), iteration(0), maximumIteration(maximumIt), epsilon(eps), annealing(ann), output(out)
    {
    }

    SimulatedAnnealing::SimulatedAnnealing(const SimulatedAnnealing& sa)
        : objective(sa.objective), newObjective(sa.newObjective), temperature(sa.temperature), iteration(sa.iteration), maximumIteration(sa.maximumIteration), epsilon(sa.epsilon), annealing(sa.annealing), output(sa.output)
    {
    }

    SimulatedAnnealing::~SimulatedAnnealing()
    {
    }

    void SimulatedAnnealing::solve()
    {
        srand(time(NULL));
        float rnd;

        init();

        //objective= 0;
        //newObjective= evaluate();
        objective= evaluate();
        while ( !stopCondition() )
        {
            //if ( iteration )



            if ( output == 1 )
                dump();
            else if ( output > 1 && iteration % output == 0 )
                dump();

            sample();
            newObjective= evaluate();

            ++iteration;

            if ( !betterObjectiveFunction(newObjective) )
            {
                rnd= urfg.generate();
                if ( rnd > temperature/fabs(objective - newObjective) )
                {
                    revert();
                    continue;
                }
            }

            decreaseTemperature();

            objective= newObjective;

/*            if ( stopCondition() )
                break;*/

            
        }
    }

    int SimulatedAnnealing::betterObjectiveFunction(double newObjective)
    {
        if ( newObjective > objective )
            return 1;
        return 0;
    }

    void SimulatedAnnealing::decreaseTemperature()
    {
        temperature*= annealing;
    }

    int SimulatedAnnealing::stopCondition()
    {
        if ( iteration >= maximumIteration )
        {
            tprintf("stop condition satisfied: maximum iteration\n");
            tprintf("\titeration: %d\n", iteration);
            tprintf("\tobjective: %f\n", objective);
            tprintf("\ttemperature: %f\n", temperature);
            return 1;
        }
        if ( temperature < epsilon )
        {
            tprintf("stop condition satisfied: temperature < epsilon\n");
            tprintf("\titeration: %d\n", iteration);
            tprintf("\tobjective: %f\n", objective);
            tprintf("\ttemperature: %f\n", temperature);
            return 1;
        }
        if ( output == 1 || (output > 1 && iteration % output == 0) )
        {
            if ( fabs(objective - lastObjective) < 0.001 )
            {
                tprintf("stop condition satisfied: (objective - newObjective) < 0.001\n");
                tprintf("\titeration: %d\n", iteration);
                tprintf("\tobjective: %f\n", objective);
                tprintf("\ttemperature: %f\n", temperature);
                return 1;
            }
            lastObjective= objective;
        }

        return 0;
    }

    void SimulatedAnnealing::dump()
    {
    }
}
