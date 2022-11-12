#pragma once
#include <d3d11.h>

class SwapChain;
class TexturedVertexBuffer;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context);
	void clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);

	void setVertexBuffer(VertexBuffer* vertex_buffer);
	void setTexturedVertexBuffer(TexturedVertexBuffer* vertex_buffer);
	void setIndexBuffer(IndexBuffer* index_buffer);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);

	void setVertexShader(VertexShader* vertex_shader);
	void setPixelShader(PixelShader* pixel_shader);

	void setConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* buffer);
	void setConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* buffer);

	void setDepthStencilState();

	void setWireframeRenderMode();
	void setSolidRenderMode();

	//void setPixelShaderSamplers(UINT startSlot, UINT nSamplers, ID3D11SamplerState* const* samplerState);


	void setPixelShaderSamplers(UINT startSlot, UINT nSamplers, ID3D11SamplerState* samplerState);

	void setShaderResources(UINT startSlot, UINT numViews, ID3D11ShaderResourceView* texture);

	ID3D11DeviceContext* getDeviceContext();

	bool release();
	~DeviceContext();
protected:
	ID3D11DeviceContext* m_device_context;
private:
	friend class ConstantBuffer;
};