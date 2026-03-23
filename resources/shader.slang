[[vk::binding(0, 0)]]
RWStructuredBuffer<float4> buffer;

static constexpr uint WIDTH = 48;
static constexpr uint HEIGHT = 96;
static constexpr uint DEPTH = 32;

[shader("compute")]
[numthreads(8, 8, 4)]
void compute(uint3 dispatchThreadID: SV_DispatchThreadID)
{
    uint index = dispatchThreadID.x + dispatchThreadID.y * WIDTH + dispatchThreadID.z * WIDTH * HEIGHT;

    buffer[index] = float4(
        dispatchThreadID.x,
        dispatchThreadID.y,
        dispatchThreadID.z,
        1.0
    );
}
