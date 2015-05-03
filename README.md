<p align="center">
<img src="https://lh3.googleusercontent.com/-yAc-j1QOZWA/VUWQFPia8lI/AAAAAAAAB2g/Jp76yAfJZd0/s640/sptriangulate.png"/>
</p>

# Cinder-Delaunay

Simple Delaunay triangulator for Cinder based on [Triangulate by Paul Bourke](http://paulbourke.net/papers/triangulate/) and [Gilles Dumoulin's C++ implementation](http://paulbourke.net/papers/triangulate/cpp.zip). Requires [Cinder glNext](https://github.com/cinder/Cinder/tree/glNext)

## Installation
Clone into your blocks repo and add the following references to your project:
* &lt;cinder_root&gt;\blocks\Cinder-Delaunay\src
* &lt;cinder_root&gt;\blocks\Cinder-Delaunay\src\Delaunay

## Use
Cinder-Delaunay has one main function, CiDelaunay::triangulate. Using it is as simple as:

```
TriMeshRef triangulatedPoints;

void DelaunayApp::setup()
{
  vector<vec3> pointsToTriangulate;
  
  //fill vector with points,
  //then call:
  triangulatedPoints = CiDelaunay::triangulate(pointsToTriangulate);
}

void DelaunayApp::draw()
{
  gl::enableWireframe();
  gl::draw(triangulatedPoints);
  
  //or whatever you want to do with it
}
```
Questions, comments, diatribes, feature requests, bugs'n'sugs? Send 'em to seth.gibson1@gmail.com. Also, I love PRs.


