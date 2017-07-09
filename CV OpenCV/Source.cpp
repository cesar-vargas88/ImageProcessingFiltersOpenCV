//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Transformación de intensidad de una imagen con OpenCV y C++											//
//																										//				
//	Este programa permite obtener el negativo, transformación logarítmica, transformación exponencial,	//
//	Contrast_Streching y filtro móvil con una máscara de 3x3 y 5x5 de una imagen.						//
//																										//
//	21/04/2017																							//
//	César Augusto Vargas Torres																			//
//																										//
//////////////////////////////////////////////////////////////////////////////////////////////////////////	

#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\videoio\videoio.hpp"
#include "opencv2\imgcodecs\imgcodecs.hpp"

#include <vector>
#include <stdio.h>
#include <Windows.h>
#include <iostream>

using namespace cv;
using namespace std;

//////////////////////////////////////////////////////////////////
//	variables para control de las Trackbar que crean ventanas	//
//////////////////////////////////////////////////////////////////

int iGris				= 0;
int iNegativo			= 0;
int iLogaritmo			= 0;
int iExponencial		= 0;
int iContrast_Streching	= 0;
int iPromedio_3x3		= 0;
int iPromedio_5x5		= 0;
int iGradiente			= 0;
int iLaplaciano			= 0;
int iHighBoost			= 0;
int iLowPassButterworth	= 0;
int iHighPassGaussian	= 0;
int iGlobalThresholding = 0;
int iOtsuMethod			= 0;
int iThresholding		= 0;

bool bGris					= false;
bool bNegativo				= false;
bool bLogaritmo				= false;
bool bExponencial			= false;
bool bContrast_Streching	= false;;
bool bPromedio_3x3			= false;
bool bPromedio_5x5			= false;
bool bGradiente				= false;
bool bLaplaciano			= false;
bool bHighBoost				= false;
bool bLowPassButterworth	= false;
bool bHighPassGaussian		= false;
bool bGlobalThresholding	= false;
bool bOtsuMethod			= false;
bool bThresholding			= false;

int iSliderLog_c		= 50;
int iSliderExp_c		= 50;
int iSliderC_S_m		= 127;
int iSliderC_S_E		= 5;
int Gradiente_a			= 255;
int Laplaciano_a		= 255;
int HighBoost_a			= 0;
int LowPassBW_Do		= 127;
int LowPassBW_n			= 1;
int Thresholding_v		= 3;
int Thresholding_k		= 50;

// Metodos GUI
void CrearVentana(const String, int move_x, int move_y, int size_x, int size_y);

// Metodos Transformaciones
Mat Gris				(Mat Imagen);
Mat Negativo			(Mat Imagen);
Mat Logaritmo			(Mat Imagen, double c);
Mat Exponencial			(Mat Imagen, double c);
Mat Contrast_Streching	(Mat Imagen, int m, int E);
Mat Promedio_3x3		(Mat Imagen);
Mat Promedio_5x5		(Mat Imagen);
Mat Gradiente			(Mat Imagen, int A);
Mat Laplaciano			(Mat Imagen, int A);
Mat HighBoost			(Mat Imagen, int A);
Mat LowPassButterworth	(Mat Imagen, int Do, int n);
Mat HighPassGaussian	(Mat Imagen);

// Segmentación
Mat Histogram(Mat Imagen);
Mat GlobalThresholding(Mat Imagen);
Mat OtsuMethod(Mat Imagen);
Mat Thresholding(Mat Imagen, int n, double k);
Mat ThresholdingHistogram(Mat Imagen, int n, double k);

