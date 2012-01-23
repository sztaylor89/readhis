#include <cstdlib>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "HisDrr.h"
#include "HisDrrHisto.h"
#include "Exceptions.h"

using namespace std;
    
static struct option long_options[] = {
    {"id",    required_argument, 0, 'i'},
    {"gx",    required_argument, 0, 'x'},
    {"gy",    required_argument, 0, 'y'},
    {"bg",    required_argument, 0, 'b'},
    {"sbg",   required_argument, 0, 's'},
    {"zero",  no_argument, 0,       'z'},
    {"info",  no_argument, 0,       'I'},
    {"list",  no_argument, 0,       'l'},
    {"listZ", no_argument, 0,       'L'},
    {"help",  no_argument, 0,       'h'},
    {0, 0, 0, 0}
};

class Options {
    public:
        unsigned getHisId () { return hisId_; }
        bool setHisId (unsigned const hisId) {
            if (hisId > 0 && hisId < 10000) {
                hisId_ = hisId;
                isIdSet_ = true;
                return true;
            } else {
                isIdSet_ = false;
                return false;
            }
        }
        bool getisIdSet() { return isIdSet_; }

        bool getListMode() { return isListMode_; }
        void setListMode (const bool b) { 
            if (b) {
                isListModeZ_ = false;
                isListMode_ = true;
            } else {
                isListMode_ = false;
            }
            
        }

        bool getInfoMode() { return isInfoMode_; }
        void setInfoMode (const bool b) { isInfoMode_ = b; }

        bool getListModeZ() { return isListModeZ_; }
        void setListModeZ (const bool b) { 
            if (b) {
                isListMode_ = false;
                isListModeZ_ = true;
            } else {
                isListModeZ_ = false;
            }
        }

        bool getZeroSup() { return isZeroSup_; }
        void setZeroSup (const bool b) { isZeroSup_ = b; }
        
        bool getGx() { return isGx_; }
        bool setGx (const bool isGx, const unsigned g0, const unsigned g1) {
            if (g0 > g1)
                return false;
            isGx_ = isGx;
            if (isGx) {
                isGy_ = false;
                g0_ = g0;
                g1_ = g1;
            }
            return true;
        }

        bool getGy() { return isGy_; }
        bool setGy (const bool isGy, const unsigned g0, const unsigned g1) {
            if (g0 > g1)
                return false;
            isGy_ = isGy;
            if (isGy) {
                isGx_ = false;
                g0_ = g0;
                g1_ = g1;
            }
            g0_ = g0;
            g1_ = g1;
            return true;
        }

        bool getBg() { return isBg_; }
        bool setBg (const bool isBg, const unsigned b0, const unsigned b1) {
            if (b0 > b1)
                return false;
            isBg_ = isBg;
            if (isBg) {
                isSBg_ = false;
                b0_ = b0;
                b1_ = b1;
            }
            return true;
        }
        
        bool getSBg() { return isSBg_; }
        bool setSBg (const bool isSBg, const unsigned b0, const unsigned b1,
                                       const unsigned b2, const unsigned b3) {
            if ( !(b3 > b2 && b2 > b1 && b1 > b0) )
                return false;
            isSBg_ = isSBg;
            if (isSBg) {
                isBg_ = false;
                b0_ = b0;
                b1_ = b1;
                b2_ = b2;
                b3_ = b3;
            }
            return true;
        }

        Options() {
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
            g0_ = 0;
            g1_ = 0;
            b0_ = 0;
            b1_ = 0;
            b2_ = 0;
            b3_ = 0;
        }
    private:
        unsigned hisId_;
        bool isIdSet_;
        bool isListMode_;
        bool isListModeZ_;
        bool isInfoMode_;
        bool isZeroSup_;
        bool isGx_;
        bool isGy_;
        bool isBg_;
        bool isSBg_;
        unsigned g0_;
        unsigned g1_;
        unsigned b0_;
        unsigned b1_;
        unsigned b2_;
        unsigned b3_;
};

enum Status {
    ok,
    warning,
    error
};

Status parseMultiArgs (char* arguments, int* argsArray, unsigned requiredArgs = 0) {
    char* tokens;
    tokens = strtok(arguments, ",");

    unsigned numberOfTokens = 0;
    while (tokens != 0) {
        if (numberOfTokens < requiredArgs)
            argsArray[numberOfTokens] = atoi(tokens);
        else
            return warning;
        tokens = strtok(0, ",");
        numberOfTokens++;
    }    
    if (numberOfTokens == requiredArgs)
        return ok;
    else
        return error;
}

