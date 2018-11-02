/*
* Programme qui affiche 5 images fractales produites par systèmes de fonctions itérées.
* \file   TD4.cpp
* \author Sofia Alvarez (1894016) et Mihaela Talpos (1894329)
* \date   30 octobre 2018
* Créé le 22 octobre 2018
*/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "ecrireimage.h"

using namespace std;

int tests();
int borneDansIntervalle(int valeur, int borneInf, int borneSup);
bool estDansIntervalle(int valeur, int borneInf, int borneSup);
void dessinePoint(Point2d coordonee, int intensite, Pixel image[tailleY][tailleX]);
double aleatoireZeroUn();
int aleatoireSelonDistribution(double prob[]);
void transformePoint(double& valeurX, double& valeurY, double transformation[]);
void calculerImage(string ficSource, string ficSortie);


int main()
{
	tests();
	calculerImage("Barnsley fern.txt", "IFS BarnsleyFern.bmp");
	calculerImage("Golden dragon agrandi.txt", "IFS Golden dragon agrandi.bmp");
	calculerImage("Golden dragon.txt", "IFS Golden dragon.bmp");
	calculerImage("Heighway dragon.txt", "IFS Heighway dragon.bmp");
	calculerImage("Sierpensky triangle.txt", "IFS Sierpensky triangle.bmp");
}

/*
*  Prend une valeur et vérifie si elle est dans l'intervalle donnée.
*  Sinon, elle retourne la borne la plus proche de la valeur.
*  \param [in] valeur, borneInf, borneSup
*  \return  Une valeur dans l'intervalle \p valeur, borneInf ou borneSup.
*/
int borneDansIntervalle(int valeur, int borneInf, int borneSup)
{
	if (valeur <= borneSup && valeur >= borneInf)
		return valeur;
	else if (valeur > borneSup) 
		return borneSup;
	else 
		return borneInf;
}

/*
*  Prend une valeur et vérifie si elle est dans l'intervalle donnée.
*  \param [in] valeur, borneInf, borneSup
*  \return  Vrai ou faux.
*/
bool estDansIntervalle(int valeur, int borneInf, int borneSup)
{
	if (valeur <= borneSup && valeur >= borneInf)
		return true;
	else
		return false;
}

/*
*  Prend une point et change l'intensité d'une image (tableau) à la coordonnée correspondante.
*  \param [in] coordonnee, intensite
*  \param [inout] image
*  \return void 
*/
void dessinePoint(Point2d coordonee, int intensite, Pixel image[tailleY][tailleX])
{
	if (estDansIntervalle(coordonee.x, 0, 499) && estDansIntervalle(coordonee.y, 0, 499)) {
		int nouvelleIntensite = image[(int)coordonee.y][(int)coordonee.x] - intensite;
		image[(int)coordonee.y][(int)coordonee.x] = borneDansIntervalle(nouvelleIntensite, 0, 255);
	}
}

/*
*  Génère une valeur aléatoire entre 0 et 1.
*  \return  Une valeur aléatoire entre 0 et 1.
*/
double aleatoireZeroUn()
{
	return rand() % RAND_MAX / (double)RAND_MAX;
}

/*
*  Vérifie où se trouve le nombre aléatoire de la fonction précédente
*  dans la distribution d'un tableau de probabilités cumulatives.
*  \param [in] prob
*  \return  L'indice de la position de la valeur
			immédiatement plus grande que le
			nombre aléatoire \p i.
*/
int aleatoireSelonDistribution(double prob[])
{
	double x = aleatoireZeroUn();
	int i = 0;

	while (1) {
		if (x <= prob[i])
			return i;
		i++;
	}
}

/*
*  Modifie une coordonnée donnée par transformation linéaire selon un tableau de transformation donné.
*  \param [in] transformation
*  \param [inout] valeurX, valeurY
*  \return  La coordonnée modifiée selon le tableau de transformation fourni \p valeurX, valeurY
*/
void transformePoint(double& valeurX, double& valeurY, double transformation[])
{
	double x = valeurX;
	double y = valeurY;
	
	valeurX = (transformation[0] * x) + (transformation[1] * y) + transformation[4];
	valeurY = (transformation[2] * x) + (transformation[3] * y) + transformation[5];
}

