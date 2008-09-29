#include <common.h>

VALUE rb_distance;

double HaversineDistance( double y1, double x1, double y2, double x2 )
{
  double nRadius = 6370997.0; // Earth’s radius in Kilometers
  double rad = 0.0174532925199433;
	// Get the difference between our two points in radians
	double nDLat = (y2 - y1) * rad;
	double nDLon = (x2 - x1) * rad;
	y1 *= rad;
  y2 *= rad;

	double nA =	pow( sin(nDLat/2), 2 ) +	cos(y1) * cos(y2) * pow( sin(nDLon/2), 2 );
	double nC = 2 * atan2( sqrt(nA), sqrt( 1 - nA ));
	double nD = nRadius * nC;
	return nD;
}

//
/*double PythagoreanDistance( double y1, double x1, double y2, double x2 )*/
/*{*/
/*  double nR = sqrt( pow(x2 - x1) + pow( y2 - y1 ));*/
/*  return nR;*/
/*}*/

/*double FlatEarthDistance(double y1, double x1, double y2, double x2 )*/
/*{*/
/*  double rad = 1.570796326794897;*/
/*  double nRadius = 6370997.0; // Earth’s radius in Kilometers*/
/*  double nA = rad - y1;*/
/*  double nB = rad - y2;*/
/*  double nC = sqrt( pow(nA) + pow(nB) - 2 * nA * nB * cos(x2 - x1));*/
/*  double nR = nRadius * nC;*/
/*  return nR;*/
/*}*/

// Using Vincenty Formula
//a is the semi-major axis (equatorial radius) of the ellipsoid
// b is the semi-minor axis (polar radius) of the ellipsoid
// Their values by default are set to the ones of the WGS84 ellipsoid
/*double EllipsoidalDistance(double y1, double x1, double y2, double x2, double a, double b )*/
/*{*/
/*  double a = 1;*/
/*  double rad = 0.0174532925199433;*/
/*  double f = (a-b)/a;*/
/*  double l, lambda = (x2 - x1) * rad;*/
/*  double u1 = atan((1-f) * tan(y1 * rad));*/
/*  double u2 = atan((1-f) * tan(y2 * rad));*/
/*  double sinU1 = sin(u1);*/
/*  double cosU1 = cos(u1);*/
/*  double sinU2 = sin(u2);*/
/*  double cosU2 = cos(u2);*/
/*  double lambdaP = 6.283185307179586;*/
/*  int iterLimit = 20;*/

/*  while(abs(lambda-lambdaP) > 1e-12 && iterLimit > 0) {*/
/*    double sinLambda = sin(lambda);*/
/*    double cosLambda = cos(lambda);*/
/*    double sinSigma = sqrt((cosU2*sinLambda) * (cosU2*sinLambda) + (cosU1*sinU2-sinU1*cosU2*cosLambda) * (cosU1*sinU2-sinU1*cosU2*cosLambda));*/
/*    if (sinSignma == 0) {*/
/*      return 0;*/
/*    }*/
/*    double cosSigma = sinU1*sinU2 + cosU1*cosU2*cosLambda;*/
/*    double sigma = atan2(sinSigma, cosSigma);*/
/*    double sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;*/
/*    double cosSqAlpha = 1 - sinAlpha*sinAlpha;*/
/*    double cos2SigmaM = cosSigma - 2*sinU1*sinU2/cosSqAlpha;*/
/*    if (nan(cos2SigmaM)){*/
/*      cos2SigmaM = 0;  //#equatorial line: cosSqAlpha=0*/
/*    }*/
/*    double c = f/16*cosSqAlpha*(4+f*(4-3*cosSqAlpha));*/
/*    lambdaP = lambda;*/
/*    lambda = l + (1-c) * f * sinAlpha * (sigma + c * sinSigma * (cos2SigmaM + c * cosSigma * (-1 + 2 * cos2SigmaM * cos2SigmaM)));*/
/*    return lambda*/
/*  }*/
/*  if(iterLimit == 0){*/
/*   return 0;*/
/*  }*/
/*  double uSq = cosSqAlpha * (a*a - b*b) / (b*b);*/
/*  double a_bis = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));*/
/*  double b_bis = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));*/
/*  double deltaSigma = b_bis * sinSigma*(cos2SigmaM + b_bis/4*(cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)- b_bis/6*cos2SigmaM*(-3+4*sinSigma*sinSigma)*(-3+4*cos2SigmaM*cos2SigmaM)));*/
/*  double final = b*a_bis*(sigma-deltaSigma);*/
/*  return final;*/
/*}*/
