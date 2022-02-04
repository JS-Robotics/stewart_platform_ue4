// Fill out your copyright notice in the Description page of Project Settings.


#include "wave_generator/wave_generator.h"

Fwave_generator::Fwave_generator(UWorld* test): _test(test) 
{
	Thread = FRunnableThread::Create( this, TEXT( "This is my thread example" ) );
	float a = _test->GetWorld()->GetRealTimeSeconds();
	UE_LOG(LogTemp, Warning, TEXT("--Thread GetWorld.GetTime() =  %f"), a);
}

Fwave_generator::~Fwave_generator(){
	if ( Thread != nullptr ){
		Thread->Kill( true );
		delete Thread;
	}
}

bool Fwave_generator::Init(){
	_stop_thread = false;
	position_ = {0,0,0};
	rotation_ = {0,0,0};
	return true;
}

uint32 Fwave_generator::Run(){
	float dt = 0;
	float time_step = 0.01;
	float wave_heave = 0.0f;
	float phase_heave = 0.0f;
	TArray<float> w = {0.2f, 0.4f, 0.6f, 0.8f, 1.0f, 1.2f, 1.4f, 1.6f, 1.8f, 2.0f};
	
	while (!_stop_thread){
		float world_time = _test->GetWorld()->GetRealTimeSeconds();
		float ttime = irregular_wave(world_time, w, 8.0f, 10.0f, 0, 0, wave_heave, phase_heave);
		UE4_tmux.Lock();
		position_ = {0,0,static_cast<float>(wave_heave*cos(phase_heave/180.0f*PI))/10};
		// position_ = {static_cast<float>(3*sin(world_time)), static_cast<float>(3*sin(world_time)), static_cast<float>(3*sin(world_time))};
		rotation_ = {0,0,0};
		// rotation_ = {static_cast<float>(10*sin(world_time)), static_cast<float>(10*sin(world_time)), static_cast<float>(10*sin(world_time))};
		UE4_tmux.Unlock();
		dt = _test->GetWorld()->GetRealTimeSeconds() - world_time;
		// UE_LOG(LogTemp, Warning, TEXT("world time: %f, and dt: %f"), world_time, dt);
		if (dt < time_step){
			FPlatformProcess::Sleep(time_step-dt);	
			
		}
		
	}
	UE_LOG(LogTemp, Warning, TEXT("Thread Finished: !_stop_thread: %s"), (!_stop_thread ? TEXT("true") : TEXT("false")));
	// _stop_thread = true;
	
	return 0;
}


void Fwave_generator::Stop(){
	_stop_thread = true;
}

void Fwave_generator::get_pose(FVector& position, FRotator& rotation)
{
	UE4_tmux.Lock();
	position = MoveTemp(position_);
	rotation = MoveTemp(rotation_);
	UE4_tmux.Unlock();
}

float Fwave_generator::pierson_moskowitz_wave_spectrum(float w, float H_s, float T_p){
	float w_p = 2.0f * PI / T_p;
	float s_pm = 5.0f/16.0f * pow(H_s, 2) * pow(w_p, 4) * pow(w,-5) * exp(-5.0f/4.0f * pow((w/w_p),4));
	return s_pm;
}

float Fwave_generator::jonswap_wave_spectrum(float w, float H_s, float T_p, float y){
	float peak_shape_parameter = T_p / sqrt(H_s);
	if (y == 0){
		if (peak_shape_parameter <= 3.6f){
			y = 5.0f;
		}
		else if (peak_shape_parameter >= 5.0f){
			y = 1.0f;
		}
		else {
			y = exp(5.75f - 1.15f * T_p/sqrt(H_s));
		}
	}
	float w_p = 2 * PI / T_p;
	float sigma = 0;
	if (w <= w_p){
		sigma = 0.07f;
	}
	else{
		sigma = 0.09f;
	}
	float a_y = 1.0f - 0.0287f * log(y);
	float s_jonswap = a_y * pow(a_y, exp(-0.5f*pow(((w-w_p)/(sigma*w_p)),2))) * pierson_moskowitz_wave_spectrum(w, H_s, T_p);
	return s_jonswap;
}

float Fwave_generator::irregular_wave(float t, TArray<float> w, float H_s, float T_p, float y, float x, float& amplitude, float& phase){
	
	// int vector_length = sizeof(w.Num());
	int vector_length =10;
	
	// for (int i =0; i < vector_length; i++)
	// {
	// 	if (i >= 10){
	// 		i = 10;
	// 	}
	// 	UE_LOG(LogTemp, Warning, TEXT("Size of frequency vector: %i, value of step[%i]: %f"), vector_length, i, w[i]);
	// 	
	// }
	
	
	float delta_w = 0.0f;
	float amplitude_i = 0.0f;
	float phase_i = 0.0f;
	for (int i =0; i < vector_length; i++){
		if (i < (vector_length - 1)){
			delta_w = w[i+1] - w[i];
		}
		else{
			delta_w = w[i] - w[i-1];
		}
		phase_i = phase_i +  (w[i]*t + FMath::FRandRange(0.0f,2*PI));  // TODO add support for k_i*x
		amplitude_i = amplitude_i + sqrt(2.0f*delta_w*jonswap_wave_spectrum(w[i], H_s, T_p, y));
	}
	amplitude = MoveTemp(amplitude_i);
	phase = MoveTemp(phase_i);
		UE_LOG(LogTemp, Warning, TEXT("Amplitude: %f , Phase: %f"), amplitude_i, phase_i);
	return t;
	// return 0;
	
}