# daisy
Daisy was intended to be more than it is, but I'm releasing it before I move on and forget about it....

As it stands, it's an example of an implementation of the Half Edge data structure, which is a nicer way of editing mesh data.

This implementation allows meshes to be defined which are made up of faces of 3 or more edges, as long as they're convex, and have no holes.  The Convert function will convert the half edge data into a triangulated indexed mesh which can be submitted to the GPU via [bgfx](https://github.com/bkaradzic/bgfx).

There are projects for Xcode and VS2015.

The Mesh class has large chunks of code lifted from the [bgfx](https://github.com/bkaradzic/bgfx) example code.

The example itself depends on [bgfx](https://github.com/bkaradzic/bgfx).
