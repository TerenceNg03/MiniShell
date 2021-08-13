//
//  main.cpp
//  myshell
//
//  Created by Adam Wu on 2021/8/7.
//

#include "driver.hh"
#include "minishell.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    std::ios::sync_with_stdio(false);
    parse::Driver driver(vector<string>(argv,argv+argc));
    driver.parse();

    return 0;
}
