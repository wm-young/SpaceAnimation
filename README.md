# SpaceAnimation

**** COSC 3P98 Final Project ****
******** Space Animation ********
***** Michael Young 4245718 *****
********** May 2011 *************

---------
Overview+
---------

Space Animation is a 35sec long animation that shows a space ship leaving a planet and after a bit he is chased by an alien ship. The alien ship flies around and fires his laser at the ship. The ship then explodes.

Programing was done in c using Microsoft Visual Studio 2008. All models were made using Milkshape 3D v1.7.4. Each model was exported from Milkshape using the built in ASCII exporter. A copy of the export file template is located in the same directory as this. This copy simply outlines the format and what each number means. The models were then read in to my program into the necessary data structures and by using OpenGL and GLUT library the information is rendered and displayed on the screen. 

THere are some models and model text files located in this directory, a lot of them are things I used, and don't necessarrily use now. however the ones that are actually used by the program are located in /space_animation/space_animation


-----------
How To Run+
-----------
- I didn't know how to make the animation into video format for open viewing, so the animation can be accessed 1 of 3 ways:

1)Run the space_animation application shortcut from the same directory as this.

2) Open the solution space_animation.sln located in the /space_animation/ folder.

3)Run the application space_animation.exe located in the /space_animation/Debug/ folder.


---------
Controls+
---------

Pressing 'Q' anytime during the animation will quit the program.


------
Notes+
------

Visual Studio gives about 200 warning for fscanf. It works fine, I just ignored these warnings. 