int main(int argc, const char** argv)
{
	// Elige la imagen que se va a cargar
	String imageName("../Imagenes/Optimized-DJI_0212.jpg");				

	if (argc > 1)	imageName = argv[1];

	// Lee la imagen y la guarda en imagen
	Mat imagen = imread(imageName, IMREAD_COLOR);			

	// Verifica que la imagen se cargará correctamente
	if (imagen.empty())										
	{
		cout << "No se pudo cargar la imagen. ";
		system("pause");
		return -1;
	}

	cout << "Presiona una tecla para actualizar las imagenes, para salir presiona ESC. ";

	//////////////////////////////////////////////////////////
	//  Crea la ventana donde se van a mostrar cada imagen	//
	//////////////////////////////////////////////////////////

	CrearVentana("Control"	, 0		, 0	, 250			, 1000			);
	CrearVentana("Normal"	, 500	, 0	, imagen.cols	, imagen.rows	);

	imshow("Normal", imagen);

	//////////////////////////////////////////////////////
	//  Crea los Trackbar en sus respectivas ventanas	//
	//////////////////////////////////////////////////////

	createTrackbar("Gris"			, "Control", &iGris					, 1);
	createTrackbar("Negativo"		, "Control", &iNegativo				, 1);
	createTrackbar("Log"			, "Control", &iLogaritmo			, 1);
	createTrackbar("Exp"			, "Control", &iExponencial			, 1);
	createTrackbar("Prom 3x3"		, "Control", &iPromedio_3x3			, 1);
	createTrackbar("Prom 5x5"		, "Control", &iPromedio_5x5			, 1);
	createTrackbar("C Streching"	, "Control", &iContrast_Streching	, 1);
	createTrackbar("Gradiente"		, "Control", &iGradiente			, 1);
	createTrackbar("Laplaciano"		, "Control", &iLaplaciano			, 1);
	createTrackbar("High Boost"		, "Control", &iHighBoost			, 1);
	createTrackbar("Butterworth"	, "Control", &iLowPassButterworth	, 1);
	createTrackbar("Gaussian"		, "Control", &iHighPassGaussian		, 1);
	createTrackbar("G Thresh"		, "Control", &iGlobalThresholding	, 1);
	createTrackbar("OTSU"			, "Control", &iOtsuMethod			, 1);
	createTrackbar("Thresh"			, "Control", &iThresholding			, 1);

	while(1)
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	Obtene los valores de los Trackbar, guarda los valores en la variable iSlider que se usó en la creación del Trackbar  	//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		getTrackbarPos("Gris"		, "Control");
		getTrackbarPos("Negativo"	, "Control");
		getTrackbarPos("Log"		, "Control");
		getTrackbarPos("Exp"		, "Control");
		getTrackbarPos("Prom 3x3"	, "Control");
		getTrackbarPos("Prom 5x5"	, "Control");
		getTrackbarPos("C Streching", "Control");
		getTrackbarPos("Gradiente"	, "Control");
		getTrackbarPos("Laplaciano"	, "Control");
		getTrackbarPos("High Boost"	, "Control");
		getTrackbarPos("Butterworth", "Control");
		getTrackbarPos("Gaussian"	, "Control");
		getTrackbarPos("G Thresh"	, "Control");
		getTrackbarPos("OTSU"		, "Control");
		getTrackbarPos("Thresh"		, "Control");

		getTrackbarPos("c", "Logaritmo");
		getTrackbarPos("c", "Exponencial");
		getTrackbarPos("m", "Contrast_Streching");
		getTrackbarPos("E", "Contrast_Streching");
		getTrackbarPos("a", "Gradiente");

		getTrackbarPos("a", "Laplaciano");
		getTrackbarPos("a", "HighBoost");
		getTrackbarPos("Do", "LowPassButterworth");
		getTrackbarPos("n", "LowPassButterworth");
		getTrackbarPos("n", "Thresholding");
		getTrackbarPos("k", "Thresholding");

		//////////////////////////////////////////////
		//	Crea y destruye ventanas de cada imagen	//
		//////////////////////////////////////////////


		if (iGris && !bGris)
		{
			CrearVentana("Gris", 250, 0, imagen.cols, imagen.rows);
			imshow("Gris", Gris(imagen));
			bGris = true;
		}
		else if (iGris)
		{
			imshow("Gris", Gris(imagen));			
		}
		else
		{
			destroyWindow("Gris");
			bGris = false;
		}
		

		if (iNegativo && !bNegativo)
		{
			CrearVentana("Negativo", 250, 0, imagen.cols, imagen.rows);
			imshow("Negativo", Negativo(imagen));
			bNegativo = true;
		}
		else if (iNegativo)
		{
			imshow("Negativo", Negativo(imagen));			
		}
		else
		{
			destroyWindow("Negativo");
			bNegativo = false;
		}


		if (iLogaritmo &&!bLogaritmo)
		{
			CrearVentana("Logaritmo", 250, 0, imagen.cols, imagen.rows+45);
			createTrackbar("c", "Logaritmo", &iSliderLog_c, 100);
			imshow("Logaritmo", Logaritmo(imagen, iSliderLog_c*1.05886));
			bLogaritmo = true;
		}
		else if (iLogaritmo)
		{
			imshow("Logaritmo", Logaritmo(imagen, iSliderLog_c*1.05886));
		}
		else
		{
			destroyWindow("Logaritmo");
			bLogaritmo = false;
		}


		if (iExponencial &&!bExponencial)
		{
			CrearVentana("Exponencial", 250, 0, imagen.cols, imagen.rows+45);
			createTrackbar("c", "Exponencial", &iSliderExp_c, 100);
			imshow("Exponencial", Exponencial(imagen, iSliderExp_c*0.01645469371));
			bExponencial = true;
		}
		else if (iExponencial)
		{
			imshow("Exponencial", Exponencial(imagen, iSliderExp_c*0.01645469371));
		}
		else
		{
			destroyWindow("Exponencial");
			bExponencial = false;
		}


		if (iContrast_Streching && !bContrast_Streching)
		{
			CrearVentana("Contrast_Streching", 250, 0, imagen.cols, imagen.rows + 90);
			createTrackbar("m", "Contrast_Streching", &iSliderC_S_m, 256);
			createTrackbar("E", "Contrast_Streching", &iSliderC_S_E, 10);
			imshow("Contrast_Streching", Contrast_Streching(imagen, iSliderC_S_m, iSliderC_S_E));
			bContrast_Streching = true;
		}
		else if (iContrast_Streching)
		{
			imshow("Contrast_Streching", Contrast_Streching(imagen, iSliderC_S_m, iSliderC_S_E));
		}
		else
		{
			destroyWindow("Contrast_Streching");
			bContrast_Streching = false;
		}


		if (iPromedio_3x3 && !bPromedio_3x3)
		{
			CrearVentana("3x3 Promedio", 250, 0, imagen.cols, imagen.rows);
			imshow("3x3 Promedio", Promedio_3x3(imagen));
			bPromedio_3x3 = true;
		}
		else if (iPromedio_3x3)
		{
			imshow("3x3 Promedio", Promedio_3x3(imagen));
		}
		else
		{
			destroyWindow("3x3 Promedio");
			bPromedio_3x3 = false;
		}


		if (iPromedio_5x5 && !bPromedio_5x5)
		{
			CrearVentana("5x5 Promedio", 250, 0, imagen.cols, imagen.rows);
			imshow("5x5 Promedio", Promedio_5x5(imagen));
			bPromedio_5x5 = true;
		}
		else if (iPromedio_5x5)
		{
			imshow("5x5 Promedio", Promedio_5x5(imagen));
		}
		else
		{
			destroyWindow("5x5 Promedio");
			bPromedio_5x5 = false;
		}


		if (iGradiente && !bGradiente)
		{
			CrearVentana("Gradiente", 250, 0, imagen.cols, imagen.rows + 45);
			createTrackbar("a", "Gradiente", &Gradiente_a, 512);
			imshow("Gradiente", Gradiente(imagen, Gradiente_a));
			bGradiente = true;
		}
		else if (iGradiente)
		{
			imshow("Gradiente", Gradiente(imagen, Gradiente_a));
		}
		else
		{
			destroyWindow("Gradiente");
			bGradiente = false;
		}


		if (iLaplaciano && !bLaplaciano)
		{
			CrearVentana("Laplaciano", 250, 0, imagen.cols, imagen.rows + 45);
			createTrackbar("a", "Laplaciano", &Laplaciano_a, 512);
			imshow("Laplaciano", Laplaciano(imagen, Laplaciano_a));
			bLaplaciano = true;
		}
		else if (iLaplaciano)
		{
			imshow("Laplaciano", Laplaciano(imagen, Laplaciano_a));
		}
		else
		{
			destroyWindow("Laplaciano");
			bLaplaciano = false;
		}


		if (iHighBoost && !bHighBoost)
		{
			CrearVentana("HighBoost", 250, 0, imagen.cols, imagen.rows + 45);
			createTrackbar("a", "HighBoost", &HighBoost_a, 10);
			imshow("HighBoost", HighBoost(imagen, HighBoost_a));
			bHighBoost = true;
		}
		else if (iHighBoost)
		{
			imshow("HighBoost", HighBoost(imagen, HighBoost_a));
		}
		else
		{
			destroyWindow("HighBoost");
			bHighBoost = false;
		}


		if (iLowPassButterworth && !bLowPassButterworth)
		{
			CrearVentana("LowPassButterworth", 250, 0, imagen.cols, imagen.rows + 90);
			createTrackbar("Do", "LowPassButterworth", &LowPassBW_Do, 256);
			createTrackbar("n", "LowPassButterworth", &LowPassBW_n, 10);
			imshow("LowPassButterworth", LowPassButterworth(imagen, LowPassBW_Do, LowPassBW_n));
			bLowPassButterworth = true;
		}
		else if (iLowPassButterworth)
		{
			imshow("LowPassButterworth", LowPassButterworth(imagen, LowPassBW_Do, LowPassBW_n));
		}
		else
		{
			destroyWindow("LowPassButterworth");
			bLowPassButterworth = false;
		}


		if (iHighPassGaussian && !bHighPassGaussian)
		{
			CrearVentana("HighPassGaussian", 250, 0, imagen.cols, imagen.rows);
			imshow("HighPassGaussian", HighPassGaussian(imagen));
			bHighPassGaussian = true;
		}
		else if (iHighPassGaussian)
		{
			imshow("HighPassGaussian", HighPassGaussian(imagen));
		}
		else
		{
			destroyWindow("HighPassGaussian");
			bHighPassGaussian = false;
		}


		if (iGlobalThresholding && !bGlobalThresholding)
		{
			CrearVentana("GlobalThresholding", 250, 0, imagen.cols, imagen.rows);
			imshow("GlobalThresholding", GlobalThresholding(imagen));
			bGlobalThresholding = true;
		}
		else if (iGlobalThresholding)
		{
			imshow("GlobalThresholding", GlobalThresholding(imagen));
		}
		else
		{
			destroyWindow("GlobalThresholding");
			bGlobalThresholding = false;
		}


		if (iOtsuMethod && !bOtsuMethod)
		{
			CrearVentana("OTSU Method", 250, 0, imagen.cols, imagen.rows);
			imshow("OTSU Method", OtsuMethod(imagen));
			bOtsuMethod = true;
		}
		else if (iOtsuMethod)
		{
			imshow("OTSU Method", OtsuMethod(imagen));
		}
		else
		{
			destroyWindow("OTSU Method");
			bOtsuMethod = false;
		}


		if (iThresholding && !bThresholding)
		{
			CrearVentana("Thresholding", 250, 0, imagen.cols, imagen.rows+90);
			createTrackbar("v", "Thresholding", &Thresholding_v, 20);
			createTrackbar("k", "Thresholding", &Thresholding_k, 100);
			imshow("Thresholding", Thresholding(imagen, Thresholding_v, Thresholding_k*2.55));
			CrearVentana("HistogramThresholding ", 250+ imagen.cols, 0, imagen.cols, imagen.rows + 90);
			imshow("HistogramThresholding ", ThresholdingHistogram(imagen, Thresholding_v, Thresholding_k*2.55));
			bThresholding = true;
		}
		else if (iThresholding)
		{
			imshow("Thresholding", Thresholding(imagen, Thresholding_v, Thresholding_k*2.55));
			imshow("HistogramThresholding ", ThresholdingHistogram(imagen, Thresholding_v, Thresholding_k*2.55));
		}
		else
		{
			destroyWindow("Thresholding");
			destroyWindow("HistogramThresholding ");
			bThresholding = false;
		}


		// El programma se detiene hasta que el usuario presione una tecla, si presiona Esc el programa se cierra

		if ((char)waitKey(0) == 27)
		{
			cout << "Cerrando...\n";
			destroyAllWindows();
			break;
		}	
	}

	return 0; 	
}

