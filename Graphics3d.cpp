#include "Graphics3d.h"

namespace Darkness {
#define INLINED 
	unsigned int Graphics::PixelsX;//X����������
	unsigned int Graphics::PixelsY;//Y����������
	unsigned int Graphics::mmX;//��ʾ��X�����С����λ�����ף�
	unsigned int Graphics::mmY;//��ʾ��Y�����С����λ�����ף�
	using namespace DirectX;
	XMVECTOR XMMultiply(const XMMATRIX& b, const XMVECTOR& a)
	{
		XMVECTOR Res = {};
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Res.m128_f32[i] += b.r[i].m128_f32[j] * a.m128_f32[j];
			}
		}
		return Res;
	}

	INLINED XMFLOAT4 XMMultiply(const XMFLOAT4& a, const XMFLOAT4X4& b)
	{
		XMFLOAT4 Res = {
			a.x * b._11 + a.y * b._21 + a.z * b._31 + a.w * b._41,
			a.x * b._12 + a.y * b._22 + a.z * b._32 + a.w * b._42,
			a.x * b._13 + a.y * b._23 + a.z * b._33 + a.w * b._43,
			a.x * b._14 + a.y * b._24 + a.z * b._34 + a.w * b._44
		};
		return Res;
	}

	XMVECTOR MakePlane(XMFLOAT3 Point[3]) {
		XMVECTOR PointPos[3] = {
			XMLoadFloat3(&Point[0]),
			XMLoadFloat3(&Point[1]),
			XMLoadFloat3(&Point[2])
		};
		XMVECTOR NormalVector = XMVector3Cross(PointPos[2] - PointPos[0], PointPos[1] - PointPos[0]);
		NormalVector.m128_f32[3] = 0;
		if (XMVector3Length(NormalVector).m128_f32[0] == 0)
		{
#ifdef _DEBUG
			throw;
#else
			XMVECTOR Temp = XMVector3Orthogonal(PointPos[1] - PointPos[0]);
			XMFLOAT4 Res;
			XMStoreFloat4(&Res, Temp);
			Res.w = 1;
			return Res;
#endif // _DEBUG
		}
		NormalVector = XMVector3Normalize(NormalVector);
		NormalVector.m128_f32[3] = -XMVector3Dot(NormalVector, PointPos[0]).m128_f32[0];
		return NormalVector;
	}

	float PlanePointDistance(XMVECTOR Normal, XMFLOAT3 Point) {
		return abs(XMVector4Dot(Normal, XMLoadFloat3(&Point)).m128_f32[0]) / XMVector3Length(Normal).m128_f32[0];
	}

	__stdcall Object3D::Object3D(
		const std::initializer_list<VertexDetail>& VertexList,
		const std::initializer_list<UINT32>& IndexList,
		HitBoxType HBT
	) : VertexData(VertexList.begin(), VertexList.end()),
		IndexData(IndexList.begin(), IndexList.end()),
		HB(HBT),
		VertexCount(VertexList.size()),
		IndexCount(IndexList.size())
	{
		this->HBParam[0] = VertexList.begin()->position.x;
		this->HBParam[1] = VertexList.begin()->position.y;
		this->HBParam[2] = VertexList.begin()->position.z;
		this->HBParam[3] = VertexList.begin()->position.x;
		this->HBParam[4] = VertexList.begin()->position.y;
		this->HBParam[5] = VertexList.begin()->position.z;
		this->UpdateHitBox();
	}

	void __stdcall Object3D::UpdateHitBox() {
		if (this->HB == Cube)
		{
			for (auto const& vertex : VertexData) {
				if (vertex.position.x < this->HBParam[0]) {
					this->HBParam[0] = vertex.position.x;
				}
				else if (vertex.position.x > this->HBParam[3]) {
					this->HBParam[3] = vertex.position.x;
				}
				if (vertex.position.y < this->HBParam[1]) {
					this->HBParam[1] = vertex.position.y;
				}
				else if (vertex.position.y > this->HBParam[4]) {
					this->HBParam[4] = vertex.position.y;
				}
				if (vertex.position.z < this->HBParam[2]) {
					this->HBParam[2] = vertex.position.z;
				}
				else if (vertex.position.z > this->HBParam[5]) {
					this->HBParam[5] = vertex.position.z;
				}
			}
		}
		else if (this->HB == Ball)
		{
			throw;
		}
	}

	bool __stdcall Object3D::Collision(const Object3D& that)const {
		if (AllEqual({ this->HB,that.HB,HitBoxType::Ball })) {
			if (
				XMVector3Length(
					XMVECTOR({ this->HBParam[0], this->HBParam[1], this->HBParam[2], 1 })
					-
					XMVECTOR({ that.HBParam[0], that.HBParam[1], that.HBParam[2], 1 })
				).m128_f32[0] >
				(CrashDistance + this->HBParam[3] + that.HBParam[3])
				)
			{
				return false;
			}
		}
		else if (AllEqual({ this->HB,that.HB,HitBoxType::Cube })) {
			if (
				(this->HBParam[3] < that.HBParam[0]) || (this->HBParam[0] > that.HBParam[3])
				||
				(this->HBParam[4] < that.HBParam[1]) || (this->HBParam[1] > that.HBParam[4])
				||
				(this->HBParam[5] < that.HBParam[2]) || (this->HBParam[2] > that.HBParam[5])
				)
			{
				return false;
			}
		}
		//More check
		{
			throw;
		}
	}


	INLINED HRESULT Graphics::Init(HWND hwnd)
	{
		HDC hdc = GetDC(hwnd);
		Width = PixelsX = GetSystemMetrics(SM_CXSCREEN);
		Height = PixelsY = GetSystemMetrics(SM_CYSCREEN);
		if (PixelsX == 0 || PixelsY == 0)
		{
			throw GraphicsException("Failed to get the size of the screen.");
		}
		mmX = GetDeviceCaps(hdc, HORZRES);
		mmY = GetDeviceCaps(hdc, VERTRES);

		ViewPort = CD3DX12_VIEWPORT(
			0.0f,
			0.0f,
			static_cast<float>(Width),
			static_cast<float>(Height)
		);
		ScissorRect = CD3DX12_RECT(
			0,
			0,
			static_cast<LONG>(Width),
			static_cast<LONG>(Height)
		);

		try
		{
			CreateFactory(hwnd);
			CreateDevice();
			CreateDirectCommandQueue();
			CreateSwapChain(hwnd, RenderTargetFormat);

			CreateRootSignatureDescriptor();
			CreateDirectCommandAllocator();
			CreateDirectCommandList();


			CreateComputeCommandAllocator();
			CreateComputeCommandQueue();
			CreateComputeCommandList();

			CreateUploadHeap();

			CreateCBV_SRV_UAV_HeapDescHeap();
			//Render target
			CreateRTV_HeapDescHeap();
			CreateRTV_Handle();
			//VertexDetail buffer
			CreateVertexBufferHeap();
			CreateVertexBuffer();

			//Index buffer
			CreateIndexBufferHeap();
			CreateIndexBuffer();

			//Constant buffer
			CreateConstantBuffer();

			CreateUAVforCS_Output();
			CreateSRVforCS_Input();

			CreateCS_ResultHeap();
			CreateCS_ResultResource();
			CreateCS_ResultSRV_Handle();

			CreateFence();
			CreateFenceEvent();


			UINT IndexCount = LoadIndexBuffer();
			UINT VertexCount = LoadVertexBuffer();

			CreateGraphicsPipeLine(L"Shader.hlsl", L"Shader.hlsl");
			CreateComputePipeLine(L"Compute.hlsl", VertexCount, IndexCount);
			CreateFakeVertexBufferAndLoad(
				{

				{ { -BorderPos, -BorderPos, 0.0f}},	// Bottom left.

				{ { -BorderPos, BorderPos, 0.0f}},	// Top left.

				{ { BorderPos, -BorderPos, 0.0f }},	// Bottom right.

				{ { BorderPos, BorderPos, 0.0f}},		// Top right.
				},
				{
					 { BorderUV[0], BorderUV[1] }, { BorderUV[0], BorderUV[0] } , { BorderUV[1], BorderUV[1] }, { BorderUV[1], BorderUV[0] }
				});

			ReleaseDC(hwnd, hdc);
		}
		catch (const HrException& ex)
		{
			MessageBoxA(hwnd, ex.what(), "DirectX initializing error", MB_OK);
			return E_FAIL;
		}

		CreateVertexBufferView();
		CreateIndexBufferView();
		CreateCBV_Handle();
		return S_OK;
	}

	// Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
	// If no such adapter can be found, *ppAdapter will be set to nullptr.
	INLINED void __stdcall Graphics::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
	{
		ComPtr<IDXGIAdapter1> adapter;
		*ppAdapter = nullptr;

		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				// If you want W software adapter, pass in "/warp" on the command line.
				continue;
			}

			// Check to see if the adapter supports Direct3D 12, but don't create the
			// actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), FeatureLevel, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}
		*ppAdapter = adapter.Detach();
	}

	INLINED void __stdcall Graphics::CreateFactory(HWND hwnd) {
		UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
		// Enable the debug layer (requires the Graphics Tools "optional feature").
		// NOTE: Enabling the debug layer after device creation will invalidate the active device.
		{
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
				
				// Enable additional debug layers.
				dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}
		}
