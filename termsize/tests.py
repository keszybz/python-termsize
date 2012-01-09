import sys
import os
import subprocess
import unittest
from test import support
import termsize

class TermsizeTests(unittest.TestCase):
    def test_does_not_crash(self):
        # There's no easy portable way to actually check the size of
        # the terminal, so let's check if it returns something
        # sensible instead.
        size = termsize.get_terminal_size()
        self.assertGreater(size.columns, 0)
        self.assertGreater(size.rows, 0)

    def test_os_environ_first(self):
        "Check if environment variables have precedence"

        with support.EnvironmentVarGuard() as env:
            env['COLUMNS'] = '777'
            size = termsize.get_terminal_size()
        self.assertEqual(size.columns, 777)

        with support.EnvironmentVarGuard() as env:
            env['ROWS'] = '888'
            size = termsize.get_terminal_size()
        self.assertEqual(size.rows, 888)

    def test_stty_match_raw(self, func=termsize.get_terminal_size_raw):
        "Check if stty returns the same raw results"

        try:
            size = subprocess.check_output(['stty', 'size']).decode().split()
            expected = (int(size[1]), int(size[0])) # reversed order
        except Exception:
            return

        try:
            actual = func()
        except Exception:
            return

        self.assertEqual(expected, actual)

    def test_stty_match(self):
        "Check if stty returns the same results ignoring env"
        try:
            os.isatty
        except AttributeError:
            self.skipTest('os.isatty not present')

        if not os.isatty(sys.__stdout__.fileno()):
            return
        with support.EnvironmentVarGuard() as env:
            del env['ROWS']
            del env['COLUMNS']
            self.test_stty_match_raw(termsize.get_terminal_size)

def test_main():
    support.run_unittest(TermsizeTests)

if __name__== '__main__':
    test_main()
