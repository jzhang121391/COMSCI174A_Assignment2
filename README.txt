CS 174A — Introduction to Computer Graphics: Assignment 2
Weight: 15 %
Maximum points: 37
Note: You can receive extra credit on this assignment. We will select the 10 best animations. We
will screen these in class and all of you will vote for your favorites. The top 3 animations will be
awarded extra points, as follows: 1st place: 10 points; 2nd place: 5 points; 3rd place: 3 points.
Collaboration: None. If you discuss this assignment with others you should submit their names along with the
assignment material.
Start working on this assignment early. You will not have time to do a satisfactory job at the last minute.
Write a program that displays an animated scene using shader based OpenGL. Your scene should include a
combination of hierarchical objects that move around. Required elements:
• [4 points] At least one two-level hierarchical object (e.g., a human arm).
• [4 points] At least one polygonal object modeled vertex by vertex (i.e., you must provide the vertices, and
normals directly).
• [4 points] At least one texture, either procedural or mapped.
• [4 points] Automated 360 degree camera fly-around using LookAt.
• [4 points] Creativity (story, colors, etc).
• [4 points] Complexity (including both animation complexity and a sophisticated shader; e.g. bump mapping).
• [5 points] Overall quality: Object and camera motion, scene construction, proper texturing, attention to detail.
• [4 points] Real-time speed. You should make sure that your animation runs in real time on fast enough
machines. “Real time” means that one simulated second corresponds roughly to one real second.
• [2 points] Display the frame rate of your program in the console window.
• [2 points] Make and submit a movie of your animation using the provided mpeg_encode utility. The movie
frame size should be 500x500 pixels. Include a 100x100 pixel representative image from your animation.
Special instructions:
• Your program executable must recreate the animation in your video. The video should not be edited.
• Note that creativity and quality amount to 9 points. You will not get a perfect score if your scene is complex,
but not creative.
• You must use the provided template code; however, you can modify it as you see fit.
• You must do the assignment from scratch. Using any piece of code from any source (e.g., previous offerings
of the course, the web, etc.) will be considered plagiarism.
Submission guidelines:
• Submit your movie and the representative image separately under the names <surname>.mpg and
<surname>.jpg, respectively, where <surname> denotes your last name.
• Submit all the files required to build and run your project in one archive named <surname>.zip. Include the
Visual Studio solution (.sln), and project (.vcproj) files, but do not submit the executable (.exe) or any
intermediary files (.ncb, .manifest, .idb, .pdb, .obj, .res, .ilk, .dep, etc.).
• If you use texture mapping in your project, submit all the images within <surname>.zip in the location
required by your program. They should not have to be moved in order for your program to run correctly.
• Include in the top level of your <surname>.zip archive a README.TXT file that explains what you did and
did not do, summarizes your animation, identifies the hierarchical, polygonal, and texture mapped objects,
and explains anything else that might be helpful to know in grading your project.