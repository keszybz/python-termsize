from ._termsize import get_terminal_size_raw, terminal_size
import os as _os
import sys as _sys

def get_terminal_size(fallback=(80, 24)):
    """Get the size of the terminal window.

    For each of the two dimensions, the environment variable, COLUMNS
    and ROWS respectively, is checked. If the variable is defined and
    the value is a positive integer, it is used.

    When COLUMNS or ROWS is not defined, which is the common case,
    the terminal connected to sys.__stdout__ is queried.

    If the terminal size cannot be successfully queried, either because
    the system doesn't support querying, or because we are not
    connected to a terminal, the value given in fallback parameter
    is used.

    The value returned is a named tuple of type terminal_size.

    Right now the values are not cached, but this might change.
    """
    # ccc, rrr are the working values
    try:
        ccc = int(_os.environ['COLUMNS'])
    except (KeyError, ValueError):
        ccc = 0

    try:
        rrr = int(_os.environ['ROWS'])
    except (KeyError, ValueError):
        rrr = 0

    # only query if necessary
    if ccc <= 0 or rrr <= 0:
        try:
            size = get_terminal_size_raw(_sys.__stdout__.fileno())
        except OSError:
            size = terminal_size(fallback)
        if ccc <= 0:
            ccc = size.columns
        if rrr <= 0:
            rrr = size.rows

    return terminal_size((ccc, rrr))
