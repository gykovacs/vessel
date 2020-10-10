#include <openipLL/thinning.h>

void openip::thinningCWSI(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* roi)
{
    BinaryMaskSet2 thinning(255, 0, BinaryMaskSet2::MATCH_ANY);
    BinaryMaskSet2 restore(255, 0, BinaryMaskSet2::MATCH_NONE);

    CWSI87_1 th11;
    CWSI87_1 th12(th11);
    th12.rotate90();
    CWSI87_1 th13(th12);
    th13.rotate90();
    CWSI87_1 th14(th13);
    th14.rotate90();
    CWSI87_2 th21;
    CWSI87_2 th22(th21);
    th22.rotate90();
    CWSI87_2 th23(th22);
    th23.rotate90();
    CWSI87_2 th24(th23);
    th24.rotate90();

    thinning.push_back(&th11);
    thinning.push_back(&th12);
    thinning.push_back(&th13);
    thinning.push_back(&th14);
    thinning.push_back(&th21);
    thinning.push_back(&th22);
    thinning.push_back(&th23);
    thinning.push_back(&th24);

    CWSI87_3 th3;
    CWSI87_4 th4;

    restore.push_back(&th3);
    restore.push_back(&th4);

    BinaryMaskSystem2 bms;
    bms.push_back(&thinning);
    bms.push_back(&restore);

    bms.generateLookUpTable();
    
    PixelSet1 out;
    output= input;
    while(1)
    {
        bms.match(output, out, &output);
        //printf("out: %d\n", out.size());
	
        out.set(output, (unsigned char)0);
	
	if ( roi )
	{
	  PixelSet1 tmp;
	  for ( unsigned int i= 0; i < out.size(); ++i )
	    if ( !roi || (*roi)(out(i)) )
	      tmp.push_back(out(i));
	  out= tmp;
	}
        //printf("%d\n", out.size());
        if ( out.size() == 0 )
            break;
    }
    
    Vector<int> pos;
    pos.push_back(1);
    pos.push_back(-input.columns + 1);
    pos.push_back(-input.columns);
    pos.push_back(-input.columns - 1);
    pos.push_back(-1);
    pos.push_back(input.columns - 1);
    pos.push_back(input.columns);
    pos.push_back(input.columns + 1);
    
    while (1)
    {
      int n= 0;
      for ( unsigned int i= input.columns + 1; i < input.n - input.columns-1; ++i )
      {
	if ( output(i) && output(i + pos(0)) && !output(i + pos(1)) && output(i + pos(2)) && !output(i + pos(3)) && !output(i + pos(4)) && !output(i + pos(5)) && !output(i + pos(6)) && ! output(i + pos(7)) )
	{
	  output(i)= 0;
	  ++n;
	}
	else if ( output(i) && !output(i + pos(0)) && !output(i + pos(1)) && output(i + pos(2)) && !output(i + pos(3)) && output(i + pos(4)) && !output(i + pos(5)) && !output(i + pos(6)) && ! output(i + pos(7)) )
	{
	  output(i)= 0;
	  ++n;
	}
	else if ( output(i) && !output(i + pos(0)) && !output(i + pos(1)) && !output(i + pos(2)) && !output(i + pos(3)) && output(i + pos(4)) && !output(i + pos(5)) && output(i + pos(6)) && ! output(i + pos(7)) )
	{
	  output(i)= 0;
	  ++n;
	}
	else if ( output(i) && output(i + pos(0)) && !output(i + pos(1)) && !output(i + pos(2)) && !output(i + pos(3)) && !output(i + pos(4)) && !output(i + pos(5)) && output(i + pos(6)) && ! output(i + pos(7)) )
	{
	  output(i)= 0;
	  ++n;
	}
      }

      if ( n == 0 )
	break;
    }
}

