#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Shader.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"
#include "cinder/Rand.h"

#include "CiDelaunay.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicApp : public App
{
public:
	void setup() override;
	void update() override;
	void draw() override;

private:
	void drawWorld(vec3 pCenter, vec3 pBounds, float pScale);

	vector<vec3>	mPoints;

	CameraPersp		mCamera;
	CameraUi		mCamUI;

	gl::VboRef		mVbo;
	gl::VboMeshRef	mMesh;
	gl::BatchRef	mBatch;

	TriMeshRef		mTris;
};

int S_NUM_PTS = 1000;
void BasicApp::setup()
{
	getWindow()->setSize(1280, 720);
	


	vec3 cEyePos(0, 0, 10);
	mCamera.setPerspective(45.0f, getWindowAspectRatio(), 0.1f, 100.0f);
	mCamera.lookAt(cEyePos, vec3(0), vec3(0, 1, 0));
	mCamera.setPivotDistance(length(cEyePos));
	mCamUI = CameraUi(&mCamera, getWindow());

	for (int i = 0; i < S_NUM_PTS; ++i)
	{
		mPoints.push_back(vec3(randFloat(-5, 5), randFloat(-5, 5), randFloat(0, 2)));
	}

	geom::BufferLayout cLayout;
	cLayout.append(geom::POSITION, 3, 0, 0, 0);
	mVbo = gl::Vbo::create(GL_ARRAY_BUFFER, mPoints, GL_STATIC_DRAW);
	mMesh = gl::VboMesh::create(S_NUM_PTS, GL_POINTS, { { cLayout, mVbo } });
	
	string cVertShader =
		"uniform mat4 ciModelViewProjection;"
		"in vec4 ciPosition;"
		"void main()"
		"{ gl_Position = ciModelViewProjection*ciPosition; }";
	string cFragShader =
		"out vec4 Color;"
		"void main()"
		"{ Color = vec4(1,1,1,1); }";
		
	gl::GlslProgRef cShader = gl::GlslProg::create(cVertShader, cFragShader);
	mBatch = gl::Batch::create(mMesh, cShader);

	mTris = CiDelaunay::triangulate(mPoints);
	
}

void BasicApp::update()
{
}

void BasicApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::setMatrices(mCamera);

	drawWorld(vec3(0), vec3(10), 2.0f);
	gl::pointSize(4.0f);
	mBatch->draw();

	gl::color(Color(0, 1, 1));
	gl::enableWireframe();
	gl::draw(*mTris.get());


}

void BasicApp::drawWorld(vec3 pCenter, vec3 pBounds, float pScale)
{
	gl::color(Color::white());
	gl::drawStrokedCube(pCenter, pBounds);

	gl::color(Color(1, 0, 0));
	gl::drawLine(pCenter, vec3(pScale, 0, 0));
	gl::color(Color(0, 1, 0));
	gl::drawLine(pCenter, vec3(0, pScale, 0));
	gl::color(Color(0, 0, 1));
	gl::drawLine(pCenter, vec3(0, 0, pScale));
}

CINDER_APP( BasicApp, RendererGl )
