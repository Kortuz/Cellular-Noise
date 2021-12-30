#version 330 core
#define MAX_SIZE_ARRAY 1000

out vec4 FragColor;

#ifdef GL_ES
precision mediump float;
#endif

in vec2 texCoord;

uniform  sampler2D tex0;

uniform int[2] resolution;
uniform float[MAX_SIZE_ARRAY] points;
uniform int lenght;

uniform int palette;
uniform int mode;

//bonita funcion para sacar un random con una seed vec2
float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
  
void main()
{
    bool isolines = false;
    vec2 st = gl_FragCoord.xy/vec2(resolution[0],resolution[1]);
    st.x *= resolution[0]/resolution[1];
    vec3 color = vec3(0.00,0.00,0.00);
    float m_dist = 1.0;  // minimum distance
    vec2 nearestPoint;
    // Iterate through the points positions
    for (int i = 0; i < lenght; i++) {
        float dist = distance(st, vec2(points[i*2],points[(i*2)+1]));

        // Keep the closer distance and point
        if (dist < m_dist)
        {
              m_dist = dist;
              nearestPoint = vec2(points[i*2],points[(i*2)+1]);
		}
    }

    // Palette
    switch(palette)
    {
        case 1:
            color = vec3(rand(nearestPoint));
            break;
        case 2:
            color.rb =nearestPoint;
            break;
        case 3:
            color = vec3(rand(nearestPoint));
            color.rb =nearestPoint;
            break;
    }

    // Mode
    switch(mode)
    {
        case 8:
            break;
        case 9:
            isolines = !isolines;
            break;
        case 0:
            color = vec3(m_dist);
            break;
    }

    // Isolines
    if(isolines)
    {
        color -= step(.7,abs(sin(50.0*m_dist)))*.3;
    }

    //FragColor = vec4(color,1.0);
    FragColor = texture(tex0, texCoord);
} 