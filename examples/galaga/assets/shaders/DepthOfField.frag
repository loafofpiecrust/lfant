
vec2 texel;
int samples = 3; //samples on the first ring
int rings = 3; //ring count

bool manualdof = true; //manual dof calculation
float ndofstart = 1.0; //near dof blur start
float ndofdist = 2.0; //near dof blur falloff distance
float fdofstart = 1.0; //far dof blur start
float fdofdist = 3.0; //far dof blur falloff distance

float CoC = 0.03f;//circle of confusion size in mm (35mm film = 0.03mm)

bool vignetting = false; //use optical lens vignetting?
float vignout = 1.3; //vignetting outer border
float vignin = 0.0; //vignetting inner border
float vignfade = 22.0; //f-stops till vignete fades

bool autofocus = true; //use autofocus in shader? disable if you use external focalDepth value
vec2 focusPoint = vec2(0.5,0.5); // autofocus point on screen (0.0,0.0 - left lower corner, 1.0,1.0 - upper right)
float maxblur = 0.75; //clamp value of max blur (0.0 = no blur,1.0 default)

float threshold = 0.7; //highlight threshold;
float gain = 13.0; //highlight gain;

float bias = 0.4; //bokeh edge bias
float fringe = 2.0; //bokeh chromatic aberration/fringing

bool noise = true; //use noise instead of pattern for sample dithering
float namount = 0.0001; //dither amount

bool depthblur = false; //blur the depth buffer?
float dbsize = 1.0; //depthblursize

/*
next part is experimental
not looking good with small sample and ring count
looks okay starting from samples = 4, rings = 4
*/

bool pentagon = false; //use pentagon as bokeh shape?
float feather = 0.4; //pentagon shape feather

//------------------------------------------


float penta(vec2 coords) //pentagonal shape
{
	float scale = float(rings) - 1.3;
	vec4  HS0 = vec4( 1.0,         0.0,         0.0,  1.0);
	vec4  HS1 = vec4( 0.309016994, 0.951056516, 0.0,  1.0);
	vec4  HS2 = vec4(-0.809016994, 0.587785252, 0.0,  1.0);
	vec4  HS3 = vec4(-0.809016994,-0.587785252, 0.0,  1.0);
	vec4  HS4 = vec4( 0.309016994,-0.951056516, 0.0,  1.0);
	vec4  HS5 = vec4( 0.0        ,0.0         , 1.0,  1.0);
	
	vec4  one = vec4( 1.0 );
	
	vec4 P = vec4((coords),vec2(scale, scale)); 
	
	vec4 dist = vec4(0.0);
	float inorout = -4.0;
	
	dist.x = dot( P, HS0 );
	dist.y = dot( P, HS1 );
	dist.z = dot( P, HS2 );
	dist.w = dot( P, HS3 );
	
	dist = smoothstep( -feather, feather, dist );
	
	inorout += dot( dist, one );
	
	dist.x = dot( P, HS4 );
	dist.y = HS5.w - abs( P.z );
	
	dist = smoothstep( -feather, feather, dist );
	inorout += dist.x;
	
	return clamp( inorout, 0.0, 1.0 );
}

float bdepth(vec2 coords) //blurring depth
{
	float d = 0.0;
	float kernel[9];
	vec2 offset[9];
	
	vec2 wh = vec2(texel.x, texel.y) * dbsize;
	
	offset[0] = vec2(-wh.x,-wh.y);
	offset[1] = vec2( 0.0, -wh.y);
	offset[2] = vec2( wh.x -wh.y);
	
	offset[3] = vec2(-wh.x,  0.0);
	offset[4] = vec2( 0.0,   0.0);
	offset[5] = vec2( wh.x,  0.0);
	
	offset[6] = vec2(-wh.x, wh.y);
	offset[7] = vec2( 0.0,  wh.y);
	offset[8] = vec2( wh.x, wh.y);
	
	kernel[0] = 1.0/16.0;   kernel[1] = 2.0/16.0;   kernel[2] = 1.0/16.0;
	kernel[3] = 2.0/16.0;   kernel[4] = 4.0/16.0;   kernel[5] = 2.0/16.0;
	kernel[6] = 1.0/16.0;   kernel[7] = 2.0/16.0;   kernel[8] = 1.0/16.0;
	
	
	for( int i=0; i<9; i++ )
	{
		float tmp = texture2D(depthTex, coords + offset[i]).r;
		d += tmp * kernel[i];
	}
	
	return d;
}


