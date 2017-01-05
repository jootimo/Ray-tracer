#ifndef _OBJLOADER_H_
#define _OBJLOADER_H_

#include <string>
#include "Objects/TriangleMesh.h"

TriangleMesh load_obj(const std::string filename);

#endif