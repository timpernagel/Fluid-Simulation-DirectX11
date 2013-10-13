/***************************************************************
Fluid2DScene.h: Describes a scene that displays a 2D fluid
simulation using Direct3D

Author: Valentin Hinov
Date: 10/09/2013
**************************************************************/
#ifndef _FLUID2DSCENE_H
#define _FLUID2DSCENE_H

#include <vector>
#include <memory>
#include "IScene.h"

class D2DTexQuad;
class Camera;
class IFrameBuffer;
class D3DGraphicsObject;
class AdvectionShader;
class ImpulseShader;
class JacobiShader;
class DivergenceShader;
class SubtractGradientShader;
class BuoyancyShader;

struct ID3D11ShaderResourceView;

using namespace std;

#define TIME_STEP 0.125f
#define IMPULSE_RADIUS 0.03f
#define INTERACTION_IMPULSE_RADIUS 0.015f
#define JACOBI_ITERATIONS 50
#define CELL_SIZE 1.0f
#define GRADIENT_SCALE 1.0f
#define VEL_DISSIPATION 0.99f
#define DENSITY_DISSIPATION 0.9999f
#define TEMPERATURE_DISSIPATION 0.99f
#define SMOKE_BUOYANCY 1.0f
#define SMOKE_WEIGHT 0.05f
#define AMBIENT_TEMPERATURE 0.0f
#define IMPULSE_TEMPERATURE 10.0f
#define IMPULSE_DENSITY	1.0f

class Fluid2DScene : public IScene {
public:
	Fluid2DScene();
	~Fluid2DScene();

	bool Initialize(_In_ IGraphicsObject* graphicsObject, HWND hwnd);
	void Update(float delta);
	bool Render();

private:
	void Advect(ID3D11ShaderResourceView* velocityField, ID3D11ShaderResourceView* advectionTarget, IFrameBuffer* renderTarget, float dissipation);
	void ApplyBuoyancy(ID3D11ShaderResourceView* velocityField, ID3D11ShaderResourceView* temperatureField, ID3D11ShaderResourceView* density, IFrameBuffer* renderTarget);
	void ApplyImpulse(Vector2 mousePoint, Vector2 amount, float radius, ID3D11ShaderResourceView* originalState, IFrameBuffer* renderTarget);
	void Jacobi(ID3D11ShaderResourceView* pressure, ID3D11ShaderResourceView* divergence, IFrameBuffer* renderTarget);
	void ComputeDivergence(ID3D11ShaderResourceView* velocityField, IFrameBuffer* renderTarget);
	void SubtractGradient(ID3D11ShaderResourceView* velocityField, ID3D11ShaderResourceView* pressure, IFrameBuffer* renderTarget);

	void SwapBuffers(IFrameBuffer** buffers); // Useful function to swap the pointers of 2 frame buffers
private:
	unique_ptr<Camera>					mCamera;
	unique_ptr<D2DTexQuad>				mTexQuad;

	unique_ptr<AdvectionShader>			mAdvectionShader;
	unique_ptr<ImpulseShader>			mImpulseShader;
	unique_ptr<JacobiShader>			mJacobiShader;
	unique_ptr<DivergenceShader>		mDivergenceShader;
	unique_ptr<SubtractGradientShader>	mSubtractGradientShader;
	unique_ptr<BuoyancyShader>			mBuoyancyShader;

	IFrameBuffer** mVelocityFrameBuffer;
	IFrameBuffer** mDensityFrameBuffer;
	IFrameBuffer** mTemperatureFrameBuffer;
	IFrameBuffer** mPressureFrameBuffer;

	IFrameBuffer*  mDivergenceFrameBuffer;

	D3DGraphicsObject* pD3dGraphicsObj;

	int textureShowing;
};

#endif