vec3 color(vec2 coords,float blur) //processing the sample
{
	vec3 col = vec3(0.0);
	
	col.r = texture2D(diffuseTex, coords + vec2(0.0,1.0)*texel*fringe*blur).r;
	col.g = texture2D(diffuseTex, coords + vec2(-0.866,-0.5)*texel*fringe*blur).g;
	col.b = texture2D(diffuseTex, coords + vec2(0.866,-0.5)*texel*fringe*blur).b;
	
	vec3 lumcoeff = vec3(0.299,0.587,0.114);
	float lum = dot(col.rgb, lumcoeff);
	float thresh = max((lum-threshold)*gain, 0.0);
	return col+mix(vec3(0.0),col,thresh*blur);
}

vec2 rand(vec2 tex, vec2 coord) //generating noise/pattern texture for dithering
{
	float noiseX = ((fract(1.0-coord.s*(tex.x/2.0))*0.25)+(fract(coord.t*(tex.y/2.0))*0.75))*2.0-1.0;
	float noiseY = ((fract(1.0-coord.s*(tex.x/2.0))*0.75)+(fract(coord.t*(tex.y/2.0))*0.25))*2.0-1.0;
	
	if (noise)
	{
		noiseX = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233))) * 43758.5453),0.0,1.0)*2.0-1.0;
		noiseY = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233)*2.0)) * 43758.5453),0.0,1.0)*2.0-1.0;
	}
	return vec2(noiseX,noiseY);
}

float linearize(vec2 range, float depth)
{
	return -range.y * range.x / (depth * (range.y - range.x) - range.y);
}

float vignette(vec2 uv)
{
	float dist = distance(uv, vec2(0.5,0.5));
//	dist = smoothstep(vignout+(fstop/vignfade), vignin+(fstop/vignfade), dist);
	return clamp(dist,0.0,1.0);
}

vec3 depthOfField(vec2 pTextureSize, vec2 pCamRange, vec2 pUV, float focalDepth, float focalLength, float fstop, float focus)
{
	CoC = focalLength*0.01f;
	texel = vec2(1.0f/pTextureSize.x, 1.0f/pTextureSize.y);
	dbsize = (1.0f/focus);

	//scene depth calculation
	float depth = 0.0f;
	
	if (depthblur)
	{
		depth = linearize(pCamRange, bdepth(pUV));
	}
	else
	{
		depth = linearize(pCamRange, texture2D(depthTex, pUV).x);
	}
	
	//focal plane calculation
	
	float fDepth = focalDepth;
	
	if (autofocus)
	{
		fDepth = linearize(pCamRange, texture2D(depthTex,focusPoint).x);
	}
	
	//dof blur factor calculation
	
	float blur = 0.0;
	
	if (manualdof)
	{    
		float a = depth-fDepth; //focal plane
		float b = (a-fdofstart)/fdofdist; //far DoF
		float c = (-a-ndofstart)/ndofdist; //near Dof
		blur = (a>0.0)?b:c;
	}
	
	else
	{
		float f = focalLength; //focal length in mm
		float d = fDepth*1000.0f; //focal plane in mm
		float o = depth*1000.0f; //depth in mm
		
		float a = (o*f)/(o-f); 
		float b = (d*f)/(d-f); 
		float c = (d-f)/(d*fstop*CoC); 
		
		blur = abs(a-b)*c;
	}
	
	blur = clamp(blur,0.0,1.0);
	
	// calculation of pattern for ditering
	
	vec2 noise = rand(pTextureSize, pUV)*namount*blur;
	
	// getting blur x and y step factor
	
	float w = (1.0/textureSize.x)*blur*maxblur+noise.x;
	float h = (1.0/textureSize.y)*blur*maxblur+noise.y;
	
	// calculation of final color
	
	vec3 col = vec3(0.0);
	
	if(blur < 0.05) //some optimization thingy
	{
		col = texture2D(diffuseTex, pUV).rgb;
	}
	
	else
	{
		col = texture2D(diffuseTex, pUV).rgb;
		float s = 1.0;
		int ringsamples;
		
		for (int i = 1; i <= rings; i += 1)
		{   
			ringsamples = i * samples;
			
			for (int j = 0 ; j < ringsamples ; j += 1)   
			{
				float step = PI*2.0 / float(ringsamples);
				float pw = (cos(float(j)*step)*float(i));
				float ph = (sin(float(j)*step)*float(i));
				float p = 1.0;
				if (pentagon)
				{ 
					p = penta(vec2(pw,ph));
				}
				col += color(pUV + vec2(pw*w,ph*h),blur)*mix(1.0,(float(i))/(float(rings)),bias)*p;  
				s += 1.0*mix(1.0,(float(i))/(float(rings)),bias)*p;   
			}
		}
		col /= s; //divide by sample count
	}
	
	if (vignetting)
	{
		col *= vignette(pUV);
	}
	
	return col;
}