#endif
		ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&Factory)));
		try
		{
			ThrowIfFailed(Factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));
		}
		catch (const HrException& e)
		{
			throw e;
		}
	}

	INLINED void __stdcall Graphics::CreateDevice() {
		if (UseWarpDevice)
		{
			ComPtr<IDXGIAdapter> warpAdapter;
			ThrowIfFailed(Factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

			try
			{
				ThrowIfFailed(D3D12CreateDevice(
					warpAdapter.Get(),
					FeatureLevel,
					IID_PPV_ARGS(&Device)
				));
			}
			catch (const HrException& e)
			{
				throw e;
			}
		}
		else
		{
			ComPtr<IDXGIAdapter1> hardwareAdapter;
			GetHardwareAdapter(Factory.Get(), &hardwareAdapter);
			
			try
			{
				ThrowIfFailed(D3D12CreateDevice(
					hardwareAdapter.Get(),
					FeatureLevel,
					IID_PPV_ARGS(&Device)
				));
			}
			catch (const HrException& e)
			{
				throw e;
			}
		}
	}

	INLINED void __stdcall Graphics::CreateDirectCommandQueue() {
		D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		try
		{
			ThrowIfFailed(Device->CreateCommandQueue(
				&QueueDesc,
				IID_PPV_ARGS(&DirectCommandQueue))
			);
		}
		catch (const HrException& e)
		{
			throw e;
		}
	}

	INLINED void __stdcall Graphics::CreateSwapChain(
		HWND hwnd,
		DXGI_FORMAT Format
	) {
		DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
		SwapChainDesc.BufferCount = FrameCount;
		SwapChainDesc.Width = Width;
		SwapChainDesc.Height = Height;
		SwapChainDesc.Format = Format;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.SampleDesc.Count = 1;
		
		try
		{
			ThrowIfFailed(Factory->CreateSwapChainForHwnd(
				DirectCommandQueue.Get(),
				hwnd,
				&SwapChainDesc,
				nullptr,
				nullptr,
				&SwapChain1
			));
		}
		catch (const HrException& e)
		{
			throw e;
		}
		try
		{
			ThrowIfFailed(SwapChain1.As(&SwapChain3));
		}
		catch (const HrException& e)
		{
			throw e;
		}
		FrameIndex = SwapChain3->GetCurrentBackBufferIndex();
	}

	INLINED void __stdcall Graphics::CreateUploadHeap() {
		D3D12_HEAP_DESC UploadHeapDesc = {
				UploadHeapSize,
				{D3D12_HEAP_TYPE_UPLOAD,D3D12_CPU_PAGE_PROPERTY_UNKNOWN,D3D12_MEMORY_POOL_UNKNOWN},
				D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
				D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS
		};
		try
		{
			ThrowIfFailed(Device->CreateHeap(
				&UploadHeapDesc,
				IID_PPV_ARGS(&pUploadHeap))
			);
		}
		catch (const HrException& e)
		{
			throw e;
		}
	}

	INLINED void __stdcall Graphics::CreateRTV_HeapDescHeap() {
		D3D12_DESCRIPTOR_HEAP_DESC RTV_DescHeapDesc = {};
		RTV_DescHeapDesc.NumDescriptors = FrameCount;
		RTV_DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		RTV_DescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		try
		{
			ThrowIfFailed(Device->CreateDescriptorHeap
			(&RTV_DescHeapDesc,
				IID_PPV_ARGS(&RTV_DescHeap))
			);
		}
		catch (const HrException& e)
		{
			throw e;
		}
		//�õ�ÿ��������Ԫ�صĴ�С
		RTV_DescriptorSize = Device->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV
		);
	}

	INLINED void __stdcall Graphics::CreateRTV_Handle() {
		CD3DX12_CPU_DESCRIPTOR_HANDLE RTV_Handle(RTV_DescHeap->GetCPUDescriptorHandleForHeapStart());
		D3D12_RENDER_TARGET_VIEW_DESC RTV_Desc = {};
		RTV_Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		RTV_Desc.Format = RenderTargetFormat;
		for (UINT i = 0; i < FrameCount; i++)
		{
			ThrowIfFailed(SwapChain3->GetBuffer(i, IID_PPV_ARGS(&RenderTargets[i])));
#ifdef _DEBUG
			ThrowIfFailed(RenderTargets[i]->SetName(L"RenderTarget"));
#endif // _DEBUG

			Device->CreateRenderTargetView(
				RenderTargets[i].Get(),
				&RTV_Desc,
				RTV_Handle
			);
			RTV_Handle.Offset(1, RTV_DescriptorSize);
		}
	}

	INLINED void __stdcall Graphics::CreateVertexBufferHeap() {
		D3D12_HEAP_DESC VertexBufferHeapDesc = {
				FLOOR(VertexBufferSize, static_cast<UINT>(256)),
				{D3D12_HEAP_TYPE_DEFAULT,D3D12_CPU_PAGE_PROPERTY_UNKNOWN,D3D12_MEMORY_POOL_UNKNOWN},
				D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
				D3D12_HEAP_FLAG_NONE
		};
		ThrowIfFailed(
			Device->CreateHeap(&VertexBufferHeapDesc,
				IID_PPV_ARGS(&pVertexBufferHeap)
			)
		);
	}

	INLINED void __stdcall Graphics::CreateVertexBuffer() {
		D3D12_RESOURCE_DESC VertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(FLOOR(VertexBufferSize, 256U));
		VertexBufferDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		ThrowIfFailed(Device->CreatePlacedResource(
			pVertexBufferHeap.Get(),
			0,
			&VertexBufferDesc,
			D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,//D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			nullptr,
			IID_PPV_ARGS(&VertexBufferResource)));
#ifdef _DEBUG
		ThrowIfFailed(VertexBufferResource->SetName(L"VrtxBfrRsrc"));
#endif // _DEBUG

	}

	INLINED void __stdcall Graphics::CreateConstantBuffer() {
		D3D12_RESOURCE_DESC ConstantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(UploadHeapSize);
		ThrowIfFailed(Device->CreatePlacedResource(
			pUploadHeap.Get(),
			0,
			&ConstantBufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&ConstantBuffer)));
