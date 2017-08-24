#pragma once
struct GLFWwindow;

class Application
{
public:

	Application();
	virtual ~Application();
	void run(const char*, unsigned int, unsigned int, bool);

	virtual void startup() = 0;
	virtual void shutdown() = 0;
	virtual void update(float) = 0;
	virtual void draw() = 0;
	
	GLFWwindow *m_window;
	bool m_gameover;
};

