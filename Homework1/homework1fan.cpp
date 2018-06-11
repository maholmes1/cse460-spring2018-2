//creates a fan of processes
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main()
{
  int n = 10;
  pid_t childpid;

  for ( int i = 0; i < n; ++i ) {
    if ( ( childpid = fork()) <= 0 )
        break;          //break, either child or error
        sleep ( 1 );
  }
  cout << "Here is process " << getpid() << " with parent " << getppid() << endl;

  return 0;
}
