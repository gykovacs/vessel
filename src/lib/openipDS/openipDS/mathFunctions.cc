#include <openipDS/mathFunctions.h>

#include <math.h>
#include <float.h>
#include <stdio.h>

//#define _USE_MATH_DEFINES
#ifndef M_PI
#define M_PI 3.141529
#endif

double openip::sech(double d)
{
    return 1/cosh(d);
}

std::string openip::type(const std::type_info& ti)
{
    if ( ti == typeid(float) )
        return std::string("float");
    if ( ti == typeid(double) )
        return std::string("double");
    if ( ti == typeid(unsigned char) )
        return std::string("unsigned char");
    else
        return std::string("NULL");
}

double openip::gauss1(float x, float sd, float alpha, float beta, float delta, float gamma)
{
    if ( fabs(sd) < FLT_EPSILON )
    {
        if ( fabs(x) < FLT_EPSILON )
            return delta*1 + gamma;
        else
            return gamma;
    }

    if ( fabs(alpha) > 0.5 )
        return gamma + delta * exp (-pow(((x*x)/(2*sd*sd))/alpha, beta)) / (sqrt(2*M_PI)*sd);
    else
        return gamma + delta * exp (-pow(((x*x)/(2*sd*sd)), beta)) / (sqrt(2*M_PI)*sd);
}

double openip::gauss2(float x, float y, float sd, float alpha, float beta, float delta, float gamma)
{
    if ( sd < FLT_EPSILON )
    {
        if ( fabs(x) < FLT_EPSILON && fabs(y) < FLT_EPSILON )
            return delta + gamma;
        else
            return gamma;
    }

    if ( fabs(alpha) > 0.5 )
        return gamma + delta * exp (-pow((x*x+y*y)/(2*sd*sd)/alpha, beta))/(2*M_PI*sd*sd);
    else
        return gamma + delta * exp (-pow((x*x+y*y)/(2*sd*sd), beta))/(2*M_PI*sd*sd);
}

double openip::gauss2_dx(float x, float y, float sd, float alpha, float beta, float delta, float gamma)
{
    if ( sd < FLT_EPSILON )
    {
        if ( fabs(x-y) < FLT_EPSILON )
            return delta + gamma;
        else
            return gamma;
    }

    return -delta * pow(-x * x - y * y, (double) (beta - 1)) * (double) beta * x * exp(pow((-x * x - y * y) * pow(sd, -0.2e1) / alpha / 0.2e1, (double) beta)) * pow(sd, -0.4e1) / alpha / M_PI / 0.2e1;
}

double openip::gauss2_dy(float x, float y, float sd, float alpha, float beta, float delta, float gamma)
{
    if ( sd < FLT_EPSILON )
    {
        if ( fabs(x-y) < FLT_EPSILON )
            return delta + gamma;
        else
            return gamma;
    }

    return -delta * pow(-x * x - y * y, (double) (beta - 1)) * (double) beta * y * exp(pow((-x * x - y * y) * pow(sd, -0.2e1) / alpha / 0.2e1, (double) beta)) * pow(sd, -0.4e1) / alpha / M_PI / 0.2e1;
}

double openip::gauss2_dxy(float x, float y, float sd, float alpha, float beta, float delta, float gamma)
{
    if ( sd < FLT_EPSILON )
    {
        if ( fabs(x-y) < FLT_EPSILON )
            return delta + gamma;
        else
            return gamma;
    }

    if ( fabs(x - y) < FLT_EPSILON )
        return 0;
    return delta * pow(-x * x - y * y, (double) (beta - 2)) * y * (double) beta * x * exp(pow(-(x * x + y * y) * pow(sd, -0.2e1) / alpha / 0.2e1, (double) beta)) * ((double) beta - 0.1e1 + pow(-(x * x + y * y) * pow(sd, -0.2e1) / alpha / 0.2e1, (double) beta) * (double) beta) * pow(sd, -0.4e1) / alpha / 0.3141592654e1;
}

double openip::gauss2_dxx(float x, float y, float sd, float alpha, float beta, float delta, float gamma)
{
    if ( sd < FLT_EPSILON )
    {
        if ( fabs(x-y) < FLT_EPSILON )
            return delta + gamma;
        else
            return gamma;
    }

    if ( fabs(x - y) < FLT_EPSILON )
        return 0;

    return delta * beta * exp(pow(-(x * x + y * y) * pow(sd, -0.2e1) / alpha / 0.2e1, beta)) * (0.2e1 * pow(-x * x - y * y, beta - 0.2e1) * x * x * beta - 0.2e1 * pow(-x * x - y * y, beta - 0.2e1) * x * x - pow(-x * x - y * y, beta - 0.1e1) + 0.2e1 * pow(-x * x - y * y, beta - 0.2e1) * beta * x * x * pow(-(x * x + y * y) * pow(sd, -0.2e1) / alpha / 0.2e1, beta)) * pow(sd, -0.4e1) / alpha / M_PI / 0.2e1;
}

double openip::gauss2_dyy(float x, float y, float sd, float alpha, float beta, float delta, float gamma)
{
    if ( sd < FLT_EPSILON )
    {
        if ( fabs(x-y) < FLT_EPSILON )
            return delta + gamma;
        else
            return gamma;
    }

    if ( fabs(x - y) < FLT_EPSILON )
        return 0;
    
    return delta * beta * exp(pow(-(x * x + y * y) * pow(sd, -0.2e1) / alpha / 0.2e1, beta)) * (0.2e1 * pow(-x * x - y * y, beta - 0.2e1) * x * x * beta - 0.2e1 * pow(-x * x - y * y, beta - 0.2e1) * x * x - pow(-x * x - y * y, beta - 0.1e1) + 0.2e1 * pow(-x * x - y * y, beta - 0.2e1) * beta * y * y * pow(-(x * x + y * y) * pow(sd, -0.2e1) / alpha / 0.2e1, beta)) * pow(sd, -0.4e1) / alpha / M_PI / 0.2e1;
}

double openip::logfilter(float x, float y, float sigma)
{
    float sigma2= sigma*sigma;
    float sigma4= sigma2*sigma2;
    return 1.0/(M_PI*sigma4)*(1 - (x*x + y*y)/(2*sigma2))*exp(-(x*x + y*y)/(2*sigma2));
}

float openip::fakt(int n)
{
    float f= 1;
    for ( int i= 2; i <= n; ++i )
        f*= i;
    return f;
}

float openip::combination(int n, int k)
{
    return fakt(n)/(fakt(k)*fakt(n-k));
}

float openip::log2(float x)
{
    return log(x)/log(2);
}