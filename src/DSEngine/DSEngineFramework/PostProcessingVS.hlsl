struct VertexToPixel
{
	float4 position : SV_POSITION;
	float2 uv		: TEXCOORD;
};

VertexToPixel main(uint id : SV_VERTEXID)
{
	VertexToPixel output;// Calculate the UV (0,0 to 2,2) via the ID
						 // x = 0, 2, 0, 2, etc.
						 // y = 0, 0, 2, 2, etc.
	output.uv = float2((id << 1) & 2, id & 2);
	// Convert uv to the (-1,1 to 3,-3) range for position
	output.position = float4(output.uv, 0, 1);
	output.position.x = output.position.x * 2 - 1;
	output.position.y = output.position.y * -2 + 1;
	return output;
}