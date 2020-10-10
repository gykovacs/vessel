#ifndef VESSEL_APPLICATION_FUNCTIONS_H
#define VESSEL_APPLICATION_FUNCTIONS_H

int validateVessel(int , char** )
{
    KFoldCrossValidator<float> v;
    KFoldCrossValidator<float> vmean;
    KFoldCrossValidator<float> vimage;
    v.m.resize(2,2);
    vimage.m.resize(2,2);
    v.m= 0;
    vimage.m= 0;
    vmean.m.resize(2,2);
    vmean.m= 0;
    int sum= 0;

    char fn1[1000];
    char fn2[1000];
    char fn3[1000];

    Image<unsigned char> va, roi, vb;

    Vector<float> results, minimums, maximums, means, stds;
    results.resize(13);
    minimums.resize(13);
    maximums.resize(13);
    means.resize(13);
    stds.resize(13);
    
    minimums= FLT_MAX;
    maximums= -FLT_MAX;
    means= 0;
    stds= 0;
    
    int sumtmp;
    int images= 0;

    while(scanf("%s%s%s",fn1,fn2,fn3) == 3)
    {
        vmean.m= 0;
        ++images;
        //printf("%s %s %s\n", fn1, fn2, fn3);
        readImage(fn1, va, READ_CHANNEL_GREEN);
        readImage(fn2, roi);
        readImage(fn3, vb, READ_CHANNEL_RED);
        sumtmp= 0;
        for ( unsigned int i= 0; i < va.n; ++i )
            if ( roi(i) > 0 )
            {
                ++sum;
                if ( va(i) > 0 && vb(i) > 0 )
                {
                    v.m(1,1)+= 1;
                    vmean.m(1,1)+= 1;
                    ++sumtmp;
                }
                if ( va(i) == 0 && vb(i) == 0 )
                {
                    v.m(0,0)+= 1;
                    vmean.m(0,0)+= 1;
                    ++sumtmp;
                }
                if ( va(i) == 0 && vb(i) > 0 )
                {
                    v.m(0,1)+= 1;
                    vmean.m(0,1)+= 1;
                    ++sumtmp;
                }
                if ( va(i) > 0 && vb(i) == 0 )
                {
                    v.m(1,0)+= 1;
                    vmean.m(1,0)+= 1;
                    ++sumtmp;
                }


            }

        //printf("%d, %d, %f\n", sum, sumtmp, v.m(1,1) + v.m(0,0) + v.m(1,0) + v.m(0,1));

        results(0)= vmean.truePositive();
	results(1)= vmean.trueNegative();
	results(2)= vmean.falsePositive();
	results(3)= vmean.falseNegative();
	results(4)= vmean.specificity();
	results(5)= vmean.sensitivity();
	results(6)= vmean.accuracy();
	results(7)= vmean.precision();
	results(8)= vmean.positivePredictiveValue();
	results(9)= vmean.negativePredictiveValue();
	results(10)= vmean.falsePositiveRate();
	results(11)= vmean.falseDiscoveryRate();
	results(12)= vmean.f1Score();
        
        for ( unsigned int j= 0; j < results.size(); ++j )
	{
	  means(j)+= results(j);
	  if ( minimums(j) > results(j) )
	    minimums(j)= results(j);
	  if ( maximums(j) < results(j) )
	    maximums(j)= results(j);
	  stds(j)+= results(j)*results(j);
	}
	
    }
    //tprintf("%d %f\n", sum, v.m(0,0) + v.m(0,1) + v.m(1,0) + v.m(1,1));
    v.dumpResultsInLine();
    //vimage.dumpResultsInLine();

    for ( unsigned int j= 0; j < results.size(); ++j )
      means(j)/= images;
    for ( unsigned int j= 0; j < results.size(); ++j )
    {
      stds(j)/= images;
      stds(j)= sqrt(stds(j) - means(j)*means(j));
    }

    tprintf("means tp: %f tn: %f fp: %f fn: %f spec: %f sens: %f acc: %f prec: %f ppv: %f npv: %f fpr: %f fdr: %f f1: %f\n", means(0), means(1), means(2), means(3), means(4), means(5), means(6), means(7), means(8), means(9), means(10), means(11), means(12));
    tprintf("minimums tp: %f tn: %f fp: %f fn: %f spec: %f sens: %f acc: %f prec: %f ppv: %f npv: %f fpr: %f fdr: %f f1: %f\n", minimums(0), minimums(1), minimums(2), minimums(3), minimums(4), minimums(5), minimums(6), minimums(7), minimums(8), minimums(9), minimums(10), minimums(11), minimums(12));
    tprintf("maximums tp: %f tn: %f fp: %f fn: %f spec: %f sens: %f acc: %f prec: %f ppv: %f npv: %f fpr: %f fdr: %f f1: %f\n", maximums(0), maximums(1), maximums(2), maximums(3), maximums(4), maximums(5), maximums(6), maximums(7), maximums(8), maximums(9), maximums(10), maximums(11), maximums(12));
    tprintf("stds tp: %f tn: %f fp: %f fn: %f spec: %f sens: %f acc: %f prec: %f ppv: %f npv: %f fpr: %f fdr: %f f1: %f\n", stds(0), stds(1), stds(2), stds(3), stds(4), stds(5), stds(6), stds(7), stds(8), stds(9), stds(10), stds(11), stds(12));

    //fprintf(stderr, "%f ", means(6));
    
    return 0;
}

