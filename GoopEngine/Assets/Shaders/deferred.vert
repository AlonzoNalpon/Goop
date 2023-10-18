
#version 460 core

layout (location=0) in vec3 fPosition;    // output fragment position
layout (location=1) in vec3 fAlbedo;      // output fragment albedo
layout (location=2) in vec3 fNormal;      // output fragment normal
layout (location=3) in vec2 fTexCoord;    // output tex coord 

layout(binding=0) 