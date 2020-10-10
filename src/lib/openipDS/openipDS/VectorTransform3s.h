/* 
 * File:   VectorTransform3.h
 * Author: gykovacs
 *
 * Created on July 20, 2011, 3:43 PM
 */

#ifndef VECTORTRANSFORM3S_H
#define	VECTORTRANSFORM3S_H

#include <openipDS/Volume.h>
#include <openipDS/VolumeVector.h>
#include <openipDS/VectorTransform3.h>
#include <string>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class ClosestFeatureVectorTransform3: public VectorTransform3<INPUT, OUTPUT>
    {
    public:
        using VectorTransform3<INPUT, OUTPUT>::getDescriptor;
	using VectorTransform3<INPUT, OUTPUT>::descriptor;
      
        ClosestFeatureVectorTransform3(int label, int n);

        ClosestFeatureVectorTransform3(const ClosestFeatureVectorTransform3& vt);

        virtual ~ClosestFeatureVectorTransform3();

        virtual void apply(Volume<INPUT>& input, VolumeVector<OUTPUT>& output, int begin= 0, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        virtual int dimensions();

        virtual Border3 getProposedBorder();
        
	int label;
	int n;
	Vector<StructuringElementBall* > ses;
    };

    template<typename INPUT, typename OUTPUT>
    ClosestFeatureVectorTransform3<INPUT, OUTPUT>::ClosestFeatureVectorTransform3(int label, int n)
    {
      this->label= label;
      stringstream ss;
      ss << "ClosestFeatureVectorTransform3 ";
      ss << label;
      ss << " ";
      ss << n;
      descriptor= ss.str();
      
      for ( int i= 1; i < n; ++i )
	ses.push_back(new StructuringElementBall(i));
    }

    template<typename INPUT, typename OUTPUT>
    ClosestFeatureVectorTransform3<INPUT, OUTPUT>::ClosestFeatureVectorTransform3(const ClosestFeatureVectorTransform3& vt)
    {
        this->descriptor= vt.descriptor;
	this->label= vt.label;
	this->n= vt.n;
    }

    template<typename INPUT, typename OUTPUT>
    ClosestFeatureVectorTransform3<INPUT, OUTPUT>::~ClosestFeatureVectorTransform3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ClosestFeatureVectorTransform3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, VolumeVector<OUTPUT>& output, int n, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
      for ( unsigned int i= 0; i < ses.size(); ++i )
	ses(i)->updateStride(input.columns, input.sliceSize);
      
      int flag= 0;
      int begin= -ses(ses.size()-1)->getMin();
      int end= input.n - ses(ses.size()-1)->getMax();
      output(n + 0)= ses.size()*2;
      output(n + 1)= ses.size()*2;
      output(n + 2)= ses.size()*2;
      output(n + 3)= -1;
      /*output(n + 4)= ses.size()*2;
      output(n + 5)= ses.size()*2;
      output(n + 6)= ses.size()*2;*/
      for ( int i= begin; i < end; ++i )
      {
	if ( !roi || (*roi)(i) )
	{
	  flag= 0;
	  for ( unsigned int j= 0; j < ses.size() && !flag ; ++j )
	  {
	    for ( unsigned int k= 0; k < ses(j)->size() && !flag; ++k )
	    {
	      if ( (*support)(i + (*(ses(j)))(k)) == label )
	      {
		int i1, i2, i3;
		int j1, j2, j3;
		ses(j)->getCoordinates3D(i, i1, i2, i3);
		ses(j)->getCoordinates3D(i + (*(ses(j)))(k), j1, j2, j3);
		
		/*output(n + 0)(i)= ((float)(i1 - j1))/(k+1);
		output(n + 1)(i)= ((float)(i2 - j2))/(k+1);
		output(n + 2)(i)= ((float)(i3 - j3))/(k+1);*/
		
		output(n + 0)(i)= ((float)(i1 - j1));
		output(n + 1)(i)= ((float)(i2 - j2));
		output(n + 2)(i)= ((float)(i3 - j3));
		
		output(n + 3)(i)= k;
		
		/*output(n + 4)(i)= abs(i1 - j1);
		output(n + 5)(i)= abs(i2 - j2);
		output(n + 6)(i)= abs(i3 - j3);*/
		flag= 1;
	      }
	    }
	  }
	}
      }
    }

    template<typename INPUT, typename OUTPUT>
    void ClosestFeatureVectorTransform3<INPUT, OUTPUT>::apply(Volume<INPUT>&, Volume<OUTPUT>&, Volume<unsigned char>*, Volume<unsigned char>*)
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 ClosestFeatureVectorTransform3<INPUT, OUTPUT>::getProposedBorder()
    {
      Border3 b(0,0,0,0,0,0);
      for ( unsigned int i= 0; i < ses.size(); ++i )
      {
	Border3 b1= Border3(ses(i)->r, ses(i)->r, ses(i)->r, ses(i)->r, ses(i)->r, ses(i)->r);
	b= maximizeBorders(b, b1);
      }
      
      return b;
    }

    template<typename INPUT, typename OUTPUT>
    int ClosestFeatureVectorTransform3<INPUT, OUTPUT>::dimensions()
    {
        return 4;
    }
}

#endif	/* VECTORTRANSFORM3_H */