void openip::thinningCWSI1(Image<unsigned char>& input, Image<unsigned char>& output)
{
    BinaryMaskSet2 thinning(255, 0, BinaryMaskSet2::MATCH_ANY);
    BinaryMaskSet2 restore(255, 0, BinaryMaskSet2::MATCH_NONE);

    CWSI87_1 th11;
    CWSI87_1 th12(th11);
    th12.rotate90();
    CWSI87_1 th13(th12);
    th13.rotate90();
    CWSI87_1 th14(th13);
    th14.rotate90();
    CWSI87_2 th21;
    CWSI87_2 th22(th21);
    th22.rotate90();
    CWSI87_2 th23(th22);
    th23.rotate90();
    CWSI87_2 th24(th23);
    th24.rotate90();

    thinning.push_back(&th11);
    thinning.push_back(&th12);
    thinning.push_back(&th13);
    thinning.push_back(&th14);
    thinning.push_back(&th21);
    thinning.push_back(&th22);
    thinning.push_back(&th23);
    thinning.push_back(&th24);

    CWSI87_3 th3;
    CWSI87_4 th4;

    restore.push_back(&th3);
    restore.push_back(&th4);

    BinaryMaskSystem2 bms;
    bms.push_back(&thinning);
    bms.push_back(&restore);

    bms.generateLookUpTable();

    PixelSet1 out;
    output= input;
    while(1)
    {
        bms.match(output, out, &output);
        out.set(output, (unsigned char)0);

        if ( out.size() == 0 )
            break;
        break;
    }
}


void openip::gradientThinningCWSI(Image<float>& image, Image<unsigned char>& input, Image<unsigned char>& output)
{
    //StructuringElementDisk sedd(1, input.columns);
    //GrayscaleDilate<unsigned char, unsigned char> gd(1);
    //gd.apply(input, output);
    //input= output;

    BinaryMaskSet2 thinning(255, 0, BinaryMaskSet2::MATCH_ANY);
    BinaryMaskSet2 restore(255, 0, BinaryMaskSet2::MATCH_NONE);

    CWSI87_1 th11;
    CWSI87_1 th12(th11);
    th12.rotate90();
    CWSI87_1 th13(th12);
    th13.rotate90();
    CWSI87_1 th14(th13);
    th14.rotate90();
    CWSI87_2 th21;
    CWSI87_2 th22(th21);
    th22.rotate90();
    CWSI87_2 th23(th22);
    th23.rotate90();
    CWSI87_2 th24(th23);
    th24.rotate90();

    thinning.push_back(&th11);
    thinning.push_back(&th12);
    thinning.push_back(&th13);
    thinning.push_back(&th14);
    thinning.push_back(&th21);
    thinning.push_back(&th22);
    thinning.push_back(&th23);
    thinning.push_back(&th24);

    CWSI87_3 th3;
    CWSI87_4 th4;

    restore.push_back(&th3);
    restore.push_back(&th4);

    BinaryMaskSystem2 bms;
    bms.push_back(&thinning);
    bms.push_back(&restore);

    bms.generateLookUpTable();

    PixelSet1 out;
    PixelSet1 out2;
    output= input;

    StructuringElementDisk ses(1, input.columns);
    ses.updateStride(input.columns);

    Vector<int> toRemove;
    Vector<int> fg;
    Vector<int> bg;
    while(1)
    {
        bms.match(output, out, &output);

        toRemove.clear();
        fg.clear();
        bg.clear();
        out2.clear();
        float meanfg= 0;
        float meanbg= 0;
        int nfg= 0;
        int nbg= 0;
        for ( unsigned int i= 0; i < out.size(); ++i )
        {
            meanfg= 0;
            meanbg= 0;
            nfg= 0;
            nbg= 0;

            for ( unsigned int j= 0; j < ses.size(); ++j )
            {
                if ( ses(j) != 0 )
                {
                    if ( output(out(i) + ses(j)) )
                    {
                        meanfg+= image(out(i) + ses(j));
                        nfg++;
                    }
                    else
                    {
                        meanbg+= image(out(i) + ses(j));
                        nbg++;
                    }
                }
            }

            meanfg/= nfg;
            meanbg/= nbg;

            //if ( fabs(meanbg - image(out(i))) < fabs(meanfg - image(out(i)))*0.1 )
            //printf("%f,%f ", meanfg, meanbg);
            if ( fabs(meanbg - image(out(i))) < fabs(meanfg - image(out(i))) &&  fabs(meanfg - image(out(i))) > 0 )
                out2.push_back(out(i));
        }

        out= out2;

        out.set(output, (unsigned char)0);

        if ( out.size() == 0 )
            break;
    }
}

