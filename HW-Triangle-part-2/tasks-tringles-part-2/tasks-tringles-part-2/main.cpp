#include <fstream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>  

#include "CRTRay.h"
#include "CRTTriangle.h"
#include "CRTVector.h"
#include "utils.h"

/// Output image resolution
static const int imageWidth = 1280;
static const int imageHeight = 720;

static const float aspectRatio = (float)imageWidth / (float)imageHeight;

static const int maxColorComponent = 255;

union RGBColor {
	struct { unsigned char r, g, b; };
	unsigned char rgb[3];
};

CRTRay generateCameraRay(const int rowIdx, const int colIdx) {
	// Find its center, based on the raster coordinates
	float x = colIdx + 0.5f;
	float y = rowIdx + 0.5f;
	// Convert raster coordinates to NDC space [0.0, 1.0]
	x /= imageWidth;
	y /= imageHeight;
	// Convert NDC coordinates to Screen space [-1.0, 1.0]
	x = (2.0f * x) - 1.0f;
	y = 1.0f - (2.0f * y);
	//Consider the aspect ratio
	x *= aspectRatio;
	// Normalaize ray direction vector
	CRTVector rayDir(CRTVector(x, y, -1.0f).normalize());
	// Return the ray with the calculated direction and origin
	return CRTRay(CRTVector(0, 0, 0), rayDir);
}

void generateNormalVectorsAndVectorSides(const std::vector<CRTTriangle>& triangles, std::vector<NormalsAndSides>& trianglesInfo) {
	for (auto& triangle : triangles) {
		trianglesInfo.push_back(NormalsAndSides(triangle));
	}
}

void solutionMultipleTriangle(const std::vector<CRTTriangle>& triangles, const char* fileName, const std::vector<RGBColor>& colors) {
	std::ofstream ppmFileStream(std::string(fileName) + ".ppm", std::ios::out | std::ios::binary);
	ppmFileStream << "P3\n";
	ppmFileStream << imageWidth << " " << imageHeight << std::endl;
	ppmFileStream << maxColorComponent << std::endl;
	RGBColor col{};

	// Generate triangles normal vectors : normalize(cross(E0, E1))
	std::vector<NormalsAndSides> trianglesInfo;
	generateNormalVectorsAndVectorSides(triangles, trianglesInfo);
	const size_t len = triangles.size();
	// At each pixel :
	for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
		for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
			col = RGBColor{ 0, 0, 0 };
			// Generate camera ray R : 3rd Lecture
			CRTRay cameraRay = generateCameraRay(rowIdx, colIdx);
			CRTVector bestP{ std::numeric_limits<float>::max(), std::numeric_limits<float>::max() , std::numeric_limits<float>::max() };
			for (size_t i = 0; i < len; i++) {
				const CRTTriangle tri = triangles[i];
				const NormalsAndSides info = trianglesInfo[i];
				// If R is not parallel to the triangle’s plane : dot(N, R) != 0
				if (!equals(cameraRay.direction.dot(info.triNormalVector), 0.0f)) {
					// If R is towards the triangle’s plane : dot(V0, N) < 0
					float rpDist = tri.v0.dot(info.triNormalVector);
					if (rpDist < 0) {
						// Find R - plane intersection point P : t = rpDist / rProj; p = t * rDir
						float rProj = info.triNormalVector.dot(cameraRay.direction);
						float t = rpDist / rProj;
						CRTVector p = cameraRay.origin + t * cameraRay.direction;
						//	Check if P is on the left of E0 : dot(N, cross(E0, V0P)) > 0
						CRTVector v0p = p - tri.v0;
						if (info.triNormalVector.dot(info.e0.cross(v0p)) > 0) {
							//	Check if P is on the left of E1 : dot(N, cross(E1, V1P)) > 0
							CRTVector v1p = p - tri.v1;
							if (info.triNormalVector.dot(info.e1.cross(v1p)) > 0) {
								//	Check if P is on the left of E2 : dot(N, cross(E2, V2P)) > 0
								CRTVector v2p = p - tri.v2;
								if (info.triNormalVector.dot(info.e2.cross(v2p)) > 0) {
									//	If P is on the left of the 3 edges, we have an intersection
									if (bestP.length() > p.length()) {
										bestP = p;
										col = colors[i];
									}
									else if (equals(bestP.length(), p.length())) {
										col.r = (col.r + colors[i].r) / 2;
										col.g = (col.g + colors[i].g) / 2;
										col.b = (col.b + colors[i].b) / 2;
									}
								}
							}
						}

					}
				}
			}
			ppmFileStream << (int)col.r << " " << (int)col.g << " " << (int)col.b << "\t";
		}
		ppmFileStream << std::endl;
	}
	ppmFileStream.close();
}


//void writeFileTask() {
//	std::ofstream ppmFileStream("task.ppm", std::ios::out | std::ios::binary);
//	ppmFileStream << "P3\n";
//	ppmFileStream << imageWidth << " " << imageHeight << std::endl;
//	ppmFileStream << maxColorComponent << std::endl;
//	union Color {
//		struct { unsigned char r, g, b; };
//		unsigned char rgb[3];
//	} col{};
//	for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
//		for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
//			col = problem1(rowIdx, colIdx);
//			ppmFileStream << col.r << " " << col.g << " " << col.b << "\t";
//		}
//		ppmFileStream << std::endl;
//	}
//
//	ppmFileStream.close();
//}

