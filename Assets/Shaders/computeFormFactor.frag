		#version 450 core
	
		in vec4 pos;
		in vec3 normal;
		in vec3 tangent;
		in vec3 binormal;
		in vec3 color;
		in vec4 customData;
		in vec2 uv0;
		in vec2 uv1;
		in vec2 uv2;
		in vec2 uv3;
		in vec2 uv4;
		in vec2 uv5;
		in vec2 uv6;
		in vec2 uv7;
		uniform sampler2D albedoSampler;
		uniform sampler2D IDSampler;
		layout (std140, binding = 0) uniform CameraUniformData
		{
			mat4 viewMatrix;
			mat4 perspectiveProjectionMatrix;
			mat4 orthoProjectionMatrix;
			vec4 position;
			vec4 NearFar;
		};
		layout (std140, binding = 1) uniform ShooterInfo
		{
			vec4 ShooterPosition;
			vec4 ShooterNormal;
			vec4 ShooterEnergy;
			vec4 ShooterSurfaceArea;
		};
		layout (std140, binding = 2) uniform ObjectMatrices
		{
			mat4 object2World;
		};
		layout (std140, binding = 3) uniform ObjectMatricesIT
		{
			mat4 object2WorldIT;
		};

		layout(location = 0) out vec4 attch0;
		layout(location = 1) out vec4 attch1;

		void main()
		{
			vec4 RecvPos = object2World * pos;
			vec3 r = ShooterPosition.xyz - RecvPos.xyz;
			float distance2 = dot(r, r);
			vec4 Normal4 = vec4(normal.x, normal.y, normal.z, 1.0f);
			vec4 WorldNormal4 = object2WorldIT * Normal4;
			vec3 WorldNormal = normalize(WorldNormal4.xyz);
            float cosi = dot(WorldNormal, normalize(r));    
            float cosj = -dot(ShooterNormal.xyz, normalize(r)); 
			const float pi = 3.1415926535;
			float Fij = max(cosi * cosj, 0) / (pi * distance2 + ShooterSurfaceArea.x);

			vec4 ViewPos = viewMatrix * RecvPos;
			vec3 proj = normalize(ViewPos.xyz);    
			proj.xy = proj.xy * 0.5 + 0.5;

			vec3 ID = texture(IDSampler, proj.xy).xyz;

			float Visable;
			if (customData.x == ID.x)
			{
				Visable = 1.0f;
			} 
			else
			{
				Visable = 0;
			}

			//Fij *= Visable;

			vec3 albedo = texture(albedoSampler, uv0).xyz;

			vec3 Delta = ShooterEnergy.xyz * albedo * ShooterSurfaceArea.x * Fij;   

			attch0.xyz = Delta * vec3(30);
			attch0.w = 1.0;

			attch1.xyz = ID;
			
			attch1.w = 1.0;
		};