/** @file  */ 
#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "util.H"
#include "gsl/gsl_histogram.h"
#include "gsl/gsl_errno.h"
/// a wrapper for the gsl histogram structure

class histogram {
 public:
/// constructor when only number of bins is known
    histogram(int nbins) {
	x=gsl_histogram_alloc(nbins);
    }
/// constructor when range is passed
    histogram(double mn, double mx, int nbins,bool logarithmic=false) {
	x=gsl_histogram_alloc(nbins);
	allocate_ranges(mn,mx,nbins,logarithmic);
    };
/// copy constructor
    histogram(const histogram &a) {
	x=gsl_histogram_clone(a.x);
    }
/// equals operator
histogram &operator=(const histogram &a) 
{
    if (this==&a) return *this;
    x=gsl_histogram_clone(a.x);
    return *this;
}
/// allocate the range - needed if we have a vector of histograms
    void allocate_ranges(double mn, double mx,int nbins, bool logarithmic=false) {
	if (logarithmic) {
	    assert(mn>0.0);
	    double *r=(double *)malloc((nbins+1)*sizeof(double));
	    if (!r) error("error allocating memory for r in allocate_ranges");
	    double gap=(log(mx)-log(mn))/(double(nbins));
	    for (int i=0;i<=nbins;i++) r[i]=exp(log(mn)+double(i)*gap);
	    gsl_histogram_set_ranges(x,r,nbins+1);
	    free(r);
	} else {
	    gsl_histogram_set_ranges_uniform(x,mn,mx);
	}
    }
/// destructor
    ~histogram(){
	gsl_histogram_free(x);
    };
///  add a value to the histogram
    void add(double val){
	if (gsl_histogram_increment(x,val)==GSL_EDOM)
	    std::cerr << "value "<< val << " out of histogram range [" 
		      << gsl_histogram_min(x) << ","
		      << gsl_histogram_max(x) << ")" << std::endl;
    };
/// and add an entire histogram
    void add(const histogram &a){
	gsl_histogram_add(x,a.x);
    };
    void operator+=(const histogram &a) {
	gsl_histogram_add(x,a.x);
    };
/// the frequency in bin i
    double freq(int i) {
	return gsl_histogram_get(x,i);
    }
/// Get the number of bins
    int nbins() {
	return gsl_histogram_bins(x);
};
/// get the range for bin i
    std::pair<double,double> range(int bn) {
	std::pair<double,double> y;
	gsl_histogram_get_range(x,bn,&y.first,&y.second);
	return y;
    }
/// Get the mean
    double mean() {
	return gsl_histogram_mean(x);
    };
///
    void printf() {
	gsl_histogram_fprintf(stdout,x,"%g","%g");
    }
    private:
	gsl_histogram *x;
};



#endif
