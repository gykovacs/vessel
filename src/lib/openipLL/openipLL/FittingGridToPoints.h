#ifndef FITTINGGRIDTOPOINTS_H
#define FITTINGGRIDTOPOINTS_H

#include <openipML/SimulatedAnnealing.h>

#include <openipDS/Vector.h>

namespace openip
{
    template<typename INPUT>
    class PointWithRange1
    {
    public:
        PointWithRange1(INPUT point, INPUT lower, INPUT upper)
        {
            this->point= point;
            this->lower= lower;
            this->upper= upper;
        }

        PointWithRange1(const PointWithRange1& p)
        {
            this->point= p.point;
            this->lower= p.lower;
            this->upper= p.upper;
        }

        ~PointWithRange1()
        {
        }

        bool operator<(const PointWithRange1& a) const
        {
            return this->point < a.point;
        }

        INPUT point;
        INPUT lower;
        INPUT upper;
    };

    template<typename INPUT>
    class FittingGridToRegions1
    {
    public:
        FittingGridToRegions1();

        FittingGridToRegions1(const FittingGridToRegions1& f);

        ~FittingGridToRegions1();

        void doFitting(Region3Set& rs, Vector<unsigned char>& mask);

        void doFitting(Vector<PointWithRange1<INPUT> >& projectedPoints);

        double evaluate(int f, PointWithRange1<INPUT>& p, Vector<PointWithRange1<INPUT> >& points);

        float n;
        float d;
        float s;
    };

    template<typename INPUT>
    FittingGridToRegions1<INPUT>::FittingGridToRegions1()
    {
    }

    template<typename INPUT>
    FittingGridToRegions1<INPUT>::FittingGridToRegions1(const FittingGridToRegions1& f)
    {
    }

    template<typename INPUT>
    FittingGridToRegions1<INPUT>::~FittingGridToRegions1()
    {
    }

    template<typename INPUT>
    void FittingGridToRegions1<INPUT>::doFitting(Region3Set& rs, Vector<unsigned char>& mask)
    {
        Vector<PointWithRange1<INPUT> > projectedPoints;

        int smin, smax, cmin, cmax, rmin, rmax;

        for ( unsigned int i= 0; i < rs.size(); ++i )
        {
            rs(i).getMinMaxCoordinates(smin, smax, rmin, rmax, cmin, cmax);
            if ( mask(0) )
                projectedPoints.push_back(PointWithRange1<INPUT>((smax + smin)/2, smin, smax));
            else if ( mask(1) )
                projectedPoints.push_back(PointWithRange1<INPUT>((rmax + rmin)/2, rmin, rmax));
            else if ( mask(2) )
                projectedPoints.push_back(PointWithRange1<INPUT>((cmax + cmin)/2, cmin, cmax));
        }

        for ( unsigned int i= 0; i < projectedPoints.size(); ++i )
            cout << projectedPoints(i).lower << " " << projectedPoints(i).point << " " << projectedPoints(i).upper << endl;
        doFitting(projectedPoints);
    }

    template<typename INPUT>
    void FittingGridToRegions1<INPUT>::doFitting(Vector<PointWithRange1<INPUT> >& projectedPoints)
    {
      std::sort(projectedPoints.begin(), projectedPoints.end());

      Vector<int> d;
      for ( unsigned int i= 0; i < projectedPoints.size(); ++i )
      {
          for ( unsigned int j= i; j < projectedPoints.size(); ++j )
          {
              if ( projectedPoints(i).upper < projectedPoints(j).lower )
                  d.push_back(projectedPoints(j).point - projectedPoints(i).point);
          }
      }

      if ( d.size() > 0 )
      {
          /*cout << "sorted" << endl;
          for ( int i= 0; i < projectedPoints.size(); ++i )
              cout << projectedPoints(i).lower << " " << projectedPoints(i).point << " " << projectedPoints(i).upper << endl;*/

          /*cout << "d" << endl;
          for ( int i= 0; i < d.size(); ++i )
              cout << d(i) << " ";
          cout << endl;*/

          Vector<float> evaluation;
          evaluation.resize(d.size());
          for ( unsigned int i= 0; i < d.size(); ++i )
              for ( unsigned int j= 0; j < projectedPoints.size(); ++j )
                  evaluation(i)+= evaluate(d(i), projectedPoints(j), projectedPoints);

          for ( unsigned int i= 0; i < evaluation.size(); ++i )
              cout << evaluation(i) << " ";
          cout << endl;

          int minIdx= 0;
          for ( unsigned int i= 0; i < evaluation.size(); ++i )
              if ( evaluation(i) < evaluation(minIdx) )
                  minIdx= i;

          int startIdx= 0;
          float startValue= FLT_MAX;
          for ( unsigned int i= 0; i < projectedPoints.size(); ++i )
          {
              float tmp= evaluate(d(minIdx), projectedPoints(i), projectedPoints);
              if ( tmp < startValue )
              {
                  startIdx= i;
                  startValue= tmp;
              }
          }

          this->d= d(minIdx);
          this->s= projectedPoints(startIdx).point;

          int minCoord= floor(float((projectedPoints(0).point - s))/(this->d) + 0.5f);
          int maxCoord= floor(float((projectedPoints(projectedPoints.size()-1).point - s))/(this->d) + 0.5f);

          this->n= maxCoord - minCoord + 1;

          //shifting origin
          this->s= this->s + minCoord * (this->d);
      }
      else
      {
          this->n= 1;
          this->s= projectedPoints(0).point;
          this->d= 0;
      }

      printf("start: %f, d: %f, n: %f\n", this->s, this->d, this->n);


    }

    template<typename INPUT>
    double FittingGridToRegions1<INPUT>::evaluate(int f, PointWithRange1<INPUT>& p, Vector<PointWithRange1<INPUT> >& projectedPoints)
    {
        int minCoord= floor(float((projectedPoints(0).point - p.point))/f + 0.5f);
        int maxCoord= floor(float((projectedPoints(projectedPoints.size()-1).point - p.point))/f + 0.5f);

        //printf("min, max, f: %d %d %d\n", minCoord, maxCoord, f);
        float result= 0;
        float minDist;
        float dist;
        for ( unsigned int i= 0; i < projectedPoints.size(); ++i )
        {
          minDist= FLT_MAX;
          for ( int j= minCoord; j <= maxCoord; ++j )
          {
              dist= sqrt((projectedPoints(i).point - (p.point + j*f))*(projectedPoints(i).point - (p.point + j*f)));
              if ( minDist > dist )
                  minDist= dist;
          }
          result+= minDist;
        }
        //printf("%f\n", result);
        return result;
    }
}

#endif // FITTINGGRIDTOPOINTS_H
