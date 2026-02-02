#pragma once

#include <d3d11.h>
#include <wrl.h>
class Texture {
public:
	Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv) : srv(srv) {}

	ID3D11ShaderResourceView* GetSrv() { return this->srv.Get(); }
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

};