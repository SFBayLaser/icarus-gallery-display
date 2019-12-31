#!/usr/bin/env python
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

try:
    import pyqtgraph.opengl as gl
except:
    print("ERROR: Must have opengl for this viewer")
    exit()

from gui import evdgui3D
import argparse
import sys
import signal
from pyqtgraph.Qt import QtGui, QtCore

from ROOT import evd

from evdmanager import geometry, evd_manager_3D

# This is to allow key commands to work when focus is on a box




def sigintHandler(*args):
    """Handler for the SIGINT signal."""
    sys.stderr.write('\r')
    sys.exit()


def main():

    parser = argparse.ArgumentParser(description='Python based 3D event display.  Requires opengl.')
    geom = parser.add_mutually_exclusive_group()
    geom.add_argument('-A', '-a', '--argoneut',
                      action='store_true',
                      help="Run with the argoneut geometry")
    geom.add_argument('-U', '-u', '--uboone',
                      action='store_true',
                      help="Run with the microboone geometry")
    geom.add_argument('-L', '-l', '--lariat',
                      action='store_true',
                      help="Run with the lariat geometry")
    parser.add_argument('file', nargs='*', help="Optional input file to use")

    args = parser.parse_args()

    app = QtGui.QApplication(sys.argv)

    if args.uboone:
        geom = geometry.microboone()
    elif args.lariat:
        geom = geometry.lariat()
    else:
        geom = geometry.argoneut()

    # If a file was passed, give it to the manager:

    manager = evd_manager_3D(geom)
    manager.setInputFiles(args.file)

    thisgui = evdgui3D(geom, manager)
    # manager.goToEvent(0)

    signal.signal(signal.SIGINT, sigintHandler)
    timer = QtCore.QTimer()
    timer.start(500)  # You may change this if you wish.
    timer.timeout.connect(lambda: None)  # Let the interpreter run each 500 ms.

    app.exec_()
    # sys.exit(app.exec_())


if __name__ == '__main__':
    main()