void openip::gradientThinningCWSI1(Image<float>& image, Image<unsigned char>& input, Image<unsigned char>& output)
{
    //StructuringElementDisk sedd(1, input.columns);
    //GrayscaleDilate<unsigned char, unsigned char> gd(1);
    //gd.apply(input, output);
    //input= output;

    BinaryMaskSet2 thinning(255, 0, BinaryMaskSet2::MATCH_ANY);
    BinaryMaskSet2 restore(255, 0, BinaryMaskSet2::MATCH_NONE);

    CWSI87_1 th11;
    CWSI87_1 th12(th11);
    th12.rotate90();
    CWSI87_1 th13(th12);
    th13.rotate90();
    CWSI87_1 th14(th13);
    th14.rotate90();
    CWSI87_2 th21;
    CWSI87_2 th22(th21);
    th22.rotate90();
    CWSI87_2 th23(th22);
    th23.rotate90();
    CWSI87_2 th24(th23);
    th24.rotate90();

    thinning.push_back(&th11);
    thinning.push_back(&th12);
    thinning.push_back(&th13);
    thinning.push_back(&th14);
    thinning.push_back(&th21);
    thinning.push_back(&th22);
    thinning.push_back(&th23);
    thinning.push_back(&th24);

    CWSI87_3 th3;
    CWSI87_4 th4;

    restore.push_back(&th3);
    restore.push_back(&th4);

    BinaryMaskSystem2 bms;
    bms.push_back(&thinning);
    bms.push_back(&restore);

    bms.generateLookUpTable();

    PixelSet1 out;
    PixelSet1 out2;
    output= input;

    StructuringElementDisk ses(1, input.columns);
    ses.updateStride(input.columns);

    Vector<int> toRemove;
    Vector<int> fg;
    Vector<int> bg;
    while(1)
    {
        bms.match(output, out, &output);

        toRemove.clear();
        fg.clear();
        bg.clear();
        out2.clear();
        float meanfg= 0;
        float meanbg= 0;
        int nfg= 0;
        int nbg= 0;
        for ( unsigned int i= 0; i < out.size(); ++i )
        {
            meanfg= 0;
            meanbg= 0;
            nfg= 0;
            nbg= 0;

            for ( unsigned int j= 0; j < ses.size(); ++j )
            {
                if ( ses(j) != 0 )
                {
                    if ( output(out(i) + ses(j)) )
                    {
                        meanfg+= image(out(i) + ses(j));
                        nfg++;
                    }
                    else
                    {
                        meanbg+= image(out(i) + ses(j));
                        nbg++;
                    }
                }
            }

            meanfg/= nfg;
            meanbg/= nbg;

            //if ( fabs(meanbg - image(out(i))) < fabs(meanfg - image(out(i)))*0.1 )
            //printf("%f,%f ", meanfg, meanbg);
            if ( fabs(meanbg - image(out(i))) < fabs(meanfg - image(out(i))) &&  fabs(meanfg - image(out(i))) > 0 )
                out2.push_back(out(i));
        }

        out= out2;

        out.set(output, (unsigned char)0);

        if ( out.size() == 0 )
            break;
        break;
    }
}


openip::CWSI87_1::CWSI87_1(int stride, unsigned char matches, unsigned char notMatches)
: BinaryMask2(stride, matches, notMatches)
{
    this->addElement(0, 0, (unsigned char)255);
    this->addElement(0, 1, (unsigned char)255);
    this->addElement(-1, 1, (unsigned char)0);
    this->addElement(-1, 0, (unsigned char)0);
    this->addElement(-1, -1, (unsigned char)0);
    this->addElement(0, -1, (unsigned char)255);
    this->addElement(1, 0, (unsigned char)255);

    this->computeMinMax();
    this->generateMaskAndPattern();
}

openip::CWSI87_2::CWSI87_2(int stride, unsigned char matches, unsigned char notMatches)
: BinaryMask2(stride, matches, notMatches)
{
    this->addElement(0, 0, (unsigned char)255);
    this->addElement(0, 1, (unsigned char)0);
    this->addElement(-1, 1, (unsigned char)0);
    this->addElement(-1, 0, (unsigned char)0);
    this->addElement(0, -1, (unsigned char)255);
    this->addElement(1, 0, (unsigned char)255);
    
    this->computeMinMax();
    this->generateMaskAndPattern();
}

openip::CWSI87_3::CWSI87_3(int stride, unsigned char matches, unsigned char notMatches)
: BinaryMask2(stride, matches, notMatches)
{
    this->addElement(0, 0, (unsigned char)255);
    this->addElement(0, 1, (unsigned char)255);
    this->addElement(0, 2, (unsigned char)0);
    this->addElement(0, -1, (unsigned char)0);

    this->computeMinMax();
    this->generateMaskAndPattern();
}

