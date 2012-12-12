TabPlayer
=========

Tim Furlong
CSCI 4229


//  Move light
 (ch == '<')
	zh += 1;
 (ch == '>')
	zh -= 1;
//  Change field of view angle
 (ch == '-' && ch>1)
	fov--;
 (ch == '+' && ch<179)
	fov++;
//  Light elevation
 (ch=='[')
	ylight -= 0.1;
 (ch==']')
	ylight += 0.1;
//  Ambient level
 (ch=='a' && ambient>0)
	ambient -= 5;
 (ch=='A' && ambient<100)
	ambient += 5;
//  Diffuse level
 (ch=='d' && diffuse>0)
	diffuse -= 5;
 (ch=='D' && diffuse<100)
	diffuse += 5;
//  Specular level
 (ch=='s' || ch=='S')
	pause_playback = 1-pause_playback;
//  Emission level
 (ch=='e' && emission>0)
	emission -= 5;
 (ch=='E' && emission<100)
	emission += 5;
//  Shininess level
 (ch=='n' && shininess>-1)
	shininess -= 1;
 (ch=='N' && shininess<7)
	shininess += 1;
 (ch=='v' || ch=='V')
	moveLightVert = 1-moveLightVert;

Instructions:

	ESC         --  exit the program
	s/S         --  pause playback
	g/G         --  pause playback

	p/P         --  toggle projection mode
	0           --  Reset view angle
	x/X           --  toggle axis
	m           --  toggle light movement
	-/+         --  Change field of view (Projection only)
	[/]         --  Increase/decrease light elevation
	Arrow Keys  --  Move around the scene
	a/A         --  adjust ambient light
	d/D         --  adjust diffuse light
	e/E         --  adjust emitted light
	t/T         --  toggle texture mode
	n/N         --  adjust shininess
	l/L 			--  toggle lighting
	v/V 			--  move light vertically
	p           --  switch projection mode
	PgUp/PgDown --  zooms in and out
	+/-         --  changes field of view
	F1          --  toggles smooth/flat shading
	F2          --  toggles light distance (1/5)


