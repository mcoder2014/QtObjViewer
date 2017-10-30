I still named the project samples, but I did not go back and clean anything up - so be careful and double-check anything that seems suspicious! I left the projects in-tact in case someone wanted to learn from them.

# Here is a brief description of the projects:

6. **GPUProfiler** - Utilize timer queries to get a flame graph view into how long processing the frame takes. This is very important to learn, because there will come a time which you need to diagnose why your content stutters or takes time in the GPU to render what is on the screen. This is a very basic implementation.

7. **ProfileVisualizer** - Taking the contents of the GPU profiling results and actually visualizing them as a sort-of flame graph (continuation of part 6).

8. **FPSCounter** - Simple logic to count how many frames are able to be rendered every second. I recall thinking there was a bug here because sometimes I would get 61 frames when locked vsync at 60 (that doesn't add up). So I think I just went through this too fast and did something incorrect (or maybe some off-by-one error somewhere). Be on the lookout!

9. **ES3Support** - I was trying to deploy to Android, and found I needed ES3 support which Qt5 at the time didn't have an QOpenGLFunctions abstraction for. This was basically here to create the ES3 abstraction, and then I had originally planned on upstreaming this to Qt. However, that never happened because I was running out of time to work on the project - this marked around when I decided I would not create more tutorials and scrapped what I had written already for 6, 7, and 8.

10. **Refactor** - Refactored the project and cleaned up the code to make it scale better towards KarmaView, which was the final sample project. I was setting up for doing higher-level tutorials based around graphics concepts instead of specifically about Qt5 and OpenGL. I continued making these copies of the code in case I changed my mind about writing more tutorials, or if someone else might find use of them.

11. **TouchEvents** - I needed to enable usage of some Qt TouchEvents for controlling the Android implementation of KarmaView for experimental purposes. This is a very raw abstraction/implementation using their event system. Basic gestures like pinch and pan, along with some simple rotation commands by touch-and-drag. (I think that's what is in here.)

12. **ObjParser** - I needed more than just a cube, so I wrote an object parser. I attempted to make this as efficient as I could at the time, but lacked full knowledge of writing optimal parsers. Knowing what I know now I could very easily improve this - but it's a decent theoretical parser example (see kabstractparser.cpp for implementation). Basically, we make use of virtual functions to react to parsing an object file. In my opinion, this is simply an OK parser - you can maybe learn from it, but don't consider it production-quality.

13. **HalfEdgeMesh** - The HEM is a data structure that makes it easy to find properties of a more complicated model. For example: Are there holes? How can I mutate a face? How can I add a point/edge/face? HEM is good for this, but not for raw output. I hear there are more advanced techniques than I a HEM, but I have not investigated them. This was purely to learn about half-edge meshes.

14. **AveragedNormals** - Instead of having a flat-faced structure where each vertex is duplicated an has a normal poking out in the direction of the face, use the half-edge mesh data structure to find shared vertexes and average the face normals together to produce a smooth surface.

15. **BufferedFileReader** - Attempted to implement a double-buffered file reader. Was reading the book "Compilers: Principles, Techniques, & Tools" (an excellent, albeit dense, read if you are interested in learning about writing compilers or other format-reading tools). I don't 100% know if buffered reading is beneficial nowadays if you are using fread, since the content is already buffered and internally in most implementations there is sequential file read-ahead at the kernel-level. I don't recommend learning from this project because the analysis is inconsequential.

16. **ES3Abstraction** - Extend the usability of our shader code by automatically marking-up the shaders so that we only have to write the shader once and can compile on either OpenGL or OpenGL ES. The version markup is different at the top of the shader file, so an abstraction is often necessary. This is not a complete abstraction, just a simple one that enabled my project to continue to work.

17. **UniformBlocks** - Utilize OpenGL uniforms for frame-specific state (or state that changes infrequently). Think of uniforms as "programmable constants" which may change per-object, per-frame, or when the user changes a setting. They of course are not intended for per-vertex.

18. **GLSLParser** - A very simple parser that simply echos unknown tokens, and otherwise can process known tokens (for example, I use this to extend GLSL to support #include directives to make it possible to share shader code). Again, this is non-optimal! use this only for learning! (see src/OpenGL/openglslparser.cpp)

19. **InstancedRendering** - Wanted to see the potential benefit of adding instanced rendering. Learned that it's very case-specific if instanced rendering will help. Instanced rendering is supposed to be useful when you can do less calculations over a wider range of input instances, so if that's not the case for your input - it may actually (in some cases) make GPU efficiency worse (for example, a proper usage of instanced rendering may be if we wanted to animate all N objects the same, so we only calculate the bone structure once). For my samples, there was no noticeable improvement in speed because my test case was too simple.

20. **IndexedMesh** - Attempted to use an indexed mesh instead of a contiguous mesh to reduce the size of the internal buffer by collapsing shared points/normals. Outside of how to make an indexed mesh, I don't recall what I learned from this, I'm sorry. :(

21. **GBufferConstruction/22_AmbientPass/23_PointLight** - This is basically the following: http://www.trentreed.net/blog/deferred-rendering-pipeline/ Note that there are many implementations, and this is a simple one which I don't recall if it handled color-space normalization properly. But you can get the gist of how deferred rendering works from it at least. I recommend supplementing this with other tutorials on deferred rendering since I never got to go back and clean my implementation up and error-check every little bit of it.

---

I know this is not probably what you were looking to hear from me, but I hope it helps. I really would love to go back and write more tutorials about what kind of stuff I'm doing now. However, I just don't have the time to do so.

Here is further reading of advanced graphics features which I do have tutorials on:
1. [exponential-shadow-maps](http://www.trentreed.net/blog/exponential-shadow-maps/)
2. [physically-based-shading-and-image-based-lighting](http://www.trentreed.net/blog/physically-based-shading-and-image-based-lighting/)
3. [screen-space-ambient-occlusion](http://www.trentreed.net/blog/screen-space-ambient-occlusion/)
