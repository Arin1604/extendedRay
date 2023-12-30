## Project 3: Intersect

Please fill this out for Intersect only. The project handout can be found [here](https://cs1230.graphics/projects/ray/1).

### Output Comparison
Run the program with the specified `.ini` file to compare your output (it should automatically save to the correct path).
> If your program can't find certain files or you aren't seeing your output images appear, make sure to:<br/>
> 1. Set your working directory to the project directory
> 2. Set the command-line argument in Qt Creator to `template_inis/intersect/<ini_file_name>.ini`
> 3. Clone the `scenefiles` submodule. If you forgot to do this when initially cloning this repository, run `git submodule update --init --recursive` in the project directory

> Note: once all images are filled in, the images will be the same size in the expected and student outputs.

| File/Method To Produce Output | Expected Output | Your Output |
| :---------------------------------------: | :--------------------------------------------------: | :-------------------------------------------------: |
| unit_cone.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/unit_cone.png) | ![Place unit_cone.png in student_outputs/intersect/required folder](student_outputs/intersect/required/unit_cone.png) |
| unit_cone_cap.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/unit_cone_cap.png) | ![Place unit_cone_cap.png in student_outputs/intersect/required folder](student_outputs/intersect/required/unit_cone_cap.png) |
| unit_cube.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/unit_cube.png) | ![Place unit_cube.png in student_outputs/intersect/required folder](student_outputs/intersect/required/unit_cube.png) |
| unit_cylinder.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/unit_cylinder.png) | ![Place unit_cylinder.png in student_outputs/intersect/required folder](student_outputs/intersect/required/unit_cylinder.png) |
| unit_sphere.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/unit_sphere.png) | ![Place unit_sphere.png in student_outputs/intersect/required folder](student_outputs/intersect/required/unit_sphere.png) |
| parse_matrix.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/parse_matrix.png) | ![Place parse_matrix.png in student_outputs/intersect/required folder](student_outputs/intersect/required/parse_matrix.png) |
| ambient_total.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/ambient_total.png) | ![Place ambient_total.png in student_outputs/intersect/required folder](student_outputs/intersect/required/ambient_total.png) |
| diffuse_total.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/diffuse_total.png) | ![Place diffuse_total.png in student_outputs/intersect/required folder](student_outputs/intersect/required/diffuse_total.png) |
| specular_total.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/specular_total.png) | ![Place specular_total.png in student_outputs/intersect/required folder](student_outputs/intersect/required/specular_total.png) |
| phong_total.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/phong_total.png) | ![Place phong_total.png in student_outputs/intersect/required folder](student_outputs/intersect/required/phong_total.png) |
| directional_light_1.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/directional_light_1.png) | ![Place directional_light_1.png in student_outputs/intersect/required folder](student_outputs/intersect/required/directional_light_1.png) |
| directional_light_2.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/directional_light_2.png) | ![Place directional_light_2.png in student_outputs/intersect/required folder](student_outputs/intersect/required/directional_light_2.png) |

### Design Choices
Overall, I delegate the functionality of render into a helper called getUpdatedPixel in rayTraceScene.cpp. I use several helper methods in my primary getIntersection method which checks the primitive type and then calls upon helper methods accordingly. I use a few specialized structs that in a sense wrap the t value of the intersection, the normal, and the shape. These structs were especially helpful when returning a normal corresponding to a specific t value whic in turn could correspond to a specific shape.

The surface struct was particularly useful for computing the primitive that was closest to the camera and were computing which side within the object's space was closest to camera. In addition, I use a ray struct which always has two glm::vec4s one for position and one for direction.

### Collaboration/References

### Known Bugs
I could not find any known bugs with the required outputs.

There are some design limitations I would like to address before illuminate:
1) I tried implementing a shape interface using the virtual functions mentioned in the advanced c++ guide. However, it failed to integrate with the way my program computes the normal and intersection. Adding the interface would allow me to improve its modularity and add new features to the existing program structure.

2) In my getViewMatrix and getInverseViewMatrix I perform some expensive calculations repeatedly. I could instead store the computations as member variables and return these variables in the getters instead. However, when I tried to implement this my code encountered some bugs which I believe arose from the rules of initializing fields in C++. I would like to optimize this for illuminate.

### Extra Credit
