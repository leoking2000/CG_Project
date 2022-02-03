#pragma once
#include "Mesh.h"
#include "utilities/ObjLoader.h"

namespace GL
{
	Mesh Create(const ObjLoader::Mesh& obj);

	Mesh GenarateCube();

	Mesh GenarateSphere(u32 prec = 48);

	Mesh GenarateQuard(u32 repet = 1);
}