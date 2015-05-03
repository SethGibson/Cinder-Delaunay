#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Rand.h"
#include "CiDelaunay.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SpherePointsApp : public App
{
public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

private:
	void drawWorld(vec3 pCenter, vec3 pBounds, float pAxes);
	
	vector<vec3>		mSpherePoints;
	gl::BatchRef		mBatch;
	CameraPersp			mCamera;
	CameraUi			mCamUI;

	TriMeshRef			mTris;
};

int S_NUM_PTS = 1000;

void SpherePointsApp::setup()
{
	getWindow()->setSize(1280, 720);
	
	for (int i = 0; i < S_NUM_PTS; ++i)
	{
		float z = randFloat(-1.f, 1.f);
		float t = randFloat(0, 2.0f*M_PI);
		float r = math<float>::sqrt(1.0f - z*z);
		float x = r*cos(t);
		float y = r*sin(t);
		if (x!=0&&y!=0&&z!=0)
			mSpherePoints.push_back(vec3(x*5.f, y*5.f, math<float>::abs(z*5.f)));

	}

	gl::VboRef cVbo = gl::Vbo::create(GL_ARRAY_BUFFER, mSpherePoints, GL_STATIC_DRAW);
	geom::BufferLayout cAttribs;
	cAttribs.append(geom::POSITION,3,0,0,0);
	gl::VboMeshRef cMesh = gl::VboMesh::create(S_NUM_PTS, GL_POINTS, { { cAttribs, cVbo } });

	string cVertShader =
		"uniform mat4 ciModelViewProjection;"
		"in vec4 ciPosition;"
		"void main(){"
		"gl_Position = ciModelViewProjection*ciPosition;}";
	string cFragShader =
		"out vec4 Color;"
		"void main(){"
		"Color = vec4(1,1,1,1);}";

	mBatch = gl::Batch::create(cMesh, gl::GlslProg::create(cVertShader, cFragShader));


	vec3 cEyePos(0, 0, 10);
	mCamera.setPerspective(45.0f, getWindowAspectRatio(), 0.1f, 100.0f);
	mCamera.lookAt(cEyePos, vec3(0), vec3(0, 1, 0));
	mCamera.setPivotDistance(length(cEyePos));
	mCamUI = CameraUi(&mCamera, getWindow());

	mTris = CiDelaunay::triangulate(mSpherePoints);
}

void SpherePointsApp::mouseDown( MouseEvent event )
{
}

void SpherePointsApp::update()
{
}

void SpherePointsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::setMatrices(mCamera);

	drawWorld(vec3(0), vec3(10), 2.0f);
	
	gl::color(Color(0, 0.5f, 0.8f));
	gl::enableWireframe();
	gl::draw(*mTris);
	gl::disableWireframe();

	gl::color(Color::white());
	gl::pointSize(3.0f);
	mBatch->draw();
}

void SpherePointsApp::drawWorld(vec3 pCenter, vec3 pBounds, float pAxes)
{
	gl::pushMatrices();
	gl::translate(pCenter);
	gl::color(Color::white());
	gl::drawStrokedCube(vec3(0), pBounds);

	gl::color(Color(1, 0, 0));
	gl::drawLine(vec3(0), vec3(pAxes,0,0));
	gl::color(Color(0, 1, 0));
	gl::drawLine(vec3(0), vec3(0, pAxes, 0));
	gl::color(Color(0, 0, 1));
	gl::drawLine(vec3(0), vec3(0, 0, pAxes));
	gl::popMatrices();

}
CINDER_APP( SpherePointsApp, RendererGl )
