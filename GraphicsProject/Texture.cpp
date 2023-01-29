#include "Texture.h"
#include <stdio.h>
#include <Windows.h>


void Texture::SetTexture(const char* texturePath)
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	FILE* pfin = fopen(texturePath, "rb");
	//Read Bitmap file header
	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pfin);
	//Read Bitmap info header
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pfin);
	//24 Bit color
	mTextHeight = abs(infoHeader.biHeight);
	mTextWidth = abs(infoHeader.biWidth);
	int size = mTextHeight * mTextWidth;
	fread(mTexture, sizeof(MRGB), size, pfin);
	
}
