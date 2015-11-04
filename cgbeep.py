import sys
import tty
import termios
import tkSnack
import Tkinter
import os
import datetime

def playnote(freq, duration):
	# Initialize sound interface
	snd = tkSnack.Sound()
	# Generate a sound of given frequency for given duration
	# Sound is produced using a sine wave
	filt = tkSnack.Filter('generator',freq, 30000, 0.0, 'sine', int(10000*duration))
	# Stop all sound definitions
	snd.stop()
	# Play sound, block all other sounds except this
	snd.play(filter = filt, blocking = 1)

# Playing sound through speaker channels
# is done through an interface provided
# by the Tkinter GUI library

# tkSnack is the interface which attaches to a Tkinter object
# and provides sound functionality to the application
root = Tkinter.Tk()									# Creates a headless Tkinter application
tkSnack.initializeSnack(root)
playnote(float(sys.argv[1]),float(sys.argv[2]))		# Command-line arguments for frequency and duration


