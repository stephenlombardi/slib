all : ms3dinfo

ms3dinfo : ms3dinfo.cpp MS3DToMesh.h MS3DToMesh.cpp ../../include/slib/BoneMatrixContainer.h ../../src/BoneMatrixContainer.cpp ../../include/slib/JointAnimation.h ../../src/JointAnimation.cpp ../../include/slib/MS3D.h ../../src/MS3D.cpp
	g++ -o ms3dinfo -I../../include/ ms3dinfo.cpp MS3DToMesh.cpp ../../src/BoneMatrixContainer.cpp ../../src/JointAnimation.cpp ../../src/MS3D.cpp

clean :
	rm ms3dinfo

