/* 
 * File:   Noises.h
 * Author: gykovacs
 *
 * Created on October 14, 2011, 3:46 PM
 */

#ifndef NOISES_H
#define	NOISES_H

#include <openipML/Noise.h>
#include <openipSC/RandomNumberGenerator.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

namespace openip
{
    template<typename T>
    class AmplifierNoise: public Noise<T>
    {
    public:
        using Noise<T>::apply;
        
        AmplifierNoise(float mean, float var);
        
        AmplifierNoise(const AmplifierNoise& a);
        
        ~AmplifierNoise();
        
        virtual void apply(T& input);
        
        GaussianRandomFloatGenerator* noise;
    };
    
    template<typename T>
    AmplifierNoise<T>::AmplifierNoise(float mean, float var)
    : Noise<T>()
    {
        noise= new GaussianRandomFloatGenerator(mean, var);
    }
    
    template<typename T>
    AmplifierNoise<T>::AmplifierNoise(const AmplifierNoise& a)
    : Noise<T>(a)
    {
        noise= new GaussianRandomFloatGenerator(*(a.noise));
    }
    
    template<typename T>
    AmplifierNoise<T>::~AmplifierNoise()
    {
        delete noise;
    }
    
    template<typename T>
    void AmplifierNoise<T>::apply(T& input)
    {
        input+= noise->generate();
    }
    
    template<typename T>
    class PoissonNoise: public Noise<T>
    {
    public:
        using Noise<T>::apply;
        
        PoissonNoise();
        
        PoissonNoise(const PoissonNoise& a);
        
        ~PoissonNoise();
        
        virtual void apply(T& input);
    };
    
    template<typename T>
    PoissonNoise<T>::PoissonNoise()
    : Noise<T>()
    {
    }
    
    template<typename T>
    PoissonNoise<T>::PoissonNoise(const PoissonNoise& a)
    : Noise<T>(a)
    {
    }
    
    template<typename T>
    PoissonNoise<T>::~PoissonNoise()
    {
    }
    
    template<typename T>
    void PoissonNoise<T>::apply(T& input)
    {
      PoissonRandomFloatGenerator pr(input);
      input= pr.generate();
    }
    
    template<typename T>
    class SaltAndPepperNoise: public Noise<T>
    {
    public:
        using Noise<T>::apply;
        
        SaltAndPepperNoise(float probability);
        
        SaltAndPepperNoise(const SaltAndPepperNoise& a);
        
        ~SaltAndPepperNoise();
        
        virtual void apply(T& input);
        
        virtual void apply(Vector<T>& input);
        
        UniformRandomFloatGenerator* noise;
        float probability;
    };
    
    template<typename T>
    SaltAndPepperNoise<T>::SaltAndPepperNoise(float probability)
    : Noise<T>()
    {
        noise= new UniformRandomFloatGenerator(0, 1);
        this->probability= probability;
    }
    
    template<typename T>
    SaltAndPepperNoise<T>::SaltAndPepperNoise(const SaltAndPepperNoise& a)
    : Noise<T>(a)
    {
        noise= new UniformRandomFloatGenerator(*(a.noise));
        probability= a.probability;
    }
    
    template<typename T>
    SaltAndPepperNoise<T>::~SaltAndPepperNoise()
    {
        delete noise;
    }
    
    template<typename T>
    void SaltAndPepperNoise<T>::apply(T& /*input*/)
    {
    }
    
    template<typename T>
    void SaltAndPepperNoise<T>::apply(Vector<T>& input)
    {
        T min= input(0), max= input(0);
        for ( unsigned int i= 1; i < input.size(); ++i )
            if ( input(i) < min )
                min= input(i);
            else if ( input(i) > max )
                max= input(i);
            
        for ( unsigned int i= 0; i < input.size(); ++i )
            if ( noise->generate() < probability )
            {
                if ( noise->generate() > 0.5 )
                    input(i)= max;
                else
                    input(i)= min;
            }
    }
    
    template<typename T>
    class UniformNoise: public Noise<T>
    {
    public:
        using Noise<T>::apply;
        
        UniformNoise(float amount);
        
        UniformNoise(const UniformNoise& a);
        
        ~UniformNoise();
        
        virtual void apply(T& input);
        
        UniformRandomFloatGenerator* noise;
        float amount;
    };
    
    template<typename T>
    UniformNoise<T>::UniformNoise(float amount)
    : Noise<T>()
    {
        noise= new UniformRandomFloatGenerator(-0.5, 0.5);
        this->amount= amount;
    }
    
    template<typename T>
    UniformNoise<T>::UniformNoise(const UniformNoise& a)
    : Noise<T>(a)
    {
        noise= new UniformRandomFloatGenerator(*(a.noise));
        amount= a.amount;
    }
    
    template<typename T>
    UniformNoise<T>::~UniformNoise()
    {
        delete noise;
    }
    
    template<typename T>
    void UniformNoise<T>::apply(T& input)
    {
        input+= noise->generate()*amount;
    }
    
