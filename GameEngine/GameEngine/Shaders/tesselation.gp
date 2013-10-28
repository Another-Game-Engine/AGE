 //GEOMETRY SHADER
 #version 120
 #extension EXT_geometry_shader4 : enable
 ///////////////////////
 void main()
 {
   //increment variable
   int i;
   vec4 vertex;
   /////////////////////////////////////////////////////////////
   //This example has two parts
   //   step a) draw the primitive pushed down the pipeline
   //            there are gl_VerticesIn # of vertices
   //            put the vertex value into gl_Position
   //            use EmitVertex => 'create' a new vertex
   //           use EndPrimitive to signal that you are done creating a primitive!
   //   step b) create a new piece of geometry
   //           I just do the same loop, but I negate the vertex.z
   //   result => the primitive is now mirrored.
   //Pass-thru!
   for(i = 0; i < gl_VerticesIn; i++)
   {
     gl_Position = gl_PositionIn[i];
     EmitVertex();
   }
   EndPrimitive();
   //New piece of geometry!
   for(i = 0; i < gl_VerticesIn; i++)
   {
     vertex = gl_PositionIn[i];
     vertex.z = -vertex.z;
     gl_Position = vertex;
     EmitVertex();
   }
   EndPrimitive();
 }