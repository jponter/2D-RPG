uniform sampler2D texture;
uniform vec2 iResolution;
uniform float iTime;


float snow(vec2 uv, float scale, float time)
{
    float w=smoothstep(1.0,0.0,-uv.y*(scale/10.0));if(w<.1)return 0.0;
    uv+=time/scale;uv.y+=time*2.0/scale;uv.x+=sin(uv.y+time*.5)/scale;
    uv*=scale;vec2 s=floor(uv),f=fract(uv),p;float k=3.0,d;
    p=.5+.35*sin(11.0*fract(sin((s+scale)*mat2(vec2(7,3),vec2(6,5)))*5.0))-f;d=length(p);k=min(d,k);
    k=smoothstep(0.0,k,sin(f.x+f.y)*0.01);
    return k*w;
}

void main( )
{
	vec2 fragCoord= gl_FragCoord;
    // Normalized pixel coordinates (from 0 to 1)
   vec2 uv = fragCoord/iResolution.xy;
   vec2 pos = gl_TexCoord[0].xy;
   

   uv=(fragCoord.xy*2.0-iResolution.xy)/min(iResolution.x,iResolution.y); 
    vec3 finalColor=vec3(0);
    float c=smoothstep(1.0,0.3,clamp(uv.y*.3+.8,0.0,.75));
    c+=snow(uv,30.0,iTime)*.3;
    c+=snow(uv,20.0,iTime)*.5;
    c+=snow(uv,15.0,iTime)*.8;
    c+=snow(uv,10.0,iTime);
    c+=snow(uv,8.0,iTime);
    c+=snow(uv,6.0,iTime);
    c+=snow(uv,5.0,iTime);
    finalColor=(vec3(c));
	vec4 OrigColour = texture2D(texture,pos);
    //gl_FragColor += vec4(finalColor,1.0) ;

	gl_FragColor = OrigColour + (vec4(finalColor,1.0)/2);
	
	
	
}