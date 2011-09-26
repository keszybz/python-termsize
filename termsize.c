#include <Python.h>

#include <sys/ioctl.h>

static PyObject* _termsize(PyObject *self, PyObject *args)
{
	int fd = 1; /* stdout */

	if(!PyArg_ParseTuple(args, "|i", &fd))
		return NULL;
	
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

	return Py_BuildValue("(ii)", w.ws_col, w.ws_row);
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
