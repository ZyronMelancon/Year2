#pragma once
#include "Application.h"


class IntroApp :
	public Application
{
public:
	IntroApp();
	~IntroApp();
protected:
	//Inherited
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;

	
private:
	
	
};

