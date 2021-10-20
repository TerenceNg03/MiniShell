//
//  ls.cpp
//  MiniShell
//
//  Created by Terence Ng on 2021/10/20.
//

#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, const char * argv[]){
    fs::directory_iterator dit(fs::current_path());
    for (auto f:dit){
        cout<<f.path().filename().string()<<'\t';
    }
    cout<<"\n";
    return 0;
}