void help() {
    cout << "USAGE:" << endl;
    cout << "  readhis [options] file.his " << endl;
    cout << endl;
    cout << "DESCRIPTION:" << endl;
    cout << "  Program reads his file using binary description from drr file." << endl;
    cout << "  Both files are required to be present in the specified path " << endl;
    cout << "  and with the same base name (e.g. t1.his and t1.drr)." << endl;
    cout << "  Result is send to standard output, use redirection " << endl;
    cout << "  in case you want to save it to file." << endl;
    cout << endl;
    cout << "OPTIONS:" << endl;
    cout << "  --id : short (-i), selects histogram id " << endl;
    cout << "         required by other options unless stated otherwise. " << endl;
    cout << endl;
    cout << "  --gx x0,x1 : short (-x), 2D histograms only, requires two " << endl;
    cout << "               integer arguments separated by coma. Sets " << endl;
    cout << "               projection on X axis, starting from column x0 to x1" << endl;
    cout << "               (including both)" << endl;
    cout << endl;
    cout << "  --gy x0,x1 : short (-y), as above, projection on Y axis " << endl;
    cout << endl;
    cout << "  --bg x0,x1 : short (-b), 2D histograms only, only with gx or " << endl;
    cout << "               gy option. Sets gate for background subtraction, " << endl;
    cout << "               from x0 to x1 (including both). " << endl;
    cout << endl;
    cout << "  --sbg x0,x1,x2,x3 : short (-s), 2D histograms only, only with " << endl;
    cout << "                      gx or gy option. Same as above exept that  " << endl;
    cout << "                      gate is split into two parts x0 to x1 and "<< endl;
    cout << "                      x2 to x3." << endl;
    cout << endl;
    cout << "  --zero : short (-z), suppresses bins with zero counts in output" << endl;
    cout << endl;
    cout << "  --info : short (-I), displays detailed information on histogram" << endl;
    cout << endl;
    cout << "  --list : short (-l), does not requires id. Displays list of" << endl;
    cout << "           histograms present in the file. " << endl;
    cout << endl;
    cout << "  --listZ : short (-L), does not requires id. As above," << endl; 
    cout << "            additionally marks empty histograms. " << endl;
    cout << endl;

    cout << "  --help  : displays this help " << endl;
    cout << endl;
}
     
