#version 440

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec2 fragCoord;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    vec3 iResolution;
    vec4 innerColor;
    vec4 outerColor;
    vec2 point1;
    vec2 point2;
    vec2 point3;
    vec2 point4;
    float k;
    float pointsScale;
    float smoothFactor;
    float fill;
    bool substraction;
    vec2 boxArea;
    float boxRadius;
    float circleScale;
    float inOutFactor;
};

layout(binding = 1) uniform sampler2D iSource;

// The MIT License
// Copyright © 2020 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


// Signed distance to a disk

// List of some other 2D distances: https://www.shadertoy.com/playlist/MXdSRf
//
// and iquilezles.org/articles/distfunctions2d


float sdCircle( in vec2 p, in float r )
{
    return length(p)-r;
}


float opUnion( float d1, float d2 )
{
    return min(d1,d2);
}

float opSubtraction( float d1, float d2 )
{
    return max(-d1,d2);
}

float opIntersection( float d1, float d2 )
{
    return max(d1,d2);
}

float opSmoothUnion( float d1, float d2, float k )
{
    if (k == 0) return opUnion(d1,d2);
    k *= 4.0;
    float h = max(k-abs(d1-d2),0.0);
    return min(d1, d2) - h*h*0.25/k;
}

float opSmoothSubtraction( float d1, float d2, float k )
{
    return -opSmoothUnion(d1,-d2,k);

    //float h = max(4.0*k-abs(d1+d2),0.0);
    //return max(-d1, d2) + h*h/(16.0*k);
}

float sdRoundedBox( in vec2 p, in vec2 b, in vec4 r )
{
    r.xy = (p.x>0.0)?r.xy : r.zw;
    r.x  = (p.y>0.0)?r.x  : r.y;
    vec2 q = abs(p)-b+r.x;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r.x;
}


void main() {
    fragColor = texture(iSource, texCoord);
    {
    	vec2 p = (2.0 * fragCoord - iResolution.xy) / iResolution.y;

    	float d = sdCircle(p, circleScale);
        float point1 = sdCircle(p - point1, pointsScale);
        float point2 = sdCircle(p - point2, pointsScale);
        float point3 = sdCircle(p - point3, pointsScale);
        float point4 = sdCircle(p - point4, pointsScale);
        float squareZone = sdRoundedBox(p, boxArea, vec4(boxRadius));
        bool inner;

        if (substraction) {
            d = opSmoothSubtraction(point1, d, k);
            d = opSmoothSubtraction(point2, d, k);
            d = opSmoothSubtraction(point3, d, k);
            d = opSmoothSubtraction(point4, d, k);
            d = opSmoothSubtraction(-squareZone, d, k);
            inner = d < -fill;
            d = inner ? -d - (fill * 2.0): d;
        } else {
            d = opSmoothSubtraction(-squareZone, d, k);
            inner = d < -fill;
            d = inner ? -d - (fill * 2.0): d;

            float points = opSmoothUnion(point2, point1, k);
            points = opSmoothUnion(point3, points, k);
            points = opSmoothUnion(point4, points, k);
            points = opSmoothSubtraction(-squareZone, points, k);
            d = opSmoothUnion(points, d, k);
            d = opSubtraction(-squareZone, d);
        }

        float fac = smoothstep(0, smoothFactor, -d);
        fragColor = mix(fragColor, mix(innerColor, outerColor, -d * inOutFactor), fac);
    }
    fragColor = fragColor * qt_Opacity;
}
