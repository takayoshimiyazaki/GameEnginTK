//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() :
	m_window(0),
	m_outputWidth(800),
	m_outputHeight(600),
	m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	m_window = window;
	m_outputWidth = std::max(width, 1);
	m_outputHeight = std::max(height, 1);

	CreateDevice();

	CreateResources();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

	// 初期化はここに書く

	// スプライトバッチを生成
	m_spriteBatch = std::make_unique<SpriteBatch>(m_d3dContext.Get());

	// キーボードを生成
	m_keyboard = std::make_unique<Keyboard>();

	// カメラの生成
	m_camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	m_camera->SetKeyboard(m_keyboard.get());
	
	// 3Dオブジェクトの静的メンバ変数の初期化
	Obj3d::InitializeStatic(m_d3dDevice,m_d3dContext,m_camera.get());

	// 土地当たりの設定
	LandShapeCommonDef lscDef;
	lscDef.pDevice = m_d3dDevice.Get();
	lscDef.pDeviceContext = m_d3dContext.Get();
	lscDef.pCamera = m_camera.get();
	// 土地当たりの共通初期化
	LandShape::InitializeCommon(lscDef);

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());

	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0.0f,
		(float)m_outputWidth, (float)m_outputHeight, 0.0f, 0.0f, 1.0f));
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	// 汎用ステート設定を生成
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	// デバッグカメラを生成
	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);
			
	// エフェクトファクトリーを生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// テクスチャのパスを指定
	m_factory->SetDirectory(L"$Resources");
	// モデルを生成
	m_objSkyDome.LoadModel(L"$Resources/skydome.cmo");

	//m_modelGround = Model::CreateFromCMO(m_d3dDevice.Get(),	L"$Resources/ground200m.cmo", *m_factory);
	m_LandShape.Initialize(L"ground200m",L"ground200m");
	m_LandShape.SetRot(Vector3(0.2f, 0, 0));

	// プレイヤーの生成
	m_player = std::make_unique<Player>(m_keyboard.get());
	m_player->Initialize();	

	// 追従カメラにプレイヤーをセット
	m_camera->SetPlayer(m_player.get());

	// 敵を生成
	m_enemyNum = rand() % 10 + 1;
	m_enemies.resize(m_enemyNum);

	for (int i = 0; i < m_enemyNum; i++)
	{
		m_enemies[i] = std::make_unique<Enemy>(m_keyboard.get());
		m_enemies[i]->Initialize();
	}

	// 画像を読み込む
	ComPtr<ID3D11Resource> resource;
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), L"$Resources/GameClear.png",
			resource.GetAddressOf(),
			m_clearImage.ReleaseAndGetAddressOf()));

	// クリアのテクスチャ
	ComPtr<ID3D11Texture2D> clear;
	DX::ThrowIfFailed(resource.As(&clear));
	
	// テクスチャの設定
	CD3D11_TEXTURE2D_DESC attackDesc;
	clear->GetDesc(&attackDesc);
	
	// テクスチャの原点をテクスチャの中心にする
	m_origin.x = float(attackDesc.Width / 2);
	m_origin.y = float(attackDesc.Height / 2);
	
	// 表示座標を画面中心に設定
	m_screenPos.x = m_outputWidth / 2.f;
	m_screenPos.y = m_outputHeight / 2.f;

	m_winFlg = false;		// 勝利フラグをfalseに設定
}

// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;
	// 毎フレーム処理を書く
	
	
	// キーボードの状態を取得
	Keyboard::State kb = m_keyboard->GetState();
	m_KeyboardTracker.Update(kb);

	m_player->Update();

	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemies.begin(); it != m_enemies.end(); it++)
	{
		//Enemy* enemy = it->get();
		//enemy->Update();

		(*it)->Update();
	}	
	

	{	// 弾丸と敵の当たり判定
		// 弾丸の当たり判定球を取得
		const Sphere& bulletSphere = m_player->GetCollisionNodeBullet();

		// 敵の数だけ処理する
		for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemies.begin(); it != m_enemies.end();)
		{
			Enemy* enemy = it->get();
			// 敵の当たり判定球を取得
			const Sphere& enemySphere = enemy->GetCollisionNodeBody();

			// 二つの球が当たっていたら
			if (CheckSphere2Sphere(bulletSphere, enemySphere))
			{
				// 敵を殺す								
				ModelEffectManager::getInstance()->Entry(
					L"$Resources/HitEffect.cmo",	// モデルファイル
					10,	// 寿命フレーム数
					Vector3(enemy->GetTrans().x, enemy->GetTrans().y + 1.0f, enemy->GetTrans().z),	// ワールド座標
					Vector3(0, 0, 0),	// 速度
					Vector3(0, 0, 0),	// 加速度
					Vector3(0, 0, 0),	// 回転角（初期）
					Vector3(90, 90, 90),	// 回転角（最終）
					Vector3(0, 0, 0),	// スケール（初期）
					Vector3(10, 10, 10)	// スケール（最終）
				);				
				// 消した要素の次の要素を指すイテレータ
				it = m_enemies.erase(it);
				m_enemyNum--;
			}
			else
			{
				// 消さなかった場合、普通にイテレータを進める
				it++;
			}
		}
	}	

	// カメラの更新	
	m_camera->Update();
	m_view = m_camera->GetViewMatrix();
	m_proj = m_camera->GetProjectMatrix();
			
	// ドームの更新	
	m_objSkyDome.Update();	
	// LandShape
	m_LandShape.Update();
		
	// エフェクトの更新	
	ModelEffectManager::getInstance()->Update();

	// 敵が全滅したら
	/*if (m_enemyNum <= 0)
	{
		m_winFlg = true;
	}*/

	// Enterキーが押されたら
	//if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keyboard::Enter))
	//{
	//	// 残っている敵を消す
	//	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemies.begin(); it != m_enemies.end(); it++)
	//	{
	//		// 消した要素の次の要素を指すイテレータ
	//		it = m_enemies.erase(it);
	//		m_enemyNum = 0;			
	//	}

	//	// 初期化
	//	m_enemyNum = rand() % 10 + 1;
	//	m_enemies.resize(m_enemyNum);

	//	for (int i = 0; i < m_enemyNum; i++)
	//	{
	//		m_enemies[i] = std::make_unique<Enemy>(m_keyboard.get());
	//		m_enemies[i]->Initialize();
	//	}

	//	m_winFlg = false;
	//}

	// 自機の地形へのめり込みを解消する
	{
		Sphere sphere = m_player->GetCollisionNodeBody();
		// 自機のワールド座標
		Vector3 trans = m_player->GetTrans();
		// 球からプレイヤーへのベクトル
		Vector3 sphere2player = trans - sphere.Center;
		// めり込み排斥ベクトル
		Vector3 reject;
		if (m_LandShape.IntersectSphere(sphere, &reject))
		{
			// めり込みを解消するように球をずらす
			sphere.Center += reject;
		}
		// 自機を移動
		m_player->SetTrans(sphere.Center + sphere2player);
		// ワールド行列を更新
		m_player->Calc();
	}

	{// 自機が地面に乗る処理
		const Vector3 vel = m_player->GetVelocity();
		if (vel.y <= 0.0f)
		{
			// 自機の頭から足元への線分
			Segment playerSegment;
			// 自機のワールド座標
			Vector3 trans = m_player->GetTrans();
			playerSegment.Start = trans + Vector3(0, 1, 0);
			// 足元50センチ下まで地面を検出
			playerSegment.End = trans + Vector3(0, -0.5f, 0);
			// 交点座標
			Vector3 inter;
			// 地形と線分の当たり判定(レイキャスト)
			if (m_LandShape.IntersectSegment(playerSegment, &inter))
			{
				// Y座標を交点に移動させる
				trans.y = inter.y;

				// 落下を終了
				m_player->StopJump();
			}
			else
			{
				// 落下を開始
				m_player->StartFall();
			}

			// 自機を移動
			m_player->SetTrans(trans);
			// ワールド行列を更新
			m_player->Calc();
		}				
	}
}

