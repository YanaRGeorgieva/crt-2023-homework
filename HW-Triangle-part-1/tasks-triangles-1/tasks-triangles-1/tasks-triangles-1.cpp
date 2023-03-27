#include <fstream>
#include <cstdlib>
#include <cstdio>

struct CRTVector {
	float x, y, z;

	CRTVector(float x, float y, float z) : x(x), y(y), z(z) {}

	float length() const {
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	CRTVector normalize() const {
		float multiplier = 1.0f / this->length();
		return CRTVector{ x * multiplier, y * multiplier, z * multiplier };
	}

	void setComponents(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	CRTVector operator+ (const CRTVector& lhs) const {
		return CRTVector{ x + lhs.x, y + lhs.y, z + lhs.z };
	}

	CRTVector operator- (const CRTVector& lhs) const {
		return CRTVector{ x - lhs.x, y - lhs.y, z - lhs.z };
	}

	CRTVector operator* (const int scalar) const {
		return CRTVector{ x * scalar, y * scalar, z * scalar };
	}

	CRTVector operator* (const float scalar) const {
		return CRTVector{ x * scalar, y * scalar, z * scalar };
	}

	CRTVector operator* (const double scalar) const {
		return CRTVector{ x * (float)scalar, y * (float)scalar, z * (float)scalar };
	}

	CRTVector cross(const CRTVector& rhs) {
		return CRTVector{
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x
		};
	}
};

const int numberOFVertices = 3;
struct CRTTriangle {
	union {
		CRTVector verticesArr[numberOFVertices];
		struct { CRTVector v0, v1, v2; };
	};

	CRTTriangle(const CRTVector& x, const CRTVector& y, const CRTVector& z) :v0(x), v1(y), v2(z) {}

	CRTVector getNormalVector() const {
		CRTVector e0 = v0 - v1;
		CRTVector e1 = v2 - v1;
		return e0.cross(e1).normalize();
	}

	float getArea() const {
		/*Vector e0 = v1 - v0;
		Vector e1 = v2 - v0;
		Vector e2 = v2 - v1;
		float vs0 = e2.length();
		float vs1 = e1.length();
		float vs2 = e0.length();
		float s = (vs0 + vs1 + vs2) / 2;
		// Heron formula
		return sqrtf(s * (s - vs1) * (s - vs0) * (s - vs2));*/
		// or use that area of the triangle is 1/2 of area of the parallelogram formed  by e0 and e1
		CRTVector e0 = v0 - v1;
		CRTVector e1 = v2 - v1;
		return e0.cross(e1).length() / 2;
	}
};

int main()
{

	/*
		Example output
		A(3, -3, 1) x B(1.75, 3.5, 0) = (-3.5, 1.75, 15.75)
		A(3, -3, 1) x B(4, 9, 3) = (-18, -5, 39)
		area(parallelogram(A(3, -3, 1), B(4, 9, 3))) = length(AxB) = 43.2435
		area(parallelogram(A(3, -3, 1), B(-12, 12, -4))) = length(AxB) = 0
		trangle v0=Vector(-1.75, -1.75, -3), v1=Vector(1.75, -1.75, -3), v2=Vector(0, 1.75, -3): normal vector (0, 0, -1) and area 6.125
		trangle v0=Vector(0, 0, -1), v1=Vector(1, 0, 1), v2=Vector(-1, 0, 1): normal vector (0, 1, 0) and area 2
		trangle v0=Vector(0.56, 1.11, 1.23), v1=Vector(0.44, -2.368, -0.54), v2=Vector(-1.56, 0.15, -1.92): normal vector (-0.75642, -0.275748, 0.59312) and area 6.11862
	*/

	// Изчислете векторното произведение(AxB) между двата вектора А = (3.5, 0, 0) и B = (1.75, 3.5, 0)
	CRTVector A(3, -3, 1);
	CRTVector B(1.75, 3.5, 0);
	CRTVector crossAB(A.cross(B));
	printf("A(%g, %g, %g) x B(%g, %g, %g) = (%g, %g, %g)\n", A.x, A.y, A.z, B.x, B.y, B.z, crossAB.x, crossAB.y, crossAB.z);

	// Изчислете векторното произведение(AxB) между двата вектора А = (3, -3, 1) и B = (4, 9, 3)
	B.setComponents(4, 9, 3);
	crossAB = A.cross(B);
	printf("A(%g, %g, %g) x B(%g, %g, %g) = (%g, %g, %g)\n", A.x, A.y, A.z, B.x, B.y, B.z, crossAB.x, crossAB.y, crossAB.z);

	// Изчислете лицето на успоредника, който се формира с векторите А = (3, -3, 1) и B = (4, 9, 3)
	B.setComponents(4, 9, 3);
	printf("area(parallelogram(A(%g, %g, %g), B(%g, %g, %g))) = length(AxB) = %g\n", A.x, A.y, A.z, B.x, B.y, B.z, A.cross(B).length());

	// Изчислете лицето на успоредника, който се формира с векторите А = (3, -3, 1) и B = (-12, 12, -4)
	B.setComponents(-12, 12, -4);
	printf("area(parallelogram(A(%g, %g, %g), B(%g, %g, %g))) = length(AxB) = %g\n", A.x, A.y, A.z, B.x, B.y, B.z, A.cross(B).length());

	/*
		Намерете нормал вектор за триъгълник със следните върхове:
		A = (-1.75, -1.75, -3)
		B = (1.75, -1.75, -3)
		C = (0, 1.75, -3)
	*/
	CRTTriangle tri(CRTVector(-1.75, -1.75, -3), CRTVector(1.75, -1.75, -3), CRTVector(0, 1.75, -3));
	CRTVector normal(tri.getNormalVector());
	float area = tri.getArea();
	printf("trangle v0=Vector(%g, %g, %g), v1=Vector(%g, %g, %g), v2=Vector(%g, %g, %g): normal vector (%g, %g, %g) and area %g\n",
		tri.v0.x, tri.v0.y, tri.v0.z,
		tri.v1.x, tri.v1.y, tri.v1.z,
		tri.v2.x, tri.v2.y, tri.v2.z,
		normal.x, normal.y, normal.z, area);

	/*
		Намерете нормал вектор за триъгълник със следните върхове:
		A = (0, 0, -1)
		B = (1, 0, 1)
		C = (-1, 0, 1)
	*/
	CRTTriangle tri1(CRTVector(0, 0, -1), CRTVector(1, 0, 1), CRTVector(-1, 0, 1));
	CRTVector normal1(tri1.getNormalVector());
	float area1 = tri1.getArea();
	printf("trangle v0=Vector(%g, %g, %g), v1=Vector(%g, %g, %g), v2=Vector(%g, %g, %g): normal vector (%g, %g, %g) and area %g\n",
		tri1.v0.x, tri1.v0.y, tri1.v0.z,
		tri1.v1.x, tri1.v1.y, tri1.v1.z,
		tri1.v2.x, tri1.v2.y, tri1.v2.z,
		normal1.x, normal1.y, normal1.z, area1);


	/*
		Намерете нормал вектор за триъгълник със следните върхове:
		A = (0.56, 1.11, 1.23)
		B = (0.44, -2.368, -0.54)
		C = (-1.56, 0.15, -1.92)
	*/
	CRTTriangle tri2(CRTVector(0.56, 1.11, 1.23), CRTVector(0.44, -2.368, -0.54), CRTVector(-1.56, 0.15, -1.92));
	CRTVector normal2(tri2.getNormalVector());
	float area2 = tri2.getArea();
	printf("trangle v0=Vector(%g, %g, %g), v1=Vector(%g, %g, %g), v2=Vector(%g, %g, %g): normal vector (%g, %g, %g) and area %g\n",
		tri2.v0.x, tri2.v0.y, tri2.v0.z,
		tri2.v1.x, tri2.v1.y, tri2.v1.z,
		tri2.v2.x, tri2.v2.y, tri2.v2.z,
		normal2.x, normal2.y, normal2.z, area2);



}
