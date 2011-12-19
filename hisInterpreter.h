#ifndef HHISINTERPRETER
#define HHISINTERPRETER

#include <vector>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "hisdrr.h"
#include "drrblock.h"

using namespace std;

///Main class processing histograms
/** 
 *
 */
class HisInterpreter {
    void gx(vector<unsigned int> &d, int g0, int g1);  /// Gate on X channels
    void gy(vector<unsigned int> &d, int g0, int g1);  /// Gate on Y channels
    void gxbg(vector<unsigned int> &d, int b0, int b1);/// Bg subtraction
    void gybg(vector<unsigned int> &d, int b0, int b1);/// Bg subtraction
    void bin1D(vector<unsigned int> &d);/// 1D bins
    void bin2D(vector<unsigned int> &d);/// 2D bins

    HisDrr &hisFile; 
    Flags &options;

    public:
    
    HisInterpreter();
};

#endif