#ifdef _DEBUG
		ThrowIfFailed(ConstantBuffer->SetName(L"CstBfr"));
#endif // _DEBUG

	}

	INLINED void __stdcall Graphics::CreateCBV_SRV_UAV_HeapDescHeap() {
		D3D12_DESCRIPTOR_HEAP_DESC CBV_SRV_UAV_DescHeapDesc;
		CBV_SRV_UAV_DescHeapDesc.NumDescriptors = DESC_NUM;
		CBV_SRV_UAV_DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		CBV_SRV_UAV_DescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		CBV_SRV_UAV_DescHeapDesc.NodeMask = 0x00000000;

		ThrowIfFailed(Device->CreateDescriptorHeap(
			&CBV_SRV_UAV_DescHeapDesc,
			IID_PPV_ARGS(&CBV_SRV_UAV_DescHeap))
		);
		//�õ�ÿ��������Ԫ�صĴ�С
		CBV_SRV_UAV_DescriptorSize = Device->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);
	}

	INLINED void __stdcall Graphics::CreateCBV_Handle()noexcept {
		D3D12_CONSTANT_BUFFER_VIEW_DESC CBV_Desc = {};
		CBV_Desc.BufferLocation = ConstantBuffer->GetGPUVirtualAddress();
		CBV_Desc.SizeInBytes = FLOOR(ConstantBufferSize,256U);

		CD3DX12_CPU_DESCRIPTOR_HANDLE CBV_DescHandle(
			CBV_SRV_UAV_DescHeap->GetCPUDescriptorHandleForHeapStart(),
			CONSTANT_BUFFER,
			CBV_SRV_UAV_DescriptorSize);

		Device->CreateConstantBufferView(
			&CBV_Desc,
			CBV_DescHandle
		);
	}
	
	INLINED void __stdcall Graphics::CreateCS_ResultHeap() {
		D3D12_HEAP_DESC RTBufferHeapDesc = {
				FLOOR(RT_BufferSize(), static_cast<UINT64>(256)),
				{D3D12_HEAP_TYPE_CUSTOM,D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,D3D12_MEMORY_POOL_L0},
				//{D3D12_HEAP_TYPE_DEFAULT,D3D12_CPU_PAGE_PROPERTY_UNKNOWN,D3D12_MEMORY_POOL_UNKNOWN},
				D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
				D3D12_HEAP_FLAG_NONE
		};
		ThrowIfFailed(Device->CreateHeap(&RTBufferHeapDesc, IID_PPV_ARGS(&RT_ResultHeap)));
	}

	INLINED void __stdcall Graphics::CreateCS_ResultResource() {
		D3D12_RESOURCE_DESC RTBufferDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			RenderTargetFormat, Width, Height);
		RTBufferDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		RTBufferDesc.DepthOrArraySize = 1;
		ThrowIfFailed(Device->CreatePlacedResource(
			RT_ResultHeap.Get(),
			0,
			&RTBufferDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			nullptr,
			IID_PPV_ARGS(&CS_ResultResource)));
