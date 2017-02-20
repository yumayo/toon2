#version 120
uniform sampler2D texture;
uniform vec2 window_size;
uniform vec4 def_color_l;
uniform vec4 def_color_r;
void main(void)
{
    // 画面の位置。
    vec2 pos = vec2( gl_FragCoord.x, window_size.y - gl_FragCoord.y);

    vec4 own = texture2D( texture, pos / 512 / 4 );

    vec4 maps[8];
    maps[0] = texture2D( texture, pos / 512 / 4 + vec2( 1,  1) / 512 );
    maps[1] = texture2D( texture, pos / 512 / 4 + vec2(-1, -1) / 512 );
    maps[2] = texture2D( texture, pos / 512 / 4 + vec2(-1,  1) / 512 );
    maps[3] = texture2D( texture, pos / 512 / 4 + vec2( 1, -1) / 512 );
    maps[4] = texture2D( texture, pos / 512 / 4 + vec2( 1,  0) / 512 );
    maps[5] = texture2D( texture, pos / 512 / 4 + vec2( 0,  1) / 512 );
    maps[6] = texture2D( texture, pos / 512 / 4 + vec2(-1,  0) / 512 );
    maps[7] = texture2D( texture, pos / 512 / 4 + vec2( 0, -1) / 512 );

    //texture2D(texture, gl_FragCoord.xy);
    //texture2D(texture, gl_FragCoord[0].st);

    // if(own != migi)
    // {
    //     gl_FragColor = texture2D( texture, pos / 128 / 4 );
    // }
    // else
    // {
    //     gl_FragColor = vec4(1);
    // }
    
    // gl_FragColor = own;
    
    vec4 res = (own + maps[0] + maps[1] + maps[2] + maps[3] + maps[4] + maps[5] + maps[6] + maps[7]) / 9;

    vec3 whi = abs(res.xyz - vec3(1));
    vec3 rig = abs(res.xyz - def_color_r.xyz);
    vec3 lef = abs(res.xyz - def_color_l.xyz);

    float t = 0.5;
    if( rig.x < t && rig.y < t && rig.z < t) // per
    {
        gl_FragColor = def_color_r;
    }
    else if(lef.x < t && lef.y < t && lef.z < t) // per
    {
        gl_FragColor = def_color_l;
    }
    else
    {
        gl_FragColor = vec4(0.1,0.1,0.1,1.0);
    }
}
