#pragma once
struct MRGB
{
	unsigned char R, G, B;
};


class Texture
{
public:
	MRGB				mTexture[800][800];
	int					mTextHeight;
	int					mTextWidth;

	Texture() {}
	void SetTexture(const char* texturePath);
};

