// worley_noise.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include "perlin.h"
#include <vector>
using namespace std;

double wrandom()
{
	return (double)rand() / (double)RAND_MAX;
}

//sx,sy,sz seed in grid(sx,sy,sz) 
//dxArr dx
//px,py,pz pixel coordinates of the seed in grid(sx,sy,sz)

void getSeedPixelCoors(const int sx, const int sy, const int sz, int gridsize,int gridxnum,int gridynum,int gridznum,
	int* dxArr,int* dyArr,int* dzArr,
	int& px, int& py, int& pz)
{
	int sx2 = sx; 
	int x0 = sx*gridsize;
	if (sx2 < 0){ sx2 = gridxnum - 1; x0 = -gridsize; }
	else if (sx2 == gridxnum ){ sx2 = 0; x0 = gridsize * gridxnum; }

	int sy2 = sy;
	int y0 = sy*gridsize;
	if (sy2 < 0){
		sy2 = gridynum - 1; y0 = -gridsize;
	}
	else if (sy2 == gridynum ) {
		sy2 = 0;
		y0 = gridsize*gridynum;
	}

	int sz2 = sz;
	int z0 = sz*gridsize;
	if (sz2 < 0){
		sz2 = gridznum - 1;
		z0 = -gridsize;
	}
	else if (sz2 == gridznum ){
		sz2 = 0;
		z0 = gridsize*gridznum;
	}

	int igrid = sz2*gridxnum*gridynum + sy2*gridxnum + sx2;
	px = x0 + dxArr[igrid];
	py = y0 + dyArr[igrid];
	pz = z0 + dzArr[igrid];

}

void getSeedPixelCoors2(const int sx, const int sy, const int sz,
	int gridsize,int gridxnum,int gridynum,int gridznum,
	vector<int>& dxArr,vector<int>& dyArr,vector<int>& dzArr,
	int& px, int& py, int& pz)
{
	int sx2 = sx; 
	int x0 = sx*gridsize;
	if (sx2 < 0){ sx2 = gridxnum - 1; x0 = -gridsize; }
	else if (sx2 == gridxnum ){ sx2 = 0; x0 = gridsize * gridxnum; }

	int sy2 = sy;
	int y0 = sy*gridsize;
	if (sy2 < 0){
		sy2 = gridynum - 1; y0 = -gridsize;
	}
	else if (sy2 == gridynum ) {
		sy2 = 0;
		y0 = gridsize*gridynum;
	}

	int sz2 = sz;
	int z0 = sz*gridsize;
	if (sz2 < 0){
		sz2 = gridznum - 1;
		z0 = -gridsize;
	}
	else if (sz2 == gridznum ){
		sz2 = 0;
		z0 = gridsize*gridznum;
	}

	int igrid = sz2*gridxnum*gridynum + sy2*gridxnum + sx2;
	px = x0 + dxArr[igrid];
	py = y0 + dyArr[igrid];
	pz = z0 + dzArr[igrid];

}


void worley(int width,int height,int depth,int gridSize,int randseed, vector<float>& retdata)
{
	srand(randseed);

	int xGridnum = width / gridSize  ;
	int yGridnum = height / gridSize  ;
	int zGridnum = depth / gridSize   ;

	retdata.resize( (size_t)width*height*depth ) ;
	
	cout << "gridnum:" << xGridnum << " " << yGridnum << " " << zGridnum << endl;

	vector<int> dxArr(xGridnum*yGridnum*zGridnum);
	vector<int> dyArr(xGridnum*yGridnum*zGridnum);
	vector<int> dzArr(xGridnum*yGridnum*zGridnum);

	cout << "begin random seeding." << endl;

	int itgrid = 0;
	for (int izg = 0; izg < zGridnum; ++izg)
	{
		for (int iyg = 0; iyg < yGridnum; ++iyg)
		{
			for (int ixg = 0; ixg < xGridnum; ++ixg)
			{
				float rx = wrandom();
				float ry = wrandom();
				float rz = wrandom();

				int rdx = rx * gridSize;
				int rdy = ry*gridSize;
				int rdz = rz*gridSize;

				int xcoor =   rdx;
				int ycoor =  rdy;
				int zcoor =   rdz;

				dxArr[itgrid] = xcoor;
				dyArr[itgrid] = ycoor;
				dzArr[itgrid] = zcoor;
				itgrid++;
			}
		}
	}
	cout << "seed is ok. begin fill buffer ..." << endl;
	float theMaxValue = 0;
	float theMinValue = 99999;
	int it = 0;
	for (int iz = 0; iz < depth; ++iz)
	{
		for (int iy = 0; iy < height; ++iy)
		{
			for (int ix = 0; ix < width; ++ix)
			{
				int xg = min(ix / gridSize,xGridnum-1);
				int yg = min(iy / gridSize,yGridnum-1);
				int zg = min(iz / gridSize,zGridnum-1);
				//printf(" debug - ") ;
				//if( ix==240 )
				//{
				//	cout<<"debug ..."<<endl;
				//}
				float dist2 = 99999;
				for (int zz = zg-1; zz <= zg+1 ; ++zz)
				{
					for (int yy = yg - 1; yy <= yg + 1; ++yy)
					{
						for (int xx = xg - 1; xx <= xg + 1; ++xx)
						{
							int seedx = 0;
							int seedy = 0;
							int seedz = 0;
							getSeedPixelCoors2(xx, yy, zz, gridSize,
								xGridnum, yGridnum, zGridnum,
								dxArr, dyArr, dzArr,
								seedx, seedy, seedz);
 
							float tdist2 = (seedx - ix)*(seedx - ix)
								+ (seedy - iy)*(seedy - iy)
								+ (seedz - iz)*(seedz - iz);
							tdist2 = sqrtf(tdist2);
							if (tdist2 < dist2)
							{
								dist2 = tdist2;
							}
						}
					}
				}
				if (dist2 > theMaxValue) theMaxValue = dist2;
				if (dist2 < theMinValue) theMinValue = dist2;
				//printf("debug %d %d %d ;; " , iz,iy,ix) ;
				retdata[iz*width*height + iy*width + ix] =  dist2 ;
				//printf(" debug + ") ;
			}
		}
		if( iz % 8==0 )cout << iz << " " ;
	}
	cout<<"min:"<<theMinValue<<",max:"<<theMaxValue<<endl;
	float diff = theMaxValue - theMinValue;
	for(int it = 0 ; it < retdata.size();++it )
	{
		float tval = (retdata[it] - theMinValue)/diff;
		retdata[it] = tval;
	}
}


