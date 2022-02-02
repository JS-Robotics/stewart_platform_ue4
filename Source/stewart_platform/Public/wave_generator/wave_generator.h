// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"

/**
 * 
 */
class STEWART_PLATFORM_API Fwave_generator: public FRunnable
{
public:
	Fwave_generator(UWorld* test);
	~Fwave_generator();

	bool Init() override;
	uint32 Run() override;
	void Stop() override;

	bool is_running(){return !_stop_thread;}

protected:
	FCriticalSection UE4_tmux;
	FRunnableThread* Thread = nullptr;
	

	
private:
	UWorld* _test;
	bool _stop_thread;
};