int main (int argc, char* argv[]) {

    int flag = 0;

    Options* options = new Options();

    while (true) {
        /* getopt_long stores the option index here. */
        int option_index = 0;

        flag = getopt_long (argc, argv, "i:x:y:b:s:zIlLh",
                        long_options, &option_index);

        /* Detect the end of the options. */
        if (flag == -1)
            break;

        switch (flag) {

            case 'i': {
                int hisId = atoi(optarg);
                if ( !options->setHisId(hisId) ) {
                    cout << "Wrong or missing histogram id " << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Histogram: " << options->getHisId() << endl;
                break;
            }

            case 'x': {
                int a[2] = {0};
                Status status = parseMultiArgs(optarg, a, 2);
                if (status == warning) {
                    cout << "Warning: option --gx with more then two arguments " << endl;
                }
                if (status == error) {
                    cout << "Error: option --gx requires two "  
                         << "arguments separated by coma e.g 10,20 " << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                if ( !(options->setGx(true, a[0], a[1])) ) {
                    cout << "Error: wrong arguments for --gx option" << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Gate on X from " << a[0] << " to " << a[1] << endl;
                break;
            }

            case 'y': {
                int a[2] = {0};
                Status status = parseMultiArgs(optarg, a, 2);
                if (status == warning) {
                    cout << "Warning: option --gy with more then two arguments" << endl;
                }
                if (status == error) {
                    cout << "Error: option --gy requires two "  
                         << "arguments separated by coma e.g 10,20 " << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                if ( !(options->setGy(true, a[0], a[1])) ) {
                    cout << "Error: wrong arguments for --gy option" << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Gate on Y from " << a[0] << " to " << a[1] << endl;
                break;
            }

            case 'b': {
                int a[2] = {0};
                Status status = parseMultiArgs(optarg, a, 2);
                if (status == warning) {
                    cout << "Warning: option --bg with more then two arguments" << endl;
                }
                if (status == error) {
                    cout << "Error: option --bg requires two "  
                         << "arguments separated by coma e.g 10,20 " << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                if ( !(options->setBg(true, a[0], a[1])) ) {
                    cout << "Error: wrong arguments for --bg option" << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Background taken from " << a[0] << " to " << a[1] << endl;
                break;
            }

            case 's': {
                int a[4] = {0};
                Status status = parseMultiArgs(optarg, a, 4);
                if (status == warning) {
                    cout << "Warning: option --sbg with more then four arguments" << endl;
                }
                if (status == error) {
                    cout << "Error: option --sbg requires four "  
                         << "arguments separated by coma e.g 10,20,30,40 " << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                if ( !(options->setSBg(true, a[0], a[1], a[2], a[3])) ) {
                    cout << "Error: wrong arguments for --sbg option" << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Background taken from " << a[0] << " to " << a[1] 
                        << " and " << a[2] << " to " << a[3] << endl;
                break;
            }

            case 'z': {
                options->setZeroSup(true);
                break;
            }

            case 'I': {
                options->setInfoMode(true);
                break;
            }

            case 'l': {
                options->setListMode(true);
                break;
            }

            case 'L':
                options->setListModeZ(true);
                break;

            case 'h':
                help();
                exit(0);
                break;
                
            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                help();
                exit(1);
            }
        }

    /* Print any remaining command line arguments (not options). */
    string fileName;
    if (optind < argc) {
        fileName = argv[optind];
    }
    cout << fileName << endl;
    
    unsigned int dot = fileName.find_last_of(".");
    string baseName = fileName.substr(0,dot);
    string drr = baseName + ".drr";
    string his = baseName + ".his";

    try {
        HisDrr *h = new HisDrr(drr, his);

        if (options->getListMode()) {
            vector<int> list;
            h->getHisList(list);
            unsigned sz = list.size();
            for (unsigned int i = 0; i < sz; i++) {
                cout << list[i] << ", ";
                if ((i+1) % 10 == 0)
                    cout << endl;

            }
            cout << endl;
            exit(0);
        }
        if (options->getListModeZ()) {
            vector<int> list;
            vector<unsigned int> d;
            h->getHisList(list);
            for (unsigned int i = 0; i < list.size(); i++) {
                h->getHistogram(d, list[i]);
                unsigned int sz = d.size();
                bool empty = true;
                for (unsigned int j = 0; j < sz; j++)
                    if (d[j] > 0) {
                        empty = false;
                        break;
                    }
                if (empty)
                    cout << "E" << list[i];
                else
                    cout << "\033[1;34m" << list[i] << "\033[0m";
                cout << ", ";
                if ((i+1) % 10 == 0)
                    cout << endl;

            }
            cout << "\033[0;30m\033[0m" << endl;
            exit(0);
        }

        int hisId = options->getHisId();
        DrrHisRecordExtended info = h->getHistogramInfo(hisId);
        if (options->getInfoMode()) {
            cout << "# INFORMATIONS: " << info.hisID << endl;
            cout << "#ID: " << info.hisID << endl;
            cout << "#hisDim: " << info.hisDim << endl;
            cout << "#halfWords: " << info.halfWords << endl;
            for (int j = 0; j < 4; j++)
                cout << "#params[" << j << "]: " << info.params[j] << endl;
            for (int j = 0; j < 4; j++)
                cout << "#raw[" << j << "]: " << info.raw[j] << endl;
            for (int j = 0; j < 4; j++)
                cout << "#scaled[" << j << "]: " << info.scaled[j] << endl;
            for (int j = 0; j < 4; j++)
                cout << "#minc[" << j << "]: " << info.minc[j] << endl;
            for (int j = 0; j < 4; j++)
                cout << "#maxc[" << j << "]: " << info.maxc[j] << endl;

            cout << "#offset: " << info.offset << endl;
            cout << "#xlabel: " << info.xlabel << endl;
            cout << "#ylabel: " << info.ylabel << endl;

            for (int j = 0; j < 4; j++)
                cout << "#calcon[" << j << "]: " << info.calcon[j] << endl;

            cout << "#title: " << info.title << endl;
            exit(0);
        }

        if (info.hisDim == 1) {
            HisDrrHisto1D* h1 = new HisDrrHisto1D(info);
            //Process here...
            delete h1;
        } else if (info.hisID == 2) {
            HisDrrHisto2D* h2 = new HisDrrHisto2D(info);
            //Process here...
            delete h2;
        } else {
            throw GenError("Error: Only 1 and 2 dimensional histograms are supported.");
        }

        delete h;
    } catch (GenError &err) {
        cout << "Error: " << err.show() << endl;
        cout << "Run readhis --help for more information" << endl;
    }
   
    delete options;
    exit(0);
}
