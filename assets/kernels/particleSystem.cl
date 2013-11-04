typedef struct Particle
{
	float4 pos;
	float4 color;
	float4 vel;
	float4 velChange;
	float4 colorChange;
	float sizeChange;
	float life;
	float size;
} Particle;

__kernel void Update (
	// OpenGL data
	__global float4* position,
	__global float4* color,
	__global float* size,

	// User data
	__global float4* velocity,
	__global float4* accel,
	__global float4* colorChange,
	__global float* sizeChange,
	__global float* lifetime,

	// Misc
	float dt/*,
	float4 gravity*/
	)
{
	unsigned int i = get_global_id(0);

	if(lifetime[i] <= 0.0f)
	{
		position[i] = 0.0f;
		return;
	}

	printf("Updating life: %f\n", lifetime[i]);
	printf("Updating pos: %v4f\n", position[i]);

	velocity[i] += (accel[i]/* + gravity*/) * dt;
	color[i] = color[i] + colorChange[i] * dt;
	size[i] = size[i] + sizeChange[i] * dt;
	position[i] = position[i] + velocity[i] * dt;
	lifetime[i] -= dt;
}

__kernel void Emit (
	// OpenGL data
	__global float4* position,
	__global float4* color,
	__global float* size,

	// User buffers
	__global float4* velocity,
	__global float4* accel,
	__global float4* colorChange,
	__global float* sizeChange,
	__global float* lifetime,

	unsigned int idx,

	// Initial Data
	__constant struct Particle* init,

	unsigned int particleCount
	)
{
	unsigned int i = 0;
	for(; i < particleCount; ++i)
	{
		if(lifetime[i] <= 0.0f)
		{
			idx = i;
			break;
		}
	}

	printf("\n[Gpu]\nIndex: %i\n", idx);

	lifetime[idx] = init->life;

	// Allocate data
	position[idx] = init->pos;
	printf("Position: %v4f\n", position[idx]);
	color[idx] = init->color;
	size[idx] = init->size;
	velocity[idx] = init->vel;
	accel[idx] = init->velChange;
	colorChange[idx] = init->colorChange;
	sizeChange[idx] = init->sizeChange;
}