//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "DebugCamera.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

	Game();

	// Initialization and management
	void Initialize(HWND window, int width, int height);

	// Basic game loop
	void Tick();

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	// Properties
	void GetDefaultSize(int& width, int& height) const;

private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();
	void Present();

	void CreateDevice();
	void CreateResources();

	void OnDeviceLost();

	// Device resources.
	HWND                                            m_window;
	int                                             m_outputWidth;
	int                                             m_outputHeight;

	D3D_FEATURE_LEVEL                               m_featureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

	Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

	// Rendering loop timer.
	DX::StepTimer                                   m_timer;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;
	// �G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �ėp�X�e�[�g�ݒ�
	std::unique_ptr<DirectX::CommonStates> m_states;

	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;

	// �f�o�b�O�J����
	std::unique_ptr<DebugCamera> m_debugCamera;

	// �G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	// ���f��
	std::unique_ptr<DirectX::Model> m_groundmodel;
	std::unique_ptr<DirectX::Model> m_skymodel;
	std::unique_ptr<DirectX::Model> m_ball;
	std::unique_ptr<DirectX::Model> m_teapot;

	//���̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldBall[20];
	DirectX::SimpleMath::Matrix m_worldTeapot[20];

	float m_angle;
	float m_x[20];
	float m_z[20];
	float m_scale;
	bool m_switch;
	DirectX::SimpleMath::Vector3 m_startPos[20];
	DirectX::SimpleMath::Vector3 m_targetPos;
};