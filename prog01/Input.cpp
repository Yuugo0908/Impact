#include "Input.h"
#include <cassert>


#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

DIMOUSESTATE Input::currentMouseState;
DIMOUSESTATE Input::prevMouseState;
XMFLOAT2 Input::mousePos;

bool Input::Initialize(HINSTANCE hInstance, HWND hwnd) {
	HRESULT result = S_FALSE;

	//�������i��x�����s�������j
	result = DirectInput8Create
	(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr
	);
	if (FAILED(result)) 	{
		assert(0);
		return result;
	}

	this->hwnd = hwnd;

	CreateKeyDevice(hInstance, hwnd);
	CreateMouceDevice(hInstance, hwnd);

	return true;
}

void Input::Update() {
	HRESULT result;

	//�L�[�{�[�h���̎擾�J�n
	result = devkeyboard->Acquire();
	// �O��̃L�[���͂�ۑ�
	memcpy(prevKeyState, keyState, sizeof(keyState));
	//�S�L�[�̓��͏�Ԃ��擾����
	result = devkeyboard->GetDeviceState(sizeof(keyState), keyState);

	//�}�E�X���̎擾�J�n
	result = devmouse->Acquire();
	prevMouseState = currentMouseState;
	// �}�E�X�̏�Ԃ��擾
	result = devmouse->GetDeviceState(sizeof(currentMouseState), &currentMouseState);
	POINT p;
	// �}�E�X���W(�X�N���[�����W)���擾����
	GetCursorPos(&p);
	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(hwnd, &p);
	// �ϊ��������W��ۑ�
	mousePos.x = (float)p.x;
	mousePos.y = (float)p.y;
	//ShowCursor(false);
}

XMFLOAT2 Input::GetMouseVelocity() {
	return XMFLOAT2((float)currentMouseState.lX, (float)currentMouseState.lY);
}

XMFLOAT2 Input::GetMousePosition() {
	return XMFLOAT2(mousePos);
}

bool Input::PushKey(BYTE keyNumber) {
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0�łȂ���Ή����Ă���
	if (keyState[keyNumber]) 	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) {
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!prevKeyState[keyNumber] && keyState[keyNumber]) 	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::PushMouse(MouseButton buttonType) {
	// �ُ�Ȉ��������o
	assert(0 <= buttonType <= 4);

	// 0�łȂ���Ή����Ă���
	if (currentMouseState.rgbButtons[buttonType]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerMouse(MouseButton buttonType) {
	// �ُ�Ȉ��������o
	assert(0 <= buttonType <= 4);

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!currentMouseState.rgbButtons[buttonType] && prevMouseState.rgbButtons[buttonType]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::CreateKeyDevice(HINSTANCE hInstance, HWND hwnd) {
	HRESULT result = S_FALSE;

	//�L�[�{�[�h�f�o�C�X�̐���
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//�r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel
	(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	return true;
}

bool Input::CreateMouceDevice(HINSTANCE hInstance, HWND hwnd) {
	HRESULT result = S_FALSE;

	//�L�[�{�[�h�f�o�C�X�̐���
	result = dinput->CreateDevice(GUID_SysMouse, &devmouse, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//���̓f�[�^�`���̃Z�b�g
	result = devmouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//�r�����䃌�x���̃Z�b�g
	result = devmouse->SetCooperativeLevel
	(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	return true;
}