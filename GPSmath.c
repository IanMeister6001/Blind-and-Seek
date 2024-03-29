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
float calcBearing(int lat,int lon,int lat2,int lon2)
{
    double teta1 = radians(lat);
    double teta2 = radians(lat2);
    double delta = radians(lon2-lon);


    double y = sinf(delta) * cosf(teta2);
    double x = cosf(teta1)*sinf(teta2) - sinf(teta1)*cosf(teta2)*cosf(delta);
    double brng = atan2(y,x);
    brng = degrees(brng);// radians to degrees
    brng = ( ((int)brng + 360) % 360 );

    return brng;
}

//////////////////////////////////////////////////////////////////////////////////////
//berekent de afstand tussen twee GPS coordinaten
//ontvangt twee GPS coordinaten, en returnt de afstand ertussen
//Auth: Damian van Hoornaar
//////////////////////////////////////////////////////////////////////////////////////
int calcDistance(int lat, int lon, int lat2, int lon2)
{
	int deltaLatSq = (lat2 - lat)*(lat2 - lat);
	int deltaLonSq = (lon2 - lon)*(lon2 - lon);
	int sum = deltaLatSq + deltaLonSq;
	return sqrtf(sum);
}

//////////////////////////////////////////////////////////////////////////////////////
//zet de int waardes om naar floats voor de bearing functie
//Auth: Damian van Hoornaar
//////////////////////////////////////////////////////////////////////////////////////
float IntToFloat(float value)
{
	value = value/100000;
	value = floor(100000*value)/100000;
	return value;
}

//////////////////////////////////////////////////////////////////////////////////////
//zet de float waardes om naar int voor de bearing functie
//Auth: Damian van Hoornaar
//////////////////////////////////////////////////////////////////////////////////////
float FloatToInt(float value)
{
	return value*100000;
}

///////////////////////////////////////////////////////////////////////////////////////
//krijgt coordinaten als string binnen en zet ze om naar een int waarde
//neemt 5 decimalen mee bij Floatval *= 1000, elke 0 die toegevoegd of eraf gehaald wordt
//zal een decimaal meer of minder respectievelijk betekenen.
//Auth: Lex Bleyenberg
///////////////////////////////////////////////////////////////////////////////////////
int RMCtoINT(char RMCval[])
{
	double Floatval = atof(RMCval);
	int degrees =   floor(Floatval/100);
	double minutes = Floatval -(degrees*100);
	//double seconds = (Floatval-degrees*100-minutes)*1000;
	Floatval = (degrees + minutes/60)*100000;
	return (int)Floatval;
}
