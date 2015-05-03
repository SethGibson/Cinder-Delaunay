#include "CiDelaunay.h"

////////////////////////////////////////////////////////////////////////
// Wrapper for Delaunay::CircumCircle
//   Params:
//		vec2	pP:		point to check
//		vec2	*pCP:	vec2[3] containing the points of the CircumCircle
//   Returns:
//		pair.first:		true if pP is inside pCP
//		pair.second:	x = CircumCircle center x
//						y = CircumCircle center y
//						z=CircumCircle radius
////////////////////////////////////////////////////////////////////////
pair<bool, vec3>
CiDelaunay::isPointinCircumCircle(vec2 pP, vec2 *pCP)
{

	double cCX, cCY, cCR;
	bool cIsInCircle = CircumCircle(pP.x, pP.y, pCP[0].x, pCP[0].y, pCP[1].x, pCP[1].y, pCP[2].x, pCP[2].y, cCX, cCY, cCR);

	return make_pair(cIsInCircle, vec3(cCX, cCY, cCR));

}
/*
///////////////////////////////////////////////////////////////////////////////
// Triangulate() :
//   Triangulation subroutine
//   Takes as input NV vertices in array pxyz
//   Returned is a list of ntri triangular faces in the array v
//   These triangles are arranged in a consistent clockwise order.
//   The triangle array 'v' should be malloced to 3 * nv
//   The vertex array pxyz must be big enough to hold 3 more points
//   The vertex array must be sorted in increasing x values say
//
//   qsort(p,nv,sizeof(XYZ),XYZCompare);
///////////////////////////////////////////////////////////////////////////////
int Triangulate(int nv, XYZ pxyz[], ITRIANGLE v[], int &ntri){
*/
TriMeshRef
CiDelaunay::triangulate(vector<vec3> pVertices)
{
	vector<XYZ>			cXYZVerts;
	vector<ITRIANGLE>	cTriangles;
	
	vector<vec3>		cVtxOutput;
	vector<uint32_t>	cIdxOutput;

	int cOutTriCount = -1;

	for (auto v = pVertices.begin(); v != pVertices.end();++v)
	{
		cXYZVerts.push_back(toXYZ(*v));
	}
	
	cTriangles.resize(cXYZVerts.size() * 3);
	qsort(cXYZVerts.data(), cXYZVerts.size(), sizeof(XYZ), XYZCompare);

	Triangulate(pVertices.size(), cXYZVerts.data(), &cTriangles[0], cOutTriCount);

	for (auto xyz = cXYZVerts.begin(); xyz != cXYZVerts.end(); ++xyz)
	{
		cVtxOutput.push_back(fromXYZ(*xyz));
	}

	for (auto tri = cTriangles.begin(); tri != cTriangles.end(); ++tri)
	{
		cIdxOutput.push_back(tri->p1);
		cIdxOutput.push_back(tri->p2);
		cIdxOutput.push_back(tri->p3);
	}

	TriMeshRef cTriangulatedMesh = TriMesh::create(TriMesh::Format().positions(3));
	cTriangulatedMesh->appendVertices(cVtxOutput.data(), cVtxOutput.size());
	cTriangulatedMesh->appendIndices(cIdxOutput.data(), cIdxOutput.size());
	
	return cTriangulatedMesh;
}

// Utilities

vec3 CiDelaunay::fromXYZ(XYZ pInput)
{ 
	return vec3(pInput.x, pInput.y, pInput.z);
}

XYZ CiDelaunay::toXYZ(vec3 pInput)
{
	XYZ ret;
	ret.x = pInput.x; ret.y = pInput.y; ret.z = pInput.z;
	return ret;
}

ivec2 CiDelaunay::fromIEDGE(IEDGE pInput)
{
	return ivec2(pInput.p1, pInput.p2);
}

IEDGE CiDelaunay::toIEDGE(ivec2 pInput)
{
	IEDGE ret;
	ret.p1 = pInput.x; ret.p2 = pInput.y;
	return ret;
}

ivec3 CiDelaunay::fromITRIANGLE(ITRIANGLE pInput)
{
	return ivec3(pInput.p1, pInput.p2, pInput.p3);
}

ITRIANGLE CiDelaunay::toITRIANGLE(ivec3 pInput)
{
	ITRIANGLE ret;
	ret.p1 = pInput.x; ret.p2 = pInput.y; ret.p3 = pInput.z;
	return ret;
}
