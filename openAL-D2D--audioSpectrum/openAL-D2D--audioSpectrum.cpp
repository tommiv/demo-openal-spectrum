#pragma once
#include "stdafx.h"
#include "openAL-D2D--audioSpectrum.h"
#include <al.h>
#include <alc.h>
#include <iostream>
#include <list>
#include <d2d1.h>
#include "dsp.h"

using namespace std;
//----------------------------------------------------------------
#define FREQ           32000
#define CAP_SIZE       512
#define MAX_LOADSTRING 100
#define PI 3.14159265
//----------------------------------------------------------------
#include "myD2D.h"
//----------------------------------------------------------------
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "libOpenAL32.dll.a")
//----------------------------------------------------------------
BOOL  Dispose = false;
struct params {
	double *VUpoints;
	double *Specpoints;
	double *freq;
	double *amp;
};
params *Params;
DWORD processAudio(params *Params);
MainWindow win;
//----------------------------------------------------------------

DWORD processAudio(params *Params) {
	/// OpenAL Init
	ALCdevice* outputDevice = alcOpenDevice(NULL);
	ALCcontext* audioContext = alcCreateContext(outputDevice, NULL);
	alcMakeContextCurrent(audioContext);
	ALCdevice* inputDevice = alcCaptureOpenDevice(NULL, FREQ, AL_FORMAT_MONO16, FREQ/2);
	alcCaptureStart(inputDevice);
	ALuint helloBuffer[16], helloSource[1];
	list<ALuint> bufferQueue;
	alGenBuffers(16, &helloBuffer[0]);
	for (int i=0; i<16; ++i) bufferQueue.push_back(helloBuffer[i]);
	alGenSources(1, &helloSource[0]);
	short buffer[FREQ*2];	// A buffer to hold captured audio
	ALCint samplesIn = 0;	// How many samples are captured
	ALint availBuffers = 0;	// Buffers to be recovered
	ALuint myBuff;			// The buffer we're using
	ALuint buffHolder[16];	// An array to hold catch the unqueued buffers
	/// End OpenAL Init

	/// Process audio
	while(!Dispose) {
		// Poll for recoverable buffers
		alGetSourcei(helloSource[0], AL_BUFFERS_PROCESSED, &availBuffers);
		if (availBuffers > 0) {
			alSourceUnqueueBuffers(helloSource[0], availBuffers, buffHolder);
			for (int i = 0; i < availBuffers; ++i) bufferQueue.push_back(buffHolder[i]); // Push the recovered buffers back on the queue
		}
		// Poll for captured audio
		alcGetIntegerv(inputDevice, ALC_CAPTURE_SAMPLES, 1, &samplesIn);
		if (samplesIn > CAP_SIZE) {
			alcCaptureSamples(inputDevice, buffer, CAP_SIZE);
			
			/// DSP
			for (int i = 0; i < CAP_SIZE; i++) Params->VUpoints[i] = (float)buffer[i];

			int N = CAP_SIZE; // samples count
			double* Re = new double[CAP_SIZE];
			double* Im = new double[CAP_SIZE];
			void* fft = NULL;
			fftCreate(N, &fft);
			fftReal(Params->VUpoints, Re, Im, N, fft);
			fftClear(&fft);

			int Nmax= (N + 1) / 2; 
			double nSamplesPerSec = 512.0;

			double *phase = new double[N];
			int i = 0; // fft result iterator
			int j = 0; // end-signal iterator
			double limit = 0.001;
			double abs2min = limit*limit * N*N;
	
			if (Re[i] >= limit) {
				Params->amp[j]   = Re[i] / N;
				Params->freq[j]  = 0.0;
				phase[j] = 0.0;
				++j;
			}
			++i;

			for(i = 1; i < Nmax; ++i) {
				double re = Re[i];
				double im = Im[i];

				double abs2 = re*re + im*im;

				if (abs2 < abs2min) continue; // throw low harmonics away
				
				Params->amp[j] = 2.0*sqrt(abs2) / N;
				phase[j] = atan2(im, re) + PI/2;
				if (phase[j] > PI) phase[j] -= 2*PI; // cos to sin
				phase[j] = phase[j]*PI / 180.0; // rad to deg
				Params->freq[j]= (nSamplesPerSec * i) / N;
				++j;
			}
			free(Re); free(Im);
			
			/// end DSP

			if (!bufferQueue.empty()) { // We just drop the data if no buffers are available
				myBuff = bufferQueue.front(); bufferQueue.pop_front();
				alBufferData(myBuff, AL_FORMAT_MONO16, buffer, CAP_SIZE*sizeof(short), FREQ);
				alSourceQueueBuffers(helloSource[0], 1, &myBuff); // Queue the buffer
				// Restart the source if needed (if we take too long and the queue dries up, the source stops playing).
				ALint sState = 0;
				alGetSourcei(helloSource[0], AL_SOURCE_STATE, &sState);

				// uncomment for echo playback
				//if (sState!=AL_PLAYING) alSourcePlay(helloSource[0]);
			}
		} // End if (samplesIn)
	}
	return 0;
}

void MainWindow::CalculateLayout() {}

void MainWindow::CreateGraphicsResources() {
	if (pRenderTarget == NULL) {
		RECT rc; GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size), &pRenderTarget);
		
		const D2D1_COLOR_F yellColor = D2D1::ColorF(1.00, 1.00, 0.00);
		pRenderTarget->CreateSolidColorBrush(yellColor, &brshYellow);
		const D2D1_COLOR_F oranColor = D2D1::ColorF(1.00, 0.65, 0.00);
		pRenderTarget->CreateSolidColorBrush(oranColor, &brshOrange);
		const D2D1_COLOR_F cyanColor = D2D1::ColorF(0.00, 0.50, 1.00);
		pRenderTarget->CreateSolidColorBrush(cyanColor, &brshCyan);
		const D2D1_COLOR_F whiteColor = D2D1::ColorF(1.00, 1.00, 1.00);
		pRenderTarget->CreateSolidColorBrush(whiteColor, &brshWhite);
		pRenderTarget->CreateSolidColorBrush(whiteColor, &brshVersatile);
		CalculateLayout();
	}
}

