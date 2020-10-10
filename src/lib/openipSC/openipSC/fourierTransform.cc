#include <openipSC/fourierTransform.h>

void openip::fourierTransform2Dc(std::complex<double>* image, int rows, int columns, std::complex<double>* ft, gsl_fft_complex_wavetable* wtr, gsl_fft_complex_workspace* wsr, gsl_fft_complex_wavetable* wtc, gsl_fft_complex_workspace* wsc)
{
    double* tmp= (double*)(&(ft[0]));
    double* timage= (double*)(&(image[0]));

    int n= rows*columns;

    for ( int j= 0; j < n; ++j )
    {
        tmp[j*2]= timage[j*2];
        tmp[j*2+1]= timage[j*2+1];
    }

    bool clearWTR= false, clearWSR= false, clearWTC= false, clearWSC= false;

    if ( wtr == NULL )
    {
        wtr= gsl_fft_complex_wavetable_alloc(columns);
        clearWTR= true;
    }
    if ( wsr == NULL )
    {
        wsr= gsl_fft_complex_workspace_alloc(columns);
        clearWSR= true;
    }
    if ( wtc == NULL )
    {
        wtc= gsl_fft_complex_wavetable_alloc(rows);
        clearWTC= true;
    }
    if ( wsc == NULL )
    {
        wsc= gsl_fft_complex_workspace_alloc(rows);
        clearWSC= true;
    }

    int num= 4;
    omp_set_num_threads(num);

    gsl_fft_complex_workspace* ws_rows[num];
    for ( int i= 0; i < num; ++i )
        ws_rows[i]= gsl_fft_complex_workspace_alloc(rows);
    gsl_fft_complex_workspace* ws_columns[num];
    for ( int i= 0; i < num; ++i )
        ws_columns[i]= gsl_fft_complex_workspace_alloc(columns);


    #pragma omp parallel for
    for ( int i= 0; i < columns; ++i )
        //gsl_fft_complex_forward(tmp + i*2, columns, rows, wtc, wsc);
        gsl_fft_complex_forward(tmp + i*2, columns, rows, wtc, ws_rows[omp_get_thread_num()]);

    #pragma omp parallel for
    for ( int i= 0; i < rows; ++i )
        //gsl_fft_complex_forward(tmp + i*2*columns, 1, columns, wtr, wsr);
        gsl_fft_complex_forward(tmp + i*2*columns, 1, columns, wtr, ws_columns[omp_get_thread_num()]);

    for ( int i= 0; i < num; ++i )
    {
        gsl_fft_complex_workspace_free(ws_rows[i]);
        gsl_fft_complex_workspace_free(ws_columns[i]);
    }

    if ( clearWTR == true )
        gsl_fft_complex_wavetable_free(wtr);
    if ( clearWSR == true )
        gsl_fft_complex_workspace_free(wsr);
    if ( clearWTC == true )
        gsl_fft_complex_wavetable_free(wtc);
    if ( clearWSC == true )
        gsl_fft_complex_workspace_free(wsc);
}
