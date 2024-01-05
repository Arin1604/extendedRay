# Extended Ray-Tracer
Improvements on my Ray-tracer. Specifically, focused on implementing lens mechanics to more realistically simulate camera features.

### Overview
My project uses rendering techniques like recursive ray tracing, phong illumination model, camera simulation, etc. to create visually powerful renders of 3-D scenes.

It parses through JSON files that contain the scene description. The 3-D objects in the scene are defined mathematically using implicit equations. Light rays are generated and then traced through the scene to compute their interactions with the shapes using the ray-tracing algorithm. I handle the final color computation by implememnting the renowned Phong Illumination Model.



---

### Repo Structure

#### **SRC**
SRC contains the main implementation of my project. It is comprised of a Camera Class, a Ray-Tracer Class (and its associated files), Shape classes, Utilities classes, and a main file. Detailed descriptions of their functions are present as in-line comments in their respective header and cpp files. Below, I provide I brief description of their roles:

##### i) Camera
My camera class represents the virtual camera in my project through which we see the 3-D scene. Its orientation is defined by 3 vectors: pos (its position), look (the direction in which it is looking), and up(the "up" direction for the camera). It defines the view matrix, and **defines the viewplane for our scene**. Collectively, these determine the view volume that is rendered in the final image.

##### ii) Ray-Tracer
raytracer and raytracescene handle the vast majority of the ray-tracing algorithm. In essence, we define light rays as mathematical objects and generate them from the Camera (or the lens in special cases). These rays are directed towards individual pixels of the aforementioned view plane and are then traced into the 3-D scene. The color of the pixel is then determined by computing the ray's interactions with the objects in the scene. The Phong Illumination Model plays a crucial role here.
![image](https://github.com/Arin1604/extendedRay/assets/113402703/0c0b882c-95e6-4ab3-b901-eb34fa60a4e6)
https://developer.nvidia.com/discover/ray-tracing

##### iii) Utils

