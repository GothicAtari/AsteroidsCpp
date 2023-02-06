# AsteroidsCpp
An adaptation of the classic Asteroids arcade game made with vector graphics.

## Description
 
This version of Asteroids utilizes a barebones vector graphics game engine, "olcConsoleGameEngine.h", to create basic shapes that interact with each other.
All objects are created using mathematical equations instead of typical sprites and images used by other game engines. A player ship is generated in the middle
of the screen with a single asteroid. 

### Controls
Use the 'left' and 'right' arrow keys to rotate the ship, the 'up' arrow key to apply thrust, and 'space bar' to fire bullets
at the asteroids. After destroying the first asteroid, two more appear! Survive as long as you can!

## Challenges
- Wrapping the objects from one end of the screen to the other.
- Making sure the asteroid objects did not spawn on top of the player to create a fair feeling.
- Creating objects and behaviors using mathematical equations. Examples are: asteroids are circles with added variance to their edges to create the jagged look,
velocities are determined using trigonometric relationships between points, and rotating the player model is an implementation of matrix multiplication!

## Known issues
- Sometimes the game will run at a minimum window size. This may be because there is a problem with the 'SetConsoleXXX' functions as they are somewhat circular and
tend to fail based on the current console properties. Currently the only fix is to attempt another boot and hope the precise ordering of calls sizes it properly.
- This game does not resize correctly!

![OneLoneCoder com - Console Game Engine - Asteroids - FPS_ 258 33 2_6_2023 2_30_45 PM](https://user-images.githubusercontent.com/103795795/217111295-95a3b05e-e0c1-4dc5-8249-1e2c1ceb79a9.png)
![OneLoneCoder com - Console Game Engine - Asteroids - FPS_ 293 04 2_6_2023 5_23_18 PM](https://user-images.githubusercontent.com/103795795/217111345-c05799dc-b801-459b-a127-e4590645f613.png)
![OneLoneCoder com - Console Game Engine - Asteroids - FPS_ 293 04 2_6_2023 5_23_43 PM](https://user-images.githubusercontent.com/103795795/217111353-d8421300-572e-4121-b9f2-b3bae08f04f9.png)


## Credit to @Javidx9 for creating "olcGameEngine.h", the skeleton of this project.

### License (OLC-3)

Copyright 2018-2022 OneLoneCoder.com

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:

1. Redistributions or derivations of source code must retain the above 
   copyright notice, this list of conditions and the following disclaimer.

2. Redistributions or derivative works in binary form must reproduce 
   the above copyright notice. This list of conditions and the following 
   disclaimer must be reproduced in the documentation and/or other 
   materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its 
   contributors may be used to endorse or promote products derived 
   from this software without specific prior written permission.
    
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