    template<typename T>
    class GaussianTranslationNoise: public Noise<T>
    {
    public:
        using Noise<T>::apply;
        
        GaussianTranslationNoise(float mean, float var);
        
        GaussianTranslationNoise(const GaussianTranslationNoise& a);
        
        ~GaussianTranslationNoise();
        
        virtual void apply(T& input);
        
        virtual void apply(Vector<T>& input);
        
        GaussianRandomFloatGenerator* noise;
    };
    
    template<typename T>
    GaussianTranslationNoise<T>::GaussianTranslationNoise(float mean, float var)
    : Noise<T>()
    {
        noise= new GaussianRandomFloatGenerator(mean, var);
    }
    
    template<typename T>
    GaussianTranslationNoise<T>::GaussianTranslationNoise(const GaussianTranslationNoise& a)
    : Noise<T>(a)
    {
        noise= new GaussianRandomFloatGenerator(*(a.noise));
    }
    
    template<typename T>
    GaussianTranslationNoise<T>::~GaussianTranslationNoise()
    {
        delete noise;
    }
    
    template<typename T>
    void GaussianTranslationNoise<T>::apply(T& input)
    {
    }
    
    template<typename T>
    void GaussianTranslationNoise<T>::apply(Vector<T>& input)
    {
        input+= noise->generate();
    }
    
    template<typename T>
    class GaussianScalingNoise: public Noise<T>
    {
    public:
        using Noise<T>::apply;
        
        GaussianScalingNoise(float mean, float var);
        
        GaussianScalingNoise(const GaussianScalingNoise& a);
        
        ~GaussianScalingNoise();
        
        virtual void apply(T& input);
        
        virtual void apply(Vector<T>& input);
        
        GaussianRandomFloatGenerator* noise;
    };
    
    template<typename T>
    GaussianScalingNoise<T>::GaussianScalingNoise(float mean, float var)
    : Noise<T>()
    {
        noise= new GaussianRandomFloatGenerator(mean, var);
    }
    
    template<typename T>
    GaussianScalingNoise<T>::GaussianScalingNoise(const GaussianScalingNoise& a)
    : Noise<T>(a)
    {
        noise= new GaussianRandomFloatGenerator(*(a.noise));
    }
    
    template<typename T>
    GaussianScalingNoise<T>::~GaussianScalingNoise()
    {
        delete noise;
    }
    
    template<typename T>
    void GaussianScalingNoise<T>::apply(T& input)
    {
    }
    
    template<typename T>
    void GaussianScalingNoise<T>::apply(Vector<T>& input)
    {
        input*= noise->generate();
    }
    
    template<typename T>
    class UniformTranslationNoise: public Noise<T>
    {
    public:
        using Noise<T>::apply;
        
        UniformTranslationNoise(float a, float b);
        
        UniformTranslationNoise(const UniformTranslationNoise& a);
        
        ~UniformTranslationNoise();
        
        virtual void apply(T& input);
        
        virtual void apply(Vector<T>& input);
        
        UniformRandomFloatGenerator* noise;
    };
    
    template<typename T>
    UniformTranslationNoise<T>::UniformTranslationNoise(float a, float b)
    : Noise<T>()
    {
        noise= new UniformRandomFloatGenerator(a, b);
    }
    
    template<typename T>
    UniformTranslationNoise<T>::UniformTranslationNoise(const UniformTranslationNoise& a)
    : Noise<T>(a)
    {
        noise= new UniformRandomFloatGenerator(*(a.noise));
    }
    
    template<typename T>
    UniformTranslationNoise<T>::~UniformTranslationNoise()
    {
        delete noise;
    }
    
    template<typename T>
    void UniformTranslationNoise<T>::apply(T& input)
    {
    }
    
    template<typename T>
    void UniformTranslationNoise<T>::apply(Vector<T>& input)
    {
        input+= noise->generate();
    }
    
    template<typename T>
    class UniformScalingNoise: public Noise<T>
    {
    public:
        using Noise<T>::apply;
        
        UniformScalingNoise(float a, float b);
        
        UniformScalingNoise(const UniformScalingNoise& a);
        
        ~UniformScalingNoise();
        
        virtual void apply(T& input);
        
        virtual void apply(Vector<T>& input);
        
        UniformRandomFloatGenerator* noise;
    };
    
    template<typename T>
    UniformScalingNoise<T>::UniformScalingNoise(float a, float b)
    : Noise<T>()
    {
        noise= new UniformRandomFloatGenerator(a, b);
    }
    
    template<typename T>
    UniformScalingNoise<T>::UniformScalingNoise(const UniformScalingNoise& a)
    : Noise<T>(a)
    {
        noise= new UniformRandomFloatGenerator(*(a.noise));
    }
    
    template<typename T>
    UniformScalingNoise<T>::~UniformScalingNoise()
    {
        delete noise;
    }
    
    template<typename T>
    void UniformScalingNoise<T>::apply(T& input)
    {
    }
    
    template<typename T>
    void UniformScalingNoise<T>::apply(Vector<T>& input)
    {
        input*= noise->generate();
    }
}

#endif	/* NOISES_H */

