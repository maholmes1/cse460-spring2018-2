#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stack>

using namespace std;
int main()
{
  int n = 10;
  pid_t childpid;
  stack<pid_t> ancestors;               //stack to hold ancestors' ids

  for ( int i = 0; i < n; ++i ) {
    if ( childpid = fork() )
        break;                          //parent or error, break
    ancestors.push ( getppid() );       //save the parent pid in stack
  }
  wait ( NULL );                        //wait for child process to finish
  cout << "This is process " << getpid() << " with child " << childpid <<
        " and ancestors: ";
  while ( !ancestors.empty() ) {        //get ancestors' id from stack
    cout << ancestors.top() << " ";
    ancestors.pop();                    //remove top item
  }
  cout <<  endl;
  return 0;
}
