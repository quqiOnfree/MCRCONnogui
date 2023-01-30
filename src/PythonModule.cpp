#include "PythonModule.h"

#include <iostream>
#include <format>
#include <Python.h>

namespace MCRCON
{
	PythonModule::PythonModule(MCRCON::GlobalVariable& v)
		: p_GlobalVariable(v)
	{
	}
}
