#version 450

// World transform object
layout(binding = 0) uniform WorldTransformObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
} wto;

// Mesh data
layout(push_constant) uniform PushConst
{
    vec4 rot;
    vec3 pos;
    vec3 scale;
} pushConst;

// Vertex
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;



vec3 QuatTranformVec3(vec4 Q, vec3 V)
{
    return  Q.xyz * 2.0 * dot(Q.xyz, V) +
            V * (Q.w * Q.w - dot(Q.xyz, Q.xyz)) + 
            cross(Q.xyz, V) * 2.0 * Q.w;
}

mat4 SetMeshTransforms()
{
    vec3 x = QuatTranformVec3(pushConst.rot, vec3(1.0, 0.0, 0.0));
    vec3 y = QuatTranformVec3(pushConst.rot, vec3(0.0, 1.0, 0.0));
    vec3 z = QuatTranformVec3(pushConst.rot, vec3(0.0, 0.0, 1.0));

    x = x * pushConst.scale.x;
    y = y * pushConst.scale.y;
    z = z * pushConst.scale.z;

    vec3 p = pushConst.pos;

    return mat4(
        x.x, x.y, x.z, 0.0f,
        y.x, y.y, y.z, 0.0f,
        z.x, z.y, z.z, 0.0f,
        p.x, p.y, p.z, 1.0f
    );
}

void main()
{
    vec4 pos_mat = SetMeshTransforms() * vec4(inPosition, 1.0);
    gl_Position = wto.proj * wto.view * wto.model * pos_mat;
    // vec3 local_pos = vec3(wto.model * vec4(inPosition, 1.0));
    // vec3 world_pos = local_pos + pushConst.pos;
    // gl_Position = wto.proj * wto.view * vec4(world_pos, 1.0);

    fragColor = inColor;
    fragTexCoord = inTexCoord;
}