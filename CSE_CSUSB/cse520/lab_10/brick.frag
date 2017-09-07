//
// Fragment shader for procedural bricks
//
// Authors: Dave Baldwin, Steve Koren, Randi Rost
//          based on a shader by Darwyn Peachey
//
// Copyright (c) 2002-2004 3Dlabs Inc. Ltd.
//
// See 3Dlabs-License.txt for license information
//  

uniform vec3  BrickColor, MortarColor;
uniform vec2  BrickSize;
uniform vec2  BrickPct;

varying vec2  MCposition;
varying float LightIntensity;

void main(void)
{
    vec3  color;
    vec2  position, useBrick;
    
    position = MCposition / BrickSize ;
    
    if (fract(position.y * 0.5) > 0.5) 
        position.x += 0.5;

    position = fract(position);

    useBrick = step(position, BrickPct);

    float x = position.x;
    float y = position.y;

    if ( y - x - 0.05 < 0.0 && y - x + 0.05 > 0.0 ) 
       useBrick.x = 0.0; 

    //useBrick = smoothstep(position + 0.8, position - 0.1, BrickPct);

/*    for (float i = 0.0; i <= BrickPct.x; i += 0.1) {
      for (float j = 0.0; j <= BrickPct.x; j += 0.1) {
        BrickPct.x -= 0.1;
        //BrickPct.y -= 0.1;
        position.x -= 0.1;
        useBrick = step(position, BrickPct);
      }
    }
*/
    color  = mix(MortarColor, BrickColor, useBrick.x * useBrick.y);
    color *= LightIntensity;
    gl_FragColor = vec4 (color, 1.0);
}