void CrearVentana(const String NombreVentana, int move_x, int move_y, int size_x, int size_y)
{
	// Crea una ventana
	namedWindow(NombreVentana, WINDOW_NORMAL);
	// Mueve la ventana a la posición move_x, move_y
	moveWindow(NombreVentana, move_x, move_y);
	// Mueve la ventana a la posición size_x, size_y
	resizeWindow(NombreVentana, size_x, size_y);
}
Mat Gris(Mat Imagen)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//	Para obtener la escala de grises de una imagen se usaran las constantes NTSC de cada canal	//
	//	Ejemplo Gris = R * 0.2989 + G * 0.5870 + B * 0.1140;										//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	Mat		mGris = Imagen.clone();
	double	dGris;
	Vec3b	vColor;

	for (int y = 0; y < mGris.rows; y++)
	{
		for (int x = 0; x < mGris.cols; x++)
		{
			// get pixel
			vColor = mGris.at<Vec3b>(Point(x, y));
			dGris = vColor[0] * 0.2989 + vColor[1] * 0.5870 + vColor[2] * 0.1140;
			
			vColor[0] = (uint8_t) dGris;
			vColor[1] = (uint8_t) dGris;
			vColor[2] = (uint8_t) dGris;
					
			// set pixel
			mGris.at<Vec3b>(Point(x, y)) = vColor;
		}
	}

	return mGris;
}
Mat Negativo(Mat Imagen)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	El negativo de una imagen se obtiene de cuando al valor máximo del canal de un pixel se le resta el valor del canal actual.	//
	//	Ejemplo Rojo = 255 - Rojo																									//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat		mNegativo = Imagen.clone();
	Vec3b	vColor;

	for (int y = 0; y < mNegativo.rows; y++)
	{
		for (int x = 0; x < mNegativo.cols; x++)
		{
			// get pixel
			vColor = mNegativo.at<Vec3b>(Point(x, y));

			vColor[0] = 255 - vColor[0];
			vColor[1] = 255 - vColor[1];
			vColor[2] = 255 - vColor[2];
			
			// set pixel
			mNegativo.at<Vec3b>(Point(x, y)) = vColor;
		}
	}

	return mNegativo;
}
Mat Logaritmo(Mat Imagen, double c)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	La transformación logarítmica se representa con la función s = c * log10(r + 1), donde c es una constante con rango de 0 a 105.886458, el cual surge de la premisa	//
	//	de que el valor máximo del canal de un pixel es de 255, por lo tanto 255 = c* log10(255+1) y si log10(255+1) = 2.40654018, entonces c = 255/2.40654018 = 105.886458	//									//																											//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat		mLogaritmo = Gris(Imagen).clone();
	Vec3b	vColor;

	for (int y = 0; y < mLogaritmo.rows; y++)
	{
		for (int x = 0; x < mLogaritmo.cols; x++)
		{
			// get pixel
			vColor = mLogaritmo.at<Vec3b>(Point(x, y));

			vColor[0] = (uint8_t) round( c *  log10(1 + vColor[0]));
			vColor[1] = (uint8_t) round( c *  log10(1 + vColor[1]));
			vColor[2] = (uint8_t) round( c *  log10(1 + vColor[2]));

			// set pixel
			mLogaritmo.at<Vec3b>(Point(x, y)) = vColor;
		}
	}

	return mLogaritmo;
}
Mat Exponencial(Mat Imagen, double c)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	La transformación exponencial se representa con la función s = c * (b^r - 1), donde c es una constante con rango de 0 a 1.645469371, el cual surge de la premisa			//
	//	de que el valor máximo del canal de un pixel es de 255, por lo tanto 255 = c * (b^255 - 1) y si (1.02^255 - 1) = 154.9709794, entonces c = 154.9709794/255 = 1.645469371	//									//																											//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat		mExponencial = Gris(Imagen).clone();
	Vec3b	vColor;

	for (int y = 0; y < mExponencial.rows; y++)
	{	
		for (int x = 0; x < mExponencial.cols; x++)
		{
			// get pixel
			vColor = mExponencial.at<Vec3b>(Point(x, y));
			
			vColor[0] = (uint8_t) round( c * (pow(1.02, vColor[0]) - 1));
			vColor[1] = (uint8_t) round( c * (pow(1.02, vColor[1]) - 1));
			vColor[2] = (uint8_t) round( c * (pow(1.02, vColor[2]) - 1));

			// set pixel
			mExponencial.at<Vec3b>(Point(x, y)) = vColor;
		}
	}

	return mExponencial;
}
Mat Contrast_Streching(Mat Imagen, int m, int E)
{	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	La transformación contrast stretching se representa con la función s = 1 / (1 + (m / r)^E), donde mr es la intensidad de la imagen, m es el punto a partir de la cual se va a	//
	//	convertir de claro a oscuro, E es la pendiente que se va a tomar. El resultado de la función dara un valor de 0 a 1, por lo que es necesario multiplicar el resultado por 255	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat		mC_S= Gris(Imagen).clone();
	Vec3b	vColor;

	for (int y = 0; y < mC_S.rows; y++)
	{
		for (int x = 0; x < mC_S.cols; x++)
		{
			// get pixel
			vColor = mC_S.at<Vec3b>(Point(x, y));

			if (vColor[0] == 0) vColor[0] = 1;
			
			vColor[0] = (uint8_t)round(255 * (1 / (1 + (pow(m / vColor[0], E)))));
			vColor[1] = vColor[0];
			vColor[2] = vColor[0];

			// set pixel
			mC_S.at<Vec3b>(Point(x, y)) = vColor;
		}
	}

	return mC_S;
}
Mat Promedio_3x3(Mat Imagen)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	El filtro promediado móvil con máscara de 3x3 se obtiene de sacar el promedio de la intensidad de los pixeles vecinos del pixel	que se esta trabajando,				//	
	//	como la máscara es de 3x3 se tomaran en cuenta 9 pixeles. En el caso de los extremos de la imagen donde no hay pixeles vecinos en todos los lados se promediara		// 
	//	solo con los pixeles disponibles.																																	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat		mProm_3x3		= Gris(Imagen.clone());
	Mat		mProm_3x3_Final = Gris(Imagen.clone());
	double	dProm_3x3;
	int     iCount;
	Vec3b	vColor;

	for (int y = 0; y < mProm_3x3.rows; y++)
	{
		for (int x = 0; x < mProm_3x3.cols; x++)
		{
			dProm_3x3 = 0;
			iCount	  = 0;

			for (int xx = x; xx < x + 3; xx++)
			{
				for (int yy = y; yy < y + 3; yy++)
				{
					if (xx - 1 >= 0 && 
						yy - 1 >= 0 &&
						xx - 1 <= mProm_3x3.cols - 1 &&
						yy - 1 <= mProm_3x3.rows - 1)
					{	
						// get pixel
						vColor = mProm_3x3.at<Vec3b>(Point(xx-1, yy-1));

						dProm_3x3 = dProm_3x3 + vColor[0];
						iCount++;
					}
				}
			}

			dProm_3x3 = dProm_3x3 / iCount;

			vColor[0] = (uint8_t) round(dProm_3x3);
			vColor[1] = (uint8_t) round(dProm_3x3);
			vColor[2] = (uint8_t) round(dProm_3x3);

			// set pixel
			mProm_3x3_Final.at<Vec3b>(Point(x, y)) = vColor;
		}
	}

	return mProm_3x3_Final;
}
Mat Promedio_5x5(Mat Imagen)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	El filtro promediado móvil con máscara de 5x5 se obtiene de sacar el promedio de la intensidad de los pixeles vecinos del pixel	que se esta trabajando,				//	
	//	como la máscara es de 5x5 se tomaran en cuenta 9 pixeles. En el caso de los extremos de la imagen donde no hay pixeles vecinos en todos los lados se promediara		// 
	//	solo con los pixeles disponibles.																																	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat		mProm_5x5		= Gris(Imagen.clone());
	Mat		mProm_5x5_Final = Gris(Imagen.clone());
	double	dProm_5x5;
	int     iCount;
	Vec3b	vColor;

	for (int y = 0; y < mProm_5x5.rows; y++)
	{
		for (int x = 0; x < mProm_5x5.cols; x++)
		{
			dProm_5x5 = 0;
			iCount	  = 0;

			for (int xx = x; xx < x + 5; xx++)
			{
				for (int yy = y; yy < y + 5; yy++)
				{
					if (xx - 2 >= 0 &&
						yy - 2 >= 0 &&
						xx - 2 <= mProm_5x5.cols - 2 &&
						yy - 2 <= mProm_5x5.rows - 2)
					{
						// get pixel
						vColor = mProm_5x5.at<Vec3b>(Point(xx - 2, yy - 2));

						dProm_5x5 = dProm_5x5 + vColor[0];
						iCount++;
					}
				}
			}

			dProm_5x5 = dProm_5x5 / iCount;

			vColor[0] = (uint8_t) round(dProm_5x5);
			vColor[1] = (uint8_t) round(dProm_5x5);
			vColor[2] = (uint8_t) round(dProm_5x5);

			// set pixel
			mProm_5x5_Final.at<Vec3b>(Point(x, y)) = vColor;
		}
	}

	return mProm_5x5_Final;
}
Mat Gradiente(Mat Imagen, int A)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	El filtro espacial gradiente se obtiene con al siguiente función f = 1/2 Gx + 1/2 Gy, donde Gx es un vector -1 0 1 y Gy es el mismo vector pero d emanera vertical	//	
	//	para obtener el resultado se usa la convolución y los vectores como máscaras. Esta operación se hace en cada uno de los pixeles de la imagen.						// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat		mGradiente			= Gris(Imagen.clone());
	Mat		mGradiente_Final	= Gris(Imagen.clone());

	double	dGradienteX;
	double	dGradienteY;
	double	dGradiente;
	Vec3b	vColor;
	int    iKernel[3] = { -1 , 0 , 1 };

	for (int x = 0; x < mGradiente.cols; x++)
	{
		for (int y = 0; y < mGradiente.rows; y++)
		{
			dGradienteX = 0;
			dGradienteY = 0;
			dGradiente  = 0;

			for (int xx = x; xx < x + 3; xx++)
			{
				if (xx - 1 >= 0 && xx - 1 <= mGradiente.cols - 1)
				{
					// get pixel
					vColor = mGradiente.at<Vec3b>(Point(xx - 1, y));

					dGradienteX += (iKernel[xx - x] * vColor[0]);
				}		
			}

			for (int yy = y; yy < y + 3; yy++)
			{
				if (yy - 1 >= 0 && yy - 1 <= mGradiente.rows - 1)
				{
					// get pixel
					vColor = mGradiente.at<Vec3b>(Point(x , yy - 1));

					dGradienteY += (iKernel[yy - y] * vColor[0]);
				}
			}

			dGradiente = (dGradienteX *.5) + (dGradienteY *.5) - 255 + A;

			if (dGradiente > 255)	dGradiente = 255;
			if (dGradiente < 0)		dGradiente = 0;

			vColor[0] = (uint8_t) round(dGradiente);
			vColor[1] = (uint8_t) round(dGradiente);
			vColor[2] = (uint8_t) round(dGradiente);

			// set pixel
			mGradiente_Final.at<Vec3b>(Point(x, y)) = vColor;
		}
	}

	return mGradiente_Final;
}
Mat Laplaciano(Mat Imagen, int A)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	El filtro espacial laplaciano es el resultado de la convolución usando la máscara -1  -1  -1, la operación se debe hacer en cada uno de los pixeles	//
	//																	                  -1   8  -1														//
	//																					  -1  -1  -1														// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat		mLaplaciano			= Gris(Imagen.clone());
	Mat		mLaplaciano_Final	= Gris(Imagen.clone());
	double	dLaplaciano;
	Vec3b	vColor;

	int    iKernel[3][3] = { -1 , -1 , -1 ,
						     -1 ,  8 , -1 ,
							 -1 , -1 , -1 };

	for (int x = 0; x < mLaplaciano.cols; x++)
	{
		for (int y = 0; y < mLaplaciano.rows; y++)
		{
			dLaplaciano = 0;

			for (int xx = x; xx < x + 3; xx++)
			{
				for (int yy = y; yy < y + 3; yy++)
				{
					if (xx - 1 >= 0 &&
						yy - 1 >= 0 &&
						xx - 1 <= mLaplaciano.cols - 1 &&
						yy - 1 <= mLaplaciano.rows - 1)
					{
						// get pixel
						vColor = mLaplaciano.at<Vec3b>(Point(xx - 1, yy - 1));

						dLaplaciano += (iKernel[xx-x][yy-y] * vColor[0]);
					}
				}
			}

			dLaplaciano += -255 + A;

			if (dLaplaciano > 255)	dLaplaciano = 255;
			if (dLaplaciano < 0)	dLaplaciano = 0;

			vColor[0] = (uint8_t) round(dLaplaciano);
			vColor[1] = (uint8_t) round(dLaplaciano);
			vColor[2] = (uint8_t) round(dLaplaciano);

			// set pixel
			mLaplaciano_Final.at<Vec3b>(Point(x, y)) = vColor;
		}
	}

	return mLaplaciano_Final;
}
Mat HighBoost(Mat Imagen, int A)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	El filtro espacial HighBost es el resultado de la convolución usando la máscara -1  -1  -1, la operación se debe hacer en cada uno de los pixeles	//
	//																	                  -1   8  -1														//
	//																					  -1  -1  -1														// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat		mHighBoost = Gris(Imagen.clone());
	Mat		mHighBoost_Final = Gris(Imagen.clone());
	double	dHighBoost;
	Vec3b	vColor;

	int    iKernel[3][3] = { -1 ,  -1 , -1 ,
							 -1 , A+8 , -1 ,
							 -1 ,  -1 , -1 };

	for (int x = 0; x < mHighBoost.cols; x++)
	{
		for (int y = 0; y < mHighBoost.rows; y++)
		{
			dHighBoost = 0;

			for (int xx = x; xx < x + 3; xx++)
			{
				for (int yy = y; yy < y + 3; yy++)
				{
					if (xx - 1 >= 0 &&
						yy - 1 >= 0 &&
						xx - 1 <= mHighBoost.cols - 1 &&
						yy - 1 <= mHighBoost.rows - 1)
					{
						// get pixel
						vColor = mHighBoost.at<Vec3b>(Point(xx - 1, yy - 1));

						dHighBoost += (iKernel[xx - x][yy - y] * vColor[0]);
					}
				}
			}

			if (dHighBoost > 255)	dHighBoost = 255;
			if (dHighBoost < 0)		dHighBoost = 0;

			vColor[0] = (uint8_t) round(dHighBoost);
			vColor[1] = (uint8_t) round(dHighBoost);
			vColor[2] = (uint8_t) round(dHighBoost);

			// set pixel
			mHighBoost_Final.at<Vec3b>(Point(x, y)) = vColor;
		}
	}

	return mHighBoost_Final;
}
Mat LowPassButterworth(Mat Imagen, int Do, int N) 
{
	//Mat		mHighBoost = Gris(Imagen.clone());
//	Mat_<float> mHighBoost1 = Imagen.clone();

	return Imagen;
}
Mat HighPassGaussian(Mat Imagen)
{
	return Imagen;
}
Mat Histogram(Mat Imagen)
{
	Mat	mImagen = Imagen.clone();
	
	Vec3b	vColor;

	int iCantidadPixeles = mImagen.rows * mImagen.cols;
	int iHistograma[256];
	int iMayor = 0;
	int iHistogramY = 0;
	
	// Inicializa el arreglo iHistograma a 0
	for (int x = 0; x < 256; x++)
	{
		iHistograma[x] = 0;
	}

	// Cuenta las intensidades en cada pixel
	for (int x = 0; x < mImagen.cols; x++)
	{
		for (int y = 0; y < mImagen.rows; y++)
		{
			// get pixel
			vColor = mImagen.at<Vec3b>(Point(x, y));
			iHistograma[vColor[0]]++;
		}
	}

	// Se obtiene  la intensidad que más se repite 
	for (int x = 0; x < 256; x++)
	{
		if (iHistograma[x] > iMayor)		iMayor = iHistograma[x];
	}

	iHistogramY = iMayor *1.01;

	Mat mHistograma(Size(256, iHistogramY), CV_8SC3);

	// Se dibuja el histograma
	for (int x = 0; x < 256; x++)
	{
		for (int y = iHistogramY-1 ; y > iHistogramY-1-iHistograma[x] ; y--)
		{
			vColor[0] = 255;
			vColor[1] = 255;
			vColor[2] = 255;
			mHistograma.at<Vec3b>(Point(x, y)) = vColor;
		}
	}

	return mHistograma;
}
Mat GlobalThresholding(Mat Imagen)
{
	Mat		mGlobalThresholding = Gris(Negativo(Imagen.clone()));
	Mat		mg = Gris(Imagen.clone());

	double  dMedia		= 0;
	int		iContador	= 0;
	double	dDelta		= 0;

	double  dMedia1		= 0;
	int		iContador1	= 0;
	double	dDelta1		= 0;

	Vec3b	vColor;

	for (int x = 0; x < mGlobalThresholding.cols; x++)
	{
		for (int y = 0; y < mGlobalThresholding.rows; y++)
		{
			// get pixel
			vColor = mGlobalThresholding.at<Vec3b>(Point(x, y));

			dMedia += vColor[0];
		}
	}

	dMedia /= mGlobalThresholding.cols * mGlobalThresholding.rows;
	dDelta	= dMedia;

	int		iCiclos = 0;

	while ((dDelta - dDelta1) > 0.001)
	{
		if (iCiclos > 0)	dDelta = dDelta1;

		dMedia		= 0;
		iContador	= 0;

		dMedia1		= 0;
		iContador1	= 0;

		for (int x = 0; x < mGlobalThresholding.cols; x++)
		{
			for (int y = 0; y < mGlobalThresholding.rows; y++)
			{
				// get pixel
				vColor = mGlobalThresholding.at<Vec3b>(Point(x, y));

				if (vColor[0] >= dDelta)
				{
					dMedia1 += vColor[0];
					iContador1++;
				}
				else
				{
					dMedia += vColor[0];
					iContador++;
				}
			}
		}

		dMedia1 /= iContador1;
		dMedia  /= iContador;

		dDelta1 = (dMedia1 + dMedia)/2;		
		iCiclos++;
	} 

	for (int x = 0; x < mGlobalThresholding.cols; x++)
	{
		for (int y = 0; y < mGlobalThresholding.rows; y++)
		{
			// get pixel
			vColor = mGlobalThresholding.at<Vec3b>(Point(x, y));

			if (vColor[0] >= dDelta)
			{
				vColor[0] = 255;
				vColor[1] = 255;
				vColor[2] = 255;
			}
			else
			{
				vColor[0] = 0;
				vColor[1] = 0;
				vColor[2] = 0;
			}
			// set pixel
			mg.at<Vec3b>(Point(x, y)) = vColor;
		}
	}
	
	return mg;
}
Mat OtsuMethod(Mat Imagen)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	El metodo Otsu consiste en segmentar la imagen a partir de un valor k, el cual se obtiene de calcular de la varianza más grande en las intensidades //
	//	de la imagen 0 - 255, dVarianza[k] = (((dmG * dPk) - dmk) * ((dmG * dPk) - dmk)) / (dPk * (1 - dPk))        										//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat		mOtsuMethod = Imagen.clone();

	int	   iHistograma[256];
	double dHistograma[256];
	double dSuma = 0;
	double dVarianza[256];
	double dVarianzaMayor = 0;
	int	   ikMayor = 0;
	double dPk = 0;
	double dmk = 0;
	double dmG = 0;

	Vec3b	vColor;

	//Limpia histograma
	for (int x = 0; x < 256; x++)
	{
		iHistograma[x] = 0;
		dHistograma[x] = 0;
		dVarianza[x]   = 0;
	}

	// Cuenta las intensidades en cada pixel
	for (int y = 0; y < mOtsuMethod.rows; y++)
	{
		for (int x = 0; x < mOtsuMethod.cols; x++)
		{
			// get pixel
			vColor = mOtsuMethod.at<Vec3b>(Point(x, y));
			iHistograma[vColor[0]]++;
		}
	}

	// Normaliza histograma
	for (int x = 0; x < 256; x++)
	{
		dHistograma[x] = (double)iHistograma[x] / (mOtsuMethod.cols*mOtsuMethod.rows);
		dSuma += dHistograma[x];
	}

	// Obtenemos mG
	for (int x = 0; x < 256; x++)
	{
		dmG += x * dHistograma[x];
	}

	for (int k = 0; k < 256; k++)
	{
		dPk = 0;
		dmk = 0;

		// Obtenemos P(k), m(k)
		for (int x = 0; x <= k; x++)
		{
			dPk += dHistograma[x];
			dmk += x * dHistograma[x];
		}
		// Obtenemos la varianza
		dVarianza[k] = (((dmG * dPk) - dmk) * ((dmG * dPk) - dmk)) / (dPk * (1 - dPk));

		// Obtiene la varianza y la K más grande 
		if (dVarianza[k] > dVarianzaMayor)
		{
			dVarianzaMayor = dVarianza[k];
			ikMayor = k;
		}
	}

	// Cuenta las intensidades en cada pixel
	for (int y = 0; y < mOtsuMethod.rows; y++)
	{
		for (int x = 0; x < mOtsuMethod.cols; x++)
		{
			// get pixel
			vColor = mOtsuMethod.at<Vec3b>(Point(x, y));

			// Si el promedio de las intensidades del mayor o igual al K se asigna la máxima intensidad
			if (vColor[0] >= ikMayor)
			{
				vColor[0] = 255;
				vColor[1] = 255;
				vColor[2] = 255;			
			}
			// Sino se asigna la minima intensidad
			else
			{
				vColor[0] = 0;
				vColor[1] = 0;
				vColor[2] = 0;
			}
			mOtsuMethod.at<Vec3b>(Point(x, y)) = vColor;
		}
	}

	return mOtsuMethod;
}
Mat Thresholding(Mat Imagen, int v, double k)
{	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Consiste en   //
	//	   										//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat		mThresholding		= Gris(Imagen.clone());
	Mat		mThresholdingLine(Size(Imagen.rows*Imagen.cols,1), CV_8SC3);

	Vec3b	vColor;

	int		iPixelActualX	= 0;
	int		iPixelActualY	= 0;
	int		iPixelActual	= 0;
	int		iContador		= 0;
	double	dPromedio		= 0;

	// Guarda la imagen original en el vector de 1 x el número de pixeles de la imagen original 

	for (int y = 0; y < Imagen.rows; y++)
	{
		for (int x = 0; x < Imagen.cols; x++)
		{
			// get pixel
			vColor = mThresholding.at <Vec3b>(Point(x, y));

			if (!(y % 2))		iPixelActualX = (y * Imagen.cols) + x;
			else				iPixelActualX = (y * Imagen.cols) + (Imagen.cols - 1 - x);

			mThresholdingLine.at <Vec3b> (Point(iPixelActualX, 0)) = vColor;
		}
	}

	// Obtiene el promediado movil de la imagen
	// Guarda la imagen original en el vector de 1 x el número de pixeles de la imagen original 

	for (int x = 0; x < Imagen.rows*Imagen.cols; x++)
	{	
		dPromedio = 0;
		iContador = 0;

		for (int z = 0; z < v; z++)
		{
			iPixelActual = x + z - (v / 2);

			if (!(iPixelActual < 0 || iPixelActual > mThresholdingLine.cols - 1))
			{
				// get pixel
				vColor = mThresholdingLine.at <Vec3b>(Point(iPixelActual, 0));

				dPromedio += vColor[0];
				iContador++;
			}
		}

		if (iContador)	dPromedio /= iContador;

		// Segmenta la imagen 

		if (dPromedio >= k)
		{
			vColor[0] = 255;
			vColor[1] = 255;
			vColor[2] = 255;
		}
		else
		{
			vColor[0] = 0;
			vColor[1] = 0;
			vColor[2] = 0;
		}

		iPixelActualY = x / Imagen.cols;
		
		if (!(iPixelActualY % 2))	iPixelActualX = (x % Imagen.cols);
		else						iPixelActualX = Imagen.cols - (x % Imagen.cols) - 1;

		mThresholding.at <Vec3b>(Point(iPixelActualX, iPixelActualY)) = vColor;
	}

	return mThresholding;
}
Mat ThresholdingHistogram(Mat Imagen, int v, double k)
{
	Mat		mThresholding = Gris(Imagen.clone());
	Mat		mThresholdingLine(Size(Imagen.rows*Imagen.cols, 1), CV_8SC3);

	Vec3b	vColor;

	int		iPixelActualX = 0;
	int		iPixelActualY = 0;
	int		iPixelActual = 0;
	int		iContador = 0;
	double	dPromedio = 0;

	// Guarda la imagen original en el vector de 1 x el número de pixeles de la imagen original 

	for (int y = 0; y < Imagen.rows; y++)
	{
		for (int x = 0; x < Imagen.cols; x++)
		{
			// get pixel
			vColor = mThresholding.at <Vec3b>(Point(x, y));

			if (!(y % 2))		iPixelActualX = (y * Imagen.cols) + x;
			else				iPixelActualX = (y * Imagen.cols) + (Imagen.cols - 1 - x);

			mThresholdingLine.at <Vec3b>(Point(iPixelActualX, 0)) = vColor;
		}
	}

	// Obtiene el promediado movil de la imagen
	// Guarda la imagen original en el vector de 1 x el número de pixeles de la imagen original 

	for (int x = 0; x < Imagen.rows*Imagen.cols; x++)
	{
		dPromedio = 0;
		iContador = 0;

		for (int z = 0; z < v; z++)
		{
			iPixelActual = x + z - (v / 2);

			if (!(iPixelActual < 0 || iPixelActual > mThresholdingLine.cols - 1))
			{
				// get pixel
				vColor = mThresholdingLine.at <Vec3b>(Point(iPixelActual, 0));

				dPromedio += vColor[0];
				iContador++;
			}
		}

		if (iContador)	dPromedio /= iContador;

		vColor[0] = (uint8_t) round(dPromedio);
		vColor[1] = (uint8_t) round(dPromedio);
		vColor[2] = (uint8_t) round(dPromedio);

		iPixelActualY = x / Imagen.cols;

		if (!(iPixelActualY % 2))	iPixelActualX = (x % Imagen.cols);
		else						iPixelActualX = Imagen.cols - (x % Imagen.cols) - 1;

		mThresholding.at <Vec3b>(Point(iPixelActualX, iPixelActualY)) = vColor;
	}

	return Histogram(mThresholding);
}


