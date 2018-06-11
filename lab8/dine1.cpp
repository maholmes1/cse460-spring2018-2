//dine1.cpp

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>

using namespace std;

SDL_mutex *mutex;  // mutex to lock eating
bool quit = false;
int nEating = 0;   // number of philosopers eating

void think( int i )
{
//  printf("\n%d thinking", i );
  SDL_Delay ( rand() % 3000);
}
void eat( int i )
{
//  printf("\n%d eating!", i );
  SDL_Delay ( rand() % 3000);
}

void take_chops( int i )
{
  //printf("\nTaking chopstick %d", i );
}

void put_chops( int i )
{
 // printf("\nReleasing chopstick %d", i );
}

int philosopher( void *data )
{
  int i;
  i = atoi ( (char *) data );
  while ( !quit ) {
    think( i );
    SDL_LockMutex ( mutex );
    take_chops ( i );
    take_chops ( (i+1) % 5 );
    nEating++;
    eat ( i );
    nEating--;
    put_chops ( i );
    put_chops ( (i+1) % 5 );
    SDL_UnlockMutex ( mutex );
  }
}

void checkCount ( int sig )
{
   if ( sig == SIGINT )
     printf("\n%d philospers eating\n", nEating );
   else if ( sig == SIGQUIT ) {
     quit = true;
     printf("\nQuitting, please wait...\n");
   }
}

int main ()
{
  struct sigaction act, actq;

  act.sa_handler = checkCount;
  sigemptyset ( &act.sa_mask );
  sigaction ( SIGINT, &act, 0 );
  actq.sa_handler = checkCount;
  sigaction ( SIGQUIT, &actq, 0 );

  SDL_Thread *p[5];               //thread identifiers
  const char *names[] = { "0", "1", "2", "3", "4" };

  mutex = SDL_CreateMutex();
  for ( int i = 0; i < 5; i++ )
    p[i] = SDL_CreateThread ( philosopher, (char *) names[i] );

  for ( int i = 0; i < 5; i++ )
    SDL_WaitThread ( p[i], NULL );
  SDL_DestroyMutex ( mutex );
  return 0;
}
