# planet-renderer

This will be an OpenGL renderer capable of rendering real-sized planets. It uses OpenGL 4.0 to be able to use double in the GPU without extensions.
Why doubles and not float? Even though we can store large values on 32-bit floats, there is not enough precision.