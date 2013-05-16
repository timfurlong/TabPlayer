TabPlayer
=========

Tim Furlong
CSCI 4229

This project is a guitar tab reader that displays how to play a guitar tab given
to the program. The program operates off of MusicXML files, which in turn fill
a SQLite database though a python script for use by OpenGL.

While you can pass in the relative file path of any MusicXML file though an additional
command line argument, a default has been programmed to easily show you how it works.

Some of the graphical difficulties I had to overcome to create this project are:
	- Creating a hand that acted as a unit. For this challenge I chose to use C++
	  in order to take advantage of a more object oriented feature set.
	- Dealing with a large set of dependancies for the hand.
		- Position of the hand depends on time, note being played, note most recently played.
		  Furthurmore, the position of each finger is dependant on previous states of
		  each cooresponding finger, the location of the fretboard, and so on.
	- Timing of the hand and finger animation had to synchronize with the duration of the
	  notes being played.


The basics of how to run the program are outlines below:


Instructions:

	ESC         --  exit the program
	Space Bar   --  pause playback
	s/S         --  Increase/Decrease speed of playback
	g/G         --  Toggle showing the guitar
	Arrow Keys  --  Move around the scene

	p/P         --  toggle projection mode
	0           --  Reset view angle
	x/X         --  toggle axis
	m           --  toggle light movement
	-/+         --  Change field of view (Projection only)
	[/]         --  Increase/decrease light elevation
	a/A         --  adjust ambient light
	d/D         --  adjust diffuse light
	e/E         --  adjust emitted light
	t/T         --  toggle texture mode
	n/N         --  adjust shininess
	l/L 			--  toggle spotlight lighting
	v/V 			--  move light vertically
	p           --  switch projection mode
	PgUp/PgDown --  zooms in and out
	+/-         --  changes field of view
	F1          --  toggles smooth/flat shading
	F2          --  toggles light distance (1/5)


