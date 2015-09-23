#include <stdio.h>
#include <GL\glew.h>
#include <CL\cl.h>
#include <CL\cl_gl.h>
#include <vector>
#include "IAppCallbacks.h"
#include "GLUTBackend.h"
#include "Body.h"
#include "CommonShaderProgram.h"
#include "CLUtil.h"
#include <GL\freeglut.h>
#include "IOUtil.h"
#include <GL\wglew.h>
#include <random>

class App : public IAppCallbacks
{
private:
	IBackend* m_backend;
	GLuint m_VB;
	std::vector<Body> m_bodies;
	ShaderProgram* m_shaderProgram;
	cl_device_id m_currentDev;
	cl_context m_clContext;
	cl_command_queue m_commandQueue;
	cl_kernel m_kernelCalcForces;
	cl_kernel m_kernelMove;
	cl_mem m_clVB;
	ulong m_last_time = 0;
	uint m_frame_count = 0;

	uint m_last_time_dt = 0;
	uint m_time_shift = 1;
	float m_dt = 0;

	float m_scale = 100.0f;
	Vector2f m_offset = Vector2f(0.0f,0.0f);
	float m_aspectRatio;
	bool m_pause = false;

public:
	App(IBackend* backend)
	{
		m_backend = backend;
	}

	virtual void OnKeyPressed(KEY key)
	{
		float offsetStep = 0.1 * m_scale;
		const float scaleStep = 0.4;

		switch (key)
		{
		case KEY::KEY_ESCAPE:
			m_backend->Terminate();
			break;

		case KEY::KEY_UP:
			m_offset.y -= offsetStep;
			m_shaderProgram->SetOffset(m_offset);
			break;
		case KEY::KEY_DOWN:
			m_offset.y += offsetStep;
			m_shaderProgram->SetOffset(m_offset);
			break;
		case KEY::KEY_LEFT:
			m_offset.x += offsetStep;
			m_shaderProgram->SetOffset(m_offset);
			break;
		case KEY::KEY_RIGHT:
			m_offset.x -= offsetStep;
			m_shaderProgram->SetOffset(m_offset);
			break;

		case KEY::KEY_q:
			m_scale += scaleStep;
			m_shaderProgram->SetScale(m_scale);
			break;
		case KEY::KEY_w:
			m_scale -= scaleStep;
			m_shaderProgram->SetScale(m_scale);
			break;
		case KEY::KEY_s:
			m_pause = !m_pause;
			break;
		}
	}

	virtual void OnMouseMove(int x, int y)
	{

	}

	virtual void OnCapturedMouseMove(int dx, int dy)
	{

	}

