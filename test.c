/*
 * ±‡“Î£∫gcc test.c -otest -L. -lpthread -lrwini
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include "ini_option.h"

#define MAX_TEST_THREAD 10   /* must be less than 10 */

static clock_t g_t = 0;
static int g_counter = 0;
static pthread_t p[MAX_TEST_THREAD] = { 0 };
static sem_t g_sem[MAX_TEST_THREAD];

static void *threadReadFunc( void *arg )
{
    int len = strlen( arg );
    int i = ((char *)arg)[len - 1] - '0';
    char name[8] = { 0 };

    strcpy( name, arg );
    sem_wait( &g_sem[i] );

    char key[32] = { 0 };
    char file[32] = { "test.ini" };
    const char *value = NULL;

    for ( i = 0; i < 1000; i ++ )
    {
        snprintf( key, 32, "test_%d", i );
        value = iniGetValue( file, NULL, key );
        printf( "%d. [%s] [Get] General/%s: value = [%s]\n", g_counter++, name, key, value );

        value = iniGetValue( file, "TEST", key );
        printf( "%d. [%s] [Get] TEST/%s: value = [%s]\n", g_counter++, name, key, value );
    }

    return NULL;
}


static void *threadWriteFunc( void *arg )
{

    int len = strlen( arg );
    int i = ((char *)arg)[len - 1] - '0';
    char name[8] = { 0 };

    strcpy( name, arg );
    sem_wait( &g_sem[i] );

    int ret = 0;
    char key[32] = { 0 };
    char value[32] = { 0 };
    char file[32] = { "test.ini" };

    for ( i = 0; i < 1000; i ++ )
    {
        snprintf( key, 32, "test_%d", i );
        snprintf( value, 32, "%d %d", i, time(NULL) + i );

        ret = iniSetValue( file, NULL, key, value ); 
        printf( "%d. [%s] [Set] General/%s: ret = [%s]\n", g_counter++, name, key, (0 == ret) ? "TRUE" : "FALSE" );

        ret = iniSetValue( file, "TEST", key, value );
        printf( "%d. [%s] [Set] TEST/%s: ret = [%s]\n", g_counter++, name, key, (0 == ret) ? "TRUE" : "FALSE" );
    }

    return NULL;
}


void mutiThreadTest()
{
    int i = 0, ret = 0;
    char name[8] = { "thread" };

    for ( i = 0; i < MAX_TEST_THREAD; i++ )
    {
        sem_init( &g_sem[i], 0, 0 );
    }

    for ( i = 0; i < MAX_TEST_THREAD; i++ )
    {
        name[6] = '0' + i;

        if ( 0 == (i % 2) )
        {
            ret = pthread_create( &p[i], NULL, threadReadFunc, name ); 
        }
        else
        {
            ret = pthread_create( &p[i], NULL, threadWriteFunc, name ); 
        }

        usleep( 10000 );

        if ( 0 == ret )
        {
            printf( "pthread_create(%d) success!\n", i );
        }
        else
        {
            printf( "pthread_create(%d) faild!\n", i );
        }

    }

    // do something

    for ( i = 0; i < MAX_TEST_THREAD; i++ )
    {
        sem_post( &g_sem[i] );
    }

    g_t = clock();

    for ( i = 0; i < MAX_TEST_THREAD; i++ )
    {
        pthread_join( p[i], NULL );
    }

    return;
}


int main( int argc, char *argv[] )
{

    if ( 4 == argc )
    {
        printf( "** input param:  '%s'  '%s'  '%s'\n", argv[1], argv[2], argv[3] );
        const char *value = iniGetValue( argv[1], argv[2], argv[3] );
        printf( "** output: value = [%s]\n", value );

        return 0;
    }

    // read write test...
    const char *value = iniGetValue( "test.ini", NULL, "test1" );
    printf( "[Get] General/test1: value = [%s]\n", value );

    value = iniGetValue( "test.ini", "TEST", "test2" );
    printf( "[Get] TEST/test2: value = [%s]\n", value );

    time_t tm = time( NULL );
    value = iniGetValue( "test.ini", NULL, "test3" );
    printf( "[Get] General/test3: value.before = [%s]\n", value );
    int flag = iniSetValue( "test.ini", NULL, "test3", ctime(&tm)  );
    printf( "[Set] General/test3: set flag= [%s]\n", (0 == flag) ? "TRUE" : "FALSE" );
    value = iniGetValue( "test.ini", NULL, "test3" );
    printf( "[Get] General/test3: value.after = [%s]\n", value );

    tm = time( NULL );
    value = iniGetValue( "test.ini", "TEST", "test4" );
    printf( "[Get] TEST/test4: value.before = [%s]\n", value );
    flag = iniSetValue( "test.ini", "TEST", "test4", ctime(&tm) );
    printf( "[Set] TEST/test4: set flag= [%s]\n", (0 == flag) ? "TRUE" : "FALSE" );
    value = iniGetValue( "test.ini", "TEST", "test4" );
    printf( "[Get] TEST/test4: value.after = [%s]\n", value );
 
    iniFlushData( "test.ini" );

    // muti-thread test...
    mutiThreadTest();
    g_t = clock() - g_t;
    printf( " ====================================================== [%f seconds]\n", ((float)g_t / CLOCKS_PER_SEC) );

    return 0;
}

