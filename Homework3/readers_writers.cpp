#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>

using namespace std;

SDL_mutex *mutex;
SDL_cond *readerQueue;
SDL_cond *writerQueue;

int readers = 0;
int writers = 0;
int counter = 0;
FILE *fp = NULL;
bool quit = false;

int reader (void *data)
{
        while ( !quit) {
                SDL_Delay ( rand() % 3000);
                SDL_LockMutex ( mutex );
                while ( !(writers == 0) )
                        SDL_CondWait ( readerQueue, mutex );

                readers++;

                SDL_UnlockMutex ( mutex );
                //read
                int count;
                rewind ( fp );
                fscanf ( fp, "%d", &count );
                printf ("\nThis is %s thread: %d\n", (char *) data, count );
                SDL_LockMutex ( mutex );
                if ( --readers == 0 )
                        SDL_CondSignal (writerQueue );
                SDL_UnlockMutex ( mutex );
        }
}

int writer ( void *data )
{
        while ( !quit ) {
                SDL_Delay ( rand() % 3000);
                SDL_LockMutex (mutex);
                while ( !( (readers == 0) && (writers == 0) ) )
                        SDL_CondWait ( writerQueue, mutex );

                writers++;
//write

                counter++;
                rewind ( fp );
                fprintf (fp, "%d", counter );
                printf("\nThis is %s thread: %d\n", (char *) data, counter );

        writers--; //one writer at a time

SDL_CondSignal (writerQueue);
SDL_CondBroadcast (readerQueue);
SDL_UnlockMutex (mutex);

}
}
void func (int sig)
{
        quit = true;
}

int main ()
{
        SDL_Thread *idr[20], *idw[3];
        char *rnames[] = { (char *)"reader 1",(char *)"reader 2",(char *)"reader 3", (char *)"reader 4",(char *)"reader 5",(char *)"reader 6",(char *)"reader 7",(char *)"reader 8",(char *)"reader 9",(char *)"reader 10",(char *)"reader 11",(char *)"reader 12",(char *)"reader 13",(char *)"reader 14",(char *)"reader 15",(char *)"reader 16",(char *)"reader 17",(char *)"reader 18",(char *)"reader 19",(char *)"reader 20"};
        //names of threads
char *wnames[] = { (char *)"writer 1", (char *)"writer 2", (char *)"writer 3" };
//names of threads

if ( ( fp = fopen ( "counter.txt", "w+" ) ) == NULL ) {
        printf("\nError opening file!\n");
        return 1;
};
const int N = 20;
mutex = SDL_CreateMutex();
readerQueue = SDL_CreateCond();
writerQueue = SDL_CreateCond();
for (int i = 0; i < N; i++ ){
        idr[i] = SDL_CreateThread (reader, rnames[i] );
        if (i >= 3) continue;
        idw[i] = SDL_CreateThread (writer, wnames[i] );
}
printf("\nwaiting..\n");
(void) signal (SIGINT, func); //catch terminal interrupts
//wait for the threads to exit
for ( int i = 0; i < N; i++ ) {
        SDL_WaitThread ( idr[i], NULL);
        if (i >= 3 ) continue;
SDL_WaitThread ( idw[i], NULL );
}
SDL_DestroyCond ( readerQueue );
SDL_DestroyCond ( writerQueue );
SDL_DestroyMutex ( mutex );
fprintf (fp, "\nDone!\n");
fclose ( fp );
return 0;
}
