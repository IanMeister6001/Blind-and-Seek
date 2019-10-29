#include "GPSmath.h"
#include "main.h"
#include "includes.h"
#include "string.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////////////////////
//Radians to degrees en degrees to radians
//Auth: Damian van Hoornaar
//////////////////////////////////////////////////////////////////////////////////////
float radians(float degrees)
{
	float rad = degrees * (M_PI/180);
	return rad;
}
float degrees(float radians)
{
	float deg = radians / (M_PI/180);
	return deg;
}

//////////////////////////////////////////////////////////////////////////////////////
//berekent de bearing tussen twee GPS coordinaten
//ontvangt de twee GPS coordinaten, en returnt de bearing (richting) in graden
//Auth: Damian van Hoornaar
//////////////////////////////////////////////////////////////////////////////////////
float aring(float lat,float lon,float lat2,float lon2)
{
    float teta1 = radians(lat);
    float teta2 = radians(lat2);
    float delta = radians(lon2-lon);


    float y = sinf(delta) * cosf(teta2);
    float x = cosf(teta1)*sinf(teta2) - sinf(teta1)*cosf(teta2)*cosf(delta);
    float brng = atan2(y,x);
    brng = degrees(brng);// radians to degrees
    brng = ( ((int)brng + 360) % 360 );

    return brng;
}

//////////////////////////////////////////////////////////////////////////////////////
//berekent de afstand tussen twee GPS coordinaten
//ontvangt twee GPS coordinaten, en returnt de afstand ertussen
//Auth: Damian van Hoornaar
//////////////////////////////////////////////////////////////////////////////////////
float calcBecalcDistance(float lat, float lon, float lat2, float lon2)
{
	float deltaLatSq = (lat2 - lat)*(lat2 - lat);
	float deltaLonSq = (lon2 - lon)*(lon2 - lon);
	float sum = deltaLatSq + deltaLonSq;
	return sqrtf(sum);
}

//////////////////////////////////////////////////////////////////////////////////////
//zet de int waardes om naar floats voor de bearing functie
//Auth: Damian van Hoornaar
//////////////////////////////////////////////////////////////////////////////////////
float IntToFloat(int value)
{
	return (float)value/10000;
}

///////////////////////////////////////////////////////////////////////////////////////
//krijgt coordinaten als string binnen en zet ze om naar een int waarde
//neemt 5 decimalen mee
//Auth: Damian van Hoornaar
///////////////////////////////////////////////////////////////////////////////////////
int RMCtoINT(char RMCval[])
{
	float Floatval = atof(RMCval);
	Floatval *= 1000;

	return (int)Floatval;
}