struct DataDuFichier
{
	int nPtsDepart, nTransfTotal, nTransfDebut, intensite;
	double transformationAffichage[6];
	double transfIFS[10][7];
};

/*
*  Produit une image fractale par systèmes de fonctions itérées.
*  \param [in] ficSource
*  \param [inout] ficSortie
*  \return void
*/
void calculerImage(string ficSource, string ficSortie)
{
	DataDuFichier imgSource;
	ifstream fichier(ficSource);

	//declaration du tableau de probabilités cumulatives en dehors de la boucle
	double prob[10];

	if (fichier.fail())
		cout << "Le fichier n'existe pas.";
	else {
		fichier >> imgSource.nPtsDepart >> imgSource.nTransfTotal >> imgSource.nTransfDebut >> imgSource.intensite;

		for (int i = 0; i < 6; i++)
			fichier >> imgSource.transformationAffichage[i];

		int i = 0;
		while (!ws(fichier).eof()) {
			for (int j = 0; j < 7; j++) {
				fichier >> imgSource.transfIFS[i][j];
				if (j == 6)
					prob[i] = (imgSource.transfIFS[i][j]) / 100.0;
			}
			i++;
		}
	}

	Pixel image[tailleX][tailleY];
	for (int i = 0; i < 500; i++) {
		for (int j = 0; j < 500; j++) {
			image[i][j] = 255;
		}
	}

	Point2d point;
	for (int i = 0; i < imgSource.nPtsDepart; i++) {
		point.x = aleatoireZeroUn();
		point.y = aleatoireZeroUn();

		for (int j = 0; j < imgSource.nTransfTotal; j++) {
			int indice = aleatoireSelonDistribution(prob);
			transformePoint(point.x, point.y, imgSource.transfIFS[indice]);
			if (j >= imgSource.nTransfDebut) {
				double x = point.x;
				double y = point.y;
				transformePoint(x, y, imgSource.transformationAffichage);
				dessinePoint({ x, y }, imgSource.intensite, image);
			}
		}
	}

	ecrireImage(image, ficSortie);
}


// Vérifie si les fonctions sont bien écrites.
int tests()
{
	//vérification de borneDansIntervalle
	cout << "Les valeurs bornees sont : ";
	for (int i = -1; i <= 4; i++) {
		int valeur = borneDansIntervalle(i, 1, 3);
		cout << valeur << " ";
	}
	cout << endl;

	//vérification de estDansIntervalle
	cout << "La valeur entree est dans l'intervalle : ";
	for (int i = -1; i <= 4; i++) {
		bool valeur = estDansIntervalle(i, 1, 3);
		cout << valeur << " ";
	}
	cout << endl;

	//vérification de dessinePoint
	Pixel image[tailleX][tailleY];
	for (int i = 0; i < 500; i++) {
		for (int j = 0; j < 500; j++) {
			image[i][j] = 255;
		}
	}

	dessinePoint({ 10, 10 }, 255, image);
	dessinePoint({ 20, 15 }, 100, image);
	dessinePoint({ 30, 20 }, 128, image);
	dessinePoint({ -1, 1 }, 255, image);
	dessinePoint({ 500, 1 }, 255, image);
	dessinePoint({ 1, -1 }, 255, image);
	dessinePoint({ 1, 500 }, 255, image);
	dessinePoint({ 30, 20 }, 128, image);

	ecrireImage(image, "imageTest.bmp");

	//vérification de aleatoireSelonDistribution et aleatoireZeroUn
	int valeurUn = 0;
	double prob[] = { 0.1, 0.35 , 1.0 };
	cout << "L'indice 1 est apparru ";
	for (int i = 0; i < 1000; i++) {
		int indice = aleatoireSelonDistribution(prob);
		if (indice == 1) {
			valeurUn++;
		}
	}
	cout << valeurUn << " fois." << endl;

	//verification de transformePoint
	double transformation[6] = { 0.5, -0.5, 0.25, 0.75, 2.0, 3.0 };
	double y = 0.7;
	double x = 0.2;

	transformePoint(x, y, transformation);
	cout << endl << "x: " << x << " y: " << y << endl;

	return 0;
}