#ifdef _DEBUG
		ThrowIfFailed(CS_ResultResource->SetName(L"RT"));
#endif // _DEBUG
		
	}

	INLINED void __stdcall Graphics::CreateCS_ResultSRV_Handle()noexcept {
		D3D12_SHADER_RESOURCE_VIEW_DESC Desc = {};
		//Desc.Buffer.StructureByteStride = sizeof(RenderTargetFormat);
		//Desc.Buffer.FirstElement = 0;
		//Desc.Buffer.NumElements = Width * static_cast<UINT64>(Height);
		Desc.Format = RenderTargetFormat;
		Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		Desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		Desc.Texture2D.MipLevels = UINT_MAX;
		Desc.Texture2D.PlaneSlice = 0;
		CD3DX12_CPU_DESCRIPTOR_HANDLE SRV_DescHandle(
			CBV_SRV_UAV_DescHeap->GetCPUDescriptorHandleForHeapStart(),
			IMAGE_IN,
			CBV_SRV_UAV_DescriptorSize);
		Device->CreateShaderResourceView(
			CS_ResultResource.Get(),
			&Desc,
			SRV_DescHandle
		);
	}

	INLINED void __stdcall Graphics::CreateUAVforCS_Output(
	)noexcept {
		D3D12_UNORDERED_ACCESS_VIEW_DESC UAV_Desc = {};
		UAV_Desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		UAV_Desc.Format = RenderTargetFormat;
		UAV_Desc.Texture2D.PlaneSlice = 0;
		UAV_Desc.Texture2D.MipSlice = 0;


		CD3DX12_CPU_DESCRIPTOR_HANDLE UAV_DescHandle(
			CBV_SRV_UAV_DescHeap->GetCPUDescriptorHandleForHeapStart(),
			IMAGE_OUT,
			CBV_SRV_UAV_DescriptorSize);
		Device->CreateUnorderedAccessView(
			CS_ResultResource.Get(),
			nullptr,
			&UAV_Desc,
			UAV_DescHandle
		);
	}

	INLINED void __stdcall Graphics::CreateSRVforCS_Input(
	)noexcept {
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC SRV_Desc = {};
			SRV_Desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			SRV_Desc.Buffer.FirstElement = 0;
			for (auto const& Object : ObjectList)
			{
				SRV_Desc.Buffer.NumElements += Object.VertexCount;
			}
			SRV_Desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			SRV_Desc.Buffer.StructureByteStride = sizeof(VertexDetail);
			SRV_Desc.Format = DXGI_FORMAT_UNKNOWN;
			SRV_Desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			CD3DX12_CPU_DESCRIPTOR_HANDLE SRV_DescHandle(
				CBV_SRV_UAV_DescHeap->GetCPUDescriptorHandleForHeapStart(),
				VERTEX,
				CBV_SRV_UAV_DescriptorSize);
			Device->CreateShaderResourceView(
				VertexBufferResource.Get(),
				&SRV_Desc,
				SRV_DescHandle
			);
		}
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC SRV_Desc = {};
			SRV_Desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			SRV_Desc.Buffer.FirstElement = 0;
			for (auto const& Object : ObjectList)
			{
				SRV_Desc.Buffer.NumElements += Object.IndexCount;
			}
			SRV_Desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			SRV_Desc.Buffer.StructureByteStride = sizeof(UINT32);
			SRV_Desc.Format = DXGI_FORMAT_UNKNOWN;
			SRV_Desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			CD3DX12_CPU_DESCRIPTOR_HANDLE SRV_DescHandle(
				CBV_SRV_UAV_DescHeap->GetCPUDescriptorHandleForHeapStart(),
				INDEX,
				CBV_SRV_UAV_DescriptorSize);

			Device->CreateShaderResourceView(
				IndexBufferResource.Get(),
				&SRV_Desc,
				SRV_DescHandle
			);
		}
	}

	INLINED void __stdcall Graphics::CreateIndexBufferHeap() {
		D3D12_HEAP_DESC IndexBufferHeapDesc = {
						FLOOR(IndexBufferSize, static_cast<UINT>(256)),
						{D3D12_HEAP_TYPE_DEFAULT,D3D12_CPU_PAGE_PROPERTY_UNKNOWN,D3D12_MEMORY_POOL_UNKNOWN},
						D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
						D3D12_HEAP_FLAG_NONE
		};
		ThrowIfFailed(
			Device->CreateHeap(&IndexBufferHeapDesc,
				IID_PPV_ARGS(&pIndexBufferHeap)
			)
		);
	}

	INLINED void __stdcall Graphics::CreateIndexBuffer() {
		D3D12_RESOURCE_DESC IndexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(FLOOR(IndexBufferSize, 256U));
		ThrowIfFailed(Device->CreatePlacedResource(
			pIndexBufferHeap.Get(),
			0,
			&IndexBufferDesc,
			D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,//D3D12_RESOURCE_STATE_INDEX_BUFFER,
			nullptr,
			IID_PPV_ARGS(&IndexBufferResource)
		));
#ifdef _DEBUG
		ThrowIfFailed(IndexBufferResource->SetName(L"IdxBfrRsrc"));
#endif // _DEBUG

	}

	INLINED size_t __stdcall Graphics::LoadIndexBuffer(
	) {
		UINT32* pIndexDataBegin = nullptr;
		D3D12_RANGE
			Range = { 0,0 };
		ThrowIfFailed(ConstantBuffer->Map(
			0,
			&Range,
			reinterpret_cast<void**>(&pIndexDataBegin)
		));
		size_t IndexCount = 0;
		for (auto const& Object : this->ObjectList)
		{
			//If More Vertices Are Given, Consider Codes Below.
			memcpy(pIndexDataBegin + IndexUploadOffset / sizeof(UINT32) + IndexCount, &Object.IndexData[0], Object.IndexCount * sizeof(UINT32));
			IndexCount += Object.IndexCount;
		}
		D3D12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			IndexBufferResource.Get(),
			D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_COPY_DEST
		);
		Range = { IndexUploadOffset,IndexUploadOffset + IndexCount };
		ConstantBuffer->Unmap(
			0,
			&Range
		);
		DirectGraphicsCommandList->ResourceBarrier(1, &Barrier);
		//��������з������ϴ��Ѹ����������ݵ�Ĭ�϶ѵ�����
		DirectGraphicsCommandList->CopyBufferRegion(
			IndexBufferResource.Get(),
			0,
			ConstantBuffer.Get(),
			IndexUploadOffset,
			(static_cast<UINT64>(IndexCount)) * sizeof(UINT32)
		);
		Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			IndexBufferResource.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_INDEX_BUFFER);
		DirectGraphicsCommandList->ResourceBarrier(1, &Barrier);
		IndexBufferView.SizeInBytes = IndexCount * sizeof(UINT);
		return IndexCount;
	}

	INLINED void __stdcall Graphics::CreateIndexBufferView()noexcept {
		IndexBufferView.BufferLocation = IndexBufferResource->GetGPUVirtualAddress();
		IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	}

	INLINED void __stdcall Graphics::CreateFakeVertexBufferAndLoad(
		std::initializer_list<XMVECTOR> Position,
		std::initializer_list<XMFLOAT2> Tex
	) {
		VB_View.SizeInBytes = sizeof(RectangleVertex[4]);
		D3D12_RESOURCE_DESC Desc = CD3DX12_RESOURCE_DESC::Buffer(VB_View.SizeInBytes);
		D3D12_HEAP_PROPERTIES Prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		ThrowIfFailed(Device->CreateCommittedResource(
			&Prop,
			D3D12_HEAP_FLAG_NONE,
			&Desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&NormalBufferResource)));
