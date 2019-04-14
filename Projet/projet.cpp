// projet.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <windows.h>

using namespace cv;
using namespace std;

//Fonction qui permet de convertir des objects string à wstring
std::wstring string_to_wstring(const std::string& s) {
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

//Procédure d'affichage de l'image initiale avec le qrcode entouré en rouge
void display(Mat &im, Mat &bbox) {
	int n = bbox.rows;
	for (int i = 0; i < n; i++)
	{
		line(im, Point2i(bbox.at<float>(i, 0), bbox.at<float>(i, 1)), Point2i(bbox.at<float>((i + 1) % n, 0), bbox.at<float>((i + 1) % n, 1)), Scalar(0, 0, 255), 3);
	}
	imshow("Résultat", im);
}

int main(int argc, char* argv[]) {
	// Lecture de l'image
	Mat inputImage;
	if (argc > 1)
		inputImage = imread(argv[1]);
	else
		inputImage = imread("../../../../Desktop/qrcode-lyon2.png", 1);

	//Instantiation de l'objet QRCodeDetector
	cv::QRCodeDetector qrDecoder = QRCodeDetector::QRCodeDetector();

	cv::Mat bbox, rectifiedImage;

	//Detection du QR code dans l'image et renvoi le quadrangle contenant le code.
	//une fois le QR code détecté ça le décode en image et returne le string sortie codé ou un string vide si le code ne peux être encodé
	std::string data = qrDecoder.detectAndDecode(inputImage, bbox, rectifiedImage);
	if (data.length() > 0)
	{
		cout << "Données décodées : " << data << endl;

		//conversion std::string -> std::wstring 
		std::wstring stemp = string_to_wstring(data);
		//conversion std::wstring -> LPCWSTR
		const LPCWSTR result = stemp.c_str();
		//ouvrir les données décodées dans le navigateur par défaut si c'est un lien sinon demande à l'utilisateur avec quel programme l'ouvrir
		ShellExecute(NULL, NULL, result, NULL, NULL, SW_SHOWNORMAL);

		display(inputImage, bbox);
		rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
		resize(rectifiedImage, rectifiedImage, Size(rectifiedImage.cols * 5, rectifiedImage.rows * 5));
		imshow("QRCode corrigé", rectifiedImage);

		waitKey(0);
	}
	else
		cout << "QRCode non détecté" << endl;
}

//Reference https://docs.opencv.org/4.0.0/de/dc3/classcv_1_1QRCodeDetector.html