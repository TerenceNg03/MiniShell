//
//  job.c
//  MiniShell
//
//  Created by Terence Ng on 2021/11/6.
//

#include "job.hpp"

using namespace std;

/* Return true if all processes in the job have stopped or completed.  */
int
job_is_stopped (job *j)
{
    process *p;

    for (p = j->first_process; p; p = p->next)
        if (!p->completed && !p->stopped)
            return 0;
    return 1;
}

/* Return true if all processes in the job have completed.  */
int
job_is_completed (job *j)
{
    process *p;

    for (p = j->first_process; p; p = p->next)
        if (!p->completed)
            return 0;
    return 1;
}