#ifdef _DEBUG
		ThrowIfFailed(NormalBufferResource->SetName(L"NrmBfrRsrc"));
#endif // _DEBUG

		RectangleVertex* pData = nullptr;
		D3D12_RANGE Range = {0,static_cast<size_t>(Desc.Width)};
		ThrowIfFailed(NormalBufferResource->Map(NULL, &Range, reinterpret_cast<void**>(&pData)));
		int i = 0;
		for (auto const& data : Position)
		{
			XMStoreFloat3(&pData[i++].position,data);
		}
		i = 0;
		for (auto const& data : Tex)
		{
			pData[i++].uv = data;
		}
		NormalBufferResource->Unmap(NULL, nullptr);

		VB_View.BufferLocation = NormalBufferResource->GetGPUVirtualAddress();
		VB_View.StrideInBytes = sizeof(RectangleVertex);
	}

	INLINED void __stdcall Graphics::CreateRootSignatureDescriptor() {
		ComPtr<ID3DBlob> GraphicsSignature;
		ComPtr<ID3DBlob> ComputeSignature;
		ComPtr<ID3DBlob> error;

		try
		{
			CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC RootSignatureDesc;

			// ��GPU��ִ��SetGraphicsRootDescriptorTable�����ǲ��޸������б��е�SRV��������ǿ���ʹ��Ĭ��Rang��Ϊ:
			// D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE
			CD3DX12_DESCRIPTOR_RANGE1 DescRanges[GraphicsDescNum] = {};
			DescRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE, DescOffset::IMAGE_IN);

			CD3DX12_ROOT_PARAMETER1 RootParameters[GraphicsRootParametersNum] = {};
			RootParameters[0].InitAsDescriptorTable(1, &DescRanges[0], D3D12_SHADER_VISIBILITY_PIXEL); //SRV��PS�ɼ�


			D3D12_STATIC_SAMPLER_DESC SampDesc[] = {
				CD3DX12_STATIC_SAMPLER_DESC(0)
			};
			SampDesc->AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			SampDesc->AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			SampDesc->AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			SampDesc->BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			SampDesc->ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//SAMPLER��PS�ɼ�
			RootSignatureDesc.Init_1_1(
				GraphicsRootParametersNum,
				RootParameters,
				__crt_countof(SampDesc),
				SampDesc,
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
			);
			ThrowIfFailed(
				D3D12SerializeVersionedRootSignature(
					&RootSignatureDesc,
					&GraphicsSignature,
					&error)
			);
		}
		catch (const HrException& e)
		{
			// Output errors to debug window.
			if (error != nullptr)
				OutputDebugStringA((char*)error->GetBufferPointer());
			throw e;
		}
		ThrowIfFailed(Device->CreateRootSignature(
			0,
			GraphicsSignature->GetBufferPointer(),
			GraphicsSignature->GetBufferSize(),
			IID_PPV_ARGS(&GraphicsRootSignature))
		);
