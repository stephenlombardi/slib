all : ms3dview

ms3dview : ms3dview.cpp MS3DToMesh.cpp MS3DToMesh.h
	g++ -o ms3dview -I../../include -lGL -lglut ms3dview.cpp MS3DToMesh.cpp ../../src/TGA.cpp ../../src/drawableobject.cpp ../../src/extensions.cpp ../../src/matrixutils.cpp ../../src/OpenGLText.cpp ../../src/shader.cpp ../../src/BoneMatrixContainer.cpp ../../src/JointAnimation.cpp ../../src/MS3D.cpp

clean :
	rm ms3dview
