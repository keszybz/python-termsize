#include <Python.h>

#if defined(MS_WINDOWS)
#  include <windows.h>
#  define TERMSIZE_USE_CONIO
#else
#  include <sys/ioctl.h>
#  include <termios.h>
#  define TERMSIZE_USE_IOCTL
#endif /* MS_WINDOWS */

/* Terminal size querying */

static PyTypeObject TerminalSizeType;

PyDoc_STRVAR(TerminalSize_docstring,
	     "A tuple of (columns, rows) for holding terminal window size");

static PyStructSequence_Field TerminalSize_fields[] = {
    {"columns", "width of the terminal window in characters"},
    {"rows", "height of the terminal window in characters"},
    {NULL, NULL}
};

static PyStructSequence_Desc TerminalSize_desc = {
    "os.terminal_size",
    TerminalSize_docstring,
    TerminalSize_fields,
    2,
};

PyDoc_STRVAR(termsize__doc__,
    "Return the size of the terminal window as (columns, rows).\n"         \
    "\n"                                                                   \
    "The optional argument fd (default standard output) specifies\n"       \
    "which file descriptor should be queried.\n"                           \
    "\n"                                                                   \
    "If the file descriptor is not connected to a terminal, an OSError\n"  \
    "is thrown.\n"                                                         \
    "\n"                                                                   \
    "This function will only be defined if an implementation is\n"         \
    "available for this system.\n"                                         \
    "\n"                                                                   \
    "get_terminal_size is the high-level function which should normally\n" \
    "be used, get_terminal_size_raw is the low-level implementation.");


static PyObject*
get_terminal_size_raw(PyObject *self, PyObject *args)
{
    int columns, rows;
    PyObject *termsize;

    int fd = fileno(stdout);
    /* Under some conditions stdout may not be connected and
     * fileno(stdout) may point to an invalid file descriptor. For example
     * GUI apps don't have valid standard streams by default.
     *
     * If this happens, and the optional fd argument is not present,
     * the ioctl below will fail returning EBADF. This is what we want.
     */

    if (!PyArg_ParseTuple(args, "|i", &fd))
        return NULL;

#ifdef TERMSIZE_USE_IOCTL
    {
        struct winsize w;
        if (ioctl(fd, TIOCGWINSZ, &w))
            return PyErr_SetFromErrno(PyExc_OSError);
        columns = w.ws_col;
        rows = w.ws_row;
    }
#endif /* TERMSIZE_USE_IOCTL */

#ifdef TERMSIZE_USE_CONIO
    {
        DWORD nhandle;
        HANDLE handle;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        switch (fd) {
        case 0: nhandle = STD_INPUT_HANDLE;
            break;
        case 1: nhandle = STD_OUTPUT_HANDLE;
            break;
        case 2: nhandle = STD_ERROR_HANDLE;
            break;
        default:
            return PyErr_Format(PyExc_ValueError, "bad file descriptor");
        }
        handle = GetStdHandle(nhandle);
        if (handle == NULL)
            return PyErr_Format(PyExc_OSError, "stdout not connected");
        if (handle == INVALID_HANDLE_VALUE)
            return PyErr_SetFromWindowsErr(0);

        if (!GetConsoleScreenBufferInfo(handle, &csbi))
            return PyErr_SetFromWindowsErr(0);

        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
#endif /* TERMSIZE_USE_CONIO */

    termsize = PyStructSequence_New(&TerminalSizeType);
    if (termsize == NULL)
        return NULL;
    PyStructSequence_SET_ITEM(termsize, 0, PyLong_FromLong(columns));
    PyStructSequence_SET_ITEM(termsize, 1, PyLong_FromLong(rows));
    if (PyErr_Occurred()) {
        Py_DECREF(termsize);
        return NULL;
    }
    return termsize;
}

static PyMethodDef methods[] = {
    {"get_terminal_size_raw", get_terminal_size_raw,
     METH_VARARGS, termsize__doc__},
    {NULL, NULL, 0, NULL} /* sentinel */
};

static struct PyModuleDef termsize = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_termsize",
    .m_size = 0,
    .m_methods = methods,
};

PyMODINIT_FUNC PyInit__termsize(void)
{
    PyObject *m = PyModule_Create(&termsize);

    if (1) {
        /* initialize TerminalSize_info */
        PyStructSequence_InitType(&TerminalSizeType, &TerminalSize_desc);
        Py_INCREF(&TerminalSizeType);
    }

    if (m != NULL)
        PyModule_AddObject(m, "terminal_size", (PyObject*) &TerminalSizeType);

    return m;
}

/* Local Variables: */
/* c-basic-offset:4 */
/* indent-tabs-mode:nil */
/* End: */
