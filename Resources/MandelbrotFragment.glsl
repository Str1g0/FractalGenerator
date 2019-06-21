#version 400 core

#define MANDELBROT false
#define JULIA	   true

in VS_OUT
{
	vec2 UV;

} fsInput;

uniform bool			u_fractalType;
uniform unsigned int	u_maxIter;
uniform float			u_zoom;
uniform vec2			u_offset;
uniform uvec2			u_canvas;
uniform vec3			u_colorModifier;
uniform vec2			u_juliaConstant;

out vec4 PixelColor;

float sq(float x)
{
	return x*x;
}

vec2 ComplexSquare(vec2 z)
{
	vec2 squared;
	squared.x = sq(z.x) - sq(z.y);
	squared.y = 2 * z.x * z.y;
	return squared;
}

float ComplexAbsolute(vec2 z)
{
	return sqrt(sq(z.x) + sq(z.y));
}

float NextComplexAbsolute(vec2 z)
{
	return sq(z.x) + sq(z.y);
}

vec3 HSBtoRGB(vec3 hsb)
{
	

	return vec3(0.f);

}

vec3 Coloring(float iteration)
{
	return vec3(0.f, iteration * 1.f / u_maxIter * 1.2, iteration * 1.6f/u_maxIter * 2.1);
}

vec3 LinearizeColor(vec2 complex, 
					unsigned int iteration)
{
	float logZn  = log(NextComplexAbsolute(complex)) / 2.;
	float offset = log(logZn/log(2.)) / log(2.);

	float iter2 = float(iteration) + 1. - offset;

	vec3 color1 = Coloring(iteration);
	vec3 color2 = Coloring(iter2);

	vec3 newColor = mix(color1, color2, iter2 - int(iter2));
	 
	return newColor;
}

void main(void)
{
	const vec4	k_setColor			= vec4(0.f, 0.f, 0.f, 1.f);
	const float k_limitThreshold	= 6.f;
	const float k_colorThreshold	= 2.f;
	const vec2	k_julia				= vec2(0.285f, 0.01f);

	vec2 point = fsInput.UV;

	point	*= u_zoom;
	point.x	*= float(u_canvas.x) / float(u_canvas.y);

	point   += u_offset;

	vec2 z = u_fractalType == MANDELBROT ? vec2(0.f) : point;

	unsigned int iteration = 0u;

	float smoothColor;
	vec3  linearized;

	for(; iteration < u_maxIter; ++iteration)
	{
		if(NextComplexAbsolute(z) > k_limitThreshold)
			break;

		if(u_fractalType == MANDELBROT)
		{
			z = ComplexSquare(z) + point;
		
			if(NextComplexAbsolute(z) > k_colorThreshold)
			{
				//smoothColor += (iteration + 1 - log(log(ComplexAbsolute(z))) / log(2.f));
				linearized  = LinearizeColor(z, iteration);
			}
		}
		else if(u_fractalType == JULIA)
		{
			z = ComplexSquare(z) + u_juliaConstant;

			if(NextComplexAbsolute(z) > k_colorThreshold)
			{
				//smoothColor += exp(-ComplexAbsolute(z));
				linearized  = LinearizeColor(z, iteration);
			}
		}

	}

	PixelColor.xyz	= linearized;
	PixelColor.a	= 1.f;
}