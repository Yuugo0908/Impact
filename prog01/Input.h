#pragma once

#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <DirectXMath.h>
#include "WinApp.h"

using namespace DirectX;

enum MouseButton {
	Left,		//!< ��
	Right,		//!< �E
	Center,		//!< �^��
};

class Input
{
private:
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //�����o�֐�
	//������
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	//�X�V
	void Update();
	// �L�[�̉������`�F�b�N
	bool PushKey(BYTE keyNumber);
	// �L�[�̃g���K�[���`�F�b�N
	bool TriggerKey(BYTE keyNumber);

	//�L�[�{�[�h�f�o�C�X����
	bool CreateKeyDevice(HINSTANCE hInstance, HWND hwnd);
	//�}�E�X�f�o�C�X����
	bool CreateMouceDevice(HINSTANCE hInstance, HWND hwnd);

	// �}�E�X�̉������`�F�b�N
	bool PushMouse(MouseButton buttonType);
	// �}�E�X�̃g���K�[���`�F�b�N
	bool TriggerMouse(MouseButton buttonType);
	//�}�E�X�̈ړ��ʎ擾
	XMFLOAT2 GetMouseVelocity();
	//�}�E�X�̈ʒu�擾
	XMFLOAT2 GetMousePosition();

private: //�����o�ϐ�
	HWND hwnd;
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devkeyboard;
	ComPtr<IDirectInputDevice8> devmouse;
	BYTE keyState[256] = {};
	BYTE prevKeyState[256] = {};

private:
	static DIMOUSESTATE currentMouseState;
	static DIMOUSESTATE prevMouseState;
	static XMFLOAT2 mousePos;
};