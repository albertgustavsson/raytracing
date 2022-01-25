# raytracing
C++ implementation of the ray tracer descibed in the books by Peter Shirley ([_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html), [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html), and [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)).  
Some modifications have been made to make the code fit my own prefered style, but functionality-wise, the implementation is pretty much as described in the books (work in progress, but in some time).

## Sample images
<img src="images/3k-100spp-ap0_01.png" alt="Rendered image" width="60%"/>

### Triangles
Support for trianges was added instead of using a sphere as the ground plane.  
In this image two triangles form the gorund plane. One is colored differently to visualise the triangles:  
<img src="images/triangles-3k.png" alt="Triangle visualisation" width="60%"/>

### Focus blur
These images show the effect of changing the aperture of the camera. The images were rendered with apertures 0.01, 0.1, 0.5 and 2.0:  
<img src="images/3k-100spp-ap0_01.png" alt="Camera with aperture=0.01" width="49%"/>
<img src="images/3k-100spp-ap0_1.png" alt="Camera with aperture=0.1" width="49%"/>  
<img src="images/3k-100spp-ap0_5.png" alt="Camera with aperture=0.5" width="49%"/>
<img src="images/3k-100spp-ap2_0.png" alt="Camera with aperture=2.0" width="49%"/>

### Emissive materials
Below, the background was set to black and an emissive sphere was added as a light source:  
<img src="images/1k2-500spp-ap0_01-lights.png" alt="Emissive sphere as light source" width="60%"/>

Here is an image depicting the Cornell box:  
<img src="images/600-500spp-cornell_box.png" alt="Cornell box" width="50%"/>
