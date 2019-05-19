#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include<string>
class PerlinNoiseVis : public olc::PixelGameEngine
{
public:
	PerlinNoiseVis()
	{
		sAppName = "PerlinNoiseVisualizer";
	}
private:
	
	float* noiseSeed   = nullptr;//random noise
	float* perlinNoise = nullptr;//output
	int outputSize     = 512;
	
	//random noise 2d
	float* noiseSeed2d = nullptr;
	float* perlinNoise2d = nullptr;
	int outputWidth = 512;
	int outputHeight = 512;



	int octavesIter = 1;//cantidad de octavas activas
	int scaleBias = 2.0f;
	bool mode2d = false;//cambia entre 1d o 2d


	bool OnUserCreate() override
	{
		outputSize  = ScreenWidth();
		//asignamos memoria al array
		noiseSeed   = new float[outputSize];
		perlinNoise = new float[outputSize];

		//agregamos al array1d con ruido a partir de numeros aleatorios
		for (int i = 0; i < outputSize; i++)
			noiseSeed[i] = (float)rand() / (float)RAND_MAX;
		
		//array 2d
		outputWidth = ScreenWidth();
		outputHeight = ScreenHeight();
		noiseSeed2d = new float[outputWidth * outputHeight];
		perlinNoise2d = new float[outputWidth * outputHeight];

		for (int i = 0; i < outputWidth * outputHeight; i++)
			noiseSeed2d[i] = (float)rand() / (float)RAND_MAX;


		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		FillRect(0,0, ScreenWidth(), ScreenHeight(), olc::BLACK);
		if (GetKey(olc::Key::M).bPressed)
			ChangeMode();
		
	
		
		if (GetKey(olc::Key::W).bPressed)
			octavesIter++;
		if (octavesIter > 10)
			octavesIter = 1;

		
		

		if (GetKey(olc::Key::D).bReleased)
			ScaleChange(true);
		else if (GetKey(olc::Key::A).bReleased)
			ScaleChange(false);

		if (!mode2d)
		{
			if (GetKey(olc::Key::SPACE).bPressed)
			{
				//agregamos al array con ruido a partir de numeros aleatorios
				for (int i = 0; i < outputSize; i++)
					noiseSeed[i] = (float)rand() / (float)RAND_MAX;
			}
			PerlinNoise1D(outputSize, noiseSeed, octavesIter,scaleBias, perlinNoise);

			//dibujar en pantalla
			for (int i = 0; i < outputSize; i++)
			{
				int yScale = -(perlinNoise[i] * (float)ScreenHeight() / 2.0f) + (float)ScreenHeight() / 2.0f;
				for (int j = yScale; j < ScreenHeight() / 2; j++)
				{
					Draw(i, j, olc::GREEN);
				}
			}
		}
		else
		{
			if (GetKey(olc::Key::SPACE).bPressed)
			{
				//agregamos al array2d con ruido nuevos numeros random
				for (int i = 0; i < outputWidth * outputHeight; i++)
					noiseSeed2d[i] = (float)rand() / (float)RAND_MAX;
			}
			PerlinNoise2D(outputWidth, outputHeight, noiseSeed2d, octavesIter, perlinNoise2d);

			//dibujar en pantalla
			for (int x = 0; x < outputWidth; x++)
			{
				for (int y = 0; y < outputHeight; y++)
				{
					olc::Pixel pxcolor;
					int pixel_bw = (int)(perlinNoise2d[y * outputWidth + x] * 12.0f);

					//determinar tono de gris
					switch (pixel_bw)
					{
					case 0: pxcolor = olc::Pixel(12, 12, 199);
						break;
					case 1: pxcolor = olc::Pixel(12, 12, 199);
						break;
					case 2: pxcolor = olc::Pixel(12, 12, 199);
						break;
					case 3: pxcolor = olc::Pixel(12, 12, 199);
						break;

					case 4: pxcolor = olc::Pixel(179, 50, 131);
						break;
					case 5: pxcolor = olc::Pixel(158, 44, 116);
						break;
					case 6: pxcolor = olc::Pixel(102, 28, 75);
						break;
					case 7: pxcolor = olc::Pixel(79, 22, 58);
						break;

					case 8: pxcolor = olc::Pixel(229, 43, 80);
						break;
					case 9: pxcolor = olc::Pixel(229, 43, 80);
						break;
					case 10: pxcolor = olc::Pixel(229, 43, 80);
						break;
					case 11: pxcolor = olc::Pixel(229, 43, 80);
						break;
					
					case 12: pxcolor = olc::Pixel(30, 229, 70);
						break;
					case 13: pxcolor = olc::Pixel(30, 229, 70);
						break;
					case 14: pxcolor = olc::Pixel(30, 229, 70);
						break;
					case 15: pxcolor = olc::Pixel(30, 229, 70);
						break;
					case 16: pxcolor = olc::Pixel(30, 229, 70);
						break;
					}
					Draw(x, y, pxcolor);
				}
			}
		
		}
		std::string octavastext = "Octavas-->" + std::to_string(octavesIter);
		std::string scalebiastext = "Scale-->" + std::to_string(scaleBias);

		DrawString(25, ScreenHeight() - 100, octavastext);
		DrawString(25, ScreenHeight() - 125, scalebiastext);
		return true;
	}

