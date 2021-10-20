//
//  clear.cpp
//  MiniShell
//
//  Created by Terence Ng on 2021/10/20.
//

#include <iostream>
#include <curses.h>
#include <term.h>
#include <unistd.h>

using namespace std;

void ClearScreen()
{
    if (!cur_term)
    {
        int result;
        setupterm( NULL, STDOUT_FILENO, &result );
        if (result <= 0) return;
    }

    char s[] = "clear";

    putp( tigetstr( s ) );
}

int main(){
    ClearScreen();
}