#if _DEBUG
		ThrowIfFailed(GraphicsRootSignature->SetName(L"GrpRtSgnt"));
#endif // _DEBUG

		try
		{
			CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC RootSignatureDesc;

			// ��GPU��ִ��SetComputeRootDescriptorTable�����ǲ��޸������б��е�SRV��������ǿ���ʹ��Ĭ��Rang��Ϊ:
			// D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE
			
			CD3DX12_DESCRIPTOR_RANGE1 DescRanges[ComputeDescNum] = {};
			DescRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE,VERTEX);
			DescRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE,INDEX);
			DescRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_NONE, CONSTANT_BUFFER);
			DescRanges[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_NONE, IMAGE_OUT);

			CD3DX12_ROOT_PARAMETER1 RootParameters[ComputeRootParametersNum] = {};
			RootParameters[0].InitAsDescriptorTable(4, &DescRanges[0],  D3D12_SHADER_VISIBILITY_ALL);//SRV CBV UAV����Shader�ɼ�


			RootSignatureDesc.Init_1_1(
				ComputeRootParametersNum,
				RootParameters,
				0,
				nullptr,
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT
			);
			ThrowIfFailed(
				D3D12SerializeVersionedRootSignature(
					&RootSignatureDesc,
					&ComputeSignature,
					&error)
			);
		}
		catch (const HrException& e)
		{
			// Output errors to debug window.
			if (error != nullptr)
				OutputDebugStringA((char*)error->GetBufferPointer());
			throw e;
		}
		ThrowIfFailed(Device->CreateRootSignature(
			0,
			ComputeSignature->GetBufferPointer(),
			ComputeSignature->GetBufferSize(),
			IID_PPV_ARGS(&ComputeRootSignature))
		);
#if _DEBUG
		ThrowIfFailed(ComputeRootSignature->SetName(L"CmptRtSgnt"));