int main() {

	////Задача 1.
	////Генерирайте картинка с резолюция по ваш избор, за която, за всеки пиксел трябва да генерирате камера лъч(подобно на Домашно 3) 
	//// и да проверите дали ако се продължи този лъч ще се пресече с триъгълника :
	////
	//// CRTTriangle tri{
	////		CRTVector(-1.75, -1.75, -3),
	////		CRTVector(1.75, -1.75, -3),
	////		CRTVector(0, 1.75, -3)
	////	};
	////
	////Приемете че камерата се намира в(0, 0, 0) на координатната система, насочена е да “гледа” в посока - Z, 
	//// image plane - a  се на намира на разстояние 1 пред камера, т.е.центъра на image plane - a е в(0, 0, -1)
	////
	////За пресичане на лъч с триъгълник може да използвате следния алгоритъм(в псевдо код) :
	////
	////	Generate triangles normal vectors : normalize(cross(E0, E1))
	////	At each pixel :
	////Generate camera ray R : 3rd Lecture
	////If R is not parallel to the triangle’s plane : dot(N, R) != 0
	////If R is towards the triangle’s plane : dot(V0, N) < 0
	////	Find R - plane intersection point P : t = rpDist / rProj; p = t * rDir
	////	Check if P is on the left of E0 : dot(N, cross(E0, V0P)) > 0
	////	Check if P is on the left of E1 : dot(N, cross(E1, V1P)) > 0
	////	Check if P is on the left of E2 : dot(N, cross(E2, V2P)) > 0
	////	If P is on the left of the 3 edges, we have an intersection
	//CRTTriangle tri{
	//	CRTVector(-1.75, -1.75, -3),
	//	CRTVector(1.75, -1.75, -3),
	//	CRTVector(0, 1.75, -3)
	//};
	//solutionMultipleTriangle({ tri }, "originalTriangle", { RGBColor{ 0, 255, 0 } });


	////Задача 2.
	////
	////Използвайте друг триъгълник(с други координати, по ваш избор за върхове).
	//CRTTriangle tri1{
	//	CRTVector(-1.75, -1.75, -3),
	//	CRTVector(1.75, -1.75, -3),
	//	CRTVector(2, 1.75, -3)
	//};
	//solutionMultipleTriangle({ tri1 }, "myTriangle", { RGBColor{ 0, 255, 0 } });

	////Задача 3.
	////
	////Добавете втори триъгълник, при проверка за пресичане на лъча трябва да обходите всички триъгълници.
	//solutionMultipleTriangle({ tri1, tri }, "twoTriangles", { RGBColor{ 0, 255, 0 }, RGBColor{ 255, 0, 0 } });

	// 
	//Задача 4.
	//
	//Създайте проста 3D форма с няколко триъгълника като ветрило, пирамида, шестоъгълник, … 
	//При пресичане имайте предвид че трябва да вземете най - близкия триъгълник до началото на лъча!

	// taken from blender export
	std::vector<CRTVector> vertices = { 
			CRTVector(0, 0, -1),
			CRTVector(0.7236, -0.52572, -0.447215),
			CRTVector(-0.276385, -0.85064, -0.447215),
			CRTVector(-0.894425, 0, -0.447215),
			CRTVector(-0.276385, 0.85064, -0.447215),
			CRTVector(0.7236, 0.52572, -0.447215),
			CRTVector(0.276385, -0.85064, 0.447215),
			CRTVector(-0.7236, -0.52572, 0.447215),
			CRTVector(-0.7236, 0.52572, 0.447215),
			CRTVector(0.276385, 0.85064, 0.447215),
			CRTVector(0.894425, 0, 0.447215),
			CRTVector(0, 0, 1) };

	const std::vector<int> faces = { 0, 1, 2, 1, 0, 5, 0, 2, 3, 0, 3, 4, 0, 4, 5, 1, 5, 10, 2, 1, 6, 3, 2, 7, 4, 3, 8, 5,
	4, 9, 1, 10, 6, 2, 6, 7, 3, 7, 8, 4, 8, 9, 5, 9, 10, 6, 10, 11, 7, 6, 11, 8, 7, 11, 9, 8, 11, 10, 9, 11 };

	// translate from origin (0,0,0) to (0,0,-5)
	for (auto& vertice : vertices) {
		vertice = vertice - CRTVector(0, 0, -5);
	}

	std::vector<CRTTriangle> triangles;
	std::vector<RGBColor> colors;

	srand(time(NULL));
	for (size_t i = 0; i < faces.size(); i += 3) {
		triangles.push_back(CRTTriangle{ vertices[faces[i]], vertices[faces[i + 1]], vertices[faces[i + 2]] });
		colors.push_back(RGBColor{ (unsigned char)(rand() % maxColorComponent), (unsigned char)(rand() % maxColorComponent), (unsigned char)(rand() % maxColorComponent) });
	}

	solutionMultipleTriangle(triangles, "polygon", colors);
	return 0;
}
