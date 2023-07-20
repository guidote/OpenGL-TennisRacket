# OpenGL-TennisRacket
### Comp371 Assignment 1 - Gabrielle Guidote 40175182

The model is creating using hierarchical modelling, the arm and body are seperate cubes that are transformed to take shape of the model. <br />
<br />
_Building : _ <br />
  in code - mkdir build  <br />
  cmake -S . -B <build_folder>  <br />
  cmake --build <build_folder> --target install  <br />
  cd build and make to build the code <br />
  ./assignment1 to run the code <br />
  <br />
_Controls : _
Controls  | Description
------------- | -------------
SPACEBAR  | Pressing the spacebar should re-position the model at a random location on the grid.
'U', 'J'  | Size up the model by pressing ‘U’ for scale-up and ‘J’ for scale-down.
'A', 'D', 'W', 'S' |  Key A → move left, D → move right, W → move up, S → move down. Hold shift and key.
'a', 'd' | Key a → rotate left 5 degrees about Y axis, d → rotate right 5 degrees about Y axis.
RIGHT, LEFT, UP, DOWN | The world orientation is changed by using keyboard input i.e. left arrow → Rx, right arrow → R-x, up arrow → Ry, down arrow → R-y.
'H' | Returns to initial world position and orientation.
'T', 'P', 'L' | Change rendering mode of the model. Key ‘P’ for points, key ‘L’ for lines, key ‘T’ for triangles.
MOUSE | Right button is pressed → use mouse movement in x direction to pan. Middle button is pressed → use mouse movement in y direction to tilt. Left button is pressed → use mouse movement to move into/out of the scene.
