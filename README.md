TabPlayer
=========

Tim Furlong
CSCI 4229


Instructions:

	ESC         --  exit the program
	0           --  Reset view angle
	x/X           --  toggle axis
	m           --  toggle light movement
	-/+         --  Change field of view (Projection only)
	Arrow Keys  --  Move around the scene
	a/A         --  adjust ambient light
	d/D         --  adjust diffuse light
	s/S         --  adjust specular light
	e/E         --  adjust emitted light
	n/N         --  adjust shininess
	l/L 			--  toggle lighting
	v/V 			--  move light vertically
	p           --  switch projection mode
	PgUp/PgDown --  zooms in and out
	+/-         --  changes field of view
	F1          --  toggles smooth/flat shading
	F2          --  toggles light distance (1/5)


== Progress Thus Far ==
So far I have created the complete graphical representation of the guitar neck
as well as some of the hand model. The skeletal hand model has proved to be
FAR more difficult than expected, so this part took the largest quantity of time.

I realize the running product of this project thus far does not reflect nearly the
amount of time put into it, but that is because I've been writing my OpenGL code to
be highly adaptive to many situations, since that is what my project requires. Have a
quick look at the source to see what I am talking about.

== What Remains to be Done ==
- I need to finish how the fingers are rendered relative to the hand base and desired
  fret location.
- I need to create a 3D hand model in Maya or Blender
- I need to bind the verticies of the hand to a 3D hand model
- I need to create a wall-clock synchronized animation to transition between the
  different hand positions.
- I need to synchronize the animation of the hand with the surronding music/tablature