	void ChangeMode()
	{
		mode2d = !mode2d;
	}

	void ScaleChange(bool dir)
	{
		//1 derecha 0 izq
		if (scaleBias == 1.0f || scaleBias == 4.0f)
		{
			scaleBias = 2.0f;
		}
		if (dir)
		{
			scaleBias += 0.2f;
		}
		else
		{
			scaleBias -= 0.2f;
		}
	}

	void PerlinNoise1D(int sizeIter, float* seed, int octaves, float scaleBias, float* output)
	{
		for (int x = 0; x < sizeIter; x++)
		{
			float noise = 0.0f;
			float scale = 1.0f;
			float scaleAcumulate = 0.0f;
			for (int i = 0; i < octaves; i++)
			{
				//obtiene la distancia entre los valores a interpolar
				int pitch = sizeIter >> i;
				//calcula el valor de la muestra 1 a partir del pitch 
				int sampleOne = (x / pitch) * pitch;
				
				//calcula el valor del segundo a partir del pitch + primer muestra
				int sampleTwo = (sampleOne + pitch) % sizeIter;

				//avance en % de donde nos encontramos en el pitch
				float blend = (float)(x - sampleOne) / (float)pitch;
				//interpolacion lineal de la muestra valor(0 a 1)
				float finalSample = (1.0f - blend) * seed[sampleOne] + blend * seed[sampleTwo];

				//agrega a nuestro acumulador de ruido
				noise += finalSample * scale;
				scaleAcumulate += scale;
				scale /= scaleBias;
			}

			//conversion de escala a rango de semilla[0,1]
			output[x] = noise / scaleAcumulate;

		}
		
	}

	void PerlinNoise2D(int width, int height, float* seed, int octaves, float* output)
	{
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				float noise = 0.0f;
				float scale = 1.0f;
				float scaleAcumulate = 0.0f;
				for (int i = 0; i < octaves; i++)
				{
					//obtiene la distancia entre los valores a interpolar
					int pitch = width >> i;
					 
					//calcula el valor de la muestra 1 a partir del pitch 
					int sampleXOne = (x / pitch) * pitch;
					int sampleYOne = (y / pitch) * pitch;
					//calcula el valor del segundo a partir del pitch + primer muestra
					int sampleXTwo = (sampleXOne + pitch) % width;
					int sampleYTwo = (sampleYOne + pitch) % height;
					

					//avance en % de donde nos encontramos en el pitch
					float xBlend = (float)(x - sampleXOne) / (float)pitch;
					float yBlend = (float)(y - sampleYOne) / (float)pitch;
					
					//interpolacion lineal de la muestra valor(0 a 1)
					float sampleAlfa = (1.0f - xBlend) * seed[sampleYOne * width + sampleXOne] + xBlend * seed[sampleYOne * width + sampleXTwo];
					float sampleBeta = (1.0f - xBlend) * seed[sampleYTwo * width + sampleXOne] + xBlend * seed[sampleYTwo * width + sampleXTwo];
					//usamos el yBlend para interpolar entre los 2 puntos del ruido

					noise += (yBlend * (sampleBeta - sampleAlfa) + sampleAlfa) * scale;
					scaleAcumulate += scale;
					scale = scale / scaleBias;
				}

				//conversion de escala a rango de semilla[0,1]
				output[y * width + x] = noise / scaleAcumulate;
			}
			
		}
	
	}
};
int main()
{
	PerlinNoiseVis demo;
	if (demo.Construct(512, 512, 2, 2))
		demo.Start();
	return 0;
}