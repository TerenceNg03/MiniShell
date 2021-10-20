//
//  sleep.cpp
//  MiniShell
//
//  Created by Terence Ng on 2021/10/20.
//

#include <unistd.h>
#include <iostream>
#include <regex>
using namespace std;

int main(int argc, const char * argv[]){
    if(argc<1){
        cerr<<"usage : sleep seconds\n";
        return -1;
    }

    regex re("([0-9]|.)*");
    std::cmatch m;
    if(regex_match(argv[1],m,re)){
        double _t = atof(argv[1]);
        unsigned int t = _t*1000000;
        usleep(t);
    }else{
        cerr<<argv[1]<<" : is not a number\n";
    }
    return 0;
}
