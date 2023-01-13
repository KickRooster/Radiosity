		#version 450 core
	
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
		uniform vec4 albedoColor;
		uniform sampler2D lightmapSampler;
		layout (std140, binding = 0) uniform CameraUniformData
		{
			mat4 viewPerspectiveProjectionMatrix;
			mat4 viewOrthoProjectionMatrix;
			vec4 position;
			vec4 NearFar;
			ivec4 FrameCount;
			ivec4 RGBMEncoding;
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

		out vec4 out_Color;

		void main()
		{
			//out_Color.xyz = texture(lightmapSampler, uv1).xyz;// albedoColor.xyz;
			
			int ID = int(customData.x);

			if (ID % 2 == 0)
			{
				out_Color.xyz = albedoColor.xyz;
			}
			else
			{
				out_Color.xyz = vec3(0, 0, 0);
			}
			
			out_Color.w = 1.0;
		};