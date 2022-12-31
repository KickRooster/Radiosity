attribute vec4 vPos;
attribute vec3 vNormal;
attribute vec3 vTangent;
attribute vec3 vBinormal;
attribute vec3 vColor;
attribute vec4 vCustomData;
attribute vec4 vScaledUV1Position;
attribute vec2 texCoord0;
attribute vec2 texCoord1;
attribute vec2 texCoord2;
attribute vec2 texCoord3;
attribute vec2 texCoord4;
attribute vec2 texCoord5;
attribute vec2 texCoord6;
attribute vec2 texCoord7;

varying vec4 CustomData;

uniform mat4 object2World;

void main()
{
	rl_Position = object2World * vPos ;
	CustomData = vCustomData;
}