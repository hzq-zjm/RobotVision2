
#include "ZCVCORE_Time.h"


#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <list>
#include <queue>
#include <exception>
using namespace std;


class UnitTimeStat
{
public:
    UnitTimeStat()
    {
    }

    ~UnitTimeStat()
    {
    }

    void AddItem( float val )
    {
        items.push_back( val );
    }

    float GetMaximum()
    {
        return vmax;
    }

    float GetMinimum()
    {
        return vmin;
    }

    float GetMedian()
    {
        return vmedian;
    }

    float GetMean()
    {
        return vmean;
    }

    float GetVariance()
    {
        return vvar;
    }

    void Calculate();
    int PrintStatis( char *buf );

private:
    list<float> items;
    float vmax, vmin, vmedian;
    float vmean, vvar;
    float vtotal;
    int vcount;
};

void UnitTimeStat::Calculate()
{
    if( items.size()==0 )
    {
        vmax = vmin = vmedian = 0;
        vmean = vvar = 0;
        vtotal = 0;
        vcount = 0;
        return;
    }

    items.sort();
    vmin = items.front();
    vmax = items.back();

    float sum=0, sum2 = 0;
    int i=0, cnt=items.size();
    int mi = cnt/2;

    list<float>::iterator miter = items.begin();
    while( miter != items.end() )
    {
        float val = *miter;
        sum += val;
        sum2 += val*val;
        if( i==mi )
            vmedian = val;

        i++;
        miter++;
    }
    vmean = sum / cnt;
    vvar = sum2/cnt - vmean*vmean;
    vvar = (float)sqrt( vvar );
    vtotal = sum;
    vcount = cnt;
}

int UnitTimeStat::PrintStatis( char *buf )
{
    int len = 0;
    len = sprintf( buf, "\tmax-%.3f, min-%.3f, med-%.3f, \n\tmean-%.3f, var-%.3f, \n\ttotal-%.3f, count-%d\n",
           vmax, vmin, vmedian, vmean, vvar, vtotal, vcount );
    return len;
}


class ModuleTimeStat
{
public:
    ModuleTimeStat()
    {
        mdlname = "global";
        bprint = false;
        bstatis = false;
    }
    ~ModuleTimeStat()
    {
    }

    void ClearStatis()
    {
        items.clear();
    }

    void SetPrint( int _bprint )
    {
        bprint = _bprint;
    }

    int GetPrint()
    {
        return bprint;
    }

    void SetStatis( int _bstatis )
    {
        bstatis = _bstatis;
    }

    int GetStatis()
    {
        return bstatis;
    }

    void AddTimeItem( char* uname, float val )
    {
        if( bprint )
            printf( "Unit %s time: %.3f ms\n", uname, val );
        if( bstatis )
            items[uname].AddItem( val );
    }

    void Calculate()
    {
        map<string,UnitTimeStat>::iterator miter = items.begin();
        while( miter != items.end() )
        {
            UnitTimeStat& u = miter->second;
            u.Calculate();
            miter++;
        }
    }

    void PrintStatis( char *filnam )
    {
        char *stat_buf = (char*)malloc(1024*16);
        if( !stat_buf ) return;
        char *stat_cur = stat_buf;

        stat_cur += sprintf( stat_cur, "The module %s's time statics is : \n", mdlname.c_str() );

        map<string,UnitTimeStat>::iterator miter = items.begin();
        while( miter != items.end() )
        {
            string s = miter->first;
            UnitTimeStat& u = miter->second;
            stat_cur += sprintf( stat_cur, "\nUnit %s time: \n", s.c_str() );
            stat_cur += u.PrintStatis( stat_cur );
            miter++;
        }

        stat_cur += sprintf( stat_cur, "\nThe end.\n\n" );

        FILE *fil = fopen( filnam, "w" );
        if( fil )
        {
            fwrite( stat_buf, 1, stat_cur-stat_buf, fil );
            fclose( fil );
        }
        else
        {
            printf( stat_buf );
        }

        free( stat_buf );
    }

private:
    string mdlname;
    int bprint, bstatis;
    map<string,UnitTimeStat> items;
};


static ModuleTimeStat globalTimeStat;


void TimeStatSetPrint( int flag )
{
    globalTimeStat.SetPrint( flag );
}

void TimeStatSetStatis( int flag )
{
    globalTimeStat.SetStatis( flag );
}

void TimeStatClearStatis()
{
    globalTimeStat.ClearStatis();
}

void TimeStatCalculate()
{
    globalTimeStat.Calculate();
}

void TimeStatPrintStatis( char *filnam )
{
    globalTimeStat.PrintStatis( filnam );
}


double TimeStatTimeBegin()
{
    double time_s = 0;

    if( !globalTimeStat.GetPrint() && !globalTimeStat.GetStatis() )
        return 0;

#if defined __GNUC__

	struct timeval start;
	gettimeofday( &start, NULL );
    time_s = 1000000.0*start.tv_sec + start.tv_usec;

#else

	LARGE_INTEGER start;
	QueryPerformanceCounter( &start );
    time_s = start.QuadPart;

#endif

    return time_s;
}

void TimeStatTimeEnd( char *uname, double time_s )
{
    double time_e, time_u;

    if( !globalTimeStat.GetPrint() && !globalTimeStat.GetStatis() )
        return;

#if defined __GNUC__

	struct timeval end;
	gettimeofday( &end, NULL );
    time_e = 1000000.0*end.tv_sec + end.tv_usec;
    time_u = time_e - time_s;
    time_u /= 1000;

#else

	LARGE_INTEGER end, freq;
    double time_f;
	QueryPerformanceCounter( &end );
	QueryPerformanceFrequency( &freq );
    time_e = end.QuadPart;
    time_f = freq.QuadPart;
    time_u = (time_e-time_s)/time_f;
    time_u *= 1000;

#endif

    globalTimeStat.AddTimeItem( uname, (float)time_u );

}





