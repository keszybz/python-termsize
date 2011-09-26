#include <Python.h>

#ifndef WIN32
#include <sys/ioctl.h>
#else
#include <windows.h>
#include <conio.h>
#endif

static PyObject* _termsize(PyObject *self, PyObject *args)
{
	int fd = 1; /* stdout */

	if(!PyArg_ParseTuple(args, "|i", &fd))
		return NULL;

	int columns, rows;
#ifndef WIN32
	struct winsize w;
	if(ioctl(fd, TIOCGWINSZ, &w)) {
		switch(errno){
		case EBADF:
			return PyErr_Format(PyExc_ValueError,
					    "bad file descriptor");
		default:
			return PyErr_SetFromErrno(PyExc_IOError);
		}
	}
	columns = w.ws_col;
	rows = w.ws_row;
#else
	int nhandle;
	switch(fd){
	case 0: nhandle = STD_INPUT_HANDLE;
		break;
	case 1: nhandle = STD_OUTPUT_HANDLE;
		break;
	case 2: nhandle = STD_ERROR_HANDLE;
		break;
	default:
		return PyErr_Format(PyExc_ValueError, "bad file descriptor");
	}

	HANDLE handle = GetStdHandle(fd);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(handle, &csbi))
		return PyErr_Format(PyExc_IOError, "error %i",
				    (int) GetLastError());

	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif

	return Py_BuildValue("(ii)", columns, rows);
}

static PyMethodDef methods[] = {
	{"termsize", _termsize, METH_VARARGS,
	 "Return the size of the terminal window as (columns, rows)."},
	{NULL, NULL, 0, NULL} /* sentinel */
};

static struct PyModuleDef termsize = {
	PyModuleDef_HEAD_INIT,
	.m_name = "termsize",
	.m_size = 0,
	.m_methods = methods,
};

PyMODINIT_FUNC PyInit_termsize(void)
{
	return PyModule_Create(&termsize);
}