#endif // _DEBUG

	}

	INLINED void __stdcall Graphics::CreateGraphicsPipeLine(
		const WCHAR* VertexShaderFileName,
		const WCHAR* PixelShaderFileName) {
		ComPtr<ID3DBlob> VertexShader;
		ComPtr<ID3DBlob> PixelShader;

#if defined(_DEBUG)
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif
		compileFlags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

		ComPtr<ID3DBlob> errors;
		try
		{
			ThrowIfFailed(D3DCompileFromFile(
				VertexShaderFileName,
				nullptr,
				nullptr,
				"VSMain",
				"vs_5_0",
				compileFlags,
				0,
				&VertexShader,
				&errors));
		}
		catch (const HrException& e)
		{
			// Output errors to debug window.
			if (errors != nullptr)
				OutputDebugStringA((char*)errors->GetBufferPointer());
			throw e;
		}
		try
		{
			ThrowIfFailed(D3DCompileFromFile(
				PixelShaderFileName,
				nullptr,
				nullptr,
				"PSMain",
				"ps_5_0",
				compileFlags,
				0,
				&PixelShader,
				&errors));
		}
		catch (const HrException& e)
		{
			// Output errors to debug window.
			if (errors != nullptr)
				OutputDebugStringA((char*)errors->GetBufferPointer());
			throw e;
		}

		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				0,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				0
			},
			{
				"TEXCOORD",
				0,
				DXGI_FORMAT_R32G32_FLOAT,
				0,
				sizeof(RectangleVertex::position),
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				0
			}
		};
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = GraphicsRootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(VertexShader.Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(PixelShader.Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = RenderTargetFormat;
		psoDesc.SampleDesc.Count = 1;

		try
		{
			ThrowIfFailed(
				Device->CreateGraphicsPipelineState(
					&psoDesc,
					IID_PPV_ARGS(&GraphicsPipelineState)
				)
			);
		}
		catch (const HrException& e)
		{
			throw e;
		}
	}

	INLINED void __stdcall Graphics::CreateComputePipeLine(
		const WCHAR* ComputeShaderFileName,
		UINT VertexCount,
		UINT IndexCount
	) {
		ComPtr<ID3DBlob> ComputeShader;
#if defined(_DEBUG)
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif
		compileFlags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
		std::string w = std::to_string(Width / BlockSize),
			h = std::to_string(Height / BlockSize),
			VC = std::to_string(VertexCount),
			TC = std::to_string(IndexCount / 3),
			Theta1 = std::to_string(-XM_PIDIV4),
			Theta2 = std::to_string(XM_PIDIV4),
			Phi1 = std::to_string(-XM_PIDIV4),
			Phi2 = std::to_string(XM_PIDIV4),
			BS = std::to_string(BlockSize),
			COLOR = std::to_string(CosmosColor.w) + "," + std::to_string(CosmosColor.x) + "," + std::to_string(CosmosColor.y) + "," + std::to_string(CosmosColor.z);
		D3D_SHADER_MACRO ShaderMacro[] = {
			{"WIDTH",w.c_str()},
		{"HEIGHT",h.c_str()},
		{"VERTEX_COUNT",VC.c_str()},
		{"TRIANGLE_COUNT",TC.c_str()},
		{"THETA1",Theta1.c_str()},
		{"THETA2",Theta2.c_str()},
		{"PHI1",Phi1.c_str()},
		{"PHI2",Phi2.c_str()},
		{"BLOCK_SIZE",BS.c_str()},
		{"BACKGROUND_COLOR",COLOR.c_str()},
		{"",""},
		{nullptr,nullptr}
		};
		ComPtr<ID3DBlob> errors;
		try
		{
			ThrowIfFailed(D3DCompileFromFile(
				ComputeShaderFileName,
				ShaderMacro,
				nullptr,
				"CSMain",
				"cs_5_0",
				compileFlags,
				0,
				&ComputeShader,
				&errors));
		}
		catch (const HrException& e)
		{
			// Output errors to debug window.
			if (errors != nullptr)
				OutputDebugStringA((char*)errors->GetBufferPointer());
			throw e;
		}
		D3D12_COMPUTE_PIPELINE_STATE_DESC Desc = {};
		Desc.CS = CD3DX12_SHADER_BYTECODE(ComputeShader.Get());
		Desc.pRootSignature = ComputeRootSignature.Get();
		Desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		try
		{
			ThrowIfFailed(Device->CreateComputePipelineState(&Desc, IID_PPV_ARGS(&ComputePipelineState)));
		}
		catch (const HrException& e)
		{
			throw e;
		}
	}

	INLINED size_t __stdcall Graphics::LoadVertexBuffer(
	) {
		VertexDetail* pVertexDataBegin = nullptr;
		D3D12_RANGE
			Range = { 0,0 };
		ThrowIfFailed(ConstantBuffer->Map(
			0,
			&Range,
			reinterpret_cast<void**>(&pVertexDataBegin)
		));
		size_t VertexCount = 0;
		for (auto const& Object : this->ObjectList)
		{
			memcpy(
				pVertexDataBegin + VertexUploadOffset / sizeof(VertexDetail),
				&Object.VertexData[0],
				VertexBufferSize
			);
			VertexCount += Object.VertexCount;
		}
#ifdef _DEBUG
		VertexDetail* pDebug = pVertexDataBegin + VertexUploadOffset / sizeof(VertexDetail);
		UNREFERENCED_PARAMETER(pDebug);
#endif // _DEBUG
		D3D12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			VertexBufferResource.Get(),
			D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_COPY_DEST);
		Range = { VertexUploadOffset,VertexUploadOffset + VertexCount };
		ConstantBuffer->Unmap(
			0,
			&Range
		);
		DirectGraphicsCommandList->ResourceBarrier(1, &Barrier);
		//��������з������ϴ��Ѹ����������ݵ�Ĭ�϶ѵ�����
		DirectGraphicsCommandList->CopyBufferRegion(
			VertexBufferResource.Get(),
			0,
			ConstantBuffer.Get(),
			VertexUploadOffset,
			VertexBufferSize
		);
		Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			VertexBufferResource.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		DirectGraphicsCommandList->ResourceBarrier(1, &Barrier);
		VertexBufferView.SizeInBytes = VertexCount * sizeof(VertexDetail);
		return VertexCount;
	}

	INLINED void __stdcall Graphics::CreateVertexBufferView()noexcept {
		VertexBufferView.BufferLocation = VertexBufferResource->GetGPUVirtualAddress();
		VertexBufferView.StrideInBytes = sizeof(VertexDetail);
	}

	INLINED void __stdcall Graphics::CreateDirectCommandAllocator() {
		try
		{
			//���������б������
			ThrowIfFailed(Device->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(&DirectCommandAllocator)));
		}
		catch (const HrException& e)
		{
			throw e;
		}
	}

	INLINED void __stdcall Graphics::CreateDirectCommandList() {
		try
		{
			//����ͼ�������б�
			ThrowIfFailed(Device->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				DirectCommandAllocator.Get(),
				GraphicsPipelineState.Get(),
				IID_PPV_ARGS(&DirectGraphicsCommandList)));
		}
		catch (const HrException& e)
		{
			throw e;
		}
