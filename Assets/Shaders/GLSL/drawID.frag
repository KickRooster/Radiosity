		#version 450 core
		
		in vec4 pos;
		in vec3 normal;
		in vec3 tangent;
		in vec3 binormal;
		in vec3 color;
		flat in vec4 customData;
		in vec2 uv0;
		in vec2 uv1;
		in vec2 uv2;
		in vec2 uv3;
		in vec2 uv4;
		in vec2 uv5;
		in vec2 uv6;
		in vec2 uv7;
		
		layout (std140, binding = 0) uniform CameraUniformData
		{
			mat4 viewPerspectiveProjectionMatrix;
			mat4 viewOrthoProjectionMatrix;
			vec4 position;
			vec4 NearFar;
			ivec4 FrameCount;
		};
		layout (std140, binding = 1) uniform CubeMatrices
		{
			mat4 ViewProjection_Positive_X;
			mat4 ViewProjection_Negative_X;
			mat4 ViewProjection_Positive_Y;
			mat4 ViewProjection_Negative_Y;
			mat4 ViewProjection_Positive_Z;
			mat4 ViewProjection_Negative_Z;
		};
		layout (std140, binding = 2) uniform ShooterInfo
		{
			vec4 ShooterPosition;
			vec4 ShooterNormal;
			vec4 ShooterEnergy;
			vec4 ShooterSurfaceArea;
		};
		layout (std140, binding = 3) uniform ObjectMatrices
		{
			mat4 object2World;
		};
		layout (std140, binding = 4) uniform ObjectMatricesIT
		{
			mat4 object2WorldIT;
		};

		layout(location = 0) out vec4 attch0;
		layout(location = 1) out vec4 attch1;

		void main()
		{
			vec4 ProjectedPos = viewPerspectiveProjectionMatrix * object2World * pos;
			ProjectedPos.xyz /= ProjectedPos.w;
			ProjectedPos.z *= 0.5;
			ProjectedPos.z += 0.5;
			attch0.xyz = vec3(customData.x, ProjectedPos.z, customData.x);
			attch0.w = 1.0;
		};