/*
 * Copyright Krzysztof Miernik 2012
 * k.a.miernik@gmail.com 
 *
 * Distributed under GNU General Public Licence v3
 */

#include <vector>
#include "Options.h"

Options::Options() {
    hisId_ = 0;
    isIdSet_ = false;
    isListMode_ = false;
    isListModeZ_ = false;
    isInfoMode_ = false;
    isZeroSup_ = false;
    isGx_ = false;
    isGy_ = false;
    isBg_ = false;
    isSBg_ = false;
    isPg_ = false;
    isBin_ = false;
    isEvery_ = false;
    polygonFile_ = "";
    bin_.push_back(1);
    bin_.push_back(1);
    every_.push_back(1);
    every_.push_back(1);
}

unsigned Options::getHisId () const { return hisId_; }
bool Options::setHisId (unsigned hisId) {
    if (hisId > 0 && hisId < 10000) {
        hisId_ = hisId;
        isIdSet_ = true;
        return true;
    } else {
        isIdSet_ = false;
        return false;
    }
}
bool Options::isIdSet() const { return isIdSet_; }

bool Options::getListMode() const { return isListMode_; }
void Options::setListMode (bool b /*=true*/) { 
    if (b) {
        isListModeZ_ = false;
        isListMode_ = true;
    } else {
        isListMode_ = false;
    }
    
}

bool Options::getListModeZ() const { return isListModeZ_; }
void Options::setListModeZ (bool b /*=true*/) { 
    if (b) {
        isListMode_ = false;
        isListModeZ_ = true;
    } else {
        isListModeZ_ = false;
    }
}

bool Options::getInfoMode() const { return isInfoMode_; }
void Options::setInfoMode (bool b /*=true*/) { isInfoMode_ = b; }


bool Options::getZeroSup() const { return isZeroSup_; }
void Options::setZeroSup (bool b /*=true*/) { isZeroSup_ = b; }

bool Options::getGx() const { return isGx_; }
bool Options::setGx (unsigned g0, unsigned g1, bool isGx /*=true*/) {
    if (g0 > g1)
        return false;
    gx_.clear();
    isGx_ = isGx;
    if (isGx) {
        gx_.resize(2, 0);
        gx_[0] = g0;
        gx_[1] = g1;
    }
    return true;
}

bool Options::getGy() const { return isGy_; }
bool Options::setGy (unsigned g0, unsigned g1, bool isGy /*=true*/) {
    if (g0 > g1)
        return false;
    gy_.clear();
    isGy_ = isGy;
    if (isGy) {
        gy_.resize(2, 0);
        gy_[0] = g0;
        gy_[1] = g1;
    }
    return true;
}

bool Options::getBg() const { return isBg_; }
bool Options::setBg (unsigned b0, unsigned b1, bool isBg /*=true*/) {
    if (b0 > b1)
        return false;

    b_.clear();
    isBg_ = isBg;
    if (isBg) {
        isSBg_ = false;
        b_.resize(2, 0);
        b_[0] = b0;
        b_[1] = b1;
    }
    return true;
}

bool Options::getSBg() const { return isSBg_; }
bool Options::setSBg (unsigned b0, unsigned b1,
                      unsigned b2, unsigned b3,
                      bool isSBg /*=true*/) {
    if ( !(b3 >= b2 && b2 >= b1 && b1 >= b0) )
        return false;

    b_.clear();
    isSBg_ = isSBg;
    if (isSBg) {
        isBg_ = false;
        b_.resize(4, 0);
        b_[0] = b0;
        b_[1] = b1;
        b_[2] = b2;
        b_[3] = b3;
    }
    return true;
}
bool Options::getBin() const { return isBin_; }
bool Options::setBin (unsigned bx, unsigned by /*= 0*/, bool isBin /*=true*/) {
    if (by == 0)
        by = bx;

    if (bx < 1 || by < 1)
        return false;

    isBin_ = isBin;
    if (bx == 1 && by == 1)
       isBin_ = false; 

    if (isBin_) {
        bin_[0] = bx;
        bin_[1] = by;
    }
    return true;
}

bool Options::getEvery() const { return isEvery_; }
bool Options::setEvery (unsigned nx, unsigned ny /*= 0*/, bool isEvery /*=true*/) {
    if (ny == 0)
        ny = nx;

    if (nx < 1 || ny < 1)
        return false;

    isEvery_ = isEvery;
    if (nx == 1 && ny == 1)
       isEvery_ = false; 

    if (isEvery_) {
        every_[0] = nx;
        every_[1] = ny;
    }
    return true;
}
void Options::getGateX(std::vector<unsigned>& rtn) const {
    rtn.clear();
    rtn.reserve(gx_.size());
    unsigned sz = gx_.size();
    for (unsigned i = 0; i < sz; ++i)
        rtn.push_back(gx_[i]);
}

void Options::getGateY(std::vector<unsigned>& rtn) const {
    rtn.clear();
    rtn.reserve(gy_.size());
    unsigned sz = gy_.size();
    for (unsigned i = 0; i < sz; ++i)
        rtn.push_back(gy_[i]);
}


void Options::getBgGate(std::vector<unsigned>& rtn) const {
    rtn.clear();
    rtn.reserve(b_.size());
    unsigned sz = b_.size();
    for (unsigned i = 0; i < sz; ++i)
        rtn.push_back(b_[i]);
}

void Options::getBinning(std::vector<unsigned>& rtn) const {
    rtn.clear();
    rtn.reserve(bin_.size());
    unsigned sz = bin_.size();
    for (unsigned i = 0; i < sz; ++i)
        rtn.push_back(bin_[i]);
}

void Options::getEveryN(std::vector<unsigned>& rtn) const {
    rtn.clear();
    rtn.reserve(every_.size());
    unsigned sz = every_.size();
    for (unsigned i = 0; i < sz; ++i)
        rtn.push_back(every_[i]);
}

void Options::setPolygon (std::string polygonFile, bool isGx, bool isPg /*=true*/) {
    isPg_ = isPg;
    if (isPg) {
        polygonFile_ = polygonFile;
        if (isGx) {
            isGx_ = true;
            isGy_ = false;
        } else {
            isGx_ = false;
            isGy_ = true;
        }
    }
}

bool Options::getPg() const { return isPg_; }
        
std::string Options::getPolygon() const {
    return polygonFile_;
}