/*
Mat		mLowPassButterworth = Gris(Imagen).clone();
Vec3b	vColor;

//////////////////////////////////////////////////////////////
//	Obtenemos el tamaño optimo de la imagen y la expandimos	//
//////////////////////////////////////////////////////////////

Mat    mPadded;
int M = getOptimalDFTSize(mLowPassButterworth.rows);
int N = getOptimalDFTSize(mLowPassButterworth.cols);
copyMakeBorder(mLowPassButterworth, mPadded, 0, M - mLowPassButterworth.rows, 0, N - mLowPassButterworth.cols, BORDER_CONSTANT, Scalar::all(0)); //Permite expandir los bordes de la imagen

																																				 //////////////////////////////////////////////////////////////////
																																				 //	Creamos las matrices para los valores reales y complejos	//
																																				 //////////////////////////////////////////////////////////////////

Mat	mPlanes[] = { Mat_<float>(mPadded), Mat::zeros(mPadded.size(), CV_32F) };
Mat mComplexI;
merge(mPlanes, 2, mComplexI);

//////////////////////////////////////////////////////
//	Obtenemos la transformada discreta de Fourier	//
//////////////////////////////////////////////////////

dft(mComplexI, mComplexI);

//////////////////////////////////////////////
//	Creamos el filtro ButterWorth Low Pass	//
//////////////////////////////////////////////

Mat mDFT_Filter = mComplexI.clone();
Mat mTmp = Mat(mDFT_Filter.rows, mDFT_Filter.cols, CV_32F);
Point centre = Point(mDFT_Filter.rows / 2, mDFT_Filter.cols / 2);
double dRadius;

for (int x = 0; x < mDFT_Filter.cols; x++)
{
	for (int y = 0; y < mDFT_Filter.rows; y++)
	{
		dRadius = (double)sqrt(pow(x - centre.x, 2.0) + pow(y - centre.y, 2.0));
		mTmp.at<float>(x, y) = (float)(1 / (1 + pow((double)(dRadius / Do), (double)(2 * n))));
	}
}

Mat toMerge[] = { mTmp, mTmp };
merge(toMerge, 2, mDFT_Filter);

//////////////////////////////////////////////////
//	Aplicamos el filtro ButterWorth Low Pass	//
//////////////////////////////////////////////////




return mLowPassButterworth;*/