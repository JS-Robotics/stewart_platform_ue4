// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"

struct WaveData{
	float heave;
	float surge;
	float sway;
	float roll;
	float pitch;
	float yaw;
};

/**
 * 
 */
class STEWART_PLATFORM_API Fwave_generator: public FRunnable
{
public:
	Fwave_generator(UWorld* test);
	~Fwave_generator();

	// FRunnable Pure Virtual functions
	bool Init() override;
	uint32 Run() override;
	void Stop() override;

	bool is_running(){return !_stop_thread;}
	void get_pose(FVector& position, FRotator& rotation);

protected:
	FCriticalSection UE4_tmux;
	FRunnableThread* Thread = nullptr;
	
private:
	float pierson_moskowitz_wave_spectrum(float w, float H_s, float T_p);
	float jonswap_wave_spectrum(float w, float H_s, float T_p, float y);
	float irregular_wave(float t, TArray<float> w, float H_s, float T_p, float y, float x, float& amplitude, float& phase);
	float body_displacment(float angle, float t, TArray<float> w, float H_s, float T_p, float y, float x, WaveData& wave_data);
	
	UWorld* _test;
	bool _stop_thread;
	FVector position_;
	FRotator rotation_;
};
