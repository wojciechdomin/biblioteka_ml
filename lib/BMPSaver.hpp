#include <bits/stdc++.h>

using namespace std;

//susing uchar = unsigned char;

void save_bmp(int width, int height, string filename, float* array){
	fstream g(filename,fstream::out|fstream::trunc);
	int bmp_header[] = {66,77,54,12,0,0,0,0,0,0,54,0,0,0,40,0,0,0,32,0,0,0,32,0,0,0,1,0,24,0,0,0,0,0,0,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	g<<noskipws;
	int width2 = width;
	int height2 = height;
	for(int i = 0; i < 54; i++){
		if(i>17 && i<19){
			g<<(unsigned char)(width2%256);
			width2/=256;

		}else
		if(i>21 && i<23){
			g<<(unsigned char)(height2%256);
			height2/=256;
		}else{
			g<<(unsigned char)bmp_header[i];
		}
	}
	int r = 0;
	for(int i = height-1; i >=0; i--){
		for(int j = 0; j < width; j++){
			for(int k = 0; k < 3; k++){
				g<<(unsigned char)int(array[height*width*(2-k)+width*i+j]*254.0);//;
				r++;
			}
		}
	}
	cout<<r<<"\n";
	g.close();

}

/*
int main(){
	unsigned char x;
	fstream f("sample.bmp",ios::out|ios::in|ios::binary);
	fstream g("durple.bmp",ios::out|ios::in|ios::binary);
	int r = 0;
	f<<noskipws;
	cout<<"[";
	int bmp_header[] = {66,77,54,12,0,0,0,0,0,0,54,0,0,0,40,0,0,0,32,0,0,0,32,0,0,0,1,0,24,0,0,0,0,0,0,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	for(int i = 0; i < 54; i++){
		f>>x;
		g<<x;
		if(x==1) cout<<i<<"\n";
		//cout<<int(x)<<",]"[i==53];
	}
	cout<<"\n";
	for(int i = 31; i >=0; i--){
		for(int j = 0; j < 32; j++){
			for(int k = 0; k < 3; k++){
				f>>x;
				if(i == 2){
					g<<(unsigned char)255;
				}else{
					if(j == 15) g<<(unsigned char)128; else
					g<<(unsigned char)0;
				}	
			}
		}
	}
	cout<<r;
	f.close();
	g.close();
	return 0;
}*/