void writeHeader(string rawfile, int w , int h , int d)
{
	{//write hdr file
		string hdrfile = rawfile + string(".hdr");
		FILE* pf2 = fopen(hdrfile.c_str(), "w");
		fprintf(pf2, "ENVI\n");
		fprintf(pf2, "description = {\n");
		fprintf(pf2, "worley3d }\n");
		fprintf(pf2, "samples = %d\n",w);
		fprintf(pf2, "lines = %d\n",h);
		fprintf(pf2, "bands = %d\n",d);
		fprintf(pf2, "header offset = 0\n");
		fprintf(pf2, "file type = ENVI Standard\n");
		fprintf(pf2, "data type = 1\n");
		fprintf(pf2, "interleave = bsq\n");
		fprintf(pf2, "sensor type = Unknown\n");
		fprintf(pf2, "byte order = 0\n");
		fprintf(pf2, "wavelength units = Unknown\n");
		fclose(pf2);
	}
}


int main(int argc, char* argv[])
{
	cout << "A program to make 3D worley noise. wangfengdev@163.com 2019-10-14." << endl;
	cout << "call:worley_noise width height depth gridsize outfile seed [invert]" << endl;

	//perlin
	//perlin() ; // i think a scale of 4 perlin is ok.2020-11-10


	{//perlinµþ¼Óworley
		vector<float> perlinnoise(256*256*256) ;
		perlin(perlinnoise) ;

		vector<float> werlynoise ;
		worley(256,256,256,16,200,werlynoise) ;

		vector<unsigned char> outdata(werlynoise.size()) ;
		for(int it =0 ; it < perlinnoise.size() ;++ it )
		{
			float noise1 = perlinnoise[it] ;
			//printf("debug %8.4f , " , noise1) ;
			outdata[it] = (perlinnoise[it]*0.90 + (1-werlynoise[it])*0.10) * 255 ;
		}
		string outfile1 = "perlinworly.raw" ;
		FILE* pf11 = fopen( outfile1.c_str() , "wb" ) ;
		fwrite(outdata.data() , 1 , outdata.size() , pf11 ) ;
		fclose(pf11) ;

		writeHeader(outfile1,256,256,256) ;

	}
	return 1 ;

	int width = 256;
	int height = 256;
	int depth = 256;
	int gridSize = 32;
	char outfile[512] = "layeredworley_grid13";
	int randseed = 0;
	int invert = 1;
	//if (argc == 7 || argc==8 )
	//{
	//	width = atof(argv[1]);
	//	height = atof(argv[2]);
	//	depth = atof(argv[3]);
	//	gridSize = atof(argv[4]);
	//	strcpy(outfile , argv[5] ) ;
	//	randseed = atof(argv[6]);
	//	if (argc == 8)
	//	{
	//		invert = atof(argv[7]);
	//	}
	//}
	//else
	//{
	//	cout << "error: no enough params." << endl;
	//	return 11;
	//}

	//perlin µþ¼Óperlin
	vector<float> worley_grid32 ;
	worley(width,height,depth,256/20,randseed+100,worley_grid32) ;

	vector<float> worley_grid16 ;
	worley(width,height,depth,256/40,randseed+200,worley_grid16) ;

	vector<float> worley_grid8 ;
	worley(width,height,depth,256/40,randseed+300,worley_grid8) ;


	unsigned char* outbuffer = new unsigned char[width*height*depth];
	int asize = width*height*depth;
	for (int it = 0; it < asize; ++it)
	{
		//outbuffer[it] = 255.0*(worley_grid32[it]+worley_grid16[it]+worley_grid8[it])/3.0 ;
		outbuffer[it] = 255.0*(worley_grid32[it]*0.5 + worley_grid16[it]*0.5 + worley_grid8[it] * 0. )/1.;
		if (invert > 0)
		{
			outbuffer[it] = 255 - outbuffer[it];
		}
		
	}

	FILE* pf = fopen(outfile, "wb");
	fwrite(outbuffer, width*height*depth, 1, pf);
	fclose(pf);

	{//write hdr file
		string hdrfile = string(outfile) + string(".hdr");
		FILE* pf2 = fopen(hdrfile.c_str(), "w");
		fprintf(pf2, "ENVI\n");
		fprintf(pf2, "description = {\n");
		fprintf(pf2, "worley3d }\n");
		fprintf(pf2, "samples = %d\n",width);
		fprintf(pf2, "lines = %d\n",height);
		fprintf(pf2, "bands = %d\n",depth);
		fprintf(pf2, "header offset = 0\n");
		fprintf(pf2, "file type = ENVI Standard\n");
		fprintf(pf2, "data type = 1\n");
		fprintf(pf2, "interleave = bsq\n");
		fprintf(pf2, "sensor type = Unknown\n");
		fprintf(pf2, "byte order = 0\n");
		fprintf(pf2, "wavelength units = Unknown\n");
		fclose(pf2);
	}



	delete[] outbuffer;


	return 0;
}