// Draws the scene.
void Game::Render()
{
	uint16_t indices[] =
	{
		0,1,2,
		2,1,3
	};

	VertexPositionNormal vertices[] =
	{
		{ Vector3(-1,+1,0), Vector3(0,0,1) },
		{ Vector3(-1,-1,0), Vector3(0,0,1) },
		{ Vector3(+1,+1,0), Vector3(0,0,1) },
		{ Vector3(+1,-1,0), Vector3(0,0,1) }
	};

	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	Clear();

	// TODO: Add your rendering code here.
	// 描画処理を書く
	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_d3dContext->RSSetState(m_states->Wireframe());


	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	// モデルの描画
	m_objSkyDome.Draw();

	/*m_modelGround->Draw(m_d3dContext.Get(),
		*m_states,
		Matrix::Identity,
		m_view,
		m_proj
	);*/

	m_LandShape.Draw();
	
	m_player->Draw();

	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemies.begin(); it != m_enemies.end(); it++)
	{		
		(*it)->Draw();
	}

	ModelEffectManager::getInstance()->Draw();
	
	m_batch->Begin();

	m_batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);

	m_batch->End();
	
	// スプライトの描画
	CommonStates states(m_d3dDevice.Get());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	if (m_winFlg)
	{
		m_spriteBatch->Draw(m_clearImage.Get(), m_screenPos, nullptr, Colors::White,
			XMConvertToRadians(0.0), m_origin);
	}

	m_spriteBatch->End();

	Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	// Clear the views.
	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// Set the viewport.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
	m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		OnDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
	m_outputWidth = std::max(width, 1);
	m_outputHeight = std::max(height, 1);

	CreateResources();

	// TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		// TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// Create the DX11 API device object, and get a corresponding context.
	HRESULT hr = D3D11CreateDevice(
		nullptr,                                // specify nullptr to use the default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
		&m_featureLevel,                        // returns feature level of device created
		m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
	);

	if (hr == E_INVALIDARG)
	{
		// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
		hr = D3D11CreateDevice(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			&featureLevels[1],
			_countof(featureLevels) - 1,
			D3D11_SDK_VERSION,
			m_d3dDevice.ReleaseAndGetAddressOf(),
			&m_featureLevel,
			m_d3dContext.ReleaseAndGetAddressOf()
		);
	}

	DX::ThrowIfFailed(hr);

#ifndef NDEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
	{
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
		{
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	// DirectX 11.1 if present
	if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
		(void)m_d3dContext.As(&m_d3dContext1);

	// TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
	// Clear the previous window size specific context.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_d3dContext->Flush();

	UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
	UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	UINT backBufferCount = 2;

	// If the swap chain already exists, resize it, otherwise create one.
	if (m_swapChain)
	{
		HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			OnDeviceLost();

			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
			// and correctly set up the new device.
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// First, retrieve the underlying DXGI Device from the D3D Device.
		ComPtr<IDXGIDevice1> dxgiDevice;
		DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

		// Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// And obtain the factory object that created it.
		ComPtr<IDXGIFactory1> dxgiFactory;
		DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		ComPtr<IDXGIFactory2> dxgiFactory2;
		if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
		{
			// DirectX 11.1 or later

			// Create a descriptor for the swap chain.
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
			swapChainDesc.Width = backBufferWidth;
			swapChainDesc.Height = backBufferHeight;
			swapChainDesc.Format = backBufferFormat;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = backBufferCount;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
			fsSwapChainDesc.Windowed = TRUE;

			// Create a SwapChain from a Win32 window.
			DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
				m_d3dDevice.Get(),
				m_window,
				&swapChainDesc,
				&fsSwapChainDesc,
				nullptr,
				m_swapChain1.ReleaseAndGetAddressOf()
			));

			DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
		}
		else
		{
			DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
			swapChainDesc.BufferCount = backBufferCount;
			swapChainDesc.BufferDesc.Width = backBufferWidth;
			swapChainDesc.BufferDesc.Height = backBufferHeight;
			swapChainDesc.BufferDesc.Format = backBufferFormat;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.OutputWindow = m_window;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.Windowed = TRUE;

			DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
		}

		// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
		DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
	}

	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// Create a view interface on the rendertarget to use on bind.
	DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

	// TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.

	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain1.Reset();
	m_swapChain.Reset();
	m_d3dContext1.Reset();
	m_d3dContext.Reset();
	m_d3dDevice1.Reset();
	m_d3dDevice.Reset();

	CreateDevice();

	CreateResources();
}