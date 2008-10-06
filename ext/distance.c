#include <common.h>

VALUE rb_distance_haversine;
// Haversine formula.
double HaversineDistance( double y1, double x1, double y2, double x2 )
{
  double nRadius = 6370997.0; // Earth’s radius in Kilometers
  double rad = 0.0174532925199433;
	double nDLat = (y2 - y1) * rad;
	double nDLon = (x2 - x1) * rad;
	y1 *= rad;
  y2 *= rad;

	double nA =	pow( sin(nDLat/2), 2 ) +	cos(y1) * cos(y2) * pow( sin(nDLon/2), 2 );
	double nC = 2 * atan2( sqrt(nA), sqrt( 1 - nA ));
	double nD = nRadius * nC;
	return nD;
}

VALUE rb_distance_euclidian;
// Euclidian, nice guy.
double EuclidianDistance( double y1, double x1, double y2, double x2 )
{
  double nR = sqrt( pow(x2 - x1, 2) + pow( y2 - y1, 2));
  return nR;
}

VALUE rb_distance_flat_earth;
// Afterall, the earth is flat!
// =P
double FlatEarthDistance(double y1, double x1, double y2, double x2 )
{
  double rad = 1.570796326794897;
  double nRadius = 6370997.0; // Earth’s radius in Kilometers
  double nA = rad - y1;
  double nB = rad - y2;
  double nC = sqrt( pow(nA,2) + pow(nB,2) - 2 * nA * nB * cos(x2 - x1));
  double nR = nRadius * nC;
  return nR;
}

VALUE rb_distance_ellipsoidal;
// Using Vincenty Formula
//a is the semi-major axis (equatorial radius) of the ellipsoid
// b is the semi-minor axis (polar radius) of the ellipsoid
// Their values by default are set to the ones of the WGS84 ellipsoid
double EllipsoidalDistance(double y1, double x1, double y2, double x2, double a, double b )
{
	a = 6378137.0;
  double rad = 0.0174532925199433;
  double f = (a-b)/a;
  double l = (x2 - x1) * rad;

  double u1 = atan((1-f) * tan(y1 * rad));
  double u2 = atan((1-f) * tan(y2 * rad));
  double sinU1 = sin(u1);
  double cosU1 = cos(u1);
  double sinU2 = sin(u2);
  double cosU2 = cos(u2);

  double lambdaP = 6.283185307179586;
  int iter_limit = 20;

  double sin_sigma;
	double cos_sq_alpha;
	double cos_sigma_m;
 	double cos_sigma ;
 	double sigma;

  while(abs(l-lambdaP) > 1e-12 && iter_limit > 0) {
    iter_limit--;
    double sin_lambda = sin(l);
    double cos_lambda = cos(l);
    sin_sigma = sqrt((cosU2*sin_lambda) * (cosU2*sin_lambda) + (cosU1*sinU2-sinU1*cosU2*cos_lambda) * (cosU1*sinU2-sinU1*cosU2*cos_lambda));
    if (sin_sigma == 0)
    {
    	return 0;
    }
    cos_sigma = sinU1*sinU2 + cosU1*cosU2*cos_lambda;
    sigma = atan2(sin_sigma, cos_sigma);
    double sin_alpha = cosU1 * cosU2 * sin_lambda / sin_sigma;
    cos_sq_alpha = 1 - sin_alpha*sin_alpha;
    cos_sigma_m = cos_sigma - 2*sinU1*sinU2/cos_sq_alpha;
    if ( cos_sigma_m == NAN )
    {
      cos_sigma_m = 0;  //#equatorial line: cos_sq_alpha=0
    }
    double c = f/16*cos_sq_alpha*(4+f*(4-3*cos_sq_alpha));

    l = l + (1-c) * f * sin_alpha * (sigma + c * sin_sigma * (cos_sigma_m + c * cos_sigma * (-1 + 2 * cos_sigma_m * cos_sigma_m)));
    return l;
  }
  if(iter_limit == 0)
  {
   return 0;
  }
  double uSq = cos_sq_alpha * (a*a - b*b) / (b*b);
  double a_bis = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
  double b_bis = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));
  double deltaSigma = b_bis * sin_sigma*(cos_sigma_m + b_bis/4*(cos_sigma*(-1+2*cos_sigma_m*cos_sigma_m)- b_bis/6*cos_sigma_m*(-3+4*sin_sigma*sin_sigma)*(-3+4*cos_sigma_m*cos_sigma_m)));
  double final = b*a_bis*(sigma-deltaSigma);
  return final;
}
