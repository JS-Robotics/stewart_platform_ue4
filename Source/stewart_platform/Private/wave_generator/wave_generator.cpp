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
	float time_step = 0.008333f;
	float wave_heave = 0.0f;
	float phase_heave = 0.0f;
	TArray<float> w = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f, 1.9f, 2.0f};
	
	while (!_stop_thread){
		// float s_pm = jonswap_wave_spectrum(1.0f, 8.0f, 10.0f, 1.0f);  // TODO there is some sort of bug with jonswap calculation
		// UE_LOG(LogTemp, Warning, TEXT("s_jon: %f"), s_pm);
		float world_time = _test->GetWorld()->GetRealTimeSeconds();
		float ttime = irregular_wave(world_time, w, 4.0f, 8.0f, 0, 0, wave_heave, phase_heave);
		UE4_tmux.Lock();
		// position_ = {0,0,static_cast<float>(static_cast<float>(3*sin(world_time)))};
		position_ = {0,0,5.0f*wave_heave};
		// position_ = {static_cast<float>(3*sin(world_time)), static_cast<float>(3*sin(world_time)), static_cast<float>(3*sin(world_time))};
		rotation_ = {0,0,0};
		// rotation_ = {static_cast<float>(10*sin(world_time)), static_cast<float>(10*sin(world_time)), static_cast<float>(10*sin(world_time))};
		UE4_tmux.Unlock();
		dt = _test->GetWorld()->GetRealTimeSeconds() - world_time;
		UE_LOG(LogTemp, Warning, TEXT("world time: %f, and dt: %f"), world_time, dt);
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
	float s_pm = 5.0f/16.0f * pow(H_s, 2) * pow(w_p, 4) * pow(w,-5) * exp(-5.0f/4.0f * pow((w/w_p),-4.0f));
	return s_pm;
}

float Fwave_generator::jonswap_wave_spectrum(float w, float H_s, float T_p, float y){
	float peak_shape_parameter = T_p / sqrt(H_s);
	float applied_y = 0;  // TODO see if this can be substituted by y
	if (y == 0){
		if (peak_shape_parameter <= 3.6f){
			applied_y = 5.0f;
		}
		else if (peak_shape_parameter >= 5.0f){
			applied_y = 1.0f;
		}
		else {
			applied_y = exp(5.75f - 1.15f * T_p/sqrt(H_s));
		}
	}
	else{
		applied_y = y;
	}
	
	float w_p = 2 * PI / T_p;
	float sigma = 0;
	if (w <= w_p){
		sigma = 0.07f;
	}
	else{
		sigma = 0.09f;
	}
	float a_y = 1.0f - 0.0287f * log(applied_y);
	float s_jonswap = a_y * pow(applied_y, exp(-0.5f*pow((w-w_p)/(sigma*w_p),2))) * pierson_moskowitz_wave_spectrum(w, H_s, T_p);
	return s_jonswap;
}

float Fwave_generator::irregular_wave(float t, TArray<float> w, float H_s, float T_p, float y, float x, float& amplitude, float& phase){  // TODO update function to correct parameters
	
	// int vector_length = sizeof(w.Num());
	int vector_length =20; // TODO get this form the vector
	
	// for (int i =0; i < vector_length; i++)
	// {
	// 	if (i >= 10){
	// 		i = 10;
	// 	}
	// 	UE_LOG(LogTemp, Warning, TEXT("Size of frequency vector: %i, value of step[%i]: %f"), vector_length, i, w[i]);
	// 	
	// }
	// FRandomStream Stream(FMath::Rand());
	FRandomStream Stream(15);
	float surface_elevation = 0;
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
		// phase_i =  w[i]*t;  // TODO add support for k_i*x
		phase_i =  (w[i]*t + Stream.FRandRange(0.0f,2*PI));  // TODO add support for k_i*x
		// phase_i = phase_i +  (w[i]*t + FMath::FRandRange(0.0f,2*PI));  // TODO add support for k_i*x
		amplitude_i = sqrt(2.0f*delta_w*jonswap_wave_spectrum(w[i], H_s, T_p, y));
		surface_elevation += amplitude_i * cos(phase_i);
		
	}
	// amplitude = MoveTemp(amplitude_i);
	amplitude = MoveTemp(surface_elevation);
	phase = MoveTemp(phase_i);
	
		UE_LOG(LogTemp, Warning, TEXT("Amplitude: %f , Phase: %f"), amplitude_i, phase_i);
	return t;
	// return 0;
	
}