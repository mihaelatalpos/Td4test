#pragma once
#include <string>
#include <fstream>
#include <cstdint>

using Pixel = uint8_t;

static constexpr unsigned tailleX = 500, tailleY = 500;

struct Point2d {
	double x, y;
};


inline bool ecrireImage(const Pixel image[tailleY][tailleX], const std::string& nomFichier)
{
	std::ofstream fichier(nomFichier, std::ios::binary);
	if (fichier.fail())
		return false;
	uint32_t tailleImageEnOctets = tailleX * tailleY;
	uint32_t resolutionImage = 72 * 10000/254;  // En pixels par mètre.
	static const char nombreMagique[] = { 'B', 'M' };
	uint32_t entete[3+10] = {
		0,0,0,
		40,
		tailleX, tailleY,
		0x080001, 0,
		tailleImageEnOctets,
		resolutionImage, resolutionImage
	};
	uint32_t tableCouleur[256];
	
	entete[2] = sizeof(nombreMagique) + sizeof(entete) + sizeof(tableCouleur);
	entete[0] = entete[2] + tailleImageEnOctets;
	
	for (unsigned i = 0; i < 256; i++)
		tableCouleur[i] = i * 0x010101;
	
	fichier.write(nombreMagique, sizeof(nombreMagique));
	fichier.write((char*)entete, sizeof(entete));
	fichier.write((char*)tableCouleur, sizeof(tableCouleur));
	fichier.write((char*)image, tailleImageEnOctets);
	return !fichier.fail();
}
