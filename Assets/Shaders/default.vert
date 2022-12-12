#version 450 core
		layout(location = 0) in vec4 vPos;
		layout(location = 1) in vec3 vNormal;
		layout(location = 2) in vec3 vTangent;
		layout(location = 3) in vec3 vBinormal;
		layout(location = 4) in vec3 vColor;
		layout(location = 5) in vec4 vCustomData;
		layout(location = 6) in vec2 texCoord0;
		layout(location = 7) in vec2 texCoord1;
		layout(location = 8) in vec2 texCoord2;
		layout(location = 9) in vec2 texCoord3;
		layout(location = 10) in vec2 texCoord4;
		layout(location = 11) in vec2 texCoord5;
		layout(location = 12) in vec2 texCoord6;
		layout(location = 13) in vec2 texCoord7;
		layout (std140, binding = 0) uniform CameraUniformData
		{
			mat4 viewMatrix;
			mat4 viewProjectionMatrix;
			vec4 position;
			vec4 NearFar;
		};
		layout (std140, binding = 1) uniform OrthoProjectionMatrix
		{
			mat4 orthoProjectionMatrix;
		};
		layout (std140, binding = 2) uniform ObjectMatrices
		{
			mat4 object2World;
		};
		layout (std140, binding = 3) uniform ObjectMatricesIT
		{
			mat4 object2WorldIT;
		};

		out vec3 normal;
		out vec3 tangent;
		out vec3 binormal;
		out vec3 color;
		out vec4 customData;
		out vec2 uv0;
		out vec2 uv1;
		out vec2 uv2;
		out vec2 uv3;
		out vec2 uv4;
		out vec2 uv5;
		out vec2 uv6;
		out vec2 uv7;
		void main()
		{
		    gl_Position = viewProjectionMatrix * object2World * vPos;
		    normal = vNormal;
		    tangent = vTangent;
		    binormal = vBinormal;
		    color = vColor;
			customData = vCustomData;
		    uv0 = texCoord0;
		    uv1 = texCoord1;
		    uv2 = texCoord2;
		    uv3 = texCoord3;
		    uv4 = texCoord4;
		    uv5 = texCoord5;
		    uv6 = texCoord6;
		    uv7 = texCoord7;
		};