void MainWindow::DiscardGraphicsResources() {
	SafeRelease(&pRenderTarget);
	SafeRelease(&brshYellow);
	SafeRelease(&brshOrange);
	SafeRelease(&brshCyan);
}

inline float myRandf(float a, float b) { return ((b-a)*((float)rand()/RAND_MAX))+a; }
inline float norm(float v, float max, float dest, float offset) { return (v*(dest/max))+offset; }

void MainWindow::OnPaint() {
	CreateGraphicsResources();
	PAINTSTRUCT ps;
	const float xoff = 20.0;
	const float yoff = 20.0;
	D2D1_SIZE_F rtSize = pRenderTarget->GetSize();

	int winX = rtSize.width;
	int winY = rtSize.height;
	BeginPaint(m_hwnd, &ps);
		pRenderTarget->BeginDraw();
		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
		// Draw frames
		pRenderTarget->DrawLine(D2D1::Point2F(10.0,	10.0),  D2D1::Point2F(winX - 10.0, 10.0), brshWhite, 1.5);// TODO: Convert to polyline/rectangle?
		pRenderTarget->DrawLine(D2D1::Point2F(winX - 10.0, 10.0), D2D1::Point2F(winX - 10.0, winY/2 - 10), brshWhite, 1.5);
		pRenderTarget->DrawLine(D2D1::Point2F(winX - 10.0, winY/2 - 10), D2D1::Point2F(10.0, winY/2 - 10), brshWhite, 1.5);
		pRenderTarget->DrawLine(D2D1::Point2F(10.0, winY/2 - 10), D2D1::Point2F(10.0, 10.0), brshWhite, 1.5);

		pRenderTarget->DrawLine(D2D1::Point2F(10.0,	winY/2+10.0),  D2D1::Point2F(winX - 10.0, winY/2 + 10.0), brshWhite, 1.5);// TODO: Convert to polyline/rectangle?
		pRenderTarget->DrawLine(D2D1::Point2F(winX - 10.0, winY/2 + 10.0), D2D1::Point2F(winX - 10.0, winY - 10), brshWhite, 1.5);
		pRenderTarget->DrawLine(D2D1::Point2F(winX - 10.0, winY - 10), D2D1::Point2F(10.0, winY - 10), brshWhite, 1.5);
		pRenderTarget->DrawLine(D2D1::Point2F(10.0, winY - 10), D2D1::Point2F(10.0,	winY/2+10.0), brshWhite, 1.5);
		// Draw HUD
		pRenderTarget->DrawLine(D2D1::Point2F(xoff, winY/4), D2D1::Point2F(winX - xoff, winY/4), brshCyan, 1.5);
		pRenderTarget->DrawLine(D2D1::Point2F(xoff, yoff), D2D1::Point2F(xoff, winY/2-yoff), brshCyan, 1.5);
		
		// Draw VU
		for (int i = 0; i < CAP_SIZE - 1; ++i) {
			pRenderTarget->DrawLine(
				D2D1::Point2F(i+xoff,  norm(Params->VUpoints[i],  65536, winY/2-yoff*2, winY/4)), // 
				D2D1::Point2F(i+xoff,  norm(Params->VUpoints[i], -65536, winY/2-yoff*2, winY/4)),
				brshOrange, 1.0
			);
		};
		// Draw Spectrum
		int level=0;
		for (int i = 0; i < (CAP_SIZE - 1)/2; ++i) {
			level = -10*log(Params->amp[i]/94); if (level > 0) level = 0;
			brshVersatile->SetColor(D2D1::ColorF(1.00, i/255.0, i/510.0));
			pRenderTarget->DrawLine(
				D2D1::Point2F(i*2+xoff, winY-yoff),
				D2D1::Point2F(i*2+xoff, norm(level, 94, winY/2-yoff*2, winY-yoff)),
				brshVersatile, 3.0
			);
		};
		if (pRenderTarget->EndDraw() == D2DERR_RECREATE_TARGET) DiscardGraphicsResources();
	EndPaint(m_hwnd, &ps);
}

void MainWindow::Resize() {
	if (pRenderTarget != NULL) {
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		pRenderTarget->Resize(size);
		CalculateLayout();
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CREATE:
			if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) return -1;
			else return 0;
		case WM_DESTROY:
			Dispose = true;
			DiscardGraphicsResources();
			SafeRelease(&pFactory);
			PostQuitMessage(0);
			return 0;
		case WM_PAINT: OnPaint(); return 0;
		case WM_SIZE: Resize(); return 0;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

HMODULE hDLL;
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	hDLL = LoadDSP();

	Params = new params;
	Params->VUpoints   = new double[CAP_SIZE];
	Params->Specpoints = new double[CAP_SIZE];
	Params->freq	   = new double[CAP_SIZE];
	Params->amp		   = new double[CAP_SIZE];
	HANDLE tProcessAudioHndl = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&processAudio, (LPVOID)Params, 0, 0);

	win.Create(L"Audio Spectrum", WS_OVERLAPPEDWINDOW);
	SetWindowPos(win.Window(), HWND_TOPMOST, 100, 100, CAP_SIZE+56, 480, SWP_SHOWWINDOW);

	MSG msg = { };
	while (!Dispose) {
		InvalidateRect(win.Window(), NULL, FALSE);
		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	FreeLibrary(hDLL);
	return (int)msg.wParam;
}