	virtual void OnRender(ulong time)
	{
		if (time - m_last_time_dt > 0)
		{
			m_dt = ((time - m_last_time_dt) / (float)m_time_shift) / 1000.0f;
			m_last_time_dt = time;
			m_time_shift = 1;
		}
		else
		{
			m_time_shift = 1;
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_VB);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Body), 0);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Body), (const GLvoid*)8);
		glDrawArrays(GL_POINTS, 0, m_bodies.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		m_backend->SwapBuffers();

		if (!m_pause)
			RunKernel();
		if (time - m_last_time > 1000)
		{
			printf("FPS: %d\n", m_frame_count);
			m_frame_count = 0;
			m_last_time = time;
		}
		else
		{
			m_frame_count++;
		}
		
	}

	virtual void OnClose()
	{

	}
	bool Init(int argc, char** argv)
	{
		GetCLDev();

		m_backend->Init(argc, argv);
		m_aspectRatio = 600.0f/800.0f;

		if (!m_backend->CreateWin(800, 600, false, "OpenGL test app"))
		{
			fprintf(stderr, "Faild to create window");
			return false;
		}

		GLenum res = glewInit();
		if (res != GLEW_OK) {
			fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
			return false;
		}

		if (!InitCL())
		{
			return false;
		}

		wglSwapIntervalEXT(0);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glFrontFace(GL_CW);
		//glCullFace(GL_BACK);
		//glEnable(GL_CULL_FACE);

		//glEnable(GL_DEPTH_TEST);

		m_bodies.push_back(Body(Vector2f(-0.8f, -0.8f), Vector2f(0.021f, 0.0f), Vector2f(-0.05f, 0.11f), Vector2f(0.0f, 0.0f)));
		m_bodies.push_back(Body(Vector2f(0.8f, 0.8f), Vector2f(0.021f, 0.0f), Vector2f(0.05f, -0.11f), Vector2f(0.0f, 0.0f)));
		m_bodies.push_back(Body(Vector2f(-0.8f, 0.8f), Vector2f(0.014f, 0.0f), Vector2f(0.01f, 0.1f), Vector2f(0.0f, 0.0f)));
		m_bodies.push_back(Body(Vector2f(0.8f, -0.8f), Vector2f(0.016f, 0.0f), Vector2f(0.01f, 0.1f), Vector2f(0.0f, 0.0f)));
		m_bodies.push_back(Body(Vector2f(0.0f, 0.0f), Vector2f(0.3f, 0.0f), Vector2f(0.0f, 0.0f), Vector2f(0.0f, 0.0f)));

		for (int i = 0; i < 5000; ++i)
		{
			m_bodies.push_back(Body(Vector2f((rand() % 100000) / 1000.0f - 1.0f, (rand() % 100000) / 1000.0f - 1.0f), Vector2f(rand() % 300 / 1000.0f, 0.0f), Vector2f((rand() % 2000) / 1000.0f - 1.0f, (rand() % 2000) / 1000.0f - 1.0f), Vector2f(0.0f, 0.0f)));
		}

		glGenBuffers(1, &m_VB);
		glBindBuffer(GL_ARRAY_BUFFER, m_VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Body) * m_bodies.size(), &m_bodies[0], GL_DYNAMIC_DRAW);

		cl_int ret = 0;
		m_clVB = clCreateFromGLBuffer(m_clContext, CL_MEM_READ_WRITE, m_VB, &ret);
		clErrNoAssert(ret);
		ret = clSetKernelArg(m_kernelCalcForces, 0, sizeof(cl_mem), (void *)&m_clVB);
		clErrNoAssert(ret);
		ret = clSetKernelArg(m_kernelMove, 0, sizeof(cl_mem), (void *)&m_clVB);
		clErrNoAssert(ret);

		m_last_time_dt = 0;
		m_time_shift = 1;
		m_dt = 0.00001;

		m_shaderProgram = new CommonShaderProgram("vertexshader.txt", "pixelshader.txt", "geometryshader.txt");
		m_shaderProgram->Compile();
		m_shaderProgram->Apply();
		m_shaderProgram->SetScale(m_scale);
		m_shaderProgram->SetOffset(m_offset);
		m_shaderProgram->SetAspectRatio(m_aspectRatio);

		return true;
	}

	void GetCLDev()
	{
		cl_int ret = 0;
		int index = 0;
		std::vector<cl_device_id> devices;
		GetDeviceIds(devices);
		for (unsigned int i = 0; i < devices.size(); ++i)
		{
			PrintDeviceInfo(i + 1, devices[i]);
		}


		printf("Enter device no:");
		scanf_s("%d", &index);
		m_currentDev = devices[index - 1];
	}

	bool InitCL()
	{
		cl_int ret = 0;
		cl_platform_id platform = nullptr;
		ret = clGetDeviceInfo(m_currentDev, CL_DEVICE_PLATFORM, sizeof(platform), &platform, nullptr);
		clErrNoAssert(ret);
		cl_context_properties props[] =
		{
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
			CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
			0
		};

		m_clContext = clCreateContext(props, 1, &m_currentDev, nullptr, nullptr, &ret);
		clErrNoAssert(ret);
		m_commandQueue = clCreateCommandQueue(m_clContext, m_currentDev, 0, &ret);
		clErrNoAssert(ret);

		std::string str;
		ReadFile("kernels.cl", str);
		const char * source_str = str.c_str();
		size_t source_size = str.length();

		cl_program program = clCreateProgramWithSource(m_clContext, 1, (const char **)&source_str, (size_t*)&source_size, &ret);
		clErrNoAssert(ret);
		ret = clBuildProgram(program, 1, &m_currentDev, ""/*"-g -s \"H:\\Dev\\OpenCL\\NBody\\NBody\\kernels.cl\""*/, nullptr, nullptr);
		if (ret != 0)
		{
			size_t len = 0;
			clGetProgramBuildInfo(program, m_currentDev, CL_PROGRAM_BUILD_LOG, 0, nullptr, &len);
			char *log = new char[len];
			clGetProgramBuildInfo(program, m_currentDev, CL_PROGRAM_BUILD_LOG, len, log, nullptr);
			printf("%s\n", log);
			system("PAUSE");
			return false;
		}

		m_kernelCalcForces = clCreateKernel(program, "calcForces", &ret);
		clErrNoAssert(ret);
		m_kernelMove = clCreateKernel(program, "move", &ret);
		clErrNoAssert(ret);
	}

	void RunKernel()
	{
		cl_int ret = 0;
		ret = clSetKernelArg(m_kernelMove, 1, sizeof(float), (void *)&m_dt);
		clErrNoAssert(ret);
		glFinish();
		ret = clEnqueueAcquireGLObjects(m_commandQueue, 1, &m_clVB, 0, 0, 0);
		clErrNoAssert(ret);
		size_t size = m_bodies.size();
		ret = clEnqueueNDRangeKernel(m_commandQueue, m_kernelCalcForces, 1, 0, &size, 0, 0, 0, 0);
		clErrNoAssert(ret);

		ret = clEnqueueNDRangeKernel(m_commandQueue, m_kernelMove, 1, 0, &size, 0, 0, 0, 0);
		clErrNoAssert(ret);
		ret = clEnqueueReleaseGLObjects(m_commandQueue, 1, &m_clVB, 0, 0, 0);
		clErrNoAssert(ret);
		clFinish(m_commandQueue);
	}

	void Run()
	{
		m_backend->Run(this);
	}
};


int main(int argc, char** argv)
{
	App* app = new App(GLUTBackend::GetInstance());
	if (!app->Init(argc, argv))
	{
		fprintf(stderr, "Init faild");
		getchar();
		return 1;
	}
	app->Run();
	delete app;
	return 0;
}