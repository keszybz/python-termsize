Querying the size of the output terminal
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. function:: get_terminal_size(fallback=(columns, rows))

   Get the size of the terminal window.

   For each of the two dimensions, the environment variable, ``COLUMNS``
   and ``ROWS`` respectively, is checked. If the variable is defined and
   the value is a positive integer, it is used.

   When ``COLUMNS`` or ``ROWS`` is not defined, which is the common case,
   the terminal connected to :data:`sys.__stdout__` is queried.

   If the terminal size cannot be successfully queried, either because
   the system doesn't support querying, or because we are not
   connected to a terminal, the value given in ``fallback`` parameter
   is used.

   The value returned is a named tuple of type :class:`terminal_size`.

.. function:: get_terminal_size_raw(fd=STDOUT_FILENO)

   Return the size of the terminal window as ``(columns, rows)``,
   tuple of type :class:`terminal_size`.

   The optional argument ``fd`` (default ``STDOUT_FILENO``, or standard
   output) specifies which file descriptor should be queried.

   If the file descriptor is not connected to a terminal, an :exc:`OSError`
   is thrown. If the function is not available for this system, a
   :exc:`NotImplementedError` is thrown.

   :func:`get_terminal_size` is the high-level function which should normally
   be used, ``get_terminal_size_raw`` is the low-level implementation.

.. class:: terminal_size(tuple)

   A tuple of ``(columns, rows)`` for holding terminal window size.

   .. attribute:: columns

      Width of the terminal window in characters.

   .. attribute:: rows
      Height of the terminal window in characters.

