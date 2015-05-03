#ifndef __CI_DELAUNAY__
#define __CI_DELAUNAY__

#include <vector>
#include "cinder\Vector.h"
#include "cinder\CinderGlm.h"
#include "cinder\TriMesh.h"
#include "Delaunay.h"

using namespace std;
using namespace ci;


namespace CiDelaunay
{

	vec3				fromXYZ(XYZ pInput);
	XYZ					toXYZ(vec3 pInput);

	ivec2				fromIEDGE(IEDGE pInput);
	IEDGE				toIEDGE(ivec2 pInput);

	ivec3				fromITRIANGLE(ITRIANGLE pInput);
	ITRIANGLE			toITRIANGLE(ivec3 pInput);
		
	pair<bool, vec3>	isPointinCircumCircle(vec2 pPt, vec2 *pCP);
	TriMeshRef			triangulate(vector<vec3> pVertices);
};
#endif