#ifdef _DEBUG
		ThrowIfFailed(DirectGraphicsCommandList->SetName(L"GrpCmdL"));
#endif // _DEBUG

	}

	INLINED void __stdcall Graphics::CreateComputeCommandAllocator() {
		try
		{
			//���������б������
			ThrowIfFailed(Device->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_COMPUTE,
				IID_PPV_ARGS(&ComputeCommandAllocator)));
		}
		catch (const HrException& e)
		{
			throw e;
		}
	}

	INLINED void __stdcall Graphics::CreateComputeCommandList() {
		try
		{
			//����ͼ�������б�
			ThrowIfFailed(Device->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_COMPUTE,
				ComputeCommandAllocator.Get(),
				ComputePipelineState.Get(),
				IID_PPV_ARGS(&ComputeCommandList)));
		}
		catch (const HrException& e)
		{
			throw e;
		}
#ifdef _DEBUG
		ThrowIfFailed(ComputeCommandList->SetName(L"CptCmdL"));
#endif // _DEBUG

	}
	INLINED void __stdcall Graphics::CreateComputeCommandQueue() {
		D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
		try
		{
			ThrowIfFailed(Device->CreateCommandQueue(
				&QueueDesc,
				IID_PPV_ARGS(&ComputeCommandQueue))
			);
		}
		catch (const HrException& e)
		{
			throw e;
		}
	}

	INLINED void __stdcall Graphics::CreateFence() {
		//����һ��ͬ�����󡪡�Χ�������ڵȴ���Ⱦ��ɣ���Ϊ����Draw Call���첽����
		ThrowIfFailed(Device->CreateFence(
			0,
			D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&GraphicsFence)));
		GraphicsFenceValue = 1;

		//����һ��ͬ�����󡪡�Χ�������ڵȴ��������
		ThrowIfFailed(Device->CreateFence(
			0,
			D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&ComputeFence)));
		ComputeFenceValue = 1;
	}
	INLINED void __stdcall Graphics::CreateFenceEvent() {
		//����һ��Eventͬ���������ڵȴ�Χ���¼�֪ͨ
		GraphicsFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (GraphicsFenceEvent == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}

		//����һ��Eventͬ���������ڵȴ�Χ���¼�֪ͨ
		ComputeFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (ComputeFenceEvent == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}
	}

	INLINED void __stdcall Graphics::StraightMove(const XMVECTOR& Direction) {
		MomentumDivideMass += 0.1f * XMVector3Normalize(Direction);
	}

	INLINED void __stdcall Graphics::Rotate(UINT x, UINT y) {
		if (x != 0 || y != 0)
		{
			if (y != 0)
			{
				int LookUp = (this->At.m128_f32[2] > this->Eye.m128_f32[2]) ? 1 : -1;
				if (
					asinf(
						XMVector2Length(this->At - this->Eye).m128_f32[0]
						/
						XMVector3Length(this->At - this->Eye).m128_f32[0])
					<=
					XM_PIDIV4 * ((static_cast<float>(y)) / PixelsY - 0.5f) * (-LookUp)
					)
				{
					this->At = this->Eye;
					this->At.m128_f32[2] += 10.0f * LookUp;
					this->Up.m128_f32[2] = 0;
					this->Up = XMVector3Normalize(this->Up);
				}
				else
				{
					XMVECTOR AxisDirection = XMVector3Cross(this->At - this->Eye, this->Up);
					AxisDirection.m128_f32[2] = 0;
					this->At = this->Eye +
						XMMultiply(
							XMMatrixRotationAxis(
								AxisDirection,
								XM_PIDIV4 * ((static_cast<float>(y)) / PixelsY - 0.5f)),
							this->At - this->Eye);
					this->Up = XMVector3Normalize(XMVector3Cross(AxisDirection, this->At - this->Eye));
				}
			}
			if (x != PixelsX / 2)
			{
				this->At = this->Eye +
					XMMultiply(
						XMMatrixRotationAxis(
							XMVECTOR({ 0, 0, 1, 0 }),
							-XM_PIDIV4 * ((static_cast<float>(x)) / PixelsX - 0.5f)
						),
						this->At - this->Eye
					);
				this->Up = XMMultiply(
					XMMatrixRotationAxis(
						XMVECTOR({ 0, 0, 1, 0 }),
						-XM_PIDIV4 * ((static_cast<float>(x)) / PixelsX - 0.5f)
					),
					this->Up);
			}
		}
	}
}