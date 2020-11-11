// perlin.cpp : Defines the entry point for the console application.
//
#include <math.h>
#include <stdio.h>
#include <vector>

using std::vector;

float random(float x, float y) {
	float dotval = x * 12.9898  + y * 78.2333 ;
	float sinval = abs( sin(dotval) ) ;
	float temp1 = sinval * 43758.54f ;
	return temp1 - (int)temp1;
}
float random3d(float x, float y,float z) {
	float dotval = x * 112.9898 + y * 178.2333 + z* 142.827 ;
	float sinval = abs(sin(dotval));
	float temp1 = sinval * 43758.54f;
	return temp1 - (int)temp1;
}


float mix(float vmin, float vmax, float x)
{
	float u = x*x*(3.0 - 2.0*x);
	return vmin + (vmax - vmin) * u;
}

float noise( float x , float y ) {
	int ix = floor(x);
	int iy = floor(y);

	float fx = x - ix;
	float fy = y - iy;
 
	// Four corners in 2D of a tile
	float a = random(ix,iy);
	float b = random(ix + 1 , iy );
	float c = random(ix , iy + 1 );
	float d = random(ix + 1 , iy + 1 );

	float ux = fx * fx * (3.f - 2.f * fx);
	float uy = fy * fy * (3.f - 2.f * fy);

	float inter0 = mix(a, b, fx);
	float inter1 = mix(c, d, fx);
	float inter2 = mix(inter0, inter1, fy);

	return inter2;
}

float noise3d(float x, float y,float z) {
	int ix = floor(x);
	int iy = floor(y);
	int iz = floor(z);
	float fx = x - ix;
	float fy = y - iy;
	float fz = z - iz;

	// Four corners in 2D of a tile
	float a = random3d(ix      , iy     , iz     );
	float b = random3d(ix + 1  , iy     , iz     );
	float c = random3d(ix      , iy + 1 , iz     );
	float d = random3d(ix + 1  , iy + 1 , iz     );

	float a1 = random3d(ix, iy, iz+1);
	float b1 = random3d(ix + 1, iy, iz+1);
	float c1 = random3d(ix, iy + 1, iz+1);
	float d1 = random3d(ix + 1, iy + 1, iz+1);

	//float ux = fx * fx * (3.f - 2.f * fx);
	//float uy = fy * fy * (3.f - 2.f * fy);
	//float uz = fz * fz * (3.f - 2.f * fz);

	float inter0 = mix(a, b, fx);
	float inter1 = mix(c, d, fx);
	float inter2 = mix(inter0, inter1, fy);

	float inter00 = mix(a1, b1, fx);
	float inter11 = mix(c1, d1, fx);
	float inter22 = mix(inter00, inter11, fy);

	float inter33 = mix(inter2, inter22, fz);

	return inter33;
}

float fbm(float ptx , float pty ) {
	// Initial values
	float value = 0.0;
	float amplitude = .5;
	float frequency = 0.;
	//
	// Loop of octaves
	int oct = 6;

	for (int i = 0; i < oct ; i++) {
		value += amplitude * noise( ptx , pty );
		ptx *= 2.f;
		pty *= 2.f;
		amplitude *= .5f;
	}
	return value;
}

float fbm3d(float ptx, float pty ,float ptz) {
	// Initial values
	float value = 0.0;
	float amplitude = .5;
	float frequency = 0.;
	//
	// Loop of octaves
	int oct = 6 ;//use 6

	for (int i = 0; i < oct; i++) {
		value += amplitude * noise3d(ptx, pty,ptz);
		ptx *= 2.f;
		pty *= 2.f;
		ptz *= 2.f;
		amplitude *= .5f;
	}
	return value;
}

int perlin(vector<float>& retdata )
{
	//char outfile[] = "perlin256";

	int xsize = 256;
	int ysize = 256;
	int zsize = 256;
	int xysize = 256 * 256;
	int asize = 256 * 256 * 256;

	float scale = 4.f;

	retdata.resize(asize) ;
	//unsigned char* buffer1 = new unsigned char[256 * 256 * 256];

	//FILE* pf = fopen(outfile, "wb");

	float valmin = 99999.f;
	float valmax = -99999.f;

	for (int iz = 0; iz < 256; ++iz)
	{
		int it0 = iz * 256 * 256;
		for (int iy = 0; iy < 256; ++iy)
		{
			for (int ix = 0; ix < 256; ++ix)
			{
				float ptx = ix * scale / 256.f  ;
				float pty = iy * scale / 256.f  ;
				float ptz = iz * scale / 256.f  ;

				float color1 = fbm3d(ptx, pty,ptz);//float color1 = fbm(ptx, pty);
				if (color1 > valmax)
				{
					valmax = color1;
				}
				if (color1 < valmin)
				{
					valmin = color1;
				}
				retdata[it0 + iy * 256 + ix] = color1;
			}
		}
		if( iz %10==0 ) printf("band:%d.", iz);
		//if (iz == 0) break;
	}

	float dist = valmax - valmin;

	for (int it = 0; it < asize; ++it)
	{
		retdata[it] = (retdata[it] - valmin) / dist ;
	}
	return 0;
}