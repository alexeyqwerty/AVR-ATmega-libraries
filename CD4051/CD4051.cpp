#include "CD4051.h"

CD4051::CD4051(Port* A, Port* B, Port* C, Port* INH)
{
	this->A = A;
	this->B = B;
	this->C = C;
	this->INH = INH;	
	
	this->A->SetAsOutput();
	this->B->SetAsOutput();
	this->C->SetAsOutput();
	this->INH->SetAsOutput();
	this->INH->Set();
}

void CD4051::SelectChannel(uint8_t channelNumber)
{
	if(channelNumber > 7) return;
	
	true&(channelNumber) ? this->A->Set() : this->A->Clear();
	true&(channelNumber>>1) ? this->B->Set() : this->B->Clear();
	true&(channelNumber>>2) ? this->C->Set() : this->C->Clear();
}
void CD4051::EnableOutputs()
{
	this->INH->Clear();
}

void CD4051::DisableOutputs()
{
	this->INH->Set();
}

CD4051::~CD4051()
{
	delete this->A;
	delete this->B;
	delete this->C;
	delete this->INH;
}

void* CD4051::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void CD4051::operator delete(void* ptr)
{
	if(ptr != nullptr) free(ptr);
}