int validateVessel2(int , char** )
{
    KFoldCrossValidator<float> v;
    KFoldCrossValidator<float> vmean;
    KFoldCrossValidator<float> vimage;
    v.m.resize(2,2);
    vimage.m.resize(2,2);
    v.m= 0;
    vimage.m= 0;
    vmean.m.resize(2,2);
    vmean.m= 0;
    int sum= 0;

    char fn1[1000];
    char fn2[1000];
    char fn3[1000];

    Image<unsigned char> va, roi, vb;

    bool vae, vbe;

    float mtp= 0;
    float mtn= 0;
    float mfp= 0;
    float mfn= 0;
    float mspec= 0;
    float msens= 0;
    float macc= 0;
    float mprec= 0;
    float mppv= 0;
    float mnpv= 0;
    float mfpr= 0;
    float mfdr= 0;
    float mf1= 0;

    int sumtmp;
    int images= 0;
    while(scanf("%s%s%s",fn1,fn2,fn3) == 3)
    {
        vmean.m= 0;
        ++images;
        //printf("%s %s %s\n", fn1, fn2, fn3);
        readImage(fn1, va, READ_CHANNEL_GREEN);
        readImage(fn2, roi);
        readImage(fn3, vb, READ_CHANNEL_GREEN);
        sumtmp= 0;
        for ( unsigned int i= 0; i < va.n; ++i )
            if ( roi(i) > 0 )
            {
                ++sum;
                if ( va(i) > 0 && vb(i) > 0 )
                {
                    v.m(1,1)+= 1;
                    vmean.m(1,1)+= 1;
                    ++sumtmp;
                }
                if ( va(i) == 0 && vb(i) == 0 )
                {
                    v.m(0,0)+= 1;
                    vmean.m(0,0)+= 1;
                    ++sumtmp;
                }
                if ( va(i) == 0 && vb(i) > 0 )
                {
                    v.m(0,1)+= 1;
                    vmean.m(0,1)+= 1;
                    ++sumtmp;
                }
                if ( va(i) > 0 && vb(i) == 0 )
                {
                    v.m(1,0)+= 1;
                    vmean.m(1,0)+= 1;
                    ++sumtmp;
                }
            }

        //printf("%d, %d, %f\n", sum, sumtmp, v.m(1,1) + v.m(0,0) + v.m(1,0) + v.m(0,1));
        vae= false;
        vbe= false;

        for ( unsigned int i= 0; i < va.n; ++i )
            if ( va(i) > 0 )
            {
                vae= true;
                break;
            }
        for ( unsigned int i= 0; i < vb.n; ++i )
            if ( vb(i) > 0 )
            {
                vbe= true;
                break;
            }
        if ( vae == false && vbe == false )
            vimage.m(0,0)+= 1;
        if ( vae == true && vbe == true )
            vimage.m(1,1)+= 1;
        if ( vae == false && vbe == true )
            vimage.m(0,1)+= 1;
        if ( vae == true && vbe == false )
            vimage.m(1,0)+= 1;

        mtp+= vmean.truePositive();
        mtn+= vmean.trueNegative();
        mfp+= vmean.falsePositive();
        mfn+= vmean.falseNegative();
        mspec+= vmean.specificity();
        msens+= vmean.sensitivity();
        macc+= vmean.accuracy();
        mprec+= vmean.precision();
        mppv+= vmean.positivePredictiveValue();
        mnpv+= vmean.negativePredictiveValue();
        mfpr+= vmean.falsePositiveRate();
        mfdr+= vmean.falseDiscoveryRate();
        mf1+= vmean.f1Score();

        vmean.dumpResultsInLine();
    }
    //tprintf("%d %f\n", sum, v.m(0,0) + v.m(0,1) + v.m(1,0) + v.m(1,1));
    //v.dumpResultsInLine();
    //vimage.dumpResultsInLine();

    mtp/= images;
    mtn/= images;
    mfp/= images;
    mfn/= images;
    mspec/= images;
    msens/= images;
    macc/= images;
    mprec/= images;
    mppv/= images;
    mnpv/= images;
    mfpr/= images;
    mfdr/= images;
    mf1/= images;

    //tprintf("tp: %f tn: %f fp: %f fn: %f spec: %f sens: %f acc: %f prec: %f ppv: %f npv: %f fpr: %f fdr: %f f1: %f\n", mtp, mtn, mfp, mfn, mspec, msens, macc, mprec, mppv, mnpv, mfpr, mfdr, mf1);

    return 0;
}

#endif