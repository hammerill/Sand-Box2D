#pragma once

#if defined(Windows)
#include <python2.7/Python.h>
#elif defined(Linux)

// If you came here because of compile error sorry lol.
// If you aren't able to find where is your installed Python 2.7 located and to change this path here
// just disable PYTHON_TEST flag in CMake (and you DO know how to do it if you enabled it right?).
#include <python2.7/Python.h>

#elif defined(Vita)
#include <python2.7/Python.h>
#endif
