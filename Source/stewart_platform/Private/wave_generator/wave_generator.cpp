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
	return true;
}

uint32 Fwave_generator::Run(){
	int b = 0;
	// while (b <= 100 && _stop_thread)
	// while (b <= 100)
	
	while (!_stop_thread)
	{
		float a = _test->GetWorld()->GetRealTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("--Thread GetWorld.GetTime() =  %f"), a);
		b++;
		FPlatformProcess::Sleep(0.25);
		
	}
	UE_LOG(LogTemp, Warning, TEXT("Thread Finished, b: %i -- !_stop_thread: %s"), b, (!_stop_thread ? TEXT("true") : TEXT("false")));
	// _stop_thread = true;
	
	return 0;
}


void Fwave_generator::Stop(){
	_stop_thread = true;
}