openip::CWSI87_4::CWSI87_4(int stride, unsigned char matches, unsigned char notMatches)
: BinaryMask2(stride, matches, notMatches)
{
    this->addElement(0, 0, (unsigned char)255);
    this->addElement(-1, 0, (unsigned char)0);
    this->addElement(1, 0, (unsigned char)255);
    this->addElement(2, 0, (unsigned char)0);

    this->computeMinMax();
    this->generateMaskAndPattern();
}

openip::BinaryMask2B::BinaryMask2B(int stride, int b)
: BinaryMask2(stride)
{
    this->b= b;
    this->insert(PositionWeightPair<unsigned char>(1, 255));
    this->insert(PositionWeightPair<unsigned char>(-stride+1, 255));
    this->insert(PositionWeightPair<unsigned char>(-stride, 255));
    this->insert(PositionWeightPair<unsigned char>(-stride-1, 255));
    this->insert(PositionWeightPair<unsigned char>(-1, 255));
    this->insert(PositionWeightPair<unsigned char>(stride-1, 255));
    this->insert(PositionWeightPair<unsigned char>(stride, 255));
    this->insert(PositionWeightPair<unsigned char>(stride+1, 255));
    this->min= -stride-1;
    this->max= stride+1;
}

unsigned char openip::BinaryMask2B::match(Image<unsigned char>& image, int n)
{
    int sum= 0;
    for ( BinaryMask2B::iterator bit= this->begin(); bit != this->end(); ++bit )
        if ( image(bit->first + n) == bit->second )
            ++sum;
    if ( sum >= b )
        return this->matches;
    return this->notMatches;
}

unsigned char openip::BinaryMask2B::match(unsigned int env)
{
    int sum= 0;
    unsigned int tmp= 1;
    for( int i= 0; i < 8; ++i, tmp=tmp<<1 )
        sum+= env & tmp;
    return sum;
}

openip::BinaryMask2C::BinaryMask2C(int stride, int c)
: BinaryMask2(stride)
{
    this->c= c;

    this->min= -stride-1;
    this->max= stride+1;
}

unsigned char openip::BinaryMask2C::match(Image<unsigned char>& image, int n)
{
    int sum= 0;

    sum+= image(n + 1) == image(n - stride + 1) ? 1 : 0;
    sum+= image(n - stride + 1) == image(n - stride) ? 1 : 0;
    sum+= image(n - stride) == image(n - stride - 1) ? 1 : 0;
    sum+= image(n - stride - 1) == image(n - 1) ? 1 : 0;
    sum+= image(n - 1) == image(n + stride - 1) ? 1 : 0;
    sum+= image(n + stride - 1) == image(n + stride) ? 1 : 0;
    sum+= image(n + stride) == image(n + stride + 1) ? 1 : 0;
    sum+= image(n + stride + 1) == image(n + 1) ? 1 : 0;

    if ( sum / 2 == c )
        return this->matches;
    return this->notMatches;
}

unsigned char openip::BinaryMask2C::match(unsigned int env)
{
    int sum= 0;
    unsigned int tmp= 1;
    unsigned int tmp2= 2;
    for( int i= 0; i < 7; ++i, tmp=tmp<<1, tmp2= tmp2<<1 )
        if ( ((env & tmp) - (int)(env & tmp2)) != 0 )
            ++sum;
    if ( ((env & tmp2) - (int)(env & 1)) != 0 )
        ++sum;
    return sum/2;
}

openip::BinaryMask2D::BinaryMask2D(int stride, int d)
: BinaryMask2(stride)
{
    this->d= d;

    this->min= -stride-1;
    this->max= stride+1;
}

unsigned char openip::BinaryMask2D::match(Image<unsigned char>& image, int n)
{
    int sum= 0;

    sum+= image(n + 1) != 0 ? 1 : 0;
    sum+= image(n - stride) != 0 ? 1 : 0;
    sum+= image(n - 1) != 0 ? 1 : 0;
    sum+= image(n + stride) != 0 ? 1 : 0;

    if ( sum == d )
        return this->matches;
    return this->notMatches;
}

unsigned char openip::BinaryMask2D::match(unsigned int env)
{
    int sum= 0;

    if ( env & 1 )
        ++sum;
    if ( env & 4 )
        ++sum;
    if ( env & 16 )
        ++sum;
    if ( env & 32 )
        ++sum;